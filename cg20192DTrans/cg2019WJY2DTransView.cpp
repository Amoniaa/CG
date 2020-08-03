
// cg2019WJY2DTransView.cpp: Ccg2019WJY2DTransView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cg2019WJY2DTrans.h"
#endif

#include "cg2019WJY2DTransDoc.h"
#include "cg2019WJY2DTransView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccg2019WJY2DTransView

IMPLEMENT_DYNCREATE(Ccg2019WJY2DTransView, CView)

BEGIN_MESSAGE_MAP(Ccg2019WJY2DTransView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// Ccg2019WJY2DTransView 构造/析构

Ccg2019WJY2DTransView::Ccg2019WJY2DTransView() noexcept
{
	// TODO: 在此处添加构造代码

}

Ccg2019WJY2DTransView::~Ccg2019WJY2DTransView()
{
}

BOOL Ccg2019WJY2DTransView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Ccg2019WJY2DTransView 绘图

void Ccg2019WJY2DTransView::OnDraw(CDC* /*pDC*/)
{
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//绘制坐标系
	CDC* pDC = GetDC();

	pDC->MoveTo(0, m_wndHeight / 2);
	pDC->LineTo(m_wndWidth, m_wndHeight / 2);
	pDC->MoveTo(m_wndWidth / 2, 0);
	pDC->LineTo(m_wndWidth / 2, m_wndHeight);

	CPen newPen;
	CPen* oldPen;

	newPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
	oldPen = (CPen*)pDC->SelectObject(&newPen);

	pDC->MoveTo(m_wndWidth / 2 + pDoc->m_wndLx, m_wndHeight / 2 - pDoc->m_wndLy);
	pDC->LineTo(m_wndWidth / 2 + pDoc->m_wndLx, m_wndHeight / 2 - pDoc->m_wndRy);
	pDC->LineTo(m_wndWidth / 2 + pDoc->m_wndRx, m_wndHeight / 2 - pDoc->m_wndRy);
	pDC->LineTo(m_wndWidth / 2 + pDoc->m_wndRx, m_wndHeight / 2 - pDoc->m_wndLy);
	pDC->LineTo(m_wndWidth / 2 + pDoc->m_wndLx, m_wndHeight / 2 - pDoc->m_wndLy);

	pDC->SelectObject(oldPen);
	newPen.DeleteObject();

	//DisplayLine(pDC, pDoc->P1, pDoc->P2, RGB(255, 0, 0));
	//DisplayPolygon(pDC, pDoc->m_pointNumber, pDoc->m_spPolygon, RGB(0, 0, 255));

	if (pDoc->m_transSelect == 0)      CalculateMatrix(pDoc->m_lineMatrix);
	else if (pDoc->m_transSelect == 1) CalculateMatrix(pDoc->m_polygonMatrix);
	else if (pDoc->m_transSelect == 3) CalculateMatrix(pDoc->m_LB_LineMatrix);
	else if (pDoc->m_transSelect == 4) CalculateMatrix(pDoc->m_WA_polygonMatrix);

	// Trans & Display General Line Space Model
	TransLine(pDoc->P1, pDoc->P2, &pDoc->TP1, &pDoc->TP2, pDoc->m_lineMatrix);
	DisplayLine(pDC, pDoc->TP1, pDoc->TP2, RGB(255, 0, 0));

	// Cohn-Sutherland Subdivision Line Clip 
	int cx1, cy1, cx2, cy2;
	pDoc->m_lineVisible = FALSE;
	cx1 = pDoc->TP1.x;    cy1 = pDoc->TP1.y;
	cx2 = pDoc->TP2.x;    cy2 = pDoc->TP2.y;

	if (ClipLine(&cx1, &cy1, &cx2, &cy2)) {

		pDoc->CP1.x = cx1;    pDoc->CP1.y = cy1;
		pDoc->CP2.x = cx2;    pDoc->CP2.y = cy2;

		DisplayLine(pDC, pDoc->CP1, pDoc->CP2, RGB(255, 255, 0));
		pDoc->m_lineVisible = TRUE;
	}

	// Trans & Display General Polygon Space Model
	TransPolygon(pDoc->m_pointNumber, pDoc->m_spPolygon, pDoc->m_transPolygon, pDoc->m_polygonMatrix);
	DisplayPolygon(pDC, pDoc->m_pointNumber, pDoc->m_transPolygon, RGB(0, 0, 255));

	// Sutherland-Hodgman Polygon Clip
	pDoc->m_polygonVisible = FALSE;

	if (ClipPolygon(pDoc->m_pointNumber, pDoc->m_transPolygon,
				    &pDoc->m_clipPointNumber, pDoc->m_clipPolygon))
	{
		DisplayPolygon(pDC, pDoc->m_clipPointNumber, pDoc->m_clipPolygon, RGB(0, 255, 255));
		pDoc->m_polygonVisible = TRUE;
	}
	
	// Trans & Display Liang-Barsky Line Space Model
	TransLine(pDoc->LB_P1, pDoc->LB_P2, &pDoc->LB_TP1, &pDoc->LB_TP2, pDoc->m_LB_LineMatrix);
	DisplayLine(pDC, pDoc->LB_TP1, pDoc->LB_TP2, RGB(0, 125, 0));

	// Liang-Barsky Subdivision Line Clip 
	int LB_cx1, LB_cy1, LB_cx2, LB_cy2;
	pDoc->m_LB_lineVisible = FALSE;
	LB_cx1 = pDoc->LB_TP1.x;    LB_cy1 = pDoc->LB_TP1.y;
	LB_cx2 = pDoc->LB_TP2.x;    LB_cy2 = pDoc->LB_TP2.y;

	if (LB_ClipLine(&LB_cx1, &LB_cy1, &LB_cx2, &LB_cy2)) {

		pDoc->LB_CP1.x = LB_cx1;    pDoc->LB_CP1.y = LB_cy1;
		pDoc->LB_CP2.x = LB_cx2;    pDoc->LB_CP2.y = LB_cy2;

		DisplayLine(pDC, pDoc->LB_CP1, pDoc->LB_CP2, RGB(0, 255, 0));
		pDoc->m_LB_lineVisible = TRUE;
	}
	
	// Trans & Display Weiler-Atherton Polygon Space Model
	TransPolygon(pDoc->m_WA_pointNumber, pDoc->m_WA_spPolygon, pDoc->m_WA_transPolygon, pDoc->m_WA_polygonMatrix);
	DisplayPolygon(pDC, pDoc->m_WA_pointNumber, pDoc->m_WA_transPolygon, RGB(138, 69, 19));

	// Weiler-Atherton Polygon Clip
	pDoc->m_WA_polygonVisible = FALSE;

	if (/*WA_*/ClipPolygon(pDoc->m_WA_pointNumber, pDoc->m_WA_transPolygon,
		&pDoc->m_WA_clipPointNumber, pDoc->m_WA_clipPolygon))
	{
		DisplayPolygon(pDC, pDoc->m_WA_clipPointNumber, pDoc->m_WA_clipPolygon, RGB(210, 180, 140));
		pDoc->m_WA_polygonVisible = TRUE;
	}
	pDoc->UpdateAllViews(this);

	// Must remember to release pDC
	//ReleaseDC(pDC);
}


// Ccg2019WJY2DTransView 打印

BOOL Ccg2019WJY2DTransView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ccg2019WJY2DTransView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Ccg2019WJY2DTransView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Ccg2019WJY2DTransView 诊断

#ifdef _DEBUG
void Ccg2019WJY2DTransView::AssertValid() const
{
	CView::AssertValid();
}

void Ccg2019WJY2DTransView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ccg2019WJY2DTransDoc* Ccg2019WJY2DTransView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ccg2019WJY2DTransDoc)));
	return (Ccg2019WJY2DTransDoc*)m_pDocument;
}
#endif //_DEBUG


