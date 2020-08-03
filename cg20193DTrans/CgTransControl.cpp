// CgTransControl.cpp : 实现文件
//

#include "stdafx.h"
#include "cg2019WJY3DTrans.h"
#include "CgTransControl.h"

#include "cg2019WJY3DTransDoc.h"

// CCgTransControl

IMPLEMENT_DYNCREATE(CCgTransControl, CFormView)

CCgTransControl::CCgTransControl()
	: CFormView(IDD_TRANSCONTROL)

{
	m_transSelect = 0;

	prjPlaneRetZ = NULL;
	prjPlaneDibImage = NULL;

	vcObjColor[BALL] = RGB(255, 0, 0);
	vcObjColor[CUBE] = RGB(255, 255, 0);
	vcObjColor[TRIANGLE] = RGB(0, 0, 255);
}

CCgTransControl::~CCgTransControl()
{
	if (prjPlaneRetZ) free(prjPlaneRetZ);
	if (prjPlaneDibImage) free(prjPlaneDibImage);
}

void CCgTransControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_TRANSSELECT, m_transSelect);
	DDX_Text(pDX, IDC_DrawSceneRunTime, DrawSceneRunTime);
}

BEGIN_MESSAGE_MAP(CCgTransControl, CFormView)
	ON_BN_CLICKED(IDC_XLEFT, &CCgTransControl::OnClickedXleft)
	ON_BN_CLICKED(IDC_XRIGHT, &CCgTransControl::OnClickedXright)
	ON_BN_CLICKED(IDC_YUP, &CCgTransControl::OnClickedYup)
	ON_BN_CLICKED(IDC_YDOWN, &CCgTransControl::OnClickedYdown)
	ON_BN_CLICKED(IDC_ZFRONT, &CCgTransControl::OnClickedZfront)
	ON_BN_CLICKED(IDC_ZBACK, &CCgTransControl::OnClickedZback)
	ON_BN_CLICKED(IDC_TRANSMODE, &CCgTransControl::OnClickedTransmode)
	ON_CBN_SELCHANGE(IDC_TRANSSELECT, &CCgTransControl::OnSelchangeTransselect)

	ON_STN_CLICKED(IDC_DrawSceneRunTime, &CCgTransControl::OnStnClickedDrawsceneruntime)
END_MESSAGE_MAP()


// CCgTransControl 诊断

#ifdef _DEBUG
void CCgTransControl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCgTransControl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCgTransControl 消息处理程序
#define OFFSET      0.5f
#define DELTAANGLE  5.0f

