
// CGMITRayTraceView.h : CCGMITRayTraceView ��Ľӿ�
//

#pragma once
#include "CGMITRayTrace.h"
#include "RayTrace.h"
#include "smallpt.h"

typedef unsigned int _vPIXEL;

class CCGMITRayTraceView : public CView
{
protected: // �������л�����
	CCGMITRayTraceView();
	DECLARE_DYNCREATE(CCGMITRayTraceView)

// ����
public:
	CCGMITRayTraceDoc* GetDocument() const;
	// RayTracer Bitmap Buffer
	int m_threadsNum;
	unsigned int *rayTracerDibImage;
	BITMAPINFO* rayTracerBitMapHeader;
	int m_rtImageWidth, m_rtImageHeight;
	char rayTracerBitMapBuffer[sizeof(BITMAPINFO) + 16];

	// RayTrace Type: SmallptCuda;SmallptOmp;SmallPPMcuda;SmallPPMomp;MIT RayTrace;
	CCgRayTrace    *m_mitRayTracer;      // MIT RayTracer class
	CCgSmallpt     *m_srtOMPRayTracer;   // OpenMP smallpt
//	CCgSmallptPPM  *m_srtPPMRayTracer;   // OpenMP smallpt with Processive Photon Mapping(PPM)



// ����
public:
	void MitRayTraceRender();
	void SmallptOmpRayTraceRender();
	/*void SmallptCudaRayTraceRender();
	void SmallptPPMRayTraceRender();
	void SmallptPPMCudaRayTraceRender();*/

	void CreateBitMap(int width, int height);

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CCGMITRayTraceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CGMITRayTraceView.cpp �еĵ��԰汾
inline CCGMITRayTraceDoc* CCGMITRayTraceView::GetDocument() const
   { return reinterpret_cast<CCGMITRayTraceDoc*>(m_pDocument); }
#endif

