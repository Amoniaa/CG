
// cg2019WJYDrawLineView.cpp: Ccg2019WJYDrawLineView 类的实现
//


#include <ctime>
#include "math.h"
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cg2019WJYDrawLine.h"
#endif

#include "cg2019WJYDrawLineDoc.h"
#include "cg2019WJYDrawLineView.h"
#define PI 3.1415926
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

float Ccg2019WJYDrawLineView::ddaError = 0.0f;
float Ccg2019WJYDrawLineView::bError = 0.0f;
float Ccg2019WJYDrawLineView::mpError = 0.0f;
float Ccg2019WJYDrawLineView::distance = 0.0f;
float Ccg2019WJYDrawLineView::ddaSmooth = 0.0f;
float Ccg2019WJYDrawLineView::bSmooth = 0.0f;
float Ccg2019WJYDrawLineView::mpSmooth = 0.0f;
float Ccg2019WJYDrawLineView::ddaRuntime = 0.0f;
float Ccg2019WJYDrawLineView::bRuntime = 0.0f;
float Ccg2019WJYDrawLineView::mpRuntime = 0.0f;
// Ccg2019WJYDrawLineView

IMPLEMENT_DYNCREATE(Ccg2019WJYDrawLineView, CView)

BEGIN_MESSAGE_MAP(Ccg2019WJYDrawLineView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_WM_SIZE()
END_MESSAGE_MAP()

// Ccg2019WJYDrawLineView 构造/析构

Ccg2019WJYDrawLineView::Ccg2019WJYDrawLineView() noexcept
{
	// TODO: 在此处添加构造代码
	m_wndWidth = 0;
	m_wndHeight = 0;
}

Ccg2019WJYDrawLineView::~Ccg2019WJYDrawLineView()
{
}

BOOL Ccg2019WJYDrawLineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void Ccg2019WJYDrawLineView::OnDraw(CDC* /*pDC*/)
{
	Ccg2019WJYDrawLineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//绘制坐标系
	CDC* pDC = GetDC();
	pDC->MoveTo(0, m_wndHeight / 2);
	pDC->LineTo(m_wndWidth, m_wndHeight / 2);
	pDC->MoveTo(m_wndWidth / 2, 0);
	pDC->LineTo(m_wndWidth / 2, m_wndHeight);
	//绘图
	if (pDoc->m_displayMode == -1) {
	}
	else if (pDoc->m_displayMode == 0) {
		//DDA算法绘制直线
		DDAline(0, 0, 400, 150, pDC);
		DDAline(0, 0, 150, 400, pDC);
		DDAline(0, 0, -400, 150, pDC);
		DDAline(0, 0, -150, 400, pDC);
		DDAline(0, 0, 400, -150, pDC);
		DDAline(0, 0, 150, -400, pDC);
		DDAline(0, 0, -400, -150, pDC);
		DDAline(0, 0, -150, -400, pDC);
	}
	else if (pDoc->m_displayMode == 1) {
		//B算法绘制直线
		Bline(0, 0, 400, 150, pDC);
		Bline(0, 0, 150, 400, pDC);
		Bline(0, 0, -400, 150, pDC);
		Bline(0, 0, -150, 400, pDC);
		Bline(0, 0, 400, -150, pDC);
		Bline(0, 0, 150, -400, pDC);
		Bline(0, 0, -400, -150, pDC);
		Bline(0, 0, -150, -400, pDC);
	}
	else if (pDoc->m_displayMode == 2) {
		//中点分割算法绘制直线
		Midpoint(0, 0, 400, 150, pDC);
		Midpoint(0, 0, 150, 400, pDC);
		Midpoint(0, 0, -400, 150, pDC);
		Midpoint(0, 0, -150, 400, pDC);
		Midpoint(0, 0, 400, -150, pDC);
		Midpoint(0, 0, 150, -400, pDC);
		Midpoint(0, 0, -400, -150, pDC);
		Midpoint(0, 0, -150, -400, pDC);
	}
	else if (pDoc->m_displayMode == 3) {
		//对比并误差分析
		DWORD begin = 0;
		DWORD end = 0;
		DWORD time = 0;

		begin = GetTickCount64();
		DDAline(0, 0, 400, 150, pDC);
		DDAline(0, 0, 150, 400, pDC);
		DDAline(0, 0, -400, 150, pDC);
		DDAline(0, 0, -150, 400, pDC);
		DDAline(0, 0, 400, -150, pDC);
		DDAline(0, 0, 150, -400, pDC);
		DDAline(0, 0, -400, -150, pDC);
		DDAline(0, 0, -150, -400, pDC);
		end = GetTickCount64();
		time = end - begin;
		ddaRuntime = time;

		Sleep(500);

		begin = GetTickCount64();
		Bline(0, 0, 400, 150, pDC);
		Bline(0, 0, 150, 400, pDC);
		Bline(0, 0, -400, 150, pDC);
		Bline(0, 0, -150, 400, pDC);
		Bline(0, 0, 400, -150, pDC);
		Bline(0, 0, 150, -400, pDC);
		Bline(0, 0, -400, -150, pDC);
		Bline(0, 0, -150, -400, pDC);
		end = GetTickCount64();
		time = end - begin;
		bRuntime = time;

		Sleep(500);

		begin = GetTickCount64();
		MidpointAnalyse(0, 0, 400, 150, pDC);
		MidpointAnalyse(0, 0, 150, 400, pDC);
		MidpointAnalyse(0, 0, -400, 150, pDC);
		MidpointAnalyse(0, 0, -150, 400, pDC);
		MidpointAnalyse(0, 0, 400, -150, pDC);
		MidpointAnalyse(0, 0, 150, -400, pDC);
		MidpointAnalyse(0, 0, -400, -150, pDC);
		MidpointAnalyse(0, 0, -150, -400, pDC);
		end = GetTickCount64();
		time = end - begin;
		mpRuntime = time;

		pDoc->UpdateAllViews(this);
	}
	else if (pDoc->m_displayMode == 4) {
		//B算法绘制圆
		BCircle(m_wndWidth / 2, m_wndHeight / 2 ,200, pDC);
	}
	else if (pDoc->m_displayMode == 5) {
		//BresenhamArc(PI / 2, 3 * PI / 4, 200, pDC);
		BresenhamArc(PI / 4, PI / 4, 50, pDC);
		BresenhamArc(PI / 4, -PI / 4, 100, pDC);
		BresenhamArc(PI / 4, 3 * PI / 2, 150, pDC);
		BresenhamArc(PI / 4, 5 * PI / 4, 200, pDC);
		BresenhamArc(PI / 4, PI, 250, pDC);
		BresenhamArc(PI / 4, 3 * PI / 4, 300, pDC);
		BresenhamArc(PI / 4, PI / 2, 350, pDC);
		BresenhamArc(PI / 4, PI / 3, 400, pDC);
	}
	
	ReleaseDC(pDC);
}

// Ccg2019WJYDrawLineView 打印

BOOL Ccg2019WJYDrawLineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ccg2019WJYDrawLineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Ccg2019WJYDrawLineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Ccg2019WJYDrawLineView 诊断

#ifdef _DEBUG
void Ccg2019WJYDrawLineView::AssertValid() const
{
	CView::AssertValid();
}

void Ccg2019WJYDrawLineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ccg2019WJYDrawLineDoc* Ccg2019WJYDrawLineView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ccg2019WJYDrawLineDoc)));
	return (Ccg2019WJYDrawLineDoc*)m_pDocument;
}
#endif //_DEBUG


