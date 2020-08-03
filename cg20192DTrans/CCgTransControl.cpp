// CCgTransControl.cpp: 实现文件
//

#include "pch.h"
#include "cg2019WJY2DTrans.h"
#include "CCgTransControl.h"

#include "cg2019WJY2DTransDoc.h"



// CCgTransControl

IMPLEMENT_DYNCREATE(CCgTransControl, CFormView)

CCgTransControl::CCgTransControl()
	: CFormView(IDD_TRANSCONTROL)
{
	m_transSelect = 0;
}

CCgTransControl::~CCgTransControl()
{
}

void CCgTransControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_TRANSSELECT, m_transSelect);
}

BEGIN_MESSAGE_MAP(CCgTransControl, CFormView)

	ON_BN_CLICKED(IDC_lEFT, &CCgTransControl::OnClickedLeft)
	ON_BN_CLICKED(IDC_RIGHT, &CCgTransControl::OnClickedRight)
	ON_BN_CLICKED(IDC_UP, &CCgTransControl::OnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CCgTransControl::OnClickedDown)

ON_CBN_SELCHANGE(IDC_TRANSSELECT, &CCgTransControl::OnSelchangeTransselect)
ON_BN_CLICKED(IDC_TRANSMODE, &CCgTransControl::OnClickedTransmode)
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

#define WND_DELTAX 10
#define WND_DELTAY 10

// CCgTransControl 消息处理程序

void CCgTransControl::OnClickedLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == 2) {
		pDoc->m_wndLx -= WND_DELTAX;
		pDoc->m_wndRx -= WND_DELTAX;
	}

	pDoc->m_transDir = 0;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedRight()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == 2) {
		pDoc->m_wndLx += WND_DELTAX;
		pDoc->m_wndRx += WND_DELTAX;
	}

	pDoc->m_transDir = 1;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedUp()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == 2) {
		pDoc->m_wndLy += WND_DELTAY;
		pDoc->m_wndRy += WND_DELTAY;
	}

	pDoc->m_transDir = 3;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnClickedDown()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	if (pDoc->m_transSelect == 2) {
		pDoc->m_wndLy -= WND_DELTAY;
		pDoc->m_wndRy -= WND_DELTAY;
	}

	pDoc->m_transDir = 2;

	pDoc->UpdateAllViews(this);
}


void CCgTransControl::OnSelchangeTransselect()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	UpdateData(TRUE);

	pDoc->m_transSelect = m_transSelect;

	UpdateData(FALSE);
}


void CCgTransControl::OnClickedTransmode()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();
	CButton* pButton = (CButton*)GetDlgItem(IDC_TRANSMODE);

	//	if (pDoc->rayTraceNow) return;

	pDoc->m_transMode++;
	if (pDoc->m_transMode > 2) pDoc->m_transMode = 0;

	if (pDoc->m_transMode == 0)      pButton->SetWindowText(_T("Move"));
	else if (pDoc->m_transMode == 1) pButton->SetWindowText(_T("Rotate"));
	else                             pButton->SetWindowText(_T("Scale"));
}




void CCgTransControl::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	CStatic* viewPlane = (CStatic*)GetDlgItem(IDC_VIEWPORT);
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	CDC* prjPlaneCDC;
	CRect    prjPlaneRet;

	prjPlaneCDC = viewPlane->GetDC();
	viewPlane->GetClientRect(&prjPlaneRet);

	// Clear BackGround of DC
	CBrush brBlack(RGB(255, 255, 255));
	prjPlaneCDC->FillRect(prjPlaneRet, &brBlack);
	
	if (pDoc->m_lineVisible) ViewTransLine(prjPlaneCDC, prjPlaneRet);
	if (pDoc->m_polygonVisible) ViewTransPolygon(prjPlaneCDC, prjPlaneRet);

	if (pDoc->m_LB_lineVisible) LB_ViewTransLine(prjPlaneCDC, prjPlaneRet);
	if (pDoc->m_WA_polygonVisible) WA_ViewTransPolygon(prjPlaneCDC, prjPlaneRet);
	
	// Must remember to release prjPlaneCDC
	ReleaseDC(prjPlaneCDC);
}


void CCgTransControl::ViewTransLine(CDC* pDC, CRect dcRect)
{
	CPen newPen;
	CPen* oldPen;
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	// Create new red-color pen to Draw Clipping Line
	newPen.CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
	oldPen = (CPen*)pDC->SelectObject(&newPen);

	int vx1, vy1, vx2, vy2;

	int wndWidth = pDoc->m_wndRx - pDoc->m_wndLx;  // Space Window size parameters.
	int wndHeight = pDoc->m_wndRy - pDoc->m_wndLy;

	float Sx = (float)(dcRect.right) / wndWidth;    // View-Space Viewport size parameters.
	float Sy = (float)(dcRect.bottom) / wndHeight;

	// Please fill the right codes below lines ...
	vx1 = (pDoc->CP1.x - pDoc->m_wndLx) * Sx;
	vy1 = (pDoc->CP1.y - pDoc->m_wndLy) * Sy;
	vx2 = (pDoc->CP2.x - pDoc->m_wndLx) * Sx;
	vy2 = (pDoc->CP2.y - pDoc->m_wndLy) * Sy;

	pDC->MoveTo(vx1, dcRect.bottom - vy1);
	pDC->LineTo(vx2, dcRect.bottom - vy2);

	// Remember: must delete newPen every time.	
	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}

