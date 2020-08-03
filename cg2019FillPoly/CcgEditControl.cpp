// CcgEditControl.cpp: 实现文件
//

#include "pch.h"
#include "cg2019WJYFillPoly.h"
#include "CcgEditControl.h"
#include "cg2019WJYFillPolyDoc.h"
#include "cg2019WJYFillPolyView.h"


// CcgEditControl

IMPLEMENT_DYNCREATE(CcgEditControl, CFormView)

CcgEditControl::CcgEditControl()
	: CFormView(IDD_EDITCONTROL)
{

}

CcgEditControl::~CcgEditControl()
{
}

void CcgEditControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcgEditControl, CFormView)
	
	ON_BN_CLICKED(IDC_SCANLINESEEDFILL, &CcgEditControl::OnClickedScanlineseedfill)
	ON_BN_CLICKED(IDC_SCANTRANSFERAET, &CcgEditControl::OnClickedScantransferaet)
	ON_BN_CLICKED(IDC_POINTFILL, &CcgEditControl::OnClickedPointfill)
	ON_BN_CLICKED(IDC_TRIANGULATION, &CcgEditControl::OnClickedTriangulation)
	ON_BN_CLICKED(IDC_PATTERNFILL, &CcgEditControl::OnBnClickedPatternfill)
END_MESSAGE_MAP()


// CcgEditControl 诊断

#ifdef _DEBUG
void CcgEditControl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CcgEditControl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CcgEditControl 消息处理程序


void CcgEditControl::OnClickedScanlineseedfill()
{
	Ccg2019WJYFillPolyDoc* pDoc = (Ccg2019WJYFillPolyDoc*)GetDocument();

	pDoc->m_displayMode = 0;

	pDoc->UpdateAllViews(this);
}


void CcgEditControl::OnClickedScantransferaet()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYFillPolyDoc* pDoc = (Ccg2019WJYFillPolyDoc*)GetDocument();

	pDoc->m_displayMode = 1;

	pDoc->UpdateAllViews(this);
}


void CcgEditControl::OnClickedPointfill()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYFillPolyDoc* pDoc = (Ccg2019WJYFillPolyDoc*)GetDocument();

	pDoc->m_displayMode = 2;

	pDoc->UpdateAllViews(this);
}


void CcgEditControl::OnClickedTriangulation()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYFillPolyDoc* pDoc = (Ccg2019WJYFillPolyDoc*)GetDocument();

	pDoc->m_displayMode = 3;

	pDoc->UpdateAllViews(this);
}



void CcgEditControl::OnBnClickedPatternfill()
{
	Ccg2019WJYFillPolyDoc* pDoc = (Ccg2019WJYFillPolyDoc*)GetDocument();

	pDoc->m_displayMode = 4;

	pDoc->UpdateAllViews(this);
}