void Ccg2019WJYDrawLineView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// 记录窗体视口尺寸
	m_wndWidth = cx;
	m_wndHeight = cy;
}

//DDA算法
void Ccg2019WJYDrawLineView::DDAline(int x0, int y0, int x1, int y1, CDC* pDC)
{
	//误差参数
	float distance = 0.0f; //生成点距理想直线的距离
	float kaverage = 0.0f; //生成点和起点连线的斜率平均值
	float xp, yp; //垂足点的坐标

	int step = abs(x1 - x0) >= abs(y1 - y0) ? abs(x1 - x0) : abs(y1 - y0);
	float dx = (float)(x1 - x0) / step;
	float dy = (float)(y1 - y0) / step;
	float m = (float)(y1 - y0) / (float)(x1 - x0);
	float x = x0 + 0.5f;
	float y = y0 + 0.5f;
	for (int i = 0; i < step; i++)
	{
		pDC->SetPixel((int)x + m_wndWidth / 2, (int)m_wndHeight / 2 - y, RGB(255, 0, 0));

		//计算垂足坐标
		xp = (y - y0 + x / m + m * x0) / (m + 1 / m);
		yp = -1 / m * (xp - x) + y;
		distance += sqrtf((x - xp) * (x - xp) + (y - yp) * (y - yp)); //生成点距离直线的距离累加值
		if (x != x0)
			kaverage += fabs(y - y0) / fabs(x - x0); //计算生成点和起点连线的斜率平均值

		x += dx ;
		y += dy;
	}
	ddaError = distance / step; //误差为距离的平均值
	ddaSmooth = fabs(kaverage / step - m); //平滑度为斜率的平均值
}

