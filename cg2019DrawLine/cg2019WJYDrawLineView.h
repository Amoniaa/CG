
// cg2019WJYDrawLineView.h: Ccg2019WJYDrawLineView 类的接口
//

#pragma once


class Ccg2019WJYDrawLineView : public CView
{
protected: // 仅从序列化创建
	Ccg2019WJYDrawLineView() noexcept;
	DECLARE_DYNCREATE(Ccg2019WJYDrawLineView)

	// 特性
public:
	Ccg2019WJYDrawLineDoc* GetDocument() const;
	int m_wndWidth, m_wndHeight;
	static float ddaError;
	static float bError;
	static float mpError;
	static float distance;
	static float ddaSmooth;
	static float bSmooth;
	static float mpSmooth;
	static float ddaRuntime;
	static float bRuntime;
	static float mpRuntime;
	static float getDdaError() { return ddaError; }
	static float getbError() { return bError; }
	static float getmpError() { return mpError; }
	static float getDdaRuntime() { return ddaRuntime; }
	static float getbRuntime() { return bRuntime; }
	static float getmpRuntime() { return mpRuntime; }
	static float getDdaSmooth() { return ddaSmooth; }
	static float getbSmooth() { return bSmooth; }
	static float getmpSmooth() { return mpSmooth; }

	// 操作
public:
	void DDAline(int x0, int y0, int x1, int y1, CDC* pDC);
	void Bline(int x0, int y0, int x1, int y1, CDC* pDC);
	void Midpoint(int x0, int y0, int x1, int y1, CDC* pDC);
	void MidpointAnalyse(int x0, int y0, int x1, int y1, CDC* pDC);
	void BCircle(int x0, int y0, int r, CDC* pDC);
	void BresenhamArc(float θ1, float θ2, float R, CDC* pDC);
	void ShortArc(float θ1, float θ2, float R, CDC* pDC);
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
	virtual ~Ccg2019WJYDrawLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



#ifndef _DEBUG  // cg2019WJYDrawLineView.cpp 中的调试版本
inline Ccg2019WJYDrawLineDoc* Ccg2019WJYDrawLineView::GetDocument() const
{
	return reinterpret_cast<Ccg2019WJYDrawLineDoc*>(m_pDocument);
}
#endif
