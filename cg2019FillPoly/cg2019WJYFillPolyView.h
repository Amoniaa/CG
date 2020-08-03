
// cg2019WJYFillPolyView.h: Ccg2019WJYFillPolyView 类的接口
//

#pragma once

#define N 32

struct BoxRect_t {
	int minX;
	int minY;
	int maxX;
	int maxY;
};

class Ccg2019WJYFillPolyView : public CView
{
protected: // 仅从序列化创建
	Ccg2019WJYFillPolyView() noexcept;
	DECLARE_DYNCREATE(Ccg2019WJYFillPolyView)

// 特性
public:
	Ccg2019WJYFillPolyDoc* GetDocument() const;

	CClientDC* m_pDC;

	int m_pNumbers = 0, m_inNumbers = 0;                     // polygon input buffer
	CPoint m_pAccord[N], m_mousePoint, seed_Point;

	int m_Begin, m_End, m_edgeNumbers, m_Scan;
	float m_yMax[N], m_yMin[N], m_Xa[N], m_Dx[N];

	bool seedFlag = false;

// 操作
public:
	void Fillpolygon(int pNumbers, CPoint* points, CDC* pDC);

	void pLoadPolygon(int pNumbers, CPoint* points);
	void pInsertLine(float x1, float y1, float x2, float y2);
	void Include();
	void UpdateXvalue();
	void pXsort(int Begin, int i);
	void pFillScan(CDC* pDC);

	void PointFillpoly(int pNumbers, CPoint* points, CDC* pDC);
	BoxRect_t getPolygonRect(int pointNumOfPolygon, CPoint tarPolygon[]);
	BOOL PointInPolygon(int pointNumOfPolygon, CPoint tarPolygon[], CPoint testPoint);

	void ScanLineFill4(int x, int y, COLORREF color, CDC* pDC);
	void FloodFill4(int x, int y, int fillColor, int oldColor, CDC* pDC);
	/*void SeedFill();
	void SelectPoint(CPoint point, std::stack<CPoint>* st);*/


	void Triangulation(CPoint* points, int pNumbers, int number);

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
	virtual ~Ccg2019WJYFillPolyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // cg2019WJYFillPolyView.cpp 中的调试版本
inline Ccg2019WJYFillPolyDoc* Ccg2019WJYFillPolyView::GetDocument() const
   { return reinterpret_cast<Ccg2019WJYFillPolyDoc*>(m_pDocument); }
#endif