void CCgTransControl::OnClickedXleft()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == SCENE) {           // Scene Trans
		if (pDoc->m_transMode == 0) pDoc->m_translateVector[0] -= OFFSET;
		else                        pDoc->m_xAngle -= DELTAANGLE;
	}
	else if (pDoc->m_transSelect == LIGHT0) {     // Light0 Trans
		pDoc->lightX[0] -= OFFSET;
	}
	else if (pDoc->m_transSelect == LIGHT1) {   // Light1 Trans
		pDoc->lightX[1] -= OFFSET;
	}
	else if (pDoc->m_transSelect == EYE) {    // Eye Trans
		pDoc->eyeX -= OFFSET;
		//		pDoc->pCreateClipBox();
	}
	else pDoc->m_transDir = 0;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedXright()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == SCENE) {           // Scene Trans
		if (pDoc->m_transMode == 0) pDoc->m_translateVector[0] += OFFSET;
		else                        pDoc->m_xAngle += DELTAANGLE;
	}
	else if (pDoc->m_transSelect == LIGHT0) {     // Light0 Trans
		pDoc->lightX[0] += OFFSET;
	}
	else if (pDoc->m_transSelect == LIGHT1) {   // Light1 Trans
		pDoc->lightX[1] += OFFSET;
	}
	else if (pDoc->m_transSelect == EYE) {      // Eye Trans
		pDoc->eyeX += OFFSET;
		//		pDoc->pCreateClipBox();
	}
	else pDoc->m_transDir = 1;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedYup()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == SCENE) {           // Scene Trans
		if (pDoc->m_transMode == 0) pDoc->m_translateVector[1] += OFFSET;
		else                        pDoc->m_yAngle += DELTAANGLE;
	}
	else if (pDoc->m_transSelect == LIGHT0) {     // Light0 Trans
		pDoc->lightY[0] += OFFSET;
	}
	else if (pDoc->m_transSelect == LIGHT1) {   // Light1 Trans
		pDoc->lightY[1] += OFFSET;
	}
	else if (pDoc->m_transSelect == EYE) {      // Eye Trans
		pDoc->eyeY += OFFSET;
		//		pDoc->pCreateClipBox();
	}
	else	pDoc->m_transDir = 3;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedYdown()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == SCENE) {           // Scene Trans
		if (pDoc->m_transMode == 0) pDoc->m_translateVector[1] -= OFFSET;
		else                        pDoc->m_yAngle -= DELTAANGLE;
	}
	else if (pDoc->m_transSelect == LIGHT0) {     // Light0 Trans
		pDoc->lightY[0] -= OFFSET;
	}
	else if (pDoc->m_transSelect == LIGHT1) {   // Light1 Trans
		pDoc->lightY[1] -= OFFSET;
	}
	else if (pDoc->m_transSelect == EYE) {      // Eye Trans
		pDoc->eyeY -= OFFSET;
		//		pDoc->pCreateClipBox();
	}
	else	pDoc->m_transDir = 2;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedZfront()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == SCENE) {           // Scene Trans
		if (pDoc->m_transMode == 0) pDoc->m_translateVector[2] += OFFSET;
		else                        pDoc->m_zAngle += DELTAANGLE;
	}
	else if (pDoc->m_transSelect == LIGHT0) {     // Light0 Trans
		pDoc->lightZ[0] += OFFSET;
	}
	else if (pDoc->m_transSelect == LIGHT1) {   // Light1 Trans
		pDoc->lightZ[1] += OFFSET;
	}
	else if (pDoc->m_transSelect == EYE) {      // Eye Trans
		pDoc->eyeZ += OFFSET;
		//		pDoc->pCreateClipBox();
	}
	else	pDoc->m_transDir = 5;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedZback()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == SCENE) {           // Scene Trans
		if (pDoc->m_transMode == 0) pDoc->m_translateVector[2] -= OFFSET;
		else                        pDoc->m_zAngle -= DELTAANGLE;
	}
	else if (pDoc->m_transSelect == LIGHT0) {     // Light0 Trans
		pDoc->lightZ[0] -= OFFSET;
	}
	else if (pDoc->m_transSelect == LIGHT1) {   // Light1 Trans
		pDoc->lightZ[1] -= OFFSET;
	}
	else if (pDoc->m_transSelect == EYE) {      // Eye Trans
		pDoc->eyeZ -= OFFSET;
		//		pDoc->pCreateClipBox();
	}
	else	pDoc->m_transDir = 4;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedTransmode()
{
	CButton *pButton = (CButton *)GetDlgItem(IDC_TRANSMODE);
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	pDoc->m_transMode++;
	if (pDoc->m_transMode > 1) pDoc->m_transMode = 0;

	if (pDoc->m_transMode == 0)  pButton->SetWindowText(_T("Translate"));
	else                         pButton->SetWindowText(_T("Rotate"));
}


void CCgTransControl::OnSelchangeTransselect()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	UpdateData(TRUE);

	pDoc->m_transSelect = m_transSelect;

	UpdateData(FALSE);
}

void CCgTransControl::CreateDisplayBitMap(int width, int height)
{
	for (int i = 0; i < sizeof(BITMAPINFOHEADER) + 16; i++) DisplayBitMapBuffer[i] = 0;
	DisplayBitMapHeader = (BITMAPINFO *)&DisplayBitMapBuffer;
	DisplayBitMapHeader->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	DisplayBitMapHeader->bmiHeader.biPlanes = 1;
	DisplayBitMapHeader->bmiHeader.biBitCount = 32;
	DisplayBitMapHeader->bmiHeader.biCompression = BI_BITFIELDS;
	DisplayBitMapHeader->bmiHeader.biWidth = width;
	DisplayBitMapHeader->bmiHeader.biHeight = -height;

	((unsigned long*)DisplayBitMapHeader->bmiColors)[0] = 255 << 16;
	((unsigned long*)DisplayBitMapHeader->bmiColors)[1] = 255 << 8;
	((unsigned long*)DisplayBitMapHeader->bmiColors)[2] = 255;

}

