
// cg2019WJY3DTransDoc.cpp : Ccg2019WJY3DTransDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cg2019WJY3DTrans.h"
#endif

#include "cg2019WJY3DTransDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ccg2019WJY3DTransDoc

IMPLEMENT_DYNCREATE(Ccg2019WJY3DTransDoc, CDocument)

BEGIN_MESSAGE_MAP(Ccg2019WJY3DTransDoc, CDocument)
END_MESSAGE_MAP()


// Ccg2019WJY3DTransDoc 构造/析构

Ccg2019WJY3DTransDoc::Ccg2019WJY3DTransDoc()
{
	// Scene Translate and Rotate parameters.
	m_xAngle = 10.0f;
	m_yAngle = -40.0f;
	m_zAngle = 0.0f;
	m_translateVector[0] = -0.0f;
	m_translateVector[1] = -0.3f;
	m_translateVector[2] = -5.0f;

	m_transDir = 0;           // 0: -X  1: +X  2: -Y 3: +Y  4: -Z  5: +Z  
	m_transMode = 0;          // 0: translate  1: rotate
	m_transSelect = 0;        // 0:BALL  1:CUBIC  2:Triangle  3:Light0  4:Light1  5:eye  6:Scene

							  // Viewpoint
	eyeX = 1.5f;          eyeY = 1.5f;         eyeZ = 5.0f;

	// Window On XOY plane
	winLx = 1.0f;  winLy = 1.0f;
	winRx = 2.0f;  winRy = 2.0f;

	zBack = -5.0f;
	zFront = -2.0f;
	ballRadius = 0.5f;

	m_onLight = FALSE;
	m_onShade = FALSE;
	m_selfRotate = TRUE;

	nrLights = 2;
	shinePara = 36;
	pointLight = 1.5f;
	ambientLight = 0.8f;
	reflectPara = 0.3f;

	lightX[0] = 1.0f;    lightY[0] = 3.0f;    lightZ[0] = -1.5f;
	lightX[1] = 2.0f;    lightY[1] = 3.5f;    lightZ[1] = -1.0f;

	// Initial Space Object Buffer
	for (int i = 0; i < SPACEOBJECTS; i++) {
		m_spaceObjects[i].polyCount = 0;
		m_spaceObjects[i].objectVisible = FALSE;
	}

	m_objectNum = 2;
	m_polygonNumbers = 0;
	m_DrawSceneRunTime = 0.0f;

	m_polygonNumbers = 0;

	pCreateClipBox();

	pCreateCube();
	pCreateBallWithQuard();
}

Ccg2019WJY3DTransDoc::~Ccg2019WJY3DTransDoc()
{
}

BOOL Ccg2019WJY3DTransDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Ccg2019WJY3DTransDoc 序列化

void Ccg2019WJY3DTransDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void Ccg2019WJY3DTransDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void Ccg2019WJY3DTransDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void Ccg2019WJY3DTransDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Ccg2019WJY3DTransDoc 诊断