// Ccg2019WJY2DTransView 消息处理程序


void Ccg2019WJY2DTransView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// 记录窗体视口尺寸
	m_wndWidth = cx;
	m_wndHeight = cy;
}

#define DELTAX 10
#define DELTAY 10
#define DELTATHETA 3.1415926/36
#define SSCALEX 0.8
#define SSCALEY 0.8
#define LSCALEX 1.2
#define LSCALEY 1.2

//计算变换矩阵
void Ccg2019WJY2DTransView::CalculateMatrix(float transMatrix[3][2])
{
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();

	switch (pDoc->m_transDir) {
	case 0: // -X
		switch (pDoc->m_transMode) {
		case 0: // Move
			TranslateMatrix(-DELTAX, 0, transMatrix);
			break;
		case 1: // rotate
			RotateMatrix(-sin(DELTATHETA), cos(DELTATHETA), transMatrix);
			break;
		case 2: // Scale
			ScaleMatrix(SSCALEX, 1, transMatrix);
			break;
		}
		break;
	case 1: // +X
		switch (pDoc->m_transMode) {
		case 0: // Move
			TranslateMatrix(DELTAX, 0, transMatrix);
			break;
		case 1: // rotate
			RotateMatrix(sin(DELTATHETA), cos(DELTATHETA), transMatrix);
			break;
		case 2: // Scale
			ScaleMatrix(LSCALEX, 1, transMatrix);
			break;
		}
		break;
	case 2: // -Y
		switch (pDoc->m_transMode) {
		case 0: // Move
			TranslateMatrix(0, -DELTAY, transMatrix);
			break;
		case 1: // rotate
			RotateMatrix(-sin(DELTATHETA), cos(DELTATHETA), transMatrix);
			break;
		case 2: // Scale
			ScaleMatrix(1, SSCALEY, transMatrix);
			break;
		}
		break;
	case 3: // +Y
		switch (pDoc->m_transMode) {
		case 0: // Move
			TranslateMatrix(0, DELTAY, transMatrix);
			break;
		case 1: // rotate
			RotateMatrix(sin(DELTATHETA), cos(DELTATHETA), transMatrix);
			break;
		case 2: // Scale
			ScaleMatrix(1, LSCALEY, transMatrix);
			break;
		}
		break;
	}
}