//B算法
void Ccg2019WJYDrawLineView::Bline(int x0, int y0, int x1, int y1, CDC* pDC) {

	//误差参数
	float distance = 0.0f; //生成点距理想直线的距离
	float kaverage = 0.0f; //生成点和起点连线的斜率平均值
	float xp, yp; //垂足点的坐标
	float m = (float)(y1 - y0) / (float)(x1 - x0);

	int x = x0, y = y0, dx = x1 - x0, dy = y1 - y0;
	int max = (abs(dy) > abs(dx)) ? abs(dy) : abs(dx);
	int min = (abs(dy) > abs(dx)) ? abs(dx) : abs(dy);
	float e = 2 * min - max;
	for (int i = 0; i < max; i++)
	{
		pDC->SetPixel((int)x + m_wndWidth / 2, (int)m_wndHeight / 2 - y, RGB(0, 0, 255));
		
		//计算垂足坐标
		xp = (y - y1 + x / m + m * x1) / (m + 1 / m);
		yp = -1 / m * (xp - x) + y;
		distance += sqrtf((x - xp) * (x - xp) + (y - yp) * (y - yp)); //生成点距离直线的距离累加值
		if (x != x0)
			kaverage += (float)fabs(y - y0) / (float)fabs(x - x0); //计算生成点和起点连线的斜率平均值

		if (e >= 0)
		{
			e = e - 2 * max;
			(abs(dy) > abs(dx)) ? (dx >= 0 ? x++ : x--) : (dy >= 0 ? y++ : y--);
		}
		e += 2 * min;
		(abs(dy) > abs(dx)) ? (dy >= 0 ? y++ : y--) : (dx >= 0 ? x++ : x--);
	}

	bError = distance / max; //误差为距离的平均值
	bSmooth = fabs(kaverage / max - m); //平滑度为斜率的平均值
}

//中点分割算法
void Ccg2019WJYDrawLineView::Midpoint(int x0, int y0, int x1, int y1, CDC* pDC) {
	if (abs(x0 - x1) <= 1 && abs(y0 - y1) <= 1) {
		pDC->SetPixel((int)((x0 + x1) / 2 + 0.5) + m_wndWidth / 2, (int)m_wndHeight / 2 - ((y0 + y1) / 2 + 0.5), RGB(0, 255, 0));	}
	else {Midpoint(x0, y0, (x0 + x1) / 2, (y0 + y1) / 2, pDC);
		Midpoint((x0 + x1) / 2, (y0 + y1) / 2, x1, y1, pDC);}
}

//中点分割算法误差分析
void Ccg2019WJYDrawLineView::MidpointAnalyse(int x0, int y0, int x1, int y1, CDC* pDC) {

	//误差参数
	float distance = 0.0f; //生成点距理想直线的距离
	float kaverage = 0.0f; //生成点和起点连线的斜率平均值
	float xp, yp; //垂足点的坐标
	static int x00 = x0, y00 = y0, x11 = x1, y11 = y1; //记录起始点的坐标
	float m = (float)(y11 - y00) / (float)(x11 - x00);
	int dx = x11 - x00, dy = y11 - y00;
	int max = (abs(dy) > abs(dx)) ? abs(dy) : abs(dx);

	if (abs(x0 - x1) <= 1 && abs(y0 - y1) <= 1) {
		pDC->SetPixel((int)((x0 + x1) / 2 + 0.5) + m_wndWidth / 2, (int)m_wndHeight / 2 - ((y0 + y1) / 2 + 0.5), RGB(0, 255, 0));
		
		//计算垂足坐标
		xp = (y00 - y1 + x00 / m + m * x1) / (m + 1 / m);
		yp = -1 / m * (xp - x00) + y00;
		distance += sqrtf((x00 - xp) * (x00 - xp) + (y00 - yp) * (y00 - yp));  //生成点距离直线的距离累加值
		kaverage += (float)fabs((y0 + y1) / 2 + 0.5 - y0) / (float)fabs((x0 + x1) / 2 + 0.5 - x0); //计算生成点和起点连线的斜率平均值
		mpSmooth = fabs(kaverage / max - m); //误差为距离的平均值
		mpError = distance / max; //平滑度为斜率的平均值
	}
	else {
		MidpointAnalyse(x0, y0, (x0 + x1) / 2, (y0 + y1) / 2, pDC);
		MidpointAnalyse((x0 + x1) / 2, (y0 + y1) / 2, x1, y1, pDC);
	}
}

