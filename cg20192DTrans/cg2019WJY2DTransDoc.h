
// cg2019WJY2DTransDoc.h: Ccg2019WJY2DTransDoc 类的接口
//


#pragma once

#define N 64
class Ccg2019WJY2DTransDoc : public CDocument
{
protected: // 仅从序列化创建
	Ccg2019WJY2DTransDoc() noexcept;
	DECLARE_DYNCREATE(Ccg2019WJY2DTransDoc)

// 特性
public:

	CPoint P1, P2;                 // Original Space Line
	CPoint TP1, TP2;               // Trans Space Line
	CPoint CP1, CP2;               // Clip Space Line
	float m_lineMatrix[3][2];
	// Liang-Barsky Line Space Model
	CPoint LB_P1, LB_P2;           // Original Space Line
	CPoint LB_TP1, LB_TP2;         // Trans Space Line
	CPoint LB_CP1, LB_CP2;         // Clip Space Line
	float m_LB_LineMatrix[3][2];
	// General Polygon Space Model
	int m_pointNumber;              // Original Space Polygon point number
	int m_clipPointNumber;          // Clip Space Polygon point number
	CPoint m_spPolygon[N];          // Original Space Polygon
	CPoint m_transPolygon[N];       // Trans Space Polygon
	CPoint m_clipPolygon[N];        // Clip Space Polygon
	float m_polygonMatrix[3][2];

	int m_transDir = 0;      // 0:-X      1:+X         2: -Y		3:+Y
	int m_transMode = 0;     // 0: Move   1: Rotate    2: Scale
	int m_transSelect = 0;   // 0: Line   1: Polygon   2: Window	3:LB_Line  4:WA_Polygon

	// Weiler-Atherton Polygon Space Model
	int m_WA_pointNumber;            // Original Space Polygon point number
	int m_WA_clipPointNumber;        // Clip Space Polygon point number
	CPoint m_WA_spPolygon[N];        // Original Space Polygon
	CPoint m_WA_transPolygon[N];     // Trans Space Polygon
	CPoint m_WA_clipPolygon[N];      // Clip Space Polygon
	float m_WA_polygonMatrix[3][2];

	BOOL m_lineVisible, m_polygonVisible;
	BOOL m_LB_lineVisible, m_WA_polygonVisible;

	int m_wndLx, m_wndLy, m_wndRx, m_wndRy;     // Space Window (Lx, Ly)-(Rx,Ry)


// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~Ccg2019WJY2DTransDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
