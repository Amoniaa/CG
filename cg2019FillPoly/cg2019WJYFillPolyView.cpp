
// cg2019WJYFillPolyView.cpp: Ccg2019WJYFillPolyView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cg2019WJYFillPoly.h"
#endif

#include "cg2019WJYFillPolyDoc.h"
#include "cg2019WJYFillPolyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <stack>
using namespace std;


// Ccg2019WJYFillPolyView

IMPLEMENT_DYNCREATE(Ccg2019WJYFillPolyView, CView)

BEGIN_MESSAGE_MAP(Ccg2019WJYFillPolyView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// Ccg2019WJYFillPolyView 构造/析构

Ccg2019WJYFillPolyView::Ccg2019WJYFillPolyView() noexcept
{
	// TODO: 在此处添加构造代码
	m_pNumbers = 0;
}

Ccg2019WJYFillPolyView::~Ccg2019WJYFillPolyView()
{
}

BOOL Ccg2019WJYFillPolyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Ccg2019WJYFillPolyView 绘图

void Ccg2019WJYFillPolyView::OnDraw(CDC* /*pDC*/)
{
	Ccg2019WJYFillPolyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}
// Ccg2019WJYFillPolyView 打印

BOOL Ccg2019WJYFillPolyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ccg2019WJYFillPolyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Ccg2019WJYFillPolyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Ccg2019WJYFillPolyView 诊断

#ifdef _DEBUG
void Ccg2019WJYFillPolyView::AssertValid() const
{
	CView::AssertValid();
}

void Ccg2019WJYFillPolyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ccg2019WJYFillPolyDoc* Ccg2019WJYFillPolyView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ccg2019WJYFillPolyDoc)));
	return (Ccg2019WJYFillPolyDoc*)m_pDocument;
}
#endif //_DEBUG


// Ccg2019WJYFillPolyView 消息处理程序
void Ccg2019WJYFillPolyView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (seedFlag) {
		seed_Point = point;
		ScanLineFill4(seed_Point.x, seed_Point.y, 0, m_pDC);
		seedFlag = false;
	}
	else if (m_pNumbers < N) {
		m_pAccord[m_pNumbers] = point;
		m_pNumbers++;

		m_mousePoint = point;
	}
	CView::OnLButtonDown(nFlags, point);
}


void Ccg2019WJYFillPolyView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pDC->MoveTo(m_pAccord[m_pNumbers - 1]);
	m_pDC->LineTo(m_pAccord[0]);

	m_pAccord[m_pNumbers] = m_pAccord[0];
	m_pNumbers++;

	Ccg2019WJYFillPolyDoc* pDoc = GetDocument();
	if (pDoc->m_displayMode == 0 ) {
		seedFlag = true;
	}
	else if (pDoc->m_displayMode == 1) {
		Fillpolygon(m_pNumbers, m_pAccord, m_pDC);//有序边表法填充
	}
	else if (pDoc->m_displayMode == 2) {
		PointFillpoly(m_pNumbers, m_pAccord, m_pDC);//逐点法填充
	}
	else if (pDoc->m_displayMode == 3) {
		Triangulation(m_pAccord, m_pNumbers - 1, m_pNumbers - 1);//三角剖分
	}
	else if (pDoc->m_displayMode == 4) {
		Fillpolygon(m_pNumbers, m_pAccord, m_pDC);//图案填充
	}
	else{
		Triangulation(m_pAccord, m_pNumbers - 1, m_pNumbers - 1);	//多边形顶点集合，当前多边形顶点数，多边形顶点数
	}
	
	m_pNumbers = 0;

	CView::OnLButtonDblClk(nFlags, point);
}


void Ccg2019WJYFillPolyView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pNumbers) {

		m_pDC->SetROP2(2);
		m_pDC->MoveTo(m_pAccord[m_pNumbers - 1]);
		m_pDC->LineTo(m_mousePoint);

		m_mousePoint = point;
		m_pDC->MoveTo(m_pAccord[m_pNumbers - 1]);
		m_pDC->LineTo(m_mousePoint);
	}

	CView::OnMouseMove(nFlags, point);
}


int Ccg2019WJYFillPolyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pDC = new CClientDC(this);

	return 0;
}

//图案
int m_patternData[12][12] = {
	{ 0,0,0,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,0,0,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 0,1,1,0,0,1,1,0,0,1,1,0 },
	{ 0,1,1,0,0,1,1,0,0,1,1,0 },
	{ 0,1,1,0,0,1,1,0,0,1,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0 }

};

