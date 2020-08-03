
// cgRayTracerView.cpp : CcgRayTracerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CcgRayTracerView ����/����

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CcgRayTracerView ����

void CcgRayTracerView::OnDraw(CDC* /*pDC*/)
{
	CcgRayTracerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// CcgRayTracerView ��ӡ

BOOL CcgRayTracerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CcgRayTracerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CcgRayTracerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CcgRayTracerView ���

#ifdef _DEBUG
void CcgRayTracerView::AssertValid() const
{
	CView::AssertValid();
}

void CcgRayTracerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcgRayTracerDoc* CcgRayTracerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcgRayTracerDoc)));
	return (CcgRayTracerDoc*)m_pDocument;
}
#endif //_DEBUG


// CcgRayTracerView ��Ϣ�������
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

	opStart = GetTickCount();        // ��¼���Կ�ʼʱ�䣬��λ�Ǻ���

	if (m_mitRayTracer) m_mitRayTracer->runRayTrace(rayTracerDibImage,pDoc->m_threadNum);

	CClientDC ClientDC(pDC->GetWindow());
	::StretchDIBits(ClientDC.m_hDC,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            rayTracerDibImage, rayTracerBitMapHeader,
		            DIB_RGB_COLORS, SRCCOPY);
	opFinish = GetTickCount();       // ��¼���Խ���ʱ�䣬��λ�Ǻ���

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

	opStart = GetTickCount();        // ��¼���Կ�ʼʱ�䣬��λ�Ǻ���

	smallptCudaRayTrace(m_rtImageWidth, m_rtImageHeight, rayTracerDibImage, pDoc->m_samplesNum);

	CClientDC ClientDC(pDC->GetWindow());
	int sx = pDCRet.right  / 2 - m_rtImageWidth  / 2;
	int sy = pDCRet.bottom / 2 - m_rtImageHeight / 2; 
	
	::StretchDIBits(ClientDC.m_hDC,
		            sx, sy, m_rtImageWidth, m_rtImageHeight,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            rayTracerDibImage, rayTracerBitMapHeader,
		            DIB_RGB_COLORS, SRCCOPY);
	opFinish = GetTickCount();       // ��¼���Խ���ʱ�䣬��λ�Ǻ���

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

	opStart = GetTickCount();        // ��¼���Կ�ʼʱ�䣬��λ�Ǻ���

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
	opFinish = GetTickCount();       // ��¼���Խ���ʱ�䣬��λ�Ǻ���

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

	opStart = GetTickCount();        // ��¼���Կ�ʼʱ�䣬��λ�Ǻ���

	smallptPPMCudaRayTrace(m_rtImageWidth, m_rtImageHeight, rayTracerDibImage, pDoc->m_samplesNum);

	CClientDC ClientDC(pDC->GetWindow());
	int sx = pDCRet.right / 2 - m_rtImageWidth / 2;
	int sy = pDCRet.bottom / 2 - m_rtImageHeight / 2;

	::StretchDIBits(ClientDC.m_hDC,
		            sx, sy, m_rtImageWidth, m_rtImageHeight,
		            0, 0, m_rtImageWidth, m_rtImageHeight,
		            rayTracerDibImage, rayTracerBitMapHeader,
		            DIB_RGB_COLORS, SRCCOPY);

	opFinish = GetTickCount();       // ��¼���Խ���ʱ�䣬��λ�Ǻ���

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

	opStart = GetTickCount();        // ��¼���Կ�ʼʱ�䣬��λ�Ǻ���

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
	opFinish = GetTickCount();       // ��¼���Խ���ʱ�䣬��λ�Ǻ���

	pDoc->m_runTime = (opFinish - opStart) / 1000.0f;

	pDoc->UpdateAllViews(this);

	opBusy = FALSE;

	// Must remember to release prjPlaneCDC every time after using.
	ReleaseDC(pDC);
}