void CCgTransControl::InitPrjPlaneData(int width, int height)
{
	// Initial the Z-Buffer prjPlaneRetZ according prjPlaneRet
	m_imageWidth = width;
	m_imageHeight = height;
	CreateDisplayBitMap(m_imageWidth, m_imageHeight);
	prjPlaneDibImage = new _VPIXEL[m_imageWidth*m_imageHeight];

	prjPlaneRetSize = m_imageWidth*m_imageHeight;
	prjPlaneRetZ = (float *)malloc(prjPlaneRetSize * sizeof(float));


}

void CCgTransControl::_SetPixel(int x, int y, int red, int green, int blue)
{
	if (prjPlaneDibImage == NULL) return;

	BOOL overSide = FALSE;
	int pos = x + y*m_imageWidth;
	int bmpSize = m_imageWidth*m_imageHeight;

	if (pos < 0) { pos = 0; overSide = TRUE; }
	if (pos >= bmpSize) { pos = bmpSize - 1; overSide = TRUE; }

	if (overSide) prjPlaneDibImage[pos] = (255 << 16) + (255 << 8);
	else          prjPlaneDibImage[pos] = (red << 16) + (green << 8) + blue;

}

void CCgTransControl::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CDC     *prjPlaneCDC;
	CRect    prjPlaneRet;
	static BOOL first = TRUE;
	CStatic *prjPlane = (CStatic *)GetDlgItem(IDC_VIEWPORT);
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	AssertValid();

	DWORD  opStart, opFinish;

	opStart = GetTickCount();       // 记录测试开始时间，单位是毫秒

	prjPlaneCDC = prjPlane->GetDC();
	prjPlane->GetClientRect(&prjPlaneRet);

	// Clear BackGround of DC
	CBrush brBlack(RGB(0, 0, 0));
	prjPlaneCDC->FillRect(prjPlaneRet, &brBlack);

	// Create a BitMap Buffer for prjPlaneDibImage and prjPlaneRetZ Buffer.
	if (first) {
		first = FALSE;
		InitPrjPlaneData(prjPlaneRet.right, prjPlaneRet.bottom);
	}

	// Reset prjPlaneRetZ and prjPlaneDibImage
	if (prjPlaneRetZ) {
		for (int i = 0; i < prjPlaneRet.bottom; i++)
			for (int j = 0; j < prjPlaneRet.right; j++)
				prjPlaneRetZ[i * prjPlaneRet.right + j] = -100.0f;
	}

	if (prjPlaneDibImage)
		for (int i = 0; i < m_imageWidth*m_imageHeight; i++) prjPlaneDibImage[i] = 0;

	for (m_objNumber = pDoc->m_objectNum - 1; m_objNumber >= 0; m_objNumber--) {

		m_whoObject = &pDoc->m_spaceObjects[m_objNumber];

		pTransToZbuffer(prjPlaneRet);

		if (pDoc->m_spaceObjects[m_objNumber].objectVisible) {
			if (pDoc->m_onLight) {
				float maxShade, minShade;

				maxShade = -100.0f;    minShade = 100.0f;
				for (int i = 0; i < m_whoObject->polyCount; i++) {
					if (m_whoObject->objectSpace[i].polyVisible) {
						for (int j = 0; j < m_whoObject->objectSpace[i].clipCount; j++) {
							if (m_whoObject->objectSpace[i].polyLight[j] > maxShade)
								maxShade = m_whoObject->objectSpace[i].polyLight[j];
							if (m_whoObject->objectSpace[i].polyLight[j] < minShade)
								minShade = m_whoObject->objectSpace[i].polyLight[j];
						}
					}
				}

				if (!pDoc->m_onShade) pDrawLightObject(prjPlaneCDC, prjPlaneRet, maxShade, minShade);
				else                  pDrawShadeLightObject(prjPlaneCDC, prjPlaneRet, maxShade, minShade);
			}
			else {
				pDrawLineObject(prjPlaneCDC, prjPlaneRet);
			}
		}
	}

	if (pDoc->m_onShade) {
		CClientDC ClientDC(prjPlaneCDC->GetWindow());
		::StretchDIBits(ClientDC.m_hDC,
			0, 0, m_imageWidth, m_imageHeight,
			0, 0, m_imageWidth, m_imageHeight,
			prjPlaneDibImage, DisplayBitMapHeader,
			DIB_RGB_COLORS, SRCCOPY);

	}
	DrawSceneRunTime = pDoc->m_DrawSceneRunTime;

	// Must remember to release prjPlaneCDC every time after using.
	ReleaseDC(prjPlaneCDC);
}