//有序边表法填充
void Ccg2019WJYFillPolyView::Fillpolygon(int pNumbers, CPoint* points, CDC* pDC)
{

	m_edgeNumbers = 0;
	pLoadPolygon(pNumbers, points);   // Polygon Loading, calculates every edge's m_yMax[],m_yMin[],m_Xa[],m_Dx[]

	//求交边集范围，因为数组已经根据y值大小进行边的排序，所以end向后移动即代表有边进入，start向后移动，代表有边退出
	m_Begin = m_End = 0;
	m_Scan = (int)m_yMax[0];          //从顶向下扫描
	Include();                        //检查是否有边进入扫描线
	UpdateXvalue();                   //检查是否有边退出扫描线
	while (m_Begin != m_End) {
		pFillScan(pDC);
		m_Scan--;
		Include();
		UpdateXvalue();
	}
}

void Ccg2019WJYFillPolyView::pLoadPolygon(int pNumbers, CPoint* points)
{
	float x1, y1, x2, y2;

	x1 = points[0].x;    y1 = points[0].y + 0.5;
	for (int i = 1; i < pNumbers; i++) {
		x2 = points[i].x;    y2 = points[i].y + 0.5;
		if (abs(int(y2 - y1)) >= 0)	//水平线不做处理
		{
			pInsertLine(x1, y1, x2, y2);
			x1 = x2;      y1 = y2;
		}
		else
			x2 = x1;
	}
}

void Ccg2019WJYFillPolyView::pInsertLine(float x1, float y1, float x2, float y2)
{
	int i;
	float Ymax, Ymin;

	Ymax = (y2 > y1) ? y2 : y1;
	Ymin = (y2 < y1) ? y2 : y1;
	i = m_edgeNumbers;
	//根据y值的大小，进行排序插入，大的在前面
	while (i != 0 && m_yMax[i - 1] < Ymax) {
		m_yMax[i] = m_yMax[i - 1];
		m_yMin[i] = m_yMin[i - 1];
		m_Xa[i] = m_Xa[i - 1];
		m_Dx[i] = m_Dx[i - 1];
		i--;
	}
	m_yMax[i] = Ymax;
	m_yMin[i] = Ymin;
	if (y2 > y1) m_Xa[i] = x2;	//根据y大小确定Xa的值，y大的会先于扫描线相交
	else         m_Xa[i] = x1;

	m_Dx[i] = (x2 - x1) / (y2 - y1);	//斜率的倒数
	m_edgeNumbers++;
}

void Ccg2019WJYFillPolyView::Include()
{
	//end向后移动，找出所有边最高点y值大于当前扫描线的边，看是否有新的边进入交集
	while (m_End < m_edgeNumbers && m_yMax[m_End] >= m_Scan) {
		//有边进入，调整起始点位置，然后将Dx调整为位移量
		m_Xa[m_End] = m_Xa[m_End] + (-0.5) * m_Dx[m_End];
		m_Dx[m_End] = -m_Dx[m_End];
		m_End++;
	}
}

void Ccg2019WJYFillPolyView::UpdateXvalue()
{
	int i, start = m_Begin;

	for (i = start; i < m_End; i++) {
		if (m_Scan > m_yMin[i]) {
			//当前边没有退出，则移动x，然后在进行排序
			m_Xa[i] += m_Dx[i];
			pXsort(m_Begin, i);
		}
		else {
			//有边退出，更新数组，然后begin++
			for (int j = i; j > m_Begin; j--) {
				m_yMin[j] = m_yMin[j - 1];
				m_Xa[j] = m_Xa[j - 1];
				m_Dx[j] = m_Dx[j - 1];
			}
			m_Begin++;
		}
	}
}

void Ccg2019WJYFillPolyView::pXsort(int Begin, int i)
{
	float temp;

	while (i > Begin&& m_Xa[i] < m_Xa[i - 1]) {
		temp = m_Xa[i];   m_Xa[i] = m_Xa[i - 1];   m_Xa[i - 1] = temp;
		temp = m_Dx[i];   m_Dx[i] = m_Dx[i - 1];   m_Dx[i - 1] = temp;
		temp = m_yMin[i]; m_yMin[i] = m_yMin[i - 1]; m_yMin[i - 1] = temp;
		i--;
	}
}


