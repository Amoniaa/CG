
// cg2019WJY3DTransDoc.h : Ccg2019WJY3DTransDoc ��Ľӿ�
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
protected: // �������л�����
	Ccg2019WJY3DTransDoc();
	DECLARE_DYNCREATE(Ccg2019WJY3DTransDoc)

	// ����
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
	// ����
public:
	void pCreateCube();				  //������ά�ռ������е���
	void pCreateBallWithQuard(); 	  //������ά�ռ������е�������
	void pCreateClipBox();			  //������ά�ռ������еĲü�������
	// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// ʵ��
public:
	virtual ~Ccg2019WJY3DTransDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