#ifdef _DEBUG
void Ccg2019WJY3DTransDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Ccg2019WJY3DTransDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Ccg2019WJY3DTransDoc 命令
void Ccg2019WJY3DTransDoc::pCreateBallWithQuard()
{
	//int i, j, polyCount;
	//float u, winCx, winCy, winCz;
	//Gpoint_t objectPoint[64][64];
	//float dh, angle, radius, height;
	//float BallCx, BallCy, BallCz;

	//// Calculate the ball's center point
	//winCx = (winLx + winRx) / 2.0f;
	//winCy = (winLy + winRy) / 2.0f;
	//winCz = 0.0f;

	//BallCz = (zFront + zBack) / 2.0f + 1.0f;
	//u = (BallCz - eyeZ) / (winCz - eyeZ);
	//BallCx = u * (winCx - eyeX) + eyeX + 0.1f;
	//BallCy = u * (winCy - eyeY) + eyeY;

	//radius = 0.0001f;
	//height = ballRadius;
	//dh = -ballRadius / 10;
	//angle = 3.1415926f / 20;
	//for (i = 0; i <= 20; i++) {
	//	if (i == 20) radius = 0.0001f;
	//	for (j = 0; j <= 40; j++) {
	//		objectPoint[i][j].y = height + BallCy;
	//		objectPoint[i][j].x = radius*sinf(j * angle) + BallCx;
	//		objectPoint[i][j].z = radius*cosf(j * angle) + BallCz;
	//	}
	//	height = height + dh;
	//	radius = sqrtf(ballRadius*ballRadius - height*height);
	//	if (fabs(radius) < 0.0001f)  radius = 0.0001f;
	//}

	//polyCount = 0;
	//Object_p ballObject = &m_spaceObjects[BALL];

	//for (i = 0; i < 20; i++) {
	//	for (j = 0; j < 40; j++) {
	//		ballObject->objectSpace[polyCount].polyCount = 5;
	//		ballObject->objectSpace[polyCount].polyObject[0].x = objectPoint[i][j].x;
	//		ballObject->objectSpace[polyCount].polyObject[0].y = objectPoint[i][j].y;
	//		ballObject->objectSpace[polyCount].polyObject[0].z = objectPoint[i][j].z;
	//		ballObject->objectSpace[polyCount].polyObject[1].x = objectPoint[i + 1][j].x;
	//		ballObject->objectSpace[polyCount].polyObject[1].y = objectPoint[i + 1][j].y;
	//		ballObject->objectSpace[polyCount].polyObject[1].z = objectPoint[i + 1][j].z;
	//		ballObject->objectSpace[polyCount].polyObject[2].x = objectPoint[i + 1][j + 1].x;
	//		ballObject->objectSpace[polyCount].polyObject[2].y = objectPoint[i + 1][j + 1].y;
	//		ballObject->objectSpace[polyCount].polyObject[2].z = objectPoint[i + 1][j + 1].z;
	//		ballObject->objectSpace[polyCount].polyObject[3].x = objectPoint[i][j + 1].x;
	//		ballObject->objectSpace[polyCount].polyObject[3].y = objectPoint[i][j + 1].y;
	//		ballObject->objectSpace[polyCount].polyObject[3].z = objectPoint[i][j + 1].z;
	//		ballObject->objectSpace[polyCount].polyObject[4].x = objectPoint[i][j].x;
	//		ballObject->objectSpace[polyCount].polyObject[4].y = objectPoint[i][j].y;
	//		ballObject->objectSpace[polyCount].polyObject[4].z = objectPoint[i][j].z;
	//		polyCount = polyCount + 1;
	//	}
	//}
	//ballObject->objCenter.x = BallCx;
	//ballObject->objCenter.y = BallCy;
	//ballObject->objCenter.z = BallCz;
	//ballObject->polyCount = polyCount;

	//// ----- Scene Description -----
	//for (i = 0; i < 4; i++)
	//	for (j = 0; j < 3; j++)
	//		if (i == j) ballObject->transMatrix[i][j] = 1.0;
	//		else        ballObject->transMatrix[i][j] = 0.0;

	//		m_polygonNumbers += polyCount;    // Count polygon numbers.
	
	int i, j, polyCount;
	float u, winCx, winCy, winCz;
	Gpoint_t objectPoint[64][64];
	float dh, angle, radius, height;
	float BallCx, BallCy, BallCz;

	// Calculate the ball's center point
	winCx = (winLx + winRx) / 2.0f;
	winCy = (winLy + winRy) / 2.0f;
	winCz = 0.0f;

	BallCz = (zFront + zBack) / 2.0f + 1.0f;
	u = (BallCz - eyeZ) / (winCz - eyeZ);
	BallCx = u * (winCx - eyeX) + eyeX + 0.1f;
	BallCy = u * (winCy - eyeY) + eyeY;             //(1.6,1.5,-2.5)

	radius = 0.0001f;
	height = ballRadius;                            //0.5
	dh = -ballRadius / 10;                          //-0.05
	angle = 3.1415926f / 10;
	for (i = 0; i <= 20; i++) {
		if (i == 20) radius = 0.0001f;
		for (j = 0; j <= 20; j++) {
			objectPoint[i][j].y = height + BallCy;
			objectPoint[i][j].x = radius * sinf(j * angle) + BallCx;
			objectPoint[i][j].z = radius * cosf(j * angle) + BallCz;
		}
		height = height + dh;
		radius = sqrtf(ballRadius * ballRadius - height * height);
		if (fabs(radius) < 0.0001f)  radius = 0.0001f;
	}

	polyCount = 0;
	Object_p ballObject = &m_spaceObjects[BALL];
	for (i = 0; i < 20; i++) {
		for (j = 0; j < 20; j++) {

			ballObject->objectSpace[polyCount].polyCount = 4;
			ballObject->objectSpace[polyCount].polyObject[0].x = objectPoint[i][j].x;
			ballObject->objectSpace[polyCount].polyObject[0].y = objectPoint[i][j].y;
			ballObject->objectSpace[polyCount].polyObject[0].z = objectPoint[i][j].z;
			ballObject->objectSpace[polyCount].polyObject[1].x = objectPoint[i + 1][j + 1].x;
			ballObject->objectSpace[polyCount].polyObject[1].y = objectPoint[i + 1][j + 1].y;
			ballObject->objectSpace[polyCount].polyObject[1].z = objectPoint[i + 1][j + 1].z;
			ballObject->objectSpace[polyCount].polyObject[2].x = objectPoint[i][j + 1].x;
			ballObject->objectSpace[polyCount].polyObject[2].y = objectPoint[i][j + 1].y;
			ballObject->objectSpace[polyCount].polyObject[2].z = objectPoint[i][j + 1].z;
			ballObject->objectSpace[polyCount].polyObject[3].x = objectPoint[i][j].x;
			ballObject->objectSpace[polyCount].polyObject[3].y = objectPoint[i][j].y;
			ballObject->objectSpace[polyCount].polyObject[3].z = objectPoint[i][j].z;
			polyCount++;
			ballObject->objectSpace[polyCount].polyCount = 4;
			ballObject->objectSpace[polyCount].polyObject[0].x = objectPoint[i][j].x;
			ballObject->objectSpace[polyCount].polyObject[0].y = objectPoint[i][j].y;
			ballObject->objectSpace[polyCount].polyObject[0].z = objectPoint[i][j].z;
			ballObject->objectSpace[polyCount].polyObject[1].x = objectPoint[i + 1][j].x;
			ballObject->objectSpace[polyCount].polyObject[1].y = objectPoint[i + 1][j].y;
			ballObject->objectSpace[polyCount].polyObject[1].z = objectPoint[i + 1][j].z;
			ballObject->objectSpace[polyCount].polyObject[2].x = objectPoint[i + 1][j + 1].x;
			ballObject->objectSpace[polyCount].polyObject[2].y = objectPoint[i + 1][j + 1].y;
			ballObject->objectSpace[polyCount].polyObject[2].z = objectPoint[i + 1][j + 1].z;
			ballObject->objectSpace[polyCount].polyObject[3].x = objectPoint[i][j].x;
			ballObject->objectSpace[polyCount].polyObject[3].y = objectPoint[i][j].y;
			ballObject->objectSpace[polyCount].polyObject[3].z = objectPoint[i][j].z;

			polyCount = polyCount + 1;
		}
	}
	ballObject->objCenter.x = BallCx;
	ballObject->objCenter.y = BallCy;
	ballObject->objCenter.z = BallCz;
	ballObject->polyCount = polyCount;

	// ----- Scene Description -----
	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++)
			if (i == j) ballObject->transMatrix[i][j] = 1.0;
			else        ballObject->transMatrix[i][j] = 0.0;

			m_polygonNumbers += polyCount;    // Count polygon numbers.
}