void Ccg2019WJYDrawLineView::BCircle(int x0, int y0, int r, CDC* pDC) {

	int x, y, d1, d2, direction, d;
	x = 0;
	y = r;
	d = 2 * (1 - r);
	while (y >= 0) {
		pDC->SetPixel(x0 + x, y0 + y, RGB(0, 0, 255));
		pDC->SetPixel(x0 - x, y0 + y, RGB(0, 0, 255));
		pDC->SetPixel(x0 - x, y0 - y, RGB(0, 0, 255));
		pDC->SetPixel(x0 + x, y0 - y, RGB(0, 0, 255));
		if (d < 0) {
			d1 = 2 * (d + y) - 1;
			if (d1 < 0) direction = 1;
			else		direction = 2;
		}
		else if (d > 0) {
			d2 = 2 * (d - x) - 1;
			if (d2 < 0) direction = 2;
			else		direction = 3;
		}
		else direction = 3;
		switch (direction) 
		{
			case 1:
				x++;
				d += 2 * x + 1;
				break;
			case 2:
				x++;
				y--;
				d += 2 * (x - y + 1);
				break;
			case 3:
				y--;
				d += -2 * y + 1;
				break;
		}
	}
}



void Ccg2019WJYDrawLineView::ShortArc(float θ1, float θ2, float R, CDC* pDC) {

	float θa, θb;
	int flag = 0;
	int x, y, d1, d2, direction, d;
	if (cos(θ1) >= 0 && sin(θ1) >= 0 && cos(θ2) >= 0 && sin(θ2) >= 0)
	{
		flag = 1;
		θa = θ1;
		θb = θ2;
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) >= 0 && cos(θ2) - 0 <= 1e-6 && sin(θ2) >= 0)
	{
		flag = 2;
		θa = PI - θ2;
		θb = PI - θ1;
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) - 0 <= 1e-6 && cos(θ2) - 0 <= 1e-6 && sin(θ2) - 0 <= 1e-6)
	{
		flag = 3;
		θa = θ1 - PI;
		θb = θ2 - PI;
	}
	else if (cos(θ1) >= 0 && sin(θ1) - 0 <= 1e-6 && cos(θ2) >= 0 && sin(θ2) - 0 <= 1e-6)
	{
		flag = 4;
		θa = 2 * PI - θ2;
		θb = 2 * PI - θ1;
	}

	x = R * cos(θa);
	y = R * sin(θa);
	d = (x - 1) * (x - 1) + (y + 1) * (y + 1) - R * R;
	while (abs(x) >= R * fabs(cos(θb))){
		if (flag == 1)
			pDC->SetPixel(m_wndWidth / 2 + x, m_wndHeight / 2 - y, RGB(0, 0, 255));
		if (flag == 2)
			pDC->SetPixel(m_wndWidth / 2 - x, m_wndHeight / 2 - y, RGB(0, 0, 255));
		if (flag == 3)
			pDC->SetPixel(m_wndWidth / 2 - x, m_wndHeight / 2 + y, RGB(0, 0, 255));
		if (flag == 4)
			pDC->SetPixel(m_wndWidth / 2 + x, m_wndHeight / 2 + y, RGB(0, 0, 255));
		if (d < 0) {
			d1 = 2 * (d + y) - 1;
			if (d1 < 0) direction = 1;
			else		direction = 2;
		}
		else if (d > 0) {
			d2 = 2 * (d - x) - 1;
			if (d2 < 0) direction = 2;
			else		direction = 3;
		}
		else direction = 3;
		switch (direction)
		{
		case 1:
			y++;
			d += 2 * y + 1;
			break;
		case 2:
			y++;
			x--;
			d += 2 * (y - x + 1);
			break;
		case 3:
			x--;
			d += -2 * x + 1;
			break;
		}
	}

}