void Ccg2019WJYFillPolyView::pFillScan(CDC* pDC)
{
	int x, y;
	Ccg2019WJYFillPolyDoc* pDoc = GetDocument();


	for (int i = m_Begin; i < m_End; i += 2) {
		
		if (pDoc->m_displayMode == 1) {
			pDC->MoveTo(m_Xa[i], m_Scan);
			pDC->LineTo(m_Xa[i + 1], m_Scan);
		}
		else if (pDoc->m_displayMode == 4) {	//图案填充
			y = m_Scan;
			for (int x = m_Xa[i]; x < m_Xa[i + 1]; x++)
				if (m_patternData[y % 12][x % 12])
					pDC->SetPixel(x, y, RGB(255, 0, 0));

		}
	}
}

//逐点法填充

void Ccg2019WJYFillPolyView::PointFillpoly(int pNumbers, CPoint *points, CDC *pDC)
{
 	BoxRect_t polyRect;
 
 	polyRect = getPolygonRect(pNumbers, points);
 
	m_pDC->MoveTo(polyRect.minX, polyRect.minY);

	m_pDC->LineTo(polyRect.minX, polyRect.maxY);
	m_pDC->LineTo(polyRect.maxX, polyRect.maxY);
	m_pDC->LineTo(polyRect.maxX, polyRect.minY);
	m_pDC->LineTo(polyRect.minX, polyRect.minY);

	CPoint testPoint;
	//从最小点到最大点一次判断，如果在该多边形内部，则进行填充
	for (testPoint.x = polyRect.minX; testPoint.x < polyRect.maxX; testPoint.x++)
		for (testPoint.y = polyRect.minY; testPoint.y < polyRect.maxY; testPoint.y++) {
			if (PointInPolygon(m_pNumbers, m_pAccord, testPoint))
				pDC->SetPixel(testPoint.x, testPoint.y, RGB(255, 255, 255));
		}
}

//得到该多边形的最大、最小的y、x值
BoxRect_t Ccg2019WJYFillPolyView::getPolygonRect(int pointNumOfPolygon, CPoint tarPolygon[])
{
	BoxRect_t boxRect;

	boxRect.minX = 50000;
	boxRect.minY = 50000;
	boxRect.maxX = -50000;
	boxRect.maxY = -50000;

	for (int i = 0; i < pointNumOfPolygon; i++) {
		if (tarPolygon[i].x < boxRect.minX) boxRect.minX = tarPolygon[i].x;
		if (tarPolygon[i].y < boxRect.minY) boxRect.minY = tarPolygon[i].y;
		if (tarPolygon[i].x > boxRect.maxX) boxRect.maxX = tarPolygon[i].x;
		if (tarPolygon[i].y > boxRect.maxY) boxRect.maxY = tarPolygon[i].y;
	}
	return boxRect;
}

//判断点是否位于区域内
BOOL Ccg2019WJYFillPolyView::PointInPolygon(int pointNumOfPolygon, CPoint tarPolygon[], CPoint testPoint)
{

	if (pointNumOfPolygon < 3)
		return false;

	bool  inSide = FALSE;
	float lineSlope, interSectX;
	int   i = 0, j = pointNumOfPolygon - 1;

	for (i = 0; i < pointNumOfPolygon; i++) {
		if ((tarPolygon[i].y < testPoint.y && tarPolygon[j].y >= testPoint.y ||
			tarPolygon[j].y < testPoint.y && tarPolygon[i].y >= testPoint.y) &&
			(tarPolygon[i].x <= testPoint.x || tarPolygon[j].x <= testPoint.x)) {
			if (tarPolygon[j].x != tarPolygon[i].x) {
				lineSlope = (float)(tarPolygon[j].y - tarPolygon[i].y) / (tarPolygon[j].x - tarPolygon[i].x);
				interSectX = (int)(tarPolygon[i].x + (testPoint.y - tarPolygon[i].y) / lineSlope);
			}
			else
				interSectX = tarPolygon[i].x;
			if (interSectX < testPoint.x)
				inSide = !inSide;
		}
		j = i;
	}

	return inSide;
}


