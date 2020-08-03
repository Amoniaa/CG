#include "stdafx.h"
#include "DrawScene.h"

#include "gl\gl.h"
#include "gl\glu.h"

CDrawScene::CDrawScene()
{
}

CDrawScene::CDrawScene(Ccg2019WJY3DTransView* pView)
{
	m_pView = pView;

	glObjColor[TRIANGLE][0] = 0.0f;
	glObjColor[TRIANGLE][1] = 0.0f;
	glObjColor[TRIANGLE][2] = 1.0f;
	glObjColor[BALL][0] = 1.0f;  glObjColor[BALL][1] = 1.0f;  glObjColor[BALL][2] = 0.0f;
	glObjColor[CUBE][0] = 0.0f;  glObjColor[CUBE][1] = 1.0f;  glObjColor[CUBE][2] = 0.0f;

}

CDrawScene::~CDrawScene()
{
}

void CDrawScene::DrawScene()
{
	int forLimitValue = 3;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	DrawBackground();

	int transSelect = pDoc->m_transSelect;

	//if (pDoc->rayTraceSelect != 1) DrawBackground();  // Need not draw if RayTracing on CView.

	//if (pDoc->m_onLight && pDoc->m_onShade) forLimitValue = pDoc->m_objectNum;

	DWORD  opStart, opFinish;

	opStart = GetTickCount();       //记录测试开始时间，单位是毫秒


	for (m_objNumber = 0; m_objNumber < pDoc->m_objectNum; m_objNumber++) {

		m_whoObject = &pDoc->m_spaceObjects[m_objNumber];

		if (BALL <= transSelect && transSelect <= TRIANGLE && m_objNumber == transSelect) CaculateMatrix();

		TransSpaceObject();

		if (transSelect <= TRIANGLE || m_objNumber > TRIANGLE) pRemoveBackFace();

		pClipSpaceObject();

		//if (pDoc->m_onLight) pLightSpaceObject();

		if (pDoc->m_onLight) {
			if (m_objNumber == BALL || m_objNumber > TRIANGLE) pLightSpaceBall();
			else                                               pLightSpaceObject();
		}

		DrawSpaceObject();

		projectSpaceObject();
	}

	opFinish = GetTickCount();      //记录测试结束时间，单位是毫秒

	pDoc->m_DrawSceneRunTime = (opFinish - opStart) / 1000.0f;

}

#define LOOKDEPTH 6.0f

