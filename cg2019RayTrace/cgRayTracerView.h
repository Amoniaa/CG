
// cgRayTracerView.h : CcgRayTracerView 类的接口
//

#pragma once

#include "smallpt.h"
#include "RayTrace.h"
#include "JobSystem.h"
#include "smallptPPM.h"
#include "RayTrace.h"

typedef unsigned int _vPIXEL;

class CcgRayTracerView : public CView
{
protected: // 仅从序列化创建
	CcgRayTracerView();
	DECLARE_DYNCREATE(CcgRayTracerView)

// 特性
public:
	CcgRayTracerDoc* GetDocument() const;

	// RayTracer Bitmap Buffer
	int m_threadsNum;
	uint32 *rayTracerDibImage;
	BITMAPINFO* rayTracerBitMapHeader;
	int m_rtImageWidth, m_rtImageHeight;
	char rayTracerBitMapBuffer[sizeof(BITMAPINFO) + 16];

	// RayTrace Type: SmallptCuda;SmallptOmp;SmallPPMcuda;SmallPPMomp;MIT RayTrace;
	CCgRayTrace    *m_mitRayTracer;      // MIT RayTracer class
	CCgSmallpt     *m_srtOMPRayTracer;   // OpenMP smallpt
	CCgSmallptPPM  *m_srtPPMRayTracer;   // OpenMP smallpt with Processive Photon Mapping(PPM)

// 操作
public:
	void MitRayTraceRender();
	void SmallptOmpRayTraceRender();
	void SmallptCudaRayTraceRender();
	void SmallptPPMRayTraceRender();
	void SmallptPPMCudaRayTraceRender();

	void CreateBitMap(int width, int height);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CcgRayTracerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // cgRayTracerView.cpp 中的调试版本
inline CcgRayTracerDoc* CcgRayTracerView::GetDocument() const
   { return reinterpret_cast<CcgRayTracerDoc*>(m_pDocument); }
#endif