//扫描线种子填充
int SetRP(int x, int y, COLORREF color, COLORREF mColor, CDC* pDC) {
	while (pDC->GetPixel(CPoint(x, y)) == mColor) {
		pDC->SetPixel(x, y, color);
		x++;
	}
	return x - 1;
}
int SetLP(int x, int y, COLORREF color, COLORREF mColor, CDC* pDC) {
	while (pDC->GetPixel(CPoint(x - 1, y)) == mColor) {
		pDC->SetPixel(--x, y, color);
	}
	return x + 1;
}
void NewLineSeed(std::stack<CPoint>* stk, int lx, int rx, int y, COLORREF color, COLORREF mColor, CDC* pDC) {
	int x, e;
	for (x = lx + 1, e = rx + 1; x < e; x++) {
		//找出每一个区间的最右像素，入栈
		if (pDC->GetPixel(CPoint(x, y)) != mColor) {
			if (pDC->GetPixel(CPoint(x - 1, y)) == mColor)
				stk->push(CPoint(x - 1, y));
		}
	}
	//把rx所在点入栈
	if (pDC->GetPixel(CPoint(x - 1, y)) == mColor)
		stk->push(CPoint(x - 1, y));
}
void Ccg2019WJYFillPolyView::ScanLineFill4(int x, int y, COLORREF color, CDC* pDC)
{
	int pRight, pLeft;
	std::stack<CPoint> stk;
	int mColor = pDC->GetPixel(x, y);	//给定种子

	stk.push(CPoint(x, y));
	while (!stk.empty()) {
		CPoint p = stk.top();	//栈顶像素出栈
		stk.pop();

		pRight = SetRP(p.x, p.y, color, mColor, pDC);	//向左向右进行填充
		pLeft = SetLP(p.x, p.y, color, mColor, pDC);

		//上下两条扫描线处理
		NewLineSeed(&stk, pLeft, pRight, p.y + 1, color, mColor, pDC);
		NewLineSeed(&stk, pLeft, pRight, p.y - 1, color, mColor, pDC);
	}
}


void Ccg2019WJYFillPolyView::FloodFill4(int x, int y, int fillColor, int oldColor, CDC* pDC)
{
	int current;
	current = pDC->GetPixel(x, y);
	if (current == oldColor) {
		pDC->SetPixel(x, y, fillColor);
		FloodFill4(x + 1, y, fillColor, oldColor, pDC);
		FloodFill4(x - 1, y, fillColor, oldColor, pDC);
		FloodFill4(x, y + 1, fillColor, oldColor, pDC);
		FloodFill4(x, y - 1, fillColor, oldColor, pDC);
	}
}

//多边形三角剖分
void Ccg2019WJYFillPolyView::Triangulation(CPoint* points, int pNumbers, int number)
{

	if (pNumbers == 3) {	//出口
		return;
	}

	int k, xMin = 100000;
	for (int j = 0; j < pNumbers; j++)	//找出当前多边形的最左侧顶点
	{
		if (points[j].x < xMin) {
			k = j;
			xMin = points[j].x;
		}
	}

	CPoint arry[3];
	arry[0] = points[k];	//最左侧顶点
	int next = (k + 1) % pNumbers;
	arry[1] = points[next];	//后一个顶点
	int pre = k - 1;
	if (pre < 0)
		pre += pNumbers;
	arry[2] = points[pre];	//前一个顶点

	//围成的该三角形内是否有其他顶点
	CPoint in_point[N];
	int in_number = 0, i = 0;
	xMin = 1000;
	for (int j = 0; j < pNumbers; j++)
	{
		if (j == k || j == next || j == pre)	//三角形的三个顶点不算
			continue;
		if (PointInPolygon(3, arry, points[j])) {	//找出在该三角形内的顶点，并找到其中的最左侧顶点
			in_point[in_number] = points[j];
			if (in_point[in_number].x < xMin) {
				i = j;
				xMin = in_point[in_number].x;
			}
			in_number++;
		}
	}
	if (in_number >= 1) {	//若存在，则链接三角形内的最左侧点，分割多边形，递归调用

		CPen pen(PS_SOLID, 1, RGB(255, 0, 0));//创建画笔对象
		CClientDC dc(this);
		CPen* pOldPen = dc.SelectObject(&pen);

		dc.MoveTo(arry[0].x, arry[0].y);	//连接线到该最左侧顶点
		dc.LineTo(points[i].x, points[i].y);
		dc.SelectObject(&pOldPen);
		

		CPoint arry_1[N], arry_2[N];	//分割成两个多边形
		int pNumbers_1 = 0, pNumbers_2 = 0;

		if (k > i) {
			int temp = i; i = k; k = temp;
		}

		for (int j = 0; j < pNumbers; j++)
		{
			if (j >= k && j <= i) {
				arry_1[pNumbers_1++] = points[j];
			}
			if (j <= k || j >= i) {
				arry_2[pNumbers_2++] = points[j];
			}
		}
		Triangulation(arry_1, pNumbers_1, pNumbers_1);
		Triangulation(arry_2, pNumbers_2, pNumbers_2);
	}
	else {	//若不存在，则分割该三角形，递归调用

		CPen pen(PS_SOLID, 1, RGB(255, 0, 0));//创建画笔对象
		CClientDC dc(this);
		CPen* pOldPen = dc.SelectObject(&pen);

		dc.MoveTo(arry[1].x, arry[1].y);	//剖分线
		dc.LineTo(arry[2].x, arry[2].y);
		dc.SelectObject(&pOldPen);
		
		for (int j = k; j < pNumbers - 1; j++)	//删除k顶点
		{
			points[j] = points[j + 1];
		}
		pNumbers--;

		Triangulation(points, pNumbers, number);
	}
}

