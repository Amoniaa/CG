
// cgRayTracerView.cpp : CcgRayTracerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cgRayTracer.h"
#endif

#include <chrono>
#include <cmath>
#include <cstdio>

#include "Random.h"
#include "RayTrace.h"
#include "JobSystem.h"
#include "cgRayTracerDoc.h"
#include "cgRayTracerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcgRayTracerView

IMPLEMENT_DYNCREATE(CcgRayTracerView, CView)

BEGIN_MESSAGE_MAP(CcgRayTracerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CcgRayTracerView 构造/析构

CcgRayTracerView::CcgRayTracerView()
{
	rayTracerDibImage = NULL;

	m_mitRayTracer = new CCgRayTrace();
	m_srtOMPRayTracer = new CCgSmallpt();
	m_srtPPMRayTracer = new CCgSmallptPPM();
}

CcgRayTracerView::~CcgRayTracerView()
{
	delete m_mitRayTracer;
	delete m_srtOMPRayTracer;
	delete m_srtPPMRayTracer;

	if (rayTracerDibImage) delete rayTracerDibImage;
}

BOOL CcgRayTracerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CcgRayTracerView 绘制

void CcgRayTracerView::OnDraw(CDC* /*pDC*/)
{
	CcgRayTracerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (pDoc->m_rayTraceNow) {
		switch (pDoc->m_rayTraceMethod) {// NULL;SmallptCuda;SmallptOmp;SmallPPMcuda;SmallPPMomp;MIT RayTrace;
		  case 0:   // Cuda smallpt  
			SmallptCudaRayTraceRender();  break;
		  case 1:   // OpenMP smallpt
			SmallptOmpRayTraceRender();  break;
		  case 2:   // Cuda smallpt with Processive PhotonMapping (PPM)
			SmallptPPMCudaRayTraceRender();
			break;
		  case 3:   // OpenMP smallpt with Processive PhotonMapping (PPM)
			SmallptPPMRayTraceRender();  break;
		  case 4:   // MIT raytrace
			MitRayTraceRender();  break;
		}
	}
}


// CcgRayTracerView 打印

BOOL CcgRayTracerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CcgRayTracerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CcgRayTracerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CcgRayTracerView 诊断

#ifdef _DEBUG
void CcgRayTracerView::AssertValid() const
{
	CView::AssertValid();
}

void CcgRayTracerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcgRayTracerDoc* CcgRayTracerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcgRayTracerDoc)));
	return (CcgRayTracerDoc*)m_pDocument;
}
#endif //_DEBUG


// CcgRayTracerView 消息处理程序
void CcgRayTracerView::CreateBitMap(int width, int height)
{
	for (int i = 0; i < sizeof(BITMAPINFOHEADER) + 16; i++) rayTracerBitMapBuffer[i] = 0;
	rayTracerBitMapHeader = (BITMAPINFO *)&rayTracerBitMapBuffer;
	rayTracerBitMapHeader->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	rayTracerBitMapHeader->bmiHeader.biPlanes = 1;
	rayTracerBitMapHeader->bmiHeader.biBitCount = 32;
	rayTracerBitMapHeader->bmiHeader.biCompression = BI_BITFIELDS;
	rayTracerBitMapHeader->bmiHeader.biWidth = width;
	rayTracerBitMapHeader->bmiHeader.biHeight = height;

	((unsigned long*)rayTracerBitMapHeader->bmiColors)[0] = 255 << 16;
	((unsigned long*)rayTracerBitMapHeader->bmiColors)[1] = 255 << 8;
	((unsigned long*)rayTracerBitMapHeader->bmiColors)[2] = 255;

}

// MIT RayTracer Render Processing Functions.
void CcgRayTracerView::MitRayTraceRender()
{
	static BOOL opBusy = FALSE;

	if (opBusy) return;

	opBusy = TRUE;

	CDC     *pDC;
	CRect    pDCRet;

	pDC = GetDC();
	GetClientRect(&pDCRet);

	// Clear BackGround of DC
	CBrush brBlack(RGB(0, 0, 0));
	pDC->FillRect(pDCRet, &brBlack);

	// Generate Bitmap Buffer
	if (rayTracerDibImage) delete rayTracerDibImage;

	m_rtImageWidth = 1280;
	m_rtImageHeight = 720;
	CreateBitMap(m_rtImageWidth, m_rtImageHeight);
	rayTracerDibImage = new _vPIXEL[m_rtImageWidth*m_rtImageHeight];

	// Calculate Ray Tracing result.
	DWORD  opStart, opFinish;
	CcgRayTracerDoc* pDoc = GetDocument();

	opStart = GetTickCount();        // 记录测试开始时间，单位是毫秒

	if (m_mitRayTracer) m_mitRayTracer->runRayTrace(rayTracerDibImage,pDoc->m_threadNum);

	CClientDC ClientDC(pDC->GetWindow());
	::StretchDIBits(ClientDC.m_hDC,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            rayTracerDibImage, rayTracerBitMapHeader,
		            DIB_RGB_COLORS, SRCCOPY);
	opFinish = GetTickCount();       // 记录测试结束时间，单位是毫秒

	pDoc->m_runTime = (opFinish - opStart) / 1000.0f;

	pDoc->UpdateAllViews(this);

	opBusy = FALSE;

	// Must remember to release prjPlaneCDC every time after using.
	ReleaseDC(pDC);
}