//缩放矩阵
void Ccg2019WJY2DTransView::ScaleMatrix(float Sx, float Sy, float m[3][2])
{
	for (int i = 0; i < 3; i++) {
		m[i][0] *= Sx;
		m[i][1] *= Sy;
	}
}

//旋转矩阵
void Ccg2019WJY2DTransView::RotateMatrix(float S, float C, float m[3][2])
{
	float temp;
	for (int i = 0; i < 3; i++) {
		temp = m[i][0];
		m[i][0] = temp * C - m[i][1] * S;
		m[i][1] = temp * S + m[i][1] * C;
	}
}

//平移矩阵
void Ccg2019WJY2DTransView::TranslateMatrix(float Tx, float Ty, float m[3][2])
{
	// 矩阵平移变换的位移量
	m[2][0] += Tx;
	m[2][1] += Ty;
}


//绘制直线
void Ccg2019WJY2DTransView::DisplayLine(CDC* pDC, CPoint p1, CPoint p2, COLORREF rgbColor)
{
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();

	CPen newPen;
	CPen* oldPen;
	CPoint VP1, VP2;

	newPen.CreatePen(PS_SOLID, 2, rgbColor);
	oldPen = (CPen*)pDC->SelectObject(&newPen);

	VP1.x = m_wndWidth / 2 + p1.x;
	VP1.y = m_wndHeight / 2 - p1.y;
	VP2.x = m_wndWidth / 2 + p2.x;
	VP2.y = m_wndHeight / 2 - p2.y;

	pDC->MoveTo(VP1);
	pDC->LineTo(VP2);

	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}

//直线2D变换
void Ccg2019WJY2DTransView::TransLine(CPoint p1, CPoint p2, CPoint* tp1, CPoint* tp2, float transMatrix[3][2])
{
	// 更改移动后的线段端点坐标p1, p2,利用矩阵乘法(x,y,1) * transMatrix[3][2]
	tp1->x = p1.x * transMatrix[0][0] +
			 p1.y * transMatrix[1][0] +
		            transMatrix[2][0];
	tp1->y = p1.x * transMatrix[0][1] +
			 p1.y * transMatrix[1][1] +
					transMatrix[2][1];
	tp2->x = p2.x * transMatrix[0][0] +
			 p2.y * transMatrix[1][0] +
					transMatrix[2][0];
	tp2->y = p2.x * transMatrix[0][1] +
			 p2.y * transMatrix[1][1] +
					transMatrix[2][1];
}