void Ccg2019WJY3DTransDoc::pCreateCube()

{
	// Create a CUBE .....
	float u, winCx, winCy, winCz;
	float radius = 0.2f, CubeLx, CubeLy, CubeLz;

	// Calculate the cube's center point
	winCx = (winLx + winRx) / 2.0f;
	winCy = (winLy + winRy) / 2.0f;
	winCz = 0.0f;

	CubeLz = (zFront + zBack) / 2.0f + 1.0f;
	u = (CubeLz - eyeZ) / (winCz - eyeZ);
	CubeLx = u * (winCx - eyeX) + eyeX + 0.3f;
	CubeLy = u * (winCy - eyeY) + eyeY;

	Object_p cubeObject = &m_spaceObjects[CUBE];
	// Left Plane
	cubeObject->objectSpace[0].polyCount = 5;
	cubeObject->objectSpace[0].polyObject[0].x = CubeLx + 0.0f;
	cubeObject->objectSpace[0].polyObject[0].y = CubeLy + 0.0f;
	cubeObject->objectSpace[0].polyObject[0].z = CubeLz + 0.0f;
	cubeObject->objectSpace[0].polyObject[1].x = CubeLx + 0.0f;
	cubeObject->objectSpace[0].polyObject[1].y = CubeLy + 0.0f;
	cubeObject->objectSpace[0].polyObject[1].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[0].polyObject[2].x = CubeLx + 0.0f;
	cubeObject->objectSpace[0].polyObject[2].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[0].polyObject[2].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[0].polyObject[3].x = CubeLx + 0.0f;
	cubeObject->objectSpace[0].polyObject[3].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[0].polyObject[3].z = CubeLz + 0.0f;
	cubeObject->objectSpace[0].polyObject[4].x = CubeLx + 0.0f;
	cubeObject->objectSpace[0].polyObject[4].y = CubeLy + 0.0f;
	cubeObject->objectSpace[0].polyObject[4].z = CubeLz + 0.0f;
	// Right Plane
	cubeObject->objectSpace[1].polyCount = 5;
	cubeObject->objectSpace[1].polyObject[0].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[1].polyObject[0].y = CubeLy + 0.0f;
	cubeObject->objectSpace[1].polyObject[0].z = CubeLz + 0.0f;
	cubeObject->objectSpace[1].polyObject[1].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[1].polyObject[1].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[1].polyObject[1].z = CubeLz + 0.0f;
	cubeObject->objectSpace[1].polyObject[2].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[1].polyObject[2].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[1].polyObject[2].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[1].polyObject[3].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[1].polyObject[3].y = CubeLy + 0.0f;
	cubeObject->objectSpace[1].polyObject[3].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[1].polyObject[4].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[1].polyObject[4].y = CubeLy + 0.0f;
	cubeObject->objectSpace[1].polyObject[4].z = CubeLz + 0.0f;
	// Down Plane
	cubeObject->objectSpace[2].polyCount = 5;
	cubeObject->objectSpace[2].polyObject[0].x = CubeLx + 0.0f;
	cubeObject->objectSpace[2].polyObject[0].y = CubeLy + 0.0f;
	cubeObject->objectSpace[2].polyObject[0].z = CubeLz + 0.0f;
	cubeObject->objectSpace[2].polyObject[1].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[2].polyObject[1].y = CubeLy + 0.0f;
	cubeObject->objectSpace[2].polyObject[1].z = CubeLz + 0.0f;
	cubeObject->objectSpace[2].polyObject[2].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[2].polyObject[2].y = CubeLy + 0.0f;
	cubeObject->objectSpace[2].polyObject[2].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[2].polyObject[3].x = CubeLx + 0.0f;
	cubeObject->objectSpace[2].polyObject[3].y = CubeLy + 0.0f;
	cubeObject->objectSpace[2].polyObject[3].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[2].polyObject[4].x = CubeLx + 0.0f;
	cubeObject->objectSpace[2].polyObject[4].y = CubeLy + 0.0f;
	cubeObject->objectSpace[2].polyObject[4].z = CubeLz + 0.0f;
	// Up Plane
	cubeObject->objectSpace[3].polyCount = 5;
	cubeObject->objectSpace[3].polyObject[0].x = CubeLx + 0.0f;
	cubeObject->objectSpace[3].polyObject[0].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[3].polyObject[0].z = CubeLz + 0.0f;
	cubeObject->objectSpace[3].polyObject[1].x = CubeLx + 0.0f;
	cubeObject->objectSpace[3].polyObject[1].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[3].polyObject[1].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[3].polyObject[2].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[3].polyObject[2].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[3].polyObject[2].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[3].polyObject[3].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[3].polyObject[3].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[3].polyObject[3].z = CubeLz + 0.0f;
	cubeObject->objectSpace[3].polyObject[4].x = CubeLx + 0.0f;
	cubeObject->objectSpace[3].polyObject[4].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[3].polyObject[4].z = CubeLz + 0.0f;
	// Front Plane
	cubeObject->objectSpace[4].polyCount = 5;
	cubeObject->objectSpace[4].polyObject[0].x = CubeLx + 0.0f;
	cubeObject->objectSpace[4].polyObject[0].y = CubeLy + 0.0f;
	cubeObject->objectSpace[4].polyObject[0].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[4].polyObject[1].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[4].polyObject[1].y = CubeLy + 0.0f;
	cubeObject->objectSpace[4].polyObject[1].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[4].polyObject[2].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[4].polyObject[2].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[4].polyObject[2].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[4].polyObject[3].x = CubeLx + 0.0f;
	cubeObject->objectSpace[4].polyObject[3].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[4].polyObject[3].z = CubeLz + 2 * radius;
	cubeObject->objectSpace[4].polyObject[4].x = CubeLx + 0.0f;
	cubeObject->objectSpace[4].polyObject[4].y = CubeLy + 0.0f;
	cubeObject->objectSpace[4].polyObject[4].z = CubeLz + 2 * radius;
	// Back Plane
	cubeObject->objectSpace[5].polyCount = 5;
	cubeObject->objectSpace[5].polyObject[0].x = CubeLx + 0.0f;
	cubeObject->objectSpace[5].polyObject[0].y = CubeLy + 0.0f;
	cubeObject->objectSpace[5].polyObject[0].z = CubeLz + 0.0f;
	cubeObject->objectSpace[5].polyObject[1].x = CubeLx + 0.0f;
	cubeObject->objectSpace[5].polyObject[1].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[5].polyObject[1].z = CubeLz + 0.0f;
	cubeObject->objectSpace[5].polyObject[2].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[5].polyObject[2].y = CubeLy + 2 * radius;
	cubeObject->objectSpace[5].polyObject[2].z = CubeLz + 0.0f;
	cubeObject->objectSpace[5].polyObject[3].x = CubeLx + 2 * radius;
	cubeObject->objectSpace[5].polyObject[3].y = CubeLy + 0.0f;
	cubeObject->objectSpace[5].polyObject[3].z = CubeLz + 0.0f;
	cubeObject->objectSpace[5].polyObject[4].x = CubeLx + 0.0f;
	cubeObject->objectSpace[5].polyObject[4].y = CubeLy + 0.0f;
	cubeObject->objectSpace[5].polyObject[4].z = CubeLz + 0.0f;

	cubeObject->polyCount = 6;
	//	for(int i = 0; i < 6; i++)
	//	  cubeObject->objectSpace[i].polyVisible = true; 

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			if (i == j) cubeObject->transMatrix[i][j] = 1.0;
			else        cubeObject->transMatrix[i][j] = 0.0;
}


