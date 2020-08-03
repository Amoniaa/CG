﻿
// cg2019WJYDrawLineDoc.cpp: Ccg2019WJYDrawLineDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cg2019WJYDrawLine.h"
#endif

#include "cg2019WJYDrawLineDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ccg2019WJYDrawLineDoc

IMPLEMENT_DYNCREATE(Ccg2019WJYDrawLineDoc, CDocument)

BEGIN_MESSAGE_MAP(Ccg2019WJYDrawLineDoc, CDocument)
END_MESSAGE_MAP()


// Ccg2019WJYDrawLineDoc 构造/析构

Ccg2019WJYDrawLineDoc::Ccg2019WJYDrawLineDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	m_displayMode = -1;
}

Ccg2019WJYDrawLineDoc::~Ccg2019WJYDrawLineDoc()
{
}

BOOL Ccg2019WJYDrawLineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Ccg2019WJYDrawLineDoc 序列化

void Ccg2019WJYDrawLineDoc::Serialize(CArchive& ar)
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
void Ccg2019WJYDrawLineDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void Ccg2019WJYDrawLineDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void Ccg2019WJYDrawLineDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Ccg2019WJYDrawLineDoc 诊断

#ifdef _DEBUG
void Ccg2019WJYDrawLineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Ccg2019WJYDrawLineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Ccg2019WJYDrawLineDoc 命令