// Cohn-Sutherland Subdivision Line Clip
int  Ccg2019WJY2DTransView::ClipLine(int* x1, int* y1, int* x2, int* y2)
{
	int visible, m_window[4];
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();
	//创建四条边
	m_window[0] = pDoc->m_wndLx;    m_window[1] = pDoc->m_wndRx;
	m_window[2] = pDoc->m_wndRy;    m_window[3] = pDoc->m_wndLy;
	for (int i = 0; i < 4; i++) { //沿着窗口四条边分别裁剪线段
		//先判断是否完全可见或完全不可见
		visible = LineVisible(x1, y1, x2, y2);
		if (visible == 1) return 1;         //整体完全可见
		if (visible == 0) return 0;         //整体完全不可见
		//其他情况：部分可见，且第一个点在窗口外
		if (LineCross(*x1, *y1, *x2, *y2, i)) {	//判断是否经过该边界
			if (i < 2 && *x2 - *x1) {           //左边界和右边界
				float m = (float)(*y2 - *y1) / (*x2 - *x1); //计算斜率
				float iy = m * (m_window[i] - *x1) + *y1;	//计算出边界与线段的交点
				//更新端点，舍弃窗口边界之外的部分
				if (i == 0) {	//左边界
					if (*x1 < *x2) {
						*x1 = m_window[i];
						*y1 = iy;
					}
					else {		
						*x2 = m_window[i];
						*y2 = iy;
					}
				}
				else {		    //右边界
					if (*x1 > *x2) {
						*x1 = m_window[i];
						*y1 = iy;
					}
					else {
						*x2 = m_window[i];
						*y2 = iy;
					}
				}
			}
			else if (*y2 - *y1) {           //上边界和下边界
				float m = (float)(*x2 - *x1) / (*y2 - *y1);
				float ix = m * (m_window[i] - *y1) + *x1;
				if (i == 2) {	//上边界
					if (*y1 > *y2) {
						*x1 = ix;
						*y1 = m_window[i];
					}
					else {
						*x2 = ix;
						*y2 = m_window[i];
					}
				}
				else {		    //下边界
					if (*y1 < *y2) {
						*x1 = ix;
						*y1 = m_window[i];
					}
					else {
						*x2 = ix;
						*y2 = m_window[i];
					}
				}
			}
		}
	}
	return 1;
}

int Ccg2019WJY2DTransView::LineVisible(int* x1, int* y1, int* x2, int* y2)
{
	int pcode1, pcode2;
	// 为0表示位于框内部
	//| 0101 | 0100 | 0110 |
	//| 0001 | 0000 | 0010 |
	//| 1001 | 1000 | 1010 |
	pcode1 = pCode(x1, y1);
	pcode2 = pCode(x2, y2);

	if (!pcode1 && !pcode2)    return 1;     //完全可见
	if ((pcode1 & pcode2) != 0)  return 0;     //完全不可见
	if (pcode1 == 0) {
		float temp;
		temp = *x1;  *x1 = *x2;  *x2 = temp;
		temp = *y1;  *y1 = *y2;  *y2 = temp;
	}
	return 2;
}

int Ccg2019WJY2DTransView::pCode(int* x, int* y)
{
	int code = 0;
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();

	if (*x <= pDoc->m_wndLx)  code |= 1;//左
	if (*x >= pDoc->m_wndRx)  code |= 2;//右
	if (*y >= pDoc->m_wndRy)  code |= 4;//上
	if (*y <= pDoc->m_wndLy)  code |= 8;//下

	return code;
}

int Ccg2019WJY2DTransView::LineCross(int x1, int y1, int x2, int y2, int i)
{
	int visible1, visible2;

	visible1 = pVisible(x1, y1, i);
	visible2 = pVisible(x2, y2, i);

	if (visible1 != visible2) return 1;
	else                      return 0;

}