// smallptCuda RayTracer Render Processing Functions.
extern "C"  int smallptCudaRayTrace(int width, int height, unsigned int *bmpImage, int samples);

void CcgRayTracerView::SmallptCudaRayTraceRender()
{
	static BOOL opBusy = FALSE;

	if (opBusy) return;

	opBusy = TRUE;

	CDC     *pDC;
	CRect    pDCRet;

	pDC = GetDC();
	GetClientRect(&pDCRet);

	// Clear BackGround of DC
	CBrush brBlack(RGB(0, 0, 0));
	pDC->FillRect(pDCRet, &brBlack);

	// Generate Bitmap Buffer
	if (rayTracerDibImage) delete rayTracerDibImage;

	m_rtImageWidth = 512;
	m_rtImageHeight = 384;
	CreateBitMap(m_rtImageWidth, -m_rtImageHeight);
	rayTracerDibImage = new _vPIXEL[m_rtImageWidth*m_rtImageHeight];

	// Calculate Ray Tracing result.
	DWORD  opStart, opFinish;
	CcgRayTracerDoc* pDoc = GetDocument();

	opStart = GetTickCount();        // 记录测试开始时间，单位是毫秒

	smallptCudaRayTrace(m_rtImageWidth, m_rtImageHeight, rayTracerDibImage, pDoc->m_samplesNum);

	CClientDC ClientDC(pDC->GetWindow());
	int sx = pDCRet.right  / 2 - m_rtImageWidth  / 2;
	int sy = pDCRet.bottom / 2 - m_rtImageHeight / 2; 
	
	::StretchDIBits(ClientDC.m_hDC,
		            sx, sy, m_rtImageWidth, m_rtImageHeight,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            rayTracerDibImage, rayTracerBitMapHeader,
		            DIB_RGB_COLORS, SRCCOPY);
	opFinish = GetTickCount();       // 记录测试结束时间，单位是毫秒

	pDoc->m_runTime = (opFinish - opStart) / 1000.0f;

	pDoc->UpdateAllViews(this);

	opBusy = FALSE;

	// Must remember to release prjPlaneCDC every time after using.
	ReleaseDC(pDC);
}

// smallptOpenMP RayTracer Render Processing Functions.
void CcgRayTracerView::SmallptOmpRayTraceRender()
{
	static BOOL opBusy = FALSE;

	if (opBusy) return;

	opBusy = TRUE;

	CDC     *pDC;
	CRect    pDCRet;

	pDC = GetDC();
	GetClientRect(&pDCRet);

	// Clear BackGround of DC
	CBrush brBlack(RGB(0, 0, 0));
	pDC->FillRect(pDCRet, &brBlack);

	// Generate Bitmap Buffer
	if (rayTracerDibImage) delete rayTracerDibImage;

	m_rtImageWidth = 512;
	m_rtImageHeight = 384;
	CreateBitMap(m_rtImageWidth, -m_rtImageHeight);
	rayTracerDibImage = new _vPIXEL[m_rtImageWidth*m_rtImageHeight];

	// Calculate Ray Tracing result.
	DWORD  opStart, opFinish;
	CcgRayTracerDoc* pDoc = GetDocument();

	opStart = GetTickCount();        // 记录测试开始时间，单位是毫秒

	if (m_srtOMPRayTracer) 
		m_srtOMPRayTracer->smallptRayTrace(m_rtImageWidth, m_rtImageHeight, rayTracerDibImage,
			                               pDoc->m_samplesNum, pDoc->m_threadNum);

	CClientDC ClientDC(pDC->GetWindow());
	int sx = pDCRet.right / 2 - m_rtImageWidth / 2;
	int sy = pDCRet.bottom / 2 - m_rtImageHeight / 2;

	::StretchDIBits(ClientDC.m_hDC,
		            sx, sy, m_rtImageWidth, m_rtImageHeight,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            rayTracerDibImage, rayTracerBitMapHeader,
		            DIB_RGB_COLORS, SRCCOPY);
	opFinish = GetTickCount();       // 记录测试结束时间，单位是毫秒

	pDoc->m_runTime = (opFinish - opStart) / 1000.0f;

	pDoc->UpdateAllViews(this);

	opBusy = FALSE;

	// Must remember to release prjPlaneCDC every time after using.
	ReleaseDC(pDC);
}