void CDrawScene::DrawBackground()
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	// Draw Space Axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);                  // X Axis 
	glVertex3f(6.0f, 0.0f, 0.0f);
	glVertex3f(-6.0f, 0.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);                  // Y Axis 
	glVertex3f(0.0f, 6.0f, 0.0f);
	glVertex3f(0.0f, -6.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);                  // Z Axis 
	glVertex3f(0.0f, 0.0f, 6.0f);
	glVertex3f(0.0f, 0.0f, -24.0f);
	glEnd();

	//float Scale[3] = { 0.4f, 0.4f,  0.4f };
	//float Rotate[3] = { 0.0f, 0.0f,  0.0f };      // No rotate
	//float Offset[3] = { 6.0f, 0.0f, -0.1f };

	//glColor3f(1.0f, 0.0f, 0.0f);
	//m_pView->m_axisFont.GLDrawText(Offset, Scale, Rotate, _T("X"));
	//glColor3f(0.0f, 1.0f, 0.0f);
	//Offset[0] = 0.0f;  Offset[1] = 6.0f;  Offset[2] = -0.1f;
	//m_pView->m_axisFont.GLDrawText(Offset, Scale, Rotate, _T("Y"));
	//glColor3f(0.0f, 1.0f, 1.0f);
	//Rotate[1] = 90.0f;                                             // Around Y 90'
	//Offset[0] = -0.1f;  Offset[1] = 0.0f;  Offset[2] = 6.5f;
	//m_pView->m_axisFont.GLDrawText(Offset, Scale, Rotate, _T("Z"));

	// Simulate the EyE
	glColor3f(0.0f, 1.0f, 1.0f);
	//glTranslatef(pDoc->eyeX, pDoc->eyeY, pDoc->eyeZ);
	//  glutSolidSphere(0.08, 8, 8);
	//glTranslatef(-pDoc->eyeX, -pDoc->eyeY, -pDoc->eyeZ);
	glBegin(GL_LINES);
	glVertex3f(pDoc->eyeX, pDoc->eyeY, pDoc->eyeZ + 0.3f);
	glVertex3f(pDoc->eyeX, pDoc->eyeY + 0.18f, pDoc->eyeZ - 0.2f);
	glVertex3f(pDoc->eyeX, pDoc->eyeY, pDoc->eyeZ + 0.3f);
	glVertex3f(pDoc->eyeX, pDoc->eyeY - 0.18f, pDoc->eyeZ - 0.2f);
	glEnd();


	// Draw View window on XOY Plane 
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(pDoc->winLx, pDoc->winLy, 0.0f);
	glVertex3f(pDoc->winRx, pDoc->winLy, 0.0f);
	glVertex3f(pDoc->winRx, pDoc->winRy, 0.0f);
	glVertex3f(pDoc->winLx, pDoc->winRy, 0.0f);
	glVertex3f(pDoc->winLx, pDoc->winLy, 0.0f);
	glEnd();

	// Link Eye with Window to create space clip box
	float x, y, z;
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(pDoc->eyeX, pDoc->eyeY, pDoc->eyeZ);         // Left Bottom
	x = (pDoc->winLx - pDoc->eyeX) * LOOKDEPTH + pDoc->eyeX;
	y = (pDoc->winLy - pDoc->eyeY) * LOOKDEPTH + pDoc->eyeY;
	z = -pDoc->eyeZ * LOOKDEPTH + pDoc->eyeZ;
	glVertex3f(x, y, z);

	glVertex3f(pDoc->eyeX, pDoc->eyeY, pDoc->eyeZ);         // Right Bottom
	x = (pDoc->winRx - pDoc->eyeX) * LOOKDEPTH + pDoc->eyeX;
	y = (pDoc->winLy - pDoc->eyeY) * LOOKDEPTH + pDoc->eyeY;
	z = -pDoc->eyeZ * LOOKDEPTH + pDoc->eyeZ;
	glVertex3f(x, y, z);

	glVertex3f(pDoc->eyeX, pDoc->eyeY, pDoc->eyeZ);         // Right Top
	x = (pDoc->winRx - pDoc->eyeX) * LOOKDEPTH + pDoc->eyeX;
	y = (pDoc->winRy - pDoc->eyeY) * LOOKDEPTH + pDoc->eyeY;
	z = -pDoc->eyeZ * LOOKDEPTH + pDoc->eyeZ;
	glVertex3f(x, y, z);

	glVertex3f(pDoc->eyeX, pDoc->eyeY, pDoc->eyeZ);         // Left Top
	x = (pDoc->winLx - pDoc->eyeX) * LOOKDEPTH + pDoc->eyeX;
	y = (pDoc->winRy - pDoc->eyeY) * LOOKDEPTH + pDoc->eyeY;
	z = -pDoc->eyeZ * LOOKDEPTH + pDoc->eyeZ;
	glVertex3f(x, y, z);
	glEnd();

	// Draw Clip Cube Front / Back Plane
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(pDoc->clipCubeFrontLB[0], pDoc->clipCubeFrontLB[1], pDoc->clipCubeFrontLB[2]);
	glVertex3f(pDoc->clipCubeFrontRB[0], pDoc->clipCubeFrontRB[1], pDoc->clipCubeFrontRB[2]);
	glVertex3f(pDoc->clipCubeFrontRT[0], pDoc->clipCubeFrontRT[1], pDoc->clipCubeFrontRT[2]);
	glVertex3f(pDoc->clipCubeFrontLT[0], pDoc->clipCubeFrontLT[1], pDoc->clipCubeFrontLT[2]);
	glVertex3f(pDoc->clipCubeFrontLB[0], pDoc->clipCubeFrontLB[1], pDoc->clipCubeFrontLB[2]);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(pDoc->clipCubeBackLB[0], pDoc->clipCubeBackLB[1], pDoc->clipCubeBackLB[2]);
	glVertex3f(pDoc->clipCubeBackRB[0], pDoc->clipCubeBackRB[1], pDoc->clipCubeBackRB[2]);
	glVertex3f(pDoc->clipCubeBackRT[0], pDoc->clipCubeBackRT[1], pDoc->clipCubeBackRT[2]);
	glVertex3f(pDoc->clipCubeBackLT[0], pDoc->clipCubeBackLT[1], pDoc->clipCubeBackLT[2]);
	glVertex3f(pDoc->clipCubeBackLB[0], pDoc->clipCubeBackLB[1], pDoc->clipCubeBackLB[2]);
	glEnd();

	// Simulate the Lights
	for (int i = 0; i < pDoc->nrLights; i++) {
		if (i == (pDoc->m_transSelect - LIGHT0)) glColor3f(1.0f, 1.0f, 1.0f);
		else                                     glColor3f(1.0f, 1.0f, 0.0f);

		//glTranslatef(pDoc->lightX[i], pDoc->lightY[i], pDoc->lightZ[i]);
		//glutSolidSphere (0.1, 8, 8);
		//glTranslatef(-pDoc->lightX[i], -pDoc->lightY[i], -pDoc->lightZ[i]);

		for (int j = 0; j < 8; j++) {
			glBegin(GL_LINES);
			glVertex3f(pDoc->lightX[i], pDoc->lightY[i], pDoc->lightZ[i]);
			glVertex3f(pDoc->lightX[i], pDoc->lightY[i] + 0.3f*sin(j*3.14 / 4), pDoc->lightZ[i] + 0.3f*cos(j*3.14 / 4));
			glEnd();
		}
	}

}