void Ccg2019WJY3DTransDoc::pCreateClipBox()
{
	float u;

	clipCubeBackLB[2] = zBack;
	u = (zBack - eyeZ) / (0 - eyeZ);
	clipCubeBackLB[0] = u * (winLx - eyeX) + eyeX;
	clipCubeBackLB[1] = u * (winLy - eyeY) + eyeY;

	clipCubeBackRB[2] = zBack;
	u = (zBack - eyeZ) / (0 - eyeZ);
	clipCubeBackRB[0] = u * (winRx - eyeX) + eyeX;
	clipCubeBackRB[1] = u * (winLy - eyeY) + eyeY;

	clipCubeBackRT[2] = zBack;
	u = (zBack - eyeZ) / (0 - eyeZ);
	clipCubeBackRT[0] = u * (winRx - eyeX) + eyeX;
	clipCubeBackRT[1] = u * (winRy - eyeY) + eyeY;

	clipCubeBackLT[2] = zBack;
	u = (zBack - eyeZ) / (0 - eyeZ);
	clipCubeBackLT[0] = u * (winLx - eyeX) + eyeX;
	clipCubeBackLT[1] = u * (winRy - eyeY) + eyeY;

	clipCubeFrontLB[2] = zFront;
	u = (zFront - eyeZ) / (0 - eyeZ);
	clipCubeFrontLB[0] = u * (winLx - eyeX) + eyeX;
	clipCubeFrontLB[1] = u * (winLy - eyeY) + eyeY;

	clipCubeFrontRB[2] = zFront;
	u = (zFront - eyeZ) / (0 - eyeZ);
	clipCubeFrontRB[0] = u * (winRx - eyeX) + eyeX;
	clipCubeFrontRB[1] = u * (winLy - eyeY) + eyeY;

	clipCubeFrontRT[2] = zFront;
	u = (zFront - eyeZ) / (0 - eyeZ);
	clipCubeFrontRT[0] = u * (winRx - eyeX) + eyeX;
	clipCubeFrontRT[1] = u * (winRy - eyeY) + eyeY;

	clipCubeFrontLT[2] = zFront;
	u = (zFront - eyeZ) / (0 - eyeZ);
	clipCubeFrontLT[0] = u * (winLx - eyeX) + eyeX;
	clipCubeFrontLT[1] = u * (winRy - eyeY) + eyeY;

	winLpaneM = (eyeX - winLx) / eyeZ;
	winRpaneM = (eyeX - winRx) / eyeZ;
	winBpaneM = (eyeY - winLy) / eyeZ;
	winTpaneM = (eyeY - winRy) / eyeZ;

}