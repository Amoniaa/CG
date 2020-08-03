
// cg2019WJY3DTransDoc.h : Ccg2019WJY3DTransDoc 类的接口
//


#pragma once

#include "TypeDefine.h"

// Trans Object Select No.
#define BALL         0
#define CUBE         1
#define TRIANGLE     2
#define LIGHT0       3
#define LIGHT1       4
#define EYE          5
#define SCENE        6

#define SPACEOBJECTS 16

class Ccg2019WJY3DTransDoc : public CDocument
{
protected: // 仅从序列化创建
	Ccg2019WJY3DTransDoc();
	DECLARE_DYNCREATE(Ccg2019WJY3DTransDoc)

	// 特性
public:
	float m_translateVector[3];                // Scene Translation parameters.
	float m_xAngle, m_yAngle, m_zAngle;        // Scene Rotation parameters.

	BOOL m_selfRotate;
	int m_transDir, m_transMode, m_transSelect;   // Transformation Direction, Mode and Object Select Number.

	float ballRadius;
	float zBack, zFront;
	float eyeX, eyeY, eyeZ;
	float winLx, winLy, winRx, winRy;    // window on XOY project plane

	float clipCubeBackLB[3], clipCubeBackRB[3];
	float clipCubeBackLT[3], clipCubeBackRT[3];
	float clipCubeFrontLB[3], clipCubeFrontRB[3];
	float clipCubeFrontLT[3], clipCubeFrontRT[3];
	float winLpaneM, winRpaneM, winBpaneM, winTpaneM;

	int      m_objectNum;
	Object_t m_spaceObjects[SPACEOBJECTS];       // Space Object Buffer

	BOOL  m_onLight, m_onShade;
	int   nrLights, shinePara;
	float lightX[5], lightY[5], lightZ[5];
	float ambientLight, pointLight, reflectPara, refractPara;

	int   m_polygonNumbers;
	float m_DrawSceneRunTime;


	float m_CaculateRunTime;
	// 操作
public:
	void pCreateCube();				  //绘制三维空间区域中的球
	void pCreateBallWithQuard(); 	  //绘制三维空间区域中的正方体
	void pCreateClipBox();			  //绘制三维空间区域中的裁剪立方体
	// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// 实现
public:
	virtual ~Ccg2019WJY3DTransDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