#define DELTAMOVE 0.1f
#define THETA 3.1415926/36

void CDrawScene::CaculateMatrix()
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	//	if (pDoc->m_selfRotate) pObjectCenter();
	switch (pDoc->m_transDir) {
	case 0: // -X Trans
		switch (pDoc->m_transMode) {
		case 0: // translate
			translate3dMatrix(-DELTAMOVE, 0, 0);
			break;
		case 1: // rotate
			//translate3dMatrix(-100, -100, -100);
			translate3dMatrix(-m_whoObject->objCenter.x, -m_whoObject->objCenter.y, -m_whoObject->objCenter.z);
			rotateX3Dmatrix(-sin(THETA), cos(THETA));
			translate3dMatrix(m_whoObject->objCenter.x, m_whoObject->objCenter.y, m_whoObject->objCenter.z);
			break;
		}
		break;
	case 1: // +X Trans
		switch (pDoc->m_transMode) {
		case 0: // translate
			translate3dMatrix(DELTAMOVE, 0, 0);
			break;
		case 1: // rotate
			translate3dMatrix(-m_whoObject->objCenter.x, -m_whoObject->objCenter.y, -m_whoObject->objCenter.z);
			rotateX3Dmatrix(sin(THETA), cos(THETA));
			translate3dMatrix(m_whoObject->objCenter.x, m_whoObject->objCenter.y, m_whoObject->objCenter.z);

			break;
		}
		break;
	case 2: // -Y Trans
		switch (pDoc->m_transMode) {
		case 0: // translate
			translate3dMatrix(0, -DELTAMOVE, 0);
			break;
		case 1: // rotate
			translate3dMatrix(-m_whoObject->objCenter.x, -m_whoObject->objCenter.y, -m_whoObject->objCenter.z);
			rotateY3Dmatrix(-sin(THETA), cos(THETA));
			translate3dMatrix(m_whoObject->objCenter.x, m_whoObject->objCenter.y, m_whoObject->objCenter.z);

			break;
		}
		break;
	case 3: // +Y Trans
		switch (pDoc->m_transMode) {
		case 0: // translate
			translate3dMatrix(0, DELTAMOVE, 0);
			break;
		case 1: // rotate
			translate3dMatrix(-m_whoObject->objCenter.x, -m_whoObject->objCenter.y, -m_whoObject->objCenter.z);
			rotateY3Dmatrix(sin(THETA), cos(THETA));
			translate3dMatrix(m_whoObject->objCenter.x, m_whoObject->objCenter.y, m_whoObject->objCenter.z);

			break;
		}
		break;
	case 4:  // +Z Trans
		switch (pDoc->m_transMode) {
		case 0: // translate
			translate3dMatrix(0, 0.0, DELTAMOVE);
			break;
		case 1: // rotate
			translate3dMatrix(-m_whoObject->objCenter.x, -m_whoObject->objCenter.y, -m_whoObject->objCenter.z);
			rotateZ3Dmatrix(sin(THETA), cos(THETA));
			translate3dMatrix(m_whoObject->objCenter.x, m_whoObject->objCenter.y, m_whoObject->objCenter.z);

			break;
		}
		break;
	case 5:  // -Z Trans
		switch (pDoc->m_transMode) {
		case 0: // translate
			translate3dMatrix(0, 0.0f, -DELTAMOVE);
			break;
		case 1: // rotate
			translate3dMatrix(-m_whoObject->objCenter.x, -m_whoObject->objCenter.y, -m_whoObject->objCenter.z);
			rotateZ3Dmatrix(-sin(THETA), cos(THETA));
			translate3dMatrix(m_whoObject->objCenter.x, m_whoObject->objCenter.y, m_whoObject->objCenter.z);

			break;
		}
		break;
	}
}

void CDrawScene::translate3dMatrix(float Tx, float Ty, float Tz)
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();
	m_whoObject->transMatrix[3][0] += Tx;
	m_whoObject->transMatrix[3][1] += Ty;
	m_whoObject->transMatrix[3][2] += Tz;
}

