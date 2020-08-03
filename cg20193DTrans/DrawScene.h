#pragma once

#include "TypeDefine.h"
#include "cg2019WJY3DTransDoc.h"
#include "cg2019WJY3DTransView.h"


class CDrawScene
{
public:
	CDrawScene();
	~CDrawScene();

	CDrawScene(Ccg2019WJY3DTransView* pView);

	void DrawScene();
	float xCenter, yCenter, zCenter;
private:
	int      m_objNumber;
	Object_p m_whoObject;           // Object pointer
	Ccg2019WJY3DTransView* m_pView;
	float glObjColor[SPACEOBJECTS][3];

	//通过OpenGL实现场景绘制，包括：三维空间的X、Y、Z坐标轴，视线区域，视点，两个光源，此函数在CDrawScene::CDrawScene()中被调用
	void DrawBackground();

	//绘制三维空间场景中的物体：球，立方体在函数CDrawScene::CDrawScene()中被调用，通过for循环语句实现
	void DrawSpaceObject();

	//绘制变换后的三维图形，在函数CDrawScene::CDrawScene()中被调用
	void TransSpaceObject();

	//绘制经过视见变换后的三维空间几何体，在函数CDrawScene::CDrawScene()中被调用
	void projectSpaceObject();

	//用于计算变换矩阵，根据不同选项：-X,+X,-Y,+Y,-Z,+Z,以及不同变换模式：translate，rotate计算出相应的变换矩阵。通过点击交互界面的相关按钮，改变按钮的参数值，在函数CDrawScene::CDrawScene()中被调用。
	void CaculateMatrix();

	//绕X轴旋转变换函数，在函数CaculateMatrix()中被调用
	void rotateX3Dmatrix(float S, float C);

	//绕Y轴旋转变换函数，在函数CaculateMatrix()中被调用
	void rotateY3Dmatrix(float S, float C);

	//绕Z轴旋转变换函数，在函数CaculateMatrix()中被调用
	void rotateZ3Dmatrix(float S, float C);
	
	//平移变换函数，在函数CaculateMatrix()中被调用
	void translate3dMatrix(float dx, float dy, float dz);

	//实现消隐的函数，在函数CDrawScene::CDrawScene()中被调用
	void pRemoveBackFace();

	//以下裁剪算法所要调用的函数，在CDrawScene::pClipSpaceObject()被调用
	void pClipSpaceObject();
	int pVisible(float x, float y, float z, int pane);
	int outPut(float x, float y, float z, int *outCount, Gpoint_p polyClip);
	int pLineCrossPane(float sx, float sy, float sz,
		float px, float py, float pz, int pane);
	int pLineInterSectPane(float sx, float sy, float sz,
		float px, float py, float pz,
		int pane, int *outCount, Gpoint_p polyClip);
	
	void pObjectCenter();     //计算物体的中心点，在pLightSpaceBall()被调用
	void pLightSpaceBall();   //判断如果是球体或者正方体则投影光线
	void pLightSpaceObject(); //如果不是球体或者正方体则无法投影光线
};

