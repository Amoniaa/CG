#include "stdafx.h"
#include "RayTrace.h"

#include <cstdio>
#include <chrono>
#include <cmath>
#include <sysinfoapi.h>

#include "JobSystem.h"
#include "PhotonMap.h"

// The number of photons in the global photon map
#define NUM_PHOTONS 2048
// The max radius to select photons from
#define MAX_PHOTON_RADIUS 100
// The max number of photons to gather
// must be a power of two minus one for the max-heap to function properly
#define PHOTONS_IN_ESTIMATE 63

// The number of photons in the caustic photon map
#define CAUSTIC_PHOTONS 2048
// The max number of caustic photons to gather
#define CAUSTIC_PHOTONS_IN_ESTIMATE 63

// The number of shadow rays to use to sample direct lighting
#define SHADOW_RAY_COUNT 25

// Max amount of bounces to compute
#define MAX_BOUNCES 3

// Number of Object in Scene
#define NUM_OBJECTS 8

CCgScene *scene;
CCgPhotonMap *photonMapper;

CCgRayTrace::CCgRayTrace()
{
	scene = new CCgScene(NUM_OBJECTS);
	photonMapper = new CCgPhotonMap();

}

CCgRayTrace::~CCgRayTrace()
{
	// Cleanup
	delete scene;
	delete photonMapper;
}

void CCgRayTrace::runRayTrace(uint32 *bmpImag, int threadNum)
{
	scheduler::startWorkers(threadNum);
	int64 time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	randutil::init(time);   

	// Setup our cornell box ¿µÄÝ¶ûºÐ×Ó
	scene->objects[0] = new PlaneObject( 0, -5,   0,  -5, 5,  0xFFEEEEEE, 0.4, 0.0, 0.0, 1.0);
	scene->objects[1] = new PlaneObject( 0,  5,   0,  -5, 5, 0xFFDEEEEEE, 0.4, 0.0, 0.0, 1.0);
	scene->objects[2] = new PlaneObject( 5,  0,   0,  -5, 5,  0xFFFF3333, 0.4, 0.0, 0.0, 1.0);
	scene->objects[3] = new PlaneObject(-5,  0,   0,  -5, 5,  0xFF3333FF, 0.4, 0.0, 0.0, 1.0);
	scene->objects[4] = new PlaneObject( 0,  0,  10,  -5, 5,  0xFFEEEEEE, 0.4, 0.0, 0.0, 1.0);
	scene->objects[5] = new SphereObject(2, -3.5, 3, 1.5,     0xFFFFFFFF, 0.0, 0.1, 0.9, 0.0);
	scene->objects[5]->refraction = 2.5;
	scene->objects[6] = new SphereObject(-2, -3.5, 5, 1.5, 0xFFFFFFFF, 0.0, 1.0, 0.0, 0.0);
	//scene->objects[7] = new SphereObject(0, 1, 5, 0.8, 0xFF33FF33, 0.4, 0.0, 0.0, 1.0, 0.2, 0, 0);

	scene->objects[7] = new PlaneObject(0, -4, 0.5, -0.5, 0.5, 0xFF00EEEE, 0.4, 0.0, 0.0, 1.0);

	int32 sample_ratio = 1;
	uint32 *pane = new uint32[1280 * 720 * sample_ratio * sample_ratio];
	Vector camera(0, 0, -12);
	renderScene(scene, camera, pane, 1280 * sample_ratio, 720 * sample_ratio);

//	uint32 *sample = new uint32[1280 * 720];
	for (int x = 0; x < 1280; x++) {
		for (int y = 0; y < 720; y++) {
			int32 red = 0;
			int32 green = 0;
			int32 blue = 0;
			for (int x0 = 0; x0 < sample_ratio; x0++) {
				for (int y0 = 0; y0 < sample_ratio; y0++) {
					int32 s = pane[x * sample_ratio + x0 + (y * sample_ratio + y0) * 1280 * sample_ratio];
					red += (s >> 16) & 0xFF;
					green += (s >> 8) & 0xFF;
					blue += s & 0xFF;
				}
			}
			red /= sample_ratio * sample_ratio;
			green /= sample_ratio * sample_ratio;
			blue /= sample_ratio * sample_ratio;
			bmpImag[x + y * 1280] = (0xFF << 24) | (red << 16) | (green << 8) | blue;
		}
	}
}