void CDrawScene::rotateX3Dmatrix(float S, float C)
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();
	for (int i = 0; i < 4; i++) {
		float temp;
		temp = m_whoObject->transMatrix[i][1] * C - m_whoObject->transMatrix[i][2] * S;
		m_whoObject->transMatrix[i][2] = m_whoObject->transMatrix[i][1] * S + m_whoObject->transMatrix[i][2] * C;
		m_whoObject->transMatrix[i][1] = temp;
	}
}

void CDrawScene::rotateY3Dmatrix(float S, float C)
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();
	for (int i = 0; i < 4; i++) {
		float temp;
		temp = m_whoObject->transMatrix[i][0] * C + m_whoObject->transMatrix[i][2] * S;
		m_whoObject->transMatrix[i][2] = -m_whoObject->transMatrix[i][0] * S + m_whoObject->transMatrix[i][2] * C;
		m_whoObject->transMatrix[i][0] = temp;
	}
}

void CDrawScene::rotateZ3Dmatrix(float S, float C)
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	for (int i = 0; i < 4; i++) {
		float temp;
		temp = m_whoObject->transMatrix[i][0] * C - m_whoObject->transMatrix[i][1] * S;
		m_whoObject->transMatrix[i][1] = m_whoObject->transMatrix[i][0] * S + m_whoObject->transMatrix[i][1] * C;
		m_whoObject->transMatrix[i][0] = temp;
	}
}

void CDrawScene::TransSpaceObject()
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();
	// First Trans Object according to its matrix
	for (int i = 0; i < m_whoObject->polyCount; i++) {
		for (int j = 0; j < m_whoObject->objectSpace[i].polyCount; j++) {
			m_whoObject->objectSpace[i].transObject[j].x =
				m_whoObject->objectSpace[i].polyObject[j].x * m_whoObject->transMatrix[0][0] +
				m_whoObject->objectSpace[i].polyObject[j].y * m_whoObject->transMatrix[1][0] +
				m_whoObject->objectSpace[i].polyObject[j].z * m_whoObject->transMatrix[2][0] +
				m_whoObject->transMatrix[3][0];
			m_whoObject->objectSpace[i].transObject[j].y =
				m_whoObject->objectSpace[i].polyObject[j].x * m_whoObject->transMatrix[0][1] +
				m_whoObject->objectSpace[i].polyObject[j].y * m_whoObject->transMatrix[1][1] +
				m_whoObject->objectSpace[i].polyObject[j].z * m_whoObject->transMatrix[2][1] +
				m_whoObject->transMatrix[3][1];
			m_whoObject->objectSpace[i].transObject[j].z =
				m_whoObject->objectSpace[i].polyObject[j].x * m_whoObject->transMatrix[0][2] +
				m_whoObject->objectSpace[i].polyObject[j].y * m_whoObject->transMatrix[1][2] +
				m_whoObject->objectSpace[i].polyObject[j].z * m_whoObject->transMatrix[2][2] +
				m_whoObject->transMatrix[3][2];
		}
	}
}

void CDrawScene::DrawSpaceObject()
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	if (m_objNumber < 3)
		glColor3f(glObjColor[m_objNumber][0],
			glObjColor[m_objNumber][1],
			glObjColor[m_objNumber][2]);
	else
		glColor3f(glObjColor[BALL][0],
			glObjColor[BALL][1],
			glObjColor[BALL][2]);

	for (int i = 0; i < m_whoObject->polyCount; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_whoObject->objectSpace[i].polyCount; j++)
			glVertex3f(m_whoObject->objectSpace[i].transObject[j].x,
				m_whoObject->objectSpace[i].transObject[j].y,
				m_whoObject->objectSpace[i].transObject[j].z);
		glEnd();
	}
}

void CDrawScene::projectSpaceObject()
{
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();
	// Project object to XOY plane
	for (int i = 0; i < m_whoObject->polyCount; i++) {
		if (m_whoObject->objectSpace[i].polyVisible) {
			for (int j = 0; j < m_whoObject->objectSpace[i].clipCount; j++) {
				m_whoObject->objectSpace[i].projectObject[j].x =
					-(m_whoObject->objectSpace[i].clipObject[j].x - pDoc->eyeX) /
					(m_whoObject->objectSpace[i].clipObject[j].z - pDoc->eyeZ) *
					pDoc->eyeZ + pDoc->eyeX;
				m_whoObject->objectSpace[i].projectObject[j].y =
					-(m_whoObject->objectSpace[i].clipObject[j].y - pDoc->eyeY) /
					(m_whoObject->objectSpace[i].clipObject[j].z - pDoc->eyeZ) *
					pDoc->eyeZ + pDoc->eyeY;
				m_whoObject->objectSpace[i].projectObject[j].z = 0.0f;
			}
		}
	}
	// Then draw project result on XOY project plane.
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < m_whoObject->polyCount; i++) {
		if (m_whoObject->objectSpace[i].polyVisible) {
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j < m_whoObject->objectSpace[i].clipCount; j++)
				glVertex3f(m_whoObject->objectSpace[i].projectObject[j].x,
					m_whoObject->objectSpace[i].projectObject[j].y,
					m_whoObject->objectSpace[i].projectObject[j].z);
			glEnd();
		}
	}
}