void CCgTransControl::ViewTransPolygon(CDC* pDC, CRect dcRect)
{
	CPen newPen;
	CPen* oldPen;
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	// Create new red-color pen to Draw Clipping Line
	newPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	oldPen = (CPen*)pDC->SelectObject(&newPen);

	CPoint m_VPolygon[N];

	int wndWidth = pDoc->m_wndRx - pDoc->m_wndLx;  // Space Window size parameters.
	int wndHeight = pDoc->m_wndRy - pDoc->m_wndLy;

	float Sx = (float)(dcRect.right) / wndWidth;    // View-Space Viewport size parameters.
	float Sy = (float)(dcRect.bottom) / wndHeight;

	// Please fill the right codes below lines ...
	for (int i = 0; i < pDoc->m_clipPointNumber; i++) {
		m_VPolygon[i].x = (pDoc->m_clipPolygon[i].x - pDoc->m_wndLx) * Sx;
		m_VPolygon[i].y = dcRect.bottom - (pDoc->m_clipPolygon[i].y - pDoc->m_wndLy) * Sy;
	}

	pDC->MoveTo(m_VPolygon[0]);
	for (int i = 1; i < pDoc->m_clipPointNumber; i++) pDC->LineTo(m_VPolygon[i]);

	// Remember: must delete newPen every time.	
	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}


void CCgTransControl::LB_ViewTransLine(CDC* pDC, CRect dcRect)
{
	CPen newPen;
	CPen* oldPen;
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	// Create new red-color pen to Draw Clipping Line
	newPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	oldPen = (CPen*)pDC->SelectObject(&newPen);

	int vx1, vy1, vx2, vy2;

	int wndWidth = pDoc->m_wndRx - pDoc->m_wndLx;  // Space Window size parameters.
	int wndHeight = pDoc->m_wndRy - pDoc->m_wndLy;

	float Sx = (float)(dcRect.right) / wndWidth;    // View-Space Viewport size parameters.
	float Sy = (float)(dcRect.bottom) / wndHeight;

	// Please fill the right codes below lines ...
	vx1 = (pDoc->LB_CP1.x - pDoc->m_wndLx) * Sx;
	vy1 = (pDoc->LB_CP1.y - pDoc->m_wndLy) * Sy;
	vx2 = (pDoc->LB_CP2.x - pDoc->m_wndLx) * Sx;
	vy2 = (pDoc->LB_CP2.y - pDoc->m_wndLy) * Sy;

	pDC->MoveTo(vx1, dcRect.bottom - vy1);
	pDC->LineTo(vx2, dcRect.bottom - vy2);

	// Remember: must delete newPen every time.	
	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}

void CCgTransControl::WA_ViewTransPolygon(CDC* pDC, CRect dcRect)
{
	CPen newPen;
	CPen* oldPen;
	Ccg2019WJY2DTransDoc* pDoc = (Ccg2019WJY2DTransDoc*)GetDocument();

	// Create new red-color pen to Draw Clipping Line
	newPen.CreatePen(PS_SOLID, 2, RGB(210, 180, 140));
	oldPen = (CPen*)pDC->SelectObject(&newPen);

	CPoint m_VPolygon[N];

	int wndWidth = pDoc->m_wndRx - pDoc->m_wndLx;  // Space Window size parameters.
	int wndHeight = pDoc->m_wndRy - pDoc->m_wndLy;

	float Sx = (float)(dcRect.right) / wndWidth;    // View-Space Viewport size parameters.
	float Sy = (float)(dcRect.bottom) / wndHeight;

	// Please fill the right codes below lines ...
	for (int i = 0; i < pDoc->m_WA_clipPointNumber; i++) {
		m_VPolygon[i].x = (pDoc->m_WA_clipPolygon[i].x - pDoc->m_wndLx) * Sx;
		m_VPolygon[i].y = dcRect.bottom - (pDoc->m_WA_clipPolygon[i].y - pDoc->m_wndLy) * Sy;
	}

	pDC->MoveTo(m_VPolygon[0]);
	for (int i = 1; i < pDoc->m_WA_clipPointNumber; i++) pDC->LineTo(m_VPolygon[i]);

	// Remember: must delete newPen every time.	
	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}