// Traces a ray and returns a computed color value
uint32 traceRay(Vector &ray_source, Vector &ray, CCgScene *scene, 
	            SceneObject *exclude, int bounce, kdnode *global_tree, 
	            kdnode *caustic_tree, Vector *light_color) 
{
	if (bounce > MAX_BOUNCES) return 0xFF000000;
	
	Vector nearest_result(0, 0, 0);
	Vector nearest_normal(0, 0, 0);
	SceneObject *nearest_obj = nullptr;

	scene->intersect(ray_source, ray, exclude, &nearest_result, &nearest_normal,
		              &nearest_obj, randutil::nextDouble());

	if (nearest_obj == nullptr) {   // Missed the scene so return a background color
		return 0xFF000000;
	} else if (nearest_result.y > 4.95 && nearest_result.x > -1 &&
	           nearest_result.x < 1 && nearest_result.z > 3 && nearest_result.z < 5) {
		                            // Hit the light source, TODO: don't hardcode this?
		return (0xFF << 24) | (min32(fastfloor(light_color->x * 0xFF) + 50, 0xFF) << 16) | 
			                  (min32(fastfloor(light_color->y * 0xFF) + 50, 0xFF) << 8) | 
			                   min32(fastfloor(light_color->z * 0xFF) + 50, 0xFF);
	} else {                        // Hit some object in the scene	
		uint32 refract_res = 0;
		uint32 reflect_res = 0;
		uint32 absorb_res = 0;

		if (nearest_obj->transmission_chance > 0) {    // refract the ray and recast
			double n = 1 / nearest_obj->refraction;    // Equation from Fundamentals of Computer Graphics
			                                           // 4th edition p 325
			double d = nearest_normal.x * ray.x + nearest_normal.y * ray.y + nearest_normal.z * ray.z;
			Vector n1(nearest_normal);
			n1.mul(d);
			n1.set(ray.x - n1.x, ray.y - n1.y, ray.z - n1.z);
			n1.mul(n);
			Vector n2(nearest_normal);
			double s = 1 - (n * n) * (1 - d * d);
			n2.mul(sqrt(s));
			n1.add(-n2.x, -n2.y, -n2.z);
			// n1 is refracted vector, step a tiny part along our refracted ray to avoid
			// having to exclude the object we just hit allowing us to hit the other side of it
			ray_source.set(nearest_result.x + n1.x * 0.01, 
				           nearest_result.y + n1.y * 0.01, 
				           nearest_result.z + n1.z * 0.01);
			ray.set(n1.x, n1.y, n1.z);
			ray.normalize();
			refract_res = traceRay(ray_source, ray, scene, nullptr, bounce + 1, 
				                   global_tree, caustic_tree, light_color);
		}
		if (nearest_obj->specular_chance > 0) {    // calculate reflection angle
			Vector n1(nearest_normal);
			n1.mul(n1.x * ray.x + n1.y * ray.y + n1.z * ray.z);
			n1.mul(2);
			ray_source.set(nearest_result.x, nearest_result.y, nearest_result.z);
			ray.set(ray.x - n1.x, ray.y - n1.y, ray.z - n1.z);
			ray.normalize();
			// continue trace
			reflect_res = traceRay(ray_source, ray, scene, nearest_obj, bounce + 1, 
				                   global_tree, caustic_tree, light_color);
		}
		if (nearest_obj->absorb_chance > 0) {          // calculate color based on global photon map,
			                                           // caustics, direct lighting, and specular effects
			photon* nearest_photons[PHOTONS_IN_ESTIMATE];
			double photon_distances[PHOTONS_IN_ESTIMATE];
			for (int i = 0; i < PHOTONS_IN_ESTIMATE; i++) {
				nearest_photons[i] = nullptr;
				photon_distances[i] = 0;
			}
			// global illumication
			double redintensity = 0.0f;
			double greenintensity = 0.0f;
			double blueintensity = 0.0f;
			{
				int found = photonMapper->find_nearest_photons(nearest_photons, photon_distances, 
					                                           PHOTONS_IN_ESTIMATE, 0, &nearest_result, 
					                                           global_tree, MAX_PHOTON_RADIUS);
				if (nearest_photons[0] != nullptr) {
					double r = photon_distances[0];
					for (int i = 0; i < found; i++) {
						photon *ph = nearest_photons[i];
						if (ph == nullptr) {
							break;
						}
						// check the angle of incidence of the photon relative to the surface normal
						double d = -nearest_normal.dot(ph->dx, ph->dy, ph->dz);
						if (d <= 0) {
							continue;
						}
						// filter out photons that were behind or infront of the surface
						// to get better results on parallel edges offset from each other
						// where the photon would not actually have a contribution normally
						Vector dist(nearest_result.x - ph->x, nearest_result.y - ph->y, nearest_result.z - ph->z);
						dist.mul((dist.x * nearest_normal.x + dist.y * nearest_normal.y + dist.z * nearest_normal.z) / (dist.z * dist.z + dist.z * dist.z + dist.z * dist.z));
						if (dist.lengthSquared() > 0.1) {
							continue;
						}
						// decrease the power of the photon by the square of the distance from
						// the sampled point
						double filter = (1 - photon_distances[i] / r);
						filter = filter * filter;
						redintensity += d * (ph->power[0] / 255.0) * filter;
						greenintensity += d * (ph->power[1] / 255.0) * filter;
						blueintensity += d * (ph->power[2] / 255.0) * filter;
					}
					// divide the intensities by the area to get a density approximation
					redintensity /= (3.141592653589 * 2 * r);
					greenintensity /= (3.141592653589 * 2 * r);
					blueintensity /= (3.141592653589 * 2 * r);
				}
			}
			double redcaustic_contribution = 0;
			double greencaustic_contribution = 0;
			double bluecaustic_contribution = 0;
			{ // caustics
				int found = photonMapper->find_nearest_photons(nearest_photons, photon_distances, 
					                                           CAUSTIC_PHOTONS_IN_ESTIMATE, 0, &nearest_result,
					                                           caustic_tree, 100);
				if (nearest_photons[0] != nullptr) {
					double r = photon_distances[0];
					for (int i = 0; i < found; i++) {
						photon *ph = nearest_photons[i];
						if (ph == nullptr) {
							break;
						}
						// check angle of incidence
						double d = -nearest_normal.dot(ph->dx, ph->dy, ph->dz);
						if (d <= 0) {
							continue;
						}
						// filter photons on a parallel yet offset plane
						Vector dist(nearest_result.x - ph->x, nearest_result.y - ph->y, nearest_result.z - ph->z);
						dist.mul((dist.x * nearest_normal.x + dist.y * nearest_normal.y + dist.z * nearest_normal.z) / (dist.z * dist.z + dist.z * dist.z + dist.z * dist.z));
						if (dist.lengthSquared() > 0.1) {
							continue;
						}
						// decrease the photons power by the distance raised to the 4th power
						// so that the caustic photons have a very aggressive falloff
						double filter = (1 - photon_distances[i] / r);
						filter = filter * filter * filter * filter;
						// the photon powers are all shifted towards white so that they still can carry
						// color if the light was colored but they generally will be a lot lighter
						redcaustic_contribution += d * filter * min(ph->power[0] / 255.0f + 0.5, 1);
						greencaustic_contribution += d * filter * min(ph->power[1] / 255.0f + 0.5, 1);
						bluecaustic_contribution += d * filter * min(ph->power[2] / 255.0f + 0.5, 1);
					}
					// divide by the area as well as an additional factor to account for the
					// power increase we performed
					redcaustic_contribution /= (3.141592653589 * 8 * r);
					greencaustic_contribution /= (3.141592653589 * 8 * r);
					bluecaustic_contribution /= (3.141592653589 * 8 * r);
				}
			}

			// calculate direct illumication with a shadow ray
			int light_count = 0;
			Vector light_source(0, 0, 0);
			Vector shadow_ray(0, 0, 0);
			Vector result(0, 0, 0);
			Vector normal(0, 0, 0);
			for (int i = 0; i < SHADOW_RAY_COUNT; i++) {
				// our light is a square so for each shadow ray we send it towards a random point
				// on the light to get a softer shadow
				double x0 = randutil::nextDouble() * 2 - 1;
				double z0 = randutil::nextDouble() * 2 + 3;
				double y0 = 4.95;
				light_source.set(x0, y0, z0);
				shadow_ray.set(light_source.x - nearest_result.x, 
					           light_source.y - nearest_result.y, 
					           light_source.z - nearest_result.z);
				double max_dist = shadow_ray.lengthSquared();
				shadow_ray.normalize();
				double dt = randutil::nextDouble();
				for (int i = 0; i < scene->size; i++) {
					if (scene->objects[i] == nearest_obj) {
						continue;
					}
					if (scene->objects[i]->intersect(&nearest_result, &shadow_ray, &result, &normal, dt)) {
						// ensure that the object we hit is in front of the light
						if (nearest_result.distSquared(&result) > max_dist) {
							continue;
						}
						// keep track of every ray that hit is in shadow
						light_count++;
						break;
					}
				}
			}
			double direct = 0;
			double specular = 0;
			if (light_count < SHADOW_RAY_COUNT) {
				// determine an approximate angle of incidence using the last shadow ray cast
				double d = nearest_normal.dot(&shadow_ray);
				if (d > 0) {
					direct += d * 0.2 * (1 - (light_count / (double)SHADOW_RAY_COUNT));
				}
				// calculate any specular effect if at least one shadow ray
				// reached the light source
				if (nearest_obj->specular_coeff != 0) {
					Vector light_dir(-nearest_result.x, 5 - nearest_result.y, 4 - nearest_result.z);
					light_dir.normalize();
					Vector v(ray_source.x - nearest_result.x, ray_source.y - nearest_result.y, ray_source.z - nearest_result.z);
					v.normalize();
					Vector h(light_dir);
					h.add(&v);
					h.normalize();
					double sp = h.dot(&nearest_normal);
					if (sp > 0) {
						specular = 0.3f * std::pow(sp, nearest_obj->specular_coeff) * (1 - (light_count / (double)SHADOW_RAY_COUNT));
					}
				}
			}

			// calculate the final intensities in each color band
			double redradiosity = direct + redcaustic_contribution + redintensity;
			if (redradiosity > 1) redradiosity = 1;
			double greenradiosity = direct + greencaustic_contribution + greenintensity;
			if (greenradiosity > 1) greenradiosity = 1;
			double blueradiosity = direct + bluecaustic_contribution + blueintensity;
			if (blueradiosity > 1) blueradiosity = 1;

			// multiply by the objects color
			int32 red = fastfloor(nearest_obj->red * redradiosity * 0xFF);
			int32 green = fastfloor(nearest_obj->green * greenradiosity * 0xFF);
			int32 blue = fastfloor(nearest_obj->blue * blueradiosity * 0xFF);
			absorb_res = (0xFF << 24) | (red << 16) | (green << 8) | blue;
		}
		// combine the results of the absorption, reflection, and transmission
		float sred = ((reflect_res >> 16) & 0xFF) / 255.0f;
		float sgreen = ((reflect_res >> 8) & 0xFF) / 255.0f;
		float sblue = ((reflect_res) & 0xFF) / 255.0f;
		float tred = ((refract_res >> 16) & 0xFF) / 255.0f;
		float tgreen = ((refract_res >> 8) & 0xFF) / 255.0f;
		float tblue = ((refract_res) & 0xFF) / 255.0f;
		float ared = ((absorb_res >> 16) & 0xFF) / 255.0f;
		float agreen = ((absorb_res >> 8) & 0xFF) / 255.0f;
		float ablue = ((absorb_res) & 0xFF) / 255.0f;
		int32 red = fastfloor((sred * nearest_obj->specular_chance + tred * nearest_obj->transmission_chance + ared * nearest_obj->absorb_chance) * 255);
		int32 green = fastfloor((sgreen * nearest_obj->specular_chance + tgreen * nearest_obj->transmission_chance + agreen * nearest_obj->absorb_chance) * 255);
		int32 blue = fastfloor((sblue * nearest_obj->specular_chance + tblue * nearest_obj->transmission_chance + ablue * nearest_obj->absorb_chance) * 255);
		int32 result = (0xFF << 24) | (red << 16) | (green << 8) | blue;
		return result;
	}
}