// Remove Object's Back Plane
void CDrawScene::pRemoveBackFace()
{
	int      i;
	float    eDx, eDy, eDz;
	float    nDx, nDy, nDz, pv;
	float    dx1, dy1, dz1, dx2, dy2, dz2;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	for (i = 0; i < m_whoObject->polyCount; i++) {

		m_whoObject->objectSpace[i].polyVisible = false;

		eDx = m_whoObject->objectSpace[i].transObject[0].x - pDoc->eyeX;
		eDy = m_whoObject->objectSpace[i].transObject[0].y - pDoc->eyeY;
		eDz = m_whoObject->objectSpace[i].transObject[0].z - pDoc->eyeZ;

		dx1 = m_whoObject->objectSpace[i].transObject[2].x -
			m_whoObject->objectSpace[i].transObject[1].x;
		dy1 = m_whoObject->objectSpace[i].transObject[2].y -
			m_whoObject->objectSpace[i].transObject[1].y;
		dz1 = m_whoObject->objectSpace[i].transObject[2].z -
			m_whoObject->objectSpace[i].transObject[1].z;
		dx2 = m_whoObject->objectSpace[i].transObject[1].x -
			m_whoObject->objectSpace[i].transObject[0].x;
		dy2 = m_whoObject->objectSpace[i].transObject[1].y -
			m_whoObject->objectSpace[i].transObject[0].y;
		dz2 = m_whoObject->objectSpace[i].transObject[1].z -
			m_whoObject->objectSpace[i].transObject[0].z;
		nDx = dy1 * dz2 - dy2 * dz1;
		nDy = dx2 * dz1 - dx1 * dz2;
		nDz = dx1 * dy2 - dx2 * dy1;

		pv = nDx * eDx + nDy * eDy + nDz * eDz;
		if (pv >  0) m_whoObject->objectSpace[i].polyVisible = 1;
		else         m_whoObject->objectSpace[i].polyVisible = 0;
	}
}

// Bellow Functions Clip Space Object after Trans
void CDrawScene::pClipSpaceObject()
{
	int  i, j, k, pv, spCross;
	int  inCount, outCount;
	Gpoint_t polyClip[10];
	float sx, sy, sz, px, py, pz;
	BOOL objectVisible, clip;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	objectVisible = false;
	for (k = 0; k < m_whoObject->polyCount; k++) {
		if (m_whoObject->objectSpace[k].polyVisible) {  // Polygon visible
			inCount = m_whoObject->objectSpace[k].polyCount;
			for (i = 0; i < inCount; i++) {
				polyClip[i].x = m_whoObject->objectSpace[k].transObject[i].x;
				polyClip[i].y = m_whoObject->objectSpace[k].transObject[i].y;
				polyClip[i].z = m_whoObject->objectSpace[k].transObject[i].z;
			}

			clip = true;
			for (i = 0; clip && i < 6; i++) {  // Clip Box 6 Pane Cube 
				outCount = 0;
				sx = polyClip[0].x;
				sy = polyClip[0].y;
				sz = polyClip[0].z;
				pv = pVisible(sx, sy, sz, i);
				if (pv) outPut(sx, sy, sz, &outCount, m_whoObject->objectSpace[k].clipObject);
				for (j = 1; j < inCount; j++) {
					px = polyClip[j].x;
					py = polyClip[j].y;
					pz = polyClip[j].z;
					spCross = pLineCrossPane(sx, sy, sz, px, py, pz, i);
					if (spCross) pLineInterSectPane(sx, sy, sz, px, py, pz, i,
						&outCount, m_whoObject->objectSpace[k].clipObject);
					sx = px;
					sy = py;
					sz = pz;
					pv = pVisible(sx, sy, sz, i);
					if (pv) outPut(sx, sy, sz, &outCount, m_whoObject->objectSpace[k].clipObject);
				}

				inCount = outCount;
				if (outCount == 0) clip = false;

				for (j = 0; clip && j < outCount; j++) {
					polyClip[j].x = m_whoObject->objectSpace[k].clipObject[j].x;
					polyClip[j].y = m_whoObject->objectSpace[k].clipObject[j].y;
					polyClip[j].z = m_whoObject->objectSpace[k].clipObject[j].z;
				}

				if (polyClip[0].x != polyClip[outCount - 1].x &&
					polyClip[0].x != polyClip[outCount - 1].y &&
					polyClip[0].x != polyClip[outCount - 1].z) {
					polyClip[outCount].x = polyClip[0].x;
					polyClip[outCount].y = polyClip[0].y;
					polyClip[outCount].z = polyClip[0].z;
					inCount++;
				}
			}

			if (outCount != 0) objectVisible = true;
			m_whoObject->objectSpace[k].clipCount = outCount;
		}
		else m_whoObject->objectSpace[k].clipCount = 0;
	}
	m_whoObject->objectVisible = objectVisible;
}