// smallptPPMCuda RayTracer Render Processing Functions.
extern "C"  int smallptPPMCudaRayTrace(int width, int height, unsigned int *bmpImage, int samples);

void CcgRayTracerView::SmallptPPMCudaRayTraceRender()
{
	static BOOL opBusy = FALSE;

	if (opBusy) return;

	opBusy = TRUE;

	CDC     *pDC;
	CRect    pDCRet;

	pDC = GetDC();
	GetClientRect(&pDCRet);

	// Clear BackGround of DC
	CBrush brBlack(RGB(0, 0, 0));
	pDC->FillRect(pDCRet, &brBlack);

	// Generate Bitmap Buffer
	if (rayTracerDibImage) delete rayTracerDibImage;

	m_rtImageWidth = 512;
	m_rtImageHeight = 384;
	CreateBitMap(m_rtImageWidth, -m_rtImageHeight);
	rayTracerDibImage = new _vPIXEL[m_rtImageWidth*m_rtImageHeight];

	// Calculate Ray Tracing result.
	DWORD  opStart, opFinish;
	CcgRayTracerDoc* pDoc = GetDocument();

	opStart = GetTickCount();        // 记录测试开始时间，单位是毫秒

	smallptPPMCudaRayTrace(m_rtImageWidth, m_rtImageHeight, rayTracerDibImage, pDoc->m_samplesNum);

	CClientDC ClientDC(pDC->GetWindow());
	int sx = pDCRet.right / 2 - m_rtImageWidth / 2;
	int sy = pDCRet.bottom / 2 - m_rtImageHeight / 2;

	::StretchDIBits(ClientDC.m_hDC,
		            sx, sy, m_rtImageWidth, m_rtImageHeight,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            rayTracerDibImage, rayTracerBitMapHeader,
		            DIB_RGB_COLORS, SRCCOPY);

	opFinish = GetTickCount();       // 记录测试结束时间，单位是毫秒

	pDoc->m_runTime = (opFinish - opStart) / 1000.0f;

	pDoc->UpdateAllViews(this);

	opBusy = FALSE;

	// Must remember to release prjPlaneCDC every time after using.
	ReleaseDC(pDC);
}

// smallptPPM RayTracer Render Processing Functions.
void CcgRayTracerView::SmallptPPMRayTraceRender()
{
	static BOOL opBusy = FALSE;

	if (opBusy) return;

	opBusy = TRUE;

	CDC     *pDC;
	CRect    pDCRet;

	pDC = GetDC();
	GetClientRect(&pDCRet);

	// Clear BackGround of DC
	CBrush brBlack(RGB(0, 0, 0));
	pDC->FillRect(pDCRet, &brBlack);

	// Generate Bitmap Buffer
	if (rayTracerDibImage) delete rayTracerDibImage;

	m_rtImageWidth = 512;
	m_rtImageHeight = 384;
	CreateBitMap(m_rtImageWidth, m_rtImageHeight);
	rayTracerDibImage = new _vPIXEL[m_rtImageWidth*m_rtImageHeight];

	// Calculate Ray Tracing result.
	DWORD  opStart, opFinish;
	CcgRayTracerDoc* pDoc = GetDocument();

	opStart = GetTickCount();        // 记录测试开始时间，单位是毫秒

	if (m_srtPPMRayTracer) 
		m_srtPPMRayTracer->smallptPPMRayTrace(m_rtImageWidth, m_rtImageHeight, rayTracerDibImage, 
			                                  pDoc->m_samplesNum, pDoc->m_threadNum);

	CClientDC ClientDC(pDC->GetWindow());
	int sx = pDCRet.right / 2 - m_rtImageWidth / 2;
	int sy = pDCRet.bottom / 2 - m_rtImageHeight / 2;

	::StretchDIBits(ClientDC.m_hDC,
		            sx, sy, m_rtImageWidth, m_rtImageHeight,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            rayTracerDibImage, rayTracerBitMapHeader,
		            DIB_RGB_COLORS, SRCCOPY);
	opFinish = GetTickCount();       // 记录测试结束时间，单位是毫秒

	pDoc->m_runTime = (opFinish - opStart) / 1000.0f;

	pDoc->UpdateAllViews(this);

	opBusy = FALSE;

	// Must remember to release prjPlaneCDC every time after using.
	ReleaseDC(pDC);
}