// data used by each job
struct render_task_data {
	int32 y;
	int32 width;
	int32 height;
	uint32 *pane;
	CCgScene *scene;
	kdnode *global_tree;
	kdnode *caustic_tree;
	Vector *light_color;
	Vector *camera;
};

// renders a row of pixels in the final image
void render_task(void *vdata)
{
	render_task_data *data = (render_task_data*)vdata;
	Vector ray(0, 0, 0);
	Vector ray_source(data->camera);
	Vector *light_dir = new Vector(0, 0, 0);
	SceneObject *exclude = nullptr;
	ray_source.set(0, 0, -12);
	double last_progress = 0;
	double fov = (data->width / 1280.0) * 64.0;
	for (int32 x = 0; x < data->width; x++) {
		ray_source.set(data->camera);
		// jitter the ray slightly to reduce artifacts in our anti-aliasing
		double x1 = randutil::nextDouble() * 0.6 - 0.3;
		double y1 = randutil::nextDouble() * 0.6 - 0.3;
		double x0 = (x - data->width / 2 + x1) / fov - ray_source.x;
		double y0 = (data->y - data->height / 2 + y1) / fov - ray_source.y;
		ray.set(x0, y0, -ray_source.z);
		ray.normalize();
		// @TODO: transform our ray to the final camera position and rotation

		// trace into the scene and set the color into the pane
		data->pane[x + data->y *data->width] = traceRay(ray_source, ray, data->scene, nullptr, 0,
			                                            data->global_tree, data->caustic_tree, data->light_color);
	}
}