int CDrawScene::outPut(float x, float y, float z, int *outCount, Gpoint_p polyClip)
{
	if (*outCount == 8) return 0;
	polyClip[*outCount].x = x;
	polyClip[*outCount].y = y;
	polyClip[*outCount].z = z;
	(*outCount)++;
	return 1;
}

int CDrawScene::pVisible(float x, float y, float z, int pane)
{
	int visible = 0;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	switch (pane) {
	case 0: // winLpane
		if (x >= pDoc->winLpaneM * z + pDoc->winLx)  visible = 1;
		break;
	case 1: // winRpane
		if (x <= pDoc->winRpaneM * z + pDoc->winRx)  visible = 1;
		break;
	case 2: // winTpane
		if (y <= pDoc->winTpaneM * z + pDoc->winRy)  visible = 1;
		break;
	case 3: // winBpane
		if (y >= pDoc->winBpaneM * z + pDoc->winLy)  visible = 1;
		break;
	case 4: // zFront
		if (z <= pDoc->zFront) visible = 1;
		break;
	case 5: // zBack
		if (z >= pDoc->zBack) visible = 1;
		break;
	}
	return visible;
}

int CDrawScene::pLineCrossPane(float sx, float sy, float sz,
	float px, float py, float pz, int pane)
{
	int sv, pv;

	sv = pVisible(sx, sy, sz, pane);
	pv = pVisible(px, py, pz, pane);

	if (sv && !pv || !sv && pv) return 1;

	return 0;
}

int CDrawScene::pLineInterSectPane(float sx, float sy, float sz,
	float px, float py, float pz,
	int pane, int *outCount, Gpoint_p polyClip)
{
	float u, ix, iy, iz;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	switch (pane) {
	case 0: // winLpane
		u = sz * pDoc->winLpaneM + pDoc->winLx - sx;
		u = u / (px - sx - pDoc->winLpaneM * (pz - sz));
		ix = (px - sx) * u + sx;
		iy = (py - sy) * u + sy;
		iz = (pz - sz) * u + sz;
		break;
	case 1: // winRpane
		u = sz * pDoc->winRpaneM + pDoc->winRx - sx;
		u = u / (px - sx - pDoc->winRpaneM * (pz - sz));
		ix = (px - sx) * u + sx;
		iy = (py - sy) * u + sy;
		iz = (pz - sz) * u + sz;
		break;
	case 2: // winTpane
		u = sz * pDoc->winTpaneM + pDoc->winRy - sy;
		u = u / (py - sy - pDoc->winTpaneM * (pz - sz));
		ix = (px - sx) * u + sx;
		iy = (py - sy) * u + sy;
		iz = (pz - sz) * u + sz;
		break;
	case 3: // winBpane
		u = sz * pDoc->winBpaneM + pDoc->winLy - sy;
		u = u / (py - sy - pDoc->winBpaneM * (pz - sz));
		ix = (px - sx) * u + sx;
		iy = (py - sy) * u + sy;
		iz = (pz - sz) * u + sz;
		break;
	case 4: // zFront
		iz = pDoc->zFront;
		u = (pDoc->zFront - sz) / (pz - sz);
		ix = (px - sx) * u + sx;
		iy = (py - sy) * u + sy;
		break;
	case 5: // xBack
		iz = pDoc->zBack;
		u = (pDoc->zBack - sz) / (pz - sz);
		ix = (px - sx) * u + sx;
		iy = (py - sy) * u + sy;
		break;
	}

	outPut(ix, iy, iz, outCount, polyClip);
	return 1;
}