//实现视见变换
void CCgTransControl::pTransToZbuffer(CRect dcRect)
{
	int i, j;
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	float vxScale = (float)dcRect.right / (pDoc->winRx - pDoc->winLx);
	float vyScale = (float)dcRect.bottom / (pDoc->winRy - pDoc->winLy);
	for (i = 0; i < m_whoObject->polyCount; i++) {
		if (m_whoObject->objectSpace[i].polyVisible) {
			for (j = 0; j < m_whoObject->objectSpace[i].clipCount; j++) {
				m_whoObject->objectSpace[i].zBufferObject[j].x =
					(int)((m_whoObject->objectSpace[i].projectObject[j].x - pDoc->winLx) *	vxScale + 0.5f);
				m_whoObject->objectSpace[i].zBufferObject[j].y = dcRect.bottom -
					(int)((m_whoObject->objectSpace[i].projectObject[j].y - pDoc->winLy) *	vyScale + 0.5f);
			}
		}
	}
}


void CCgTransControl::pDrawLineObject(CDC *pDC, CRect dcRect)
{
	CPen newPen;
	CPen *oldPen;
	int i, j, x1, y1, x2, y2;
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	if (m_objNumber <= TRIANGLE)
		newPen.CreatePen(PS_SOLID, 1, vcObjColor[m_objNumber]);
	else
		newPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

	oldPen = (CPen *)pDC->SelectObject(&newPen);

	// Draw Object in DC
	//CRgn clipRgn;
	//clipRgn.CreateRectRgn(dcRect.left, dcRect.top, dcRect.right, dcRect.bottom);
	//pDC->SelectClipRgn(&clipRgn);

	for (i = 0; i < m_whoObject->polyCount; i++) {
		if (m_whoObject->objectSpace[i].polyVisible) {
			x1 = m_whoObject->objectSpace[i].zBufferObject[0].x;
			y1 = m_whoObject->objectSpace[i].zBufferObject[0].y;
			for (j = 1; j < m_whoObject->objectSpace[i].clipCount; j++) {
				x2 = m_whoObject->objectSpace[i].zBufferObject[j].x;
				y2 = m_whoObject->objectSpace[i].zBufferObject[j].y;
				// draw a line
				pDC->MoveTo(x1, y1);
				pDC->LineTo(x2, y2);
				x1 = x2;
				y1 = y2;
			}
		}
	}

	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}

void CCgTransControl::pDrawLightObject(CDC *pDC, CRect dcRect, float maxShade, float minShade)
{
	CRgn rgn;
	CBrush br;
	int i, j, color;
	float shadeValue;
	tagPOINT Point[10];
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)GetDocument();

	// Draw Object in DC
	float shadeWidth = maxShade - minShade;
	for (i = 0; i < m_whoObject->polyCount; i++) {
		if (m_whoObject->objectSpace[i].polyVisible) {
			for (j = 0; j < m_whoObject->objectSpace[i].clipCount; j++) {
				Point[j].x = m_whoObject->objectSpace[i].zBufferObject[j].x;
				Point[j].y = m_whoObject->objectSpace[i].zBufferObject[j].y;
			}

			float sum = 0;
			for (int k = 0; k < 4; k++) sum += m_whoObject->objectSpace[i].polyLight[k];

			shadeValue = sum / 4.0f;
			color = (int)(255 * (shadeValue - minShade) / shadeWidth);
			if (color > 255) color = 255;

			switch (m_objNumber) {
			case BALL: br.CreateSolidBrush(RGB(color, color, 0)); break;
			case CUBE: br.CreateSolidBrush(RGB(0, color, 0)); break;
			case TRIANGLE: br.CreateSolidBrush(RGB(0, 0, color)); break;
			default:br.CreateSolidBrush(RGB(color, color, 0)); break;
			}

			rgn.CreatePolygonRgn(Point, m_whoObject->objectSpace[i].clipCount, WINDING);
			pDC->FillRgn(&rgn, &br);

			br.DeleteObject();
			rgn.DeleteObject();
		}
	}
}
void CCgTransControl::pDrawShadeLightObject(CDC *pDC, CRect dcRect, float maxShade, float minShade)
{
	int i, j, color[10];
	float shadeValue;
	int  x[10], y[10];
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)GetDocument();

	float shadeWidth = maxShade - minShade;

	// Draw Object in DC
	for (i = 0; i < m_whoObject->polyCount; i++) {
		if (m_whoObject->objectSpace[i].polyVisible) {
			for (j = 0; j < m_whoObject->objectSpace[i].clipCount; j++) {
				x[j] = m_whoObject->objectSpace[i].zBufferObject[j].x;
				y[j] = m_whoObject->objectSpace[i].zBufferObject[j].y;

				shadeValue = m_whoObject->objectSpace[i].polyLight[j];
				color[j] = (int)(255 * (shadeValue - minShade) / shadeWidth);
				if (color[j] > 255) color[j] = 255;
			}
			polyCount = i;
			if (m_whoObject->objectSpace[i].clipCount >= 2)
				FillPolygon(pDC, m_whoObject->objectSpace[i].clipCount, x, y, color, dcRect);
		}
	}
}