int Ccg2019WJY2DTransView::pVisible(int x, int y, int i)
{
	int visible = 0;
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();

	switch (i) {
	case 0: //左边界
		if (x >= pDoc->m_wndLx)  visible = 1; break;
	case 1: //右边界
		if (x <= pDoc->m_wndRx)  visible = 1; break;
	case 2: //上边界
		if (y <= pDoc->m_wndRy)  visible = 1; break;
	case 3: //下边界
		if (y >= pDoc->m_wndLy)  visible = 1; break;
	}
	return visible;
}

// Liang-Barsky Line Clip
int Ccg2019WJY2DTransView::LB_ClipLine(int* x1, int* y1, int* x2, int* y2)
{
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();
	float dx = *x1 - *x2, dy = *y1 - *y2;
	if (dy == 0) {	// 水平
		if (*x1 > * x2) {	//交换
			int temp = *x2;	*x2 = *x1;	*x1 = temp;
		}
		if (*y1 >= pDoc->m_wndRy || *y1 <= pDoc->m_wndLy)	//超出范围
			return 0;
		if ((*x1 >= pDoc->m_wndRx && *x2 >= pDoc->m_wndRx) || (*x1 <= pDoc->m_wndLx && *x2 <= pDoc->m_wndLx))	//超出范围
			return 0;
		if (*x1 <= pDoc->m_wndLx)
			*x1 = pDoc->m_wndLx;
		if (*x2 >= pDoc->m_wndRx)
			*x2 = pDoc->m_wndRx;
	}
	else if (dx == 0) {   //垂直
		if (*y1 > * y2) {	// 交换
			int temp = *y2;	*y2 = *y1;	*y1 = temp;
		}
		if (*x1 >= pDoc->m_wndRx || *x1 <= pDoc->m_wndLx)	//超出范围
			return 0;
		if ((*y1 >= pDoc->m_wndRy && *y2 >= pDoc->m_wndRy) || (*y1 <= pDoc->m_wndLy && *y2 <= pDoc->m_wndLy))	//超出范围
			return 0;
		if (*y1 <= pDoc->m_wndLy)
			*y1 = pDoc->m_wndLy;
		if (*y2 >= pDoc->m_wndRy)
			*y2 = pDoc->m_wndRy;
	}
	else {  //一般情况
		float x11, x22, y11, y22;

		int p1 = *x1 - *x2;
		int p2 = *x2 - *x1;
		int p3 = *y1 - *y2;
		int p4 = *y2 - *y1;

		int q1 = *x1 - pDoc->m_wndLx;
		int q2 = pDoc->m_wndRx - *x1;
		int q3 = *y1 - pDoc->m_wndLy;
		int q4 = pDoc->m_wndRy - *y1;

		float u1 = (float)q1 / p1;
		float u2 = (float)q2 / p2;
		float u3 = (float)q3 / p3;
		float u4 = (float)q4 / p4;

		int pk1, pk2, pk3, pk4;
		float uk1, uk2, uk3, uk4;
		if (p1 < 0) {
			pk1 = p1;
			uk1 = u1;
			pk3 = p2;
			uk3 = u2;
		}
		else {
			pk3 = p1;
			uk3 = u1;
			pk1 = p2;
			uk1 = u2;
		}
		if (p3 < 0) {
			pk2 = p3;
			uk2 = u3;
			pk4 = p4;
			uk4 = u4;
		}
		else {
			pk4 = p3;
			uk4 = u3;
			pk2 = p4;
			uk2 = u4;
		}

		float umax = max(0, max(uk1, uk2));
		float umin = min(1, min(uk3, uk4));

		if (umax > umin)
			return 0;
		else {
			x11 = *x1 + umax * (*x2 - *x1);
			y11 = *y1 + umax * (*y2 - *y1);
			x22 = *x1 + umin * (*x2 - *x1);
			y22 = *y1 + umin * (*y2 - *y1);

			*x1 = (int)(x11 + 0.5);
			*x2 = (int)(x22 + 0.5);
			*y1 = (int)(y11 + 0.5);
			*y2 = (int)(y22 + 0.5);
		}
		return 1;
	}
}