void CDrawScene::pObjectCenter()              //求中心点
{
	int i, j, polyCount;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	xMin = yMin = zMin = 3000.0f;
	xMax = yMax = zMax = -3000.0f;
	for (i = 0; i < m_whoObject->polyCount; i++) {
		polyCount = m_whoObject->objectSpace[i].polyCount;
		for (j = 0; j < polyCount; j++) {
			if (m_whoObject->objectSpace[i].transObject[j].x < xMin)
				xMin = m_whoObject->objectSpace[i].transObject[j].x;
			if (m_whoObject->objectSpace[i].transObject[j].y < yMin)
				yMin = m_whoObject->objectSpace[i].transObject[j].y;
			if (m_whoObject->objectSpace[i].transObject[j].z < zMin)
				zMin = m_whoObject->objectSpace[i].transObject[j].z;
			if (m_whoObject->objectSpace[i].transObject[j].x > xMax)
				xMax = m_whoObject->objectSpace[i].transObject[j].x;
			if (m_whoObject->objectSpace[i].transObject[j].y > yMax)
				yMax = m_whoObject->objectSpace[i].transObject[j].y;
			if (m_whoObject->objectSpace[i].transObject[j].z > zMax)
				zMax = m_whoObject->objectSpace[i].transObject[j].z;
		}
	}
	xCenter = m_whoObject->objCenter.x = (xMax + xMin) / 2.0f;
	yCenter = m_whoObject->objCenter.y = (yMax + yMin) / 2.0f;
	zCenter = m_whoObject->objCenter.z = (zMax + zMin) / 2.0f;
}

void CDrawScene::pLightSpaceBall()
{
	float nX, nY, nZ, Distance;
	float h1, h2, hX, hY, hZ, eyeX, eyeY, eyeZ, lightX, lightY, lightZ;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	pObjectCenter();

	for (int i = 0; i < m_whoObject->polyCount; i++) {
		if (m_whoObject->objectSpace[i].polyVisible)
			for (int j = 0; j < m_whoObject->objectSpace[i].clipCount; j++)
				m_whoObject->objectSpace[i].polyLight[j] = pDoc->ambientLight * pDoc->reflectPara;
	}

	for (int k = 0; k < pDoc->nrLights; k++) {
		for (int i = 0; i < m_whoObject->polyCount; i++) {
			if (m_whoObject->objectSpace[i].polyVisible) {
				for (int j = 0; j < m_whoObject->objectSpace[i].clipCount; j++) {
					eyeX = pDoc->eyeX - m_whoObject->objectSpace[i].clipObject[j].x;
					eyeY = pDoc->eyeY - m_whoObject->objectSpace[i].clipObject[j].y;
					eyeZ = pDoc->eyeZ - m_whoObject->objectSpace[i].clipObject[j].z;
					h1 = sqrtf(eyeX * eyeX + eyeY * eyeY + eyeZ * eyeZ);
					eyeX = eyeX / h1;    eyeY = eyeY / h1;    eyeZ = eyeZ / h1;

					lightX = pDoc->lightX[k] - m_whoObject->objectSpace[i].clipObject[j].x;
					lightY = pDoc->lightY[k] - m_whoObject->objectSpace[i].clipObject[j].y;
					lightZ = pDoc->lightZ[k] - m_whoObject->objectSpace[i].clipObject[j].z;
					h2 = Distance = sqrtf(lightX * lightX + lightY * lightY + lightZ * lightZ);
					lightX = lightX / h2;    lightY = lightY / h2;    lightZ = lightZ / h2;

					hX = eyeX + lightX;      hY = eyeY + lightY;      hZ = eyeZ + lightZ;
					h1 = sqrtf(hX * hX + hY * hY + hZ * hZ);
					hX = hX / h1;    hY = hY / h1;    hZ = hZ / h1;

					nX = m_whoObject->objectSpace[i].clipObject[j].x - m_whoObject->objCenter.x;
					nY = m_whoObject->objectSpace[i].clipObject[j].y - m_whoObject->objCenter.y;
					nZ = m_whoObject->objectSpace[i].clipObject[j].z - m_whoObject->objCenter.z;
					h1 = sqrtf(nX * nX + nY * nY + nZ * nZ);
					nX = nX / h1;    nY = nY / h1;    nZ = nZ / h1;

					h2 = nX * hX + nY * hY + nZ * hZ;
					h1 = nX * lightX + nY * lightY + nZ * lightZ;
					m_whoObject->objectSpace[i].polyLight[j] +=
						(pDoc->pointLight * h1 * pDoc->reflectPara +
							pDoc->pointLight * (float)pow(h2, pDoc->shinePara) * pDoc->reflectPara) /
							(Distance + 10);
					if (m_whoObject->objectSpace[i].polyLight[j] < 0)
						m_whoObject->objectSpace[i].polyLight[j] = 0;
				}
			}
		}
		Distance = 0.0f;
	}
}