void CCgTransControl::FillPolygon(CDC *pDC, int n, int *x, int *y, int *color, CRect dcRect)
{
	pdges = 0;
	ibegin = iend = 0;
	Loadpolygon(n, x, y, color);
	if (pdges <2) return;
	scan = (int)ymax[0];
	Include();
	UpdateXvalue();
	while (ibegin != iend) {
		Fillscan(pDC, dcRect);
		scan--;
		Include();
		UpdateXvalue();
	}
}

void CCgTransControl::Loadpolygon(int n, int *x, int *y, int *color)
{
	int i, color1, color2;
	float x1, x2, y1, y2;

	color1 = color[0];
	x1 = (float)x[0];     y1 = y[0] + 0.5f;
	for (i = 1; i < n; i++) {
		color2 = color[i];
		x2 = (float)x[i];   y2 = y[i] + 0.5f;
		if (y2 - y1)   PolyInsert(x1, y1, x2, y2, color1, color2);
		x1 = x2;     y1 = y2;  color1 = color2;
	}
	color2 = color[0];
	x2 = (float)x[0];             y2 = y[0] + 0.5f;
	if (y2 - y1)   PolyInsert(x1, y1, x2, y2, color1, color2);
}

void CCgTransControl::PolyInsert(float x1, float y1, float x2, float y2,
	int color1, int color2)
{
	int i;
	float maxY, minY, slope;

	i = pdges;
	maxY = (y1>y2) ? y1 : y2;
	minY = (y1<y2) ? y1 : y2;
	while (i > 0 && maxY > ymax[i - 1]) {
		ymax[i] = ymax[i - 1];
		ymin[i] = ymin[i - 1];
		Xa[i] = Xa[i - 1];
		Dx[i] = Dx[i - 1];
		Sc[i] = Sc[i - 1];
		Dc[i] = Dc[i - 1];
		i--;
	}
	ymax[i] = maxY;
	ymin[i] = minY;
	Dx[i] = (x2 - x1) / (y2 - y1);
	if (y1 > y2) { Xa[i] = x1; Sc[i] = (float)color1; Dc[i] = (float)color2 - color1; }
	else { Xa[i] = x2; Sc[i] = (float)color2; Dc[i] = (float)color1 - color2; }
	slope = (float)fabs(1 / (y2 - y1));
	Dc[i] = Dc[i] * slope;
	pdges++;
}

void CCgTransControl::Include()
{
	while (iend < pdges && ymax[iend] > scan) {
		Dx[iend] = -Dx[iend];
		iend++;
	}
}

void CCgTransControl::UpdateXvalue()
{
	int i, j, start;

	start = ibegin;
	for (i = start; i < iend; i++) {
		if (ymin[i] < scan) {
			Sc[i] += Dc[i];
			Xa[i] += Dx[i];
			XSort(ibegin, i);
		}
		else {
			for (j = i; j > ibegin; j--) {
				ymin[j] = ymin[j - 1];
				Xa[j] = Xa[j - 1];
				Dx[j] = Dx[j - 1];

				Sc[j] = Sc[j - 1];
				Dc[j] = Dc[j - 1];
			}
			ibegin++;
		}
	}
}