//绘制多边形
void Ccg2019WJY2DTransView::DisplayPolygon(CDC* pDC, int pointNumber,
	CPoint transPolygon[N], COLORREF rgbColor)
{
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();

	CPen newPen;
	CPen* oldPen;
	CPoint VPolygon[N];

	newPen.CreatePen(PS_SOLID, 2, rgbColor);
	oldPen = (CPen*)pDC->SelectObject(&newPen);

	for (int i = 0; i < pointNumber; i++) {
		VPolygon[i].x = m_wndWidth / 2 + transPolygon[i].x;
		VPolygon[i].y = m_wndHeight / 2 - transPolygon[i].y;
	}

	pDC->MoveTo(VPolygon[0]);
	for (int i = 1; i < pointNumber; i++) pDC->LineTo(VPolygon[i]);

	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}


void Ccg2019WJY2DTransView::TransPolygon(int pointNumber, CPoint spPolygon[N],
	CPoint transPolygon[N], float transMatrix[3][2])
{
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();

	for (int i = 0; i < pointNumber; i++) {
		transPolygon[i].x = spPolygon[i].x * transMatrix[0][0] +
							spPolygon[i].y * transMatrix[1][0] +
											 transMatrix[2][0];
		transPolygon[i].y = spPolygon[i].x * transMatrix[0][1] +
							spPolygon[i].y * transMatrix[1][1] +
											 transMatrix[2][1];
	}
}


// Sutherland-Hodgman Polygon Clip
int Ccg2019WJY2DTransView::ClipPolygon(int n, CPoint* tPoints, int* cn, CPoint* cPoints)
{
	int Nin, Nout, ix, iy, Sx, Sy;
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();

	Nin = n;
	for (int i = 0; i < 4; i++) {  // Along the window border
		*cn = 0;
		for (int j = 0; j < Nin; j++) {  // Scan polygon every point and line.
			if (j > 0) {
				if (LineCross(Sx, Sy, tPoints[j].x, tPoints[j].y, i)) {
					interSect(Sx, Sy, tPoints[j].x, tPoints[j].y, i, &ix, &iy);
					outPut(ix, iy, cn, cPoints);
				}
			}
			Sx = tPoints[j].x;
			Sy = tPoints[j].y;
			if (pVisible(Sx, Sy, i)) outPut(Sx, Sy, cn, cPoints);
		}

		Nin = *cn;
		if (*cn == 0) return 0;
		for (int j = 0; j < Nin; j++) {
			tPoints[j].x = cPoints[j].x;
			tPoints[j].y = cPoints[j].y;
		}

		if (cPoints[0].x != cPoints[Nin - 1].x ||
			cPoints[0].y != cPoints[Nin - 1].y) {

			tPoints[Nin].x = cPoints[Nin].x = cPoints[0].x;
			tPoints[Nin].y = cPoints[Nin].y = cPoints[0].y;

			Nin++;
			*cn = Nin;
		}
	}
	return 1;
}

void Ccg2019WJY2DTransView::interSect(int Sx, int  Sy, int Px, int Py,
	int  i, int* ix, int* iy)
{
	Ccg2019WJY2DTransDoc* pDoc = GetDocument();

	// Please fill in the right code below lines ...
	switch (i) {
	case 0: // Left
		*ix = pDoc->m_wndLx;
		*iy = (Sy - Py) * (pDoc->m_wndLx - Px) / (Sx - Px) + Py;
		break;
	case 1: // Right
		*ix = pDoc->m_wndRx;
		*iy = (Sy - Py) * (pDoc->m_wndRx - Px) / (Sx - Px) + Py;
		break;
	case 2: // Top
		*iy = pDoc->m_wndRy;
		*ix = (Sx - Px) * (pDoc->m_wndRy - Py) / (Sy - Py) + Px;
		break;
	case 3: // Bottom
		*iy = pDoc->m_wndLy;
		*ix = (Sx - Px) * (pDoc->m_wndLy - Py) / (Sy - Py) + Px;
		break;
	}
}

void Ccg2019WJY2DTransView::outPut(int x, int y, int* cn, CPoint* cPoints)
{
	cPoints[*cn].x = x;
	cPoints[*cn].y = y;
	(*cn)++;
}

// Weiler-Atherton Polygon Clip
int Ccg2019WJY2DTransView::WA_ClipPolygon(int n, CPoint* tPoints, int* cn, CPoint* cPoints)
{
	return 1;
}
