
// cg2019WJY2DTransDoc.cpp: Ccg2019WJY2DTransDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cg2019WJY2DTrans.h"
#endif

#include "cg2019WJY2DTransDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ccg2019WJY2DTransDoc

IMPLEMENT_DYNCREATE(Ccg2019WJY2DTransDoc, CDocument)

BEGIN_MESSAGE_MAP(Ccg2019WJY2DTransDoc, CDocument)
END_MESSAGE_MAP()


// Ccg2019WJY2DTransDoc 构造/析构

Ccg2019WJY2DTransDoc::Ccg2019WJY2DTransDoc() noexcept
{
	// General Line Space Model
	P1.x = 0;   P1.y = 0;
	P2.x = 200;   P2.y = 200;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++)
			if (i == j) m_lineMatrix[i][j] = 1.0f;
			else        m_lineMatrix[i][j] = 0.0f;

	// Liang-Barsky Line Space Model
	LB_P1.x = 30;   LB_P1.y = 30;
	LB_P2.x = 230;   LB_P2.y = 230;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++)
			if (i == j) m_LB_LineMatrix[i][j] = 1.0f;
			else        m_LB_LineMatrix[i][j] = 0.0f;

	// General Polygon Space Model
	m_pointNumber = 4;
	m_spPolygon[0].x = 0;      m_spPolygon[0].y = 0;
	m_spPolygon[1].x = 150;    m_spPolygon[1].y = 100;
	m_spPolygon[2].x = 200;    m_spPolygon[2].y = 0;
	m_spPolygon[3].x = 0;      m_spPolygon[3].y = 0;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++)
			if (i == j) m_polygonMatrix[i][j] = 1.0f;
			else        m_polygonMatrix[i][j] = 0.0f;

	// Weiler-Atherton Polygon Space Model
	m_WA_pointNumber = 6;
	m_WA_spPolygon[0].x = 0;      m_WA_spPolygon[0].y = 0;
	m_WA_spPolygon[1].x = 100;    m_WA_spPolygon[1].y = 150;
	m_WA_spPolygon[2].x = 150;    m_WA_spPolygon[2].y = 70;
	m_WA_spPolygon[3].x = 200;    m_WA_spPolygon[3].y = 150;
	m_WA_spPolygon[4].x = 250;    m_WA_spPolygon[4].y = 0;
	m_WA_spPolygon[5].x = 0;      m_WA_spPolygon[5].y = 0;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++)
			if (i == j) m_WA_polygonMatrix[i][j] = 1.0f;
			else        m_WA_polygonMatrix[i][j] = 0.0f;

	m_transDir = 0;      // 0:-X      1:+X         2: -Y         3:+Y
	m_transMode = 0;     // 0: Move   1: Rotate    2: Scale
	m_transSelect = 0;   // 0: Line   1: Polygon   2: Window  3:LB_Line  4:WA_Polygon

	m_wndLx = 0;
	m_wndLy = 0;
	m_wndRx = 150;
	m_wndRy = 150;

	m_lineVisible = 0;
	m_polygonVisible = 0;

}

Ccg2019WJY2DTransDoc::~Ccg2019WJY2DTransDoc()
{
}

BOOL Ccg2019WJY2DTransDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Ccg2019WJY2DTransDoc 序列化

void Ccg2019WJY2DTransDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void Ccg2019WJY2DTransDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void Ccg2019WJY2DTransDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void Ccg2019WJY2DTransDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Ccg2019WJY2DTransDoc 诊断

#ifdef _DEBUG
void Ccg2019WJY2DTransDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Ccg2019WJY2DTransDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Ccg2019WJY2DTransDoc 命令