void Ccg2019WJYDrawLineView::BresenhamArc(float θ1, float θ2, float R, CDC* pDC) {

	if (θ1 == θ2)
	{
		ShortArc(0, PI / 2, R, pDC);
		ShortArc(PI / 2, PI, R, pDC);
		ShortArc(PI, 3 * PI / 2, R, pDC);
		ShortArc(3 * PI / 2, 0, R, pDC);
	}
	else if (cos(θ1) >= 0 && sin(θ1) >= 0 && cos(θ2) >= 0 && sin(θ2) >= 0)
	{
		ShortArc(θ1, θ2, R, pDC);
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) >= 0 && cos(θ2) - 0 <= 1e-6 && sin(θ2) >= 0)
	{
		ShortArc(θ1, θ2, R, pDC);
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) - 0 <= 1e-6 && cos(θ2) - 0 <= 1e-6 && sin(θ2) - 0 <= 1e-6)
	{
		ShortArc(θ1, θ2, R, pDC);
	}
	else if (cos(θ1) >= 0 && sin(θ1) - 0 <= 1e-6 && cos(θ2) >= 0 && sin(θ2) - 0 <= 1e-6)
	{
		ShortArc(θ1, θ2, R, pDC);
	}

	else if (cos(θ1) >= 0 && sin(θ1) >= 0 && cos(θ2) - 0 <= 1e-6 && sin(θ2) >= 0)
	{
		ShortArc(θ1, PI / 2, R, pDC);
		ShortArc(PI / 2, θ2, R, pDC);
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) >= 0 && cos(θ2) - 0 <= 1e-6 && sin(θ2) - 0 <= 1e-6)
	{
		ShortArc(θ1, PI, R, pDC);
		ShortArc(PI, θ2, R, pDC);
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) - 0 <= 1e-6 && cos(θ2) >= 0 && sin(θ2) - 0 <= 1e-6)
	{
		ShortArc(θ1, 3 * PI / 2, R, pDC);
		ShortArc(3 * PI / 2, θ2, R, pDC);
	}
	else if (cos(θ1) >= 0 && sin(θ1) - 0 <= 1e-6 && cos(θ2) - 0 >= 0 && sin(θ2) >= 0)
	{
		ShortArc(θ1, 0, R, pDC);
		ShortArc(0, θ2, R, pDC);
	}

	else if (cos(θ1) >= 0 && sin(θ1) >= 0 && cos(θ2) - 0 <= 1e-6 && sin(θ2) - 0 <= 1e-6)
	{
		ShortArc(θ1, PI / 2, R, pDC);
		ShortArc(PI / 2, PI, R, pDC);
		ShortArc(PI, θ2, R, pDC);
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) >= 0 && cos(θ2) - 0 >= 0 && sin(θ2) - 0 <= 1e-6)
	{
		ShortArc(θ1, PI, R, pDC);
		ShortArc(PI, 3 * PI / 2, R, pDC);
		ShortArc(3 * PI / 2, θ2, R, pDC);
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) - 0 <= 1e-6 && cos(θ2) >= 0 && sin(θ2) >= 0)
	{
		ShortArc(θ1, 3 * PI / 2, R, pDC);
		ShortArc(3 * PI / 2, 0, R, pDC);
		ShortArc(0, θ2, R, pDC);
	}
	else if (cos(θ1) >= 0 && sin(θ1) - 0 <= 1e-6 && cos(θ2) <= 1e-6 && sin(θ2) >= 0)
	{
		ShortArc(θ1, 0, R, pDC);
		ShortArc(0, PI / 2, R, pDC);
		ShortArc(PI / 2, θ2, R, pDC);
	}

	else if (cos(θ1) >= 0 && sin(θ1) >= 0 && cos(θ2) >= 0 && sin(θ2) - 0 <= 1e-6)
	{
		ShortArc(θ1, PI / 2, R, pDC);
		ShortArc(PI / 2, PI, R, pDC);
		ShortArc(PI, 3 * PI / 2, R, pDC);
		ShortArc(3 * PI / 2, θ2, R, pDC);
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) >= 0 && cos(θ2) - 0 >= 0 && sin(θ2) >= 0)
	{
		ShortArc(θ1, PI, R, pDC);
		ShortArc(PI, 3 * PI / 2, R, pDC);
		ShortArc(3 * PI / 2, 0, R, pDC);
		ShortArc(0, θ2, R, pDC);
	}
	else if (cos(θ1) - 0 <= 1e-6 && sin(θ1) - 0 <= 1e-6 && cos(θ2) - 0 <= 1e-6 && sin(θ2) >= 0)
	{
		ShortArc(θ1, 3 * PI / 2, R, pDC);
		ShortArc(3 * PI / 2, 0, R, pDC);
		ShortArc(0, PI / 2, R, pDC);
		ShortArc(PI / 2, θ2, R, pDC);
	}
	else if (cos(θ1) >= 0 && sin(θ1) - 0 <= 1e-6 && cos(θ2) <= 1e-6 && sin(θ2) <= 1e-6)
	{
		ShortArc(θ1, 0, R, pDC);
		ShortArc(0, PI / 2, R, pDC);
		ShortArc(PI / 2, PI, R, pDC);
		ShortArc(PI, θ2, R, pDC);
	}
	
}