//
//void Ccg2019WJYFillPolyView::SeedFill()
//{
//	BoxRect_t polyRect = GetPolygonRect(max_num, m_pAccord);
//	std::stack<CPoint> st1, st2;
//	painted = new bool[(wm_width + 1) * (wm_height + 1)];
//
//	memset(painted, 0, (wm_width + 1) * (wm_height + 1));
//	st1.push(m_mousePoint);
//	painted[m_mousePoint.y * wm_width + m_mousePoint.x] = true;
//
//	while (!st1.empty() || !st2.empty())
//	{
//		while (!st1.empty())
//		{
//			CPoint point = st1.top();
//			st1.pop();
//			SelectPoint(point, &st2);
//			m_pDC->SetPixel(point, RGB(255, 0, 0));
//
//			/*if (JudgeRect(point))
//				FillRect(point);
//			else
//				SearchAndFillPoint(point);*/
//		}
//		while (!st2.empty())
//		{
//			CPoint point = st2.top();
//			st2.pop();
//			SelectPoint(point, &st1);
//			m_pDC->SetPixel(point, RGB(255, 0, 0));
//
//			/*if (JudgeRect(point))
//				FillRect(point);
//			else
//				SearchAndFillPoint(point);*/
//		}
//	}
//}
//
//void Ccg2019WJYFillPolyView::SelectPoint(CPoint point, std::stack<CPoint>* st)
//{
//	if (!painted[point.y * wm_width + point.x + 1] && PointInPolygon(max_num, m_pAccord, CPoint(point.x + 1, point.y)))
//	{
//		st->push(CPoint(point.x + 1, point.y));
//		painted[point.y * wm_width + point.x + 1] = true;
//	}
//	if (!painted[(point.y + 1) * wm_width + point.x + 1] && PointInPolygon(max_num, m_pAccord, CPoint(point.x + 1, point.y + 1)))
//	{
//		st->push(CPoint(point.x + 1, point.y + 1));
//		painted[(point.y + 1) * wm_width + point.x + 1] = true;
//	}
//	if (!painted[(point.y - 1) * wm_width + point.x + 1] && PointInPolygon(max_num, m_pAccord, CPoint(point.x + 1, point.y - 1)))
//	{
//		st->push(CPoint(point.x + 1, point.y - 1));
//		painted[(point.y - 1) * wm_width + point.x + 1] = true;
//	}
//	if (!painted[(point.y + 1) * wm_width + point.x - 1] && PointInPolygon(max_num, m_pAccord, CPoint(point.x - 1, point.y + 1)))
//	{
//		st->push(CPoint(point.x - 1, point.y + 1));
//		painted[(point.y + 1) * wm_width + point.x - 1] = true;
//	}
//	if (!painted[point.y * wm_width + point.x - 1] && PointInPolygon(max_num, m_pAccord, CPoint(point.x - 1, point.y)))
//	{
//		st->push(CPoint(point.x - 1, point.y));
//		painted[point.y * wm_width + point.x - 1] = true;
//	}
//	if (!painted[(point.y - 1) * wm_width + point.x - 1] && PointInPolygon(max_num, m_pAccord, CPoint(point.x - 1, point.y - 1)))
//	{
//		st->push(CPoint(point.x - 1, point.y - 1));
//		painted[(point.y - 1) * wm_width + point.x - 1] = true;
//	}
//	if (!painted[(point.y + 1) * wm_width + point.x] && PointInPolygon(max_num, m_pAccord, CPoint(point.x, point.y + 1)))
//	{
//		st->push(CPoint(point.x, point.y + 1));
//		painted[(point.y + 1) * wm_width + point.x] = true;
//	}
//	if (!painted[(point.y - 1) * wm_width + point.x] && PointInPolygon(max_num, m_pAccord, CPoint(point.x, point.y - 1)))
//	{
//		st->push(CPoint(point.x, point.y - 1));
//		painted[(point.y - 1) * wm_width + point.x] = true;
//	}
//
//}