void CCgRayTrace::renderScene(CCgScene *scene, Vector &camera, uint32 *pane, int32 width, int32 height)
{
	// photon mapping
	// Based on "A Practical Guide to Global Illumination using Photon Maps" from Siggraph 2000
	// https://graphics.stanford.edu/courses/cs348b-00/course8.pdf

	Vector ray(0, 0, 0);
	Vector light_source(0, 4.96, 4);
	SceneObject *nearest_obj = nullptr;
	Vector nearest_result(0, 0, 0);
	Vector nearest_normal(0, 0, 0);
	Vector light_color(0.6, 0.6, 0.6);

	// calculate the global photon tree
	kdnode *global_tree = photonMapper->createPhotonMap(NUM_PHOTONS, light_source, light_color, scene);
	// calculate the caustic photon tree
	kdnode *caustic_tree = photonMapper->createCausticPhotonMap(CAUSTIC_PHOTONS, light_source, light_color, scene);
									 
	// rendering
	for (int32 y = 0; y < height; y++) {
		render_task_data *data = new render_task_data;
		data->y = y;
		data->width = width;
		data->height = height;
		data->pane = pane;
		data->scene = scene;
		data->global_tree = global_tree;
		data->caustic_tree = caustic_tree;
		data->light_color = &light_color;
		data->camera = &camera;
		scheduler::submit(render_task, data);
	}

	scheduler::waitForCompletion();

	photonMapper->deleteTree(global_tree);
	photonMapper->deleteTree(caustic_tree);
}