void CCgTransControl::XSort(int begin, int i)
{
	float temp;
	while (i > begin && Xa[i] < Xa[i - 1]) {
		temp = ymin[i];  ymin[i] = ymin[i - 1];  ymin[i - 1] = temp;
		temp = Xa[i];    Xa[i] = Xa[i - 1];      Xa[i - 1] = temp;
		temp = Sc[i];    Sc[i] = Sc[i - 1];      Sc[i - 1] = temp;
		temp = Dx[i];    Dx[i] = Dx[i - 1];      Dx[i - 1] = temp;
		temp = Dc[i];    Dc[i] = Dc[i - 1];      Dc[i - 1] = temp;

		i--;
	}
}

void CCgTransControl::Fillscan(CDC *pDC, CRect dcRect)
{
	//	float zb,zt;
	float z;
	tagPOINT Point;
	COLORREF rgbColor;
	float dColor, color;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)GetDocument();

	Point.y = (int)scan;
	for (int i = ibegin; i < iend; i += 2) {
		color = Sc[i];
		dColor = (Sc[i + 1] - Sc[i]) / (Xa[i + 1] - Xa[i]);
		for (Point.x = (int)Xa[i]; Point.x < (int)Xa[i + 1]; Point.x++) {

			//////////////// Segment for Z-Buffer Hidden processing. ////////////////
			int position = Point.y * dcRect.right + Point.x;
			z = CalculateZValue(Point.x, Point.y, dcRect, polyCount);
			if (0 <= position && position <= prjPlaneRetSize) {
				if (z > prjPlaneRetZ[position]) {
					switch (m_objNumber) {
					case BALL: _SetPixel(Point.x, Point.y, color, color, 0); break;
					case CUBE: _SetPixel(Point.x, Point.y, 0, color, 0); break;
					case TRIANGLE: _SetPixel(Point.x, Point.y, 0, 0, color); break;
					default:_SetPixel(Point.x, Point.y, color, color, 0); break;
					}
					prjPlaneRetZ[position] = z;
				}
			}

			color += dColor;//颜色渐变
		}
	}
}

float CCgTransControl::CalculateZValue(int x, int y, CRect dcRect, int i)
{
	int j;
	float lx, ly, lz;
	float cx, cy, kx, ky;
	float sx[5], sy[5], sz[5];
	float denominator, space_z;
	float project_x, project_y, project_z = 0;
	float n1x, n1y, n1z, n2x, n2y, n2z, fx, fy, fz;
	Ccg2019WJY3DTransDoc *pDoc = (Ccg2019WJY3DTransDoc *)GetDocument();

	project_x = ((float)x / (float)dcRect.right) + pDoc->winLx;
	project_y = ((float)dcRect.bottom - (float)y) / (float)dcRect.bottom + pDoc->winLy;

	for (j = 0; j < 3; j++) {
		sx[j] = m_whoObject->objectSpace[i].clipObject[j].x;
		sy[j] = m_whoObject->objectSpace[i].clipObject[j].y;
		sz[j] = m_whoObject->objectSpace[i].clipObject[j].z;
	}

	n1x = sx[1] - sx[0];  n1y = sy[1] - sy[0]; n1z = sz[1] - sz[0];
	n2x = sx[2] - sx[0];  n2y = sy[2] - sy[0]; n2z = sz[2] - sz[0];

	fx = n1y*n2z - n1z*n2y;
	fy = n1z*n2x - n1x*n2z;
	fz = n1x*n2y - n1y*n2x;

	lx = pDoc->eyeX - project_x;
	ly = pDoc->eyeY - project_y;
	lz = pDoc->eyeZ - project_z;

	cx = lx / lz;            cy = ly / lz;
	kx = pDoc->eyeX - cx * pDoc->eyeZ;
	ky = pDoc->eyeY - cy * pDoc->eyeZ;

	denominator = fz + fy*cy + fx*cx;
	space_z = ((fx*sx[0] + fy*sy[0] + fz*sz[0]) - (fy*ky + fx*kx)) / denominator;

	return space_z;
}




void CCgTransControl::OnStnClickedDrawsceneruntime()
{
	// TODO: 在此添加控件通知处理程序代码
}