void CDrawScene::pLightSpaceObject()
{
	float nX, nY, nZ, Distance;
	float dx1, dy1, dz1, dx2, dy2, dz2;
	float h1, h2, hX, hY, hZ, eyeX, eyeY, eyeZ, lightX, lightY, lightZ;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)m_pView->GetDocument();

	for (int k = 0; k < pDoc->nrLights; k++) {
		for (int i = 0; i < m_whoObject->polyCount; i++) {
			if (m_whoObject->objectSpace[i].polyVisible) {
				for (int j = 0; j < m_whoObject->objectSpace[i].clipCount; j++) {
					eyeX = pDoc->eyeX - m_whoObject->objectSpace[i].clipObject[j].x;
					eyeY = pDoc->eyeY - m_whoObject->objectSpace[i].clipObject[j].y;
					eyeZ = pDoc->eyeZ - m_whoObject->objectSpace[i].clipObject[j].z;
					h1 = sqrtf(eyeX * eyeX + eyeY * eyeY + eyeZ * eyeZ);
					eyeX = eyeX / h1;    eyeY = eyeY / h1;    eyeZ = eyeZ / h1;

					lightX = pDoc->lightX[k] - m_whoObject->objectSpace[i].clipObject[j].x;
					lightY = pDoc->lightY[k] - m_whoObject->objectSpace[i].clipObject[j].y;
					lightZ = pDoc->lightZ[k] - m_whoObject->objectSpace[i].clipObject[j].z;
					h2 = Distance = sqrtf(lightX * lightX + lightY * lightY + lightZ * lightZ);
					lightX = lightX / h2;    lightY = lightY / h2;    lightZ = lightZ / h2;

					hX = eyeX + lightX;      hY = eyeY + lightY;      hZ = eyeZ + lightZ;
					h1 = sqrtf(hX * hX + hY * hY + hZ * hZ);
					hX = hX / h1;    hY = hY / h1;    hZ = hZ / h1;
					/*
					fPoint = j+1;
					if (fPoint >= m_whoObject->objectSpace[i].clipCount) fPoint = 0;
					bPoint = j-1;
					if (bPoint < 0) bPoint = m_whoObject->objectSpace[i].clipCount-1;

					dx1 = m_whoObject->objectSpace[i].clipObject[fPoint].x -
					m_whoObject->objectSpace[i].clipObject[j].x;
					dy1 = m_whoObject->objectSpace[i].clipObject[fPoint].y -
					m_whoObject->objectSpace[i].clipObject[j].y;
					dz1 = m_whoObject->objectSpace[i].clipObject[fPoint].z -
					m_whoObject->objectSpace[i].clipObject[j].z;
					dx2 = m_whoObject->objectSpace[i].clipObject[j].x -
					m_whoObject->objectSpace[i].clipObject[bPoint].x;
					dy2 = m_whoObject->objectSpace[i].clipObject[j].y -
					m_whoObject->objectSpace[i].clipObject[bPoint].y;
					dz2 = m_whoObject->objectSpace[i].clipObject[j].z -
					m_whoObject->objectSpace[i].clipObject[bPoint].z;
					*/
					dx1 = m_whoObject->objectSpace[i].clipObject[2].x -
						m_whoObject->objectSpace[i].clipObject[1].x;
					dy1 = m_whoObject->objectSpace[i].clipObject[2].y -
						m_whoObject->objectSpace[i].clipObject[1].y;
					dz1 = m_whoObject->objectSpace[i].clipObject[2].z -
						m_whoObject->objectSpace[i].clipObject[1].z;
					dx2 = m_whoObject->objectSpace[i].clipObject[1].x -
						m_whoObject->objectSpace[i].clipObject[0].x;
					dy2 = m_whoObject->objectSpace[i].clipObject[1].y -
						m_whoObject->objectSpace[i].clipObject[0].y;
					dz2 = m_whoObject->objectSpace[i].clipObject[1].z -
						m_whoObject->objectSpace[i].clipObject[0].z;

					nX = -(dy1 * dz2 - dy2 * dz1);
					nY = -(dx2 * dz1 - dx1 * dz2);
					nZ = -(dx1 * dy2 - dx2 * dy1);

					h1 = sqrtf(nX * nX + nY * nY + nZ * nZ);
					nX = nX / h1;    nY = nY / h1;    nZ = nZ / h1;

					h2 = nX * hX + nY * hY + nZ * hZ;
					h1 = nX * lightX + nY * lightY + nZ * lightZ;
					m_whoObject->objectSpace[i].polyLight[j] =
						pDoc->ambientLight * pDoc->reflectPara +
						(pDoc->pointLight * h1 * pDoc->reflectPara +
							pDoc->pointLight * (float)pow(h2, pDoc->shinePara) * pDoc->reflectPara) /
							(Distance + 10);
					if (m_whoObject->objectSpace[i].polyLight[j] < 0)
						m_whoObject->objectSpace[i].polyLight[j] = 0;
				}
			}
		}
	}
}