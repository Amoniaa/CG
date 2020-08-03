// CCgEditControl.cpp: 实现文件
//

#include "pch.h"
#include "cg2019WJYDrawLine.h"
#include "CCgEditControl.h"
#include "cg2019WJYDrawLineDoc.h"
#include "cg2019WJYDrawLineView.h"

// CCgEditControl

IMPLEMENT_DYNCREATE(CCgEditControl, CFormView)

CCgEditControl::CCgEditControl()
	: CFormView(IDD_EDITCONTROL)
{
	m_DDArunStr = _T("0");
	m_BrunStr = _T("0");
	m_MPrunStr = _T("0");
	m_DDAerrStr = _T("0");
	m_BerrStr = _T("0");
	m_MPerrStr = _T("0");
	m_DDAsmoStr = _T("0");
	m_BsmoStr = _T("0");
	m_MPsmoStr = _T("0");
}

CCgEditControl::~CCgEditControl()
{
}

void CCgEditControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DDARUN, m_DDArunStr);
	DDX_Text(pDX, IDC_BRUN, m_BrunStr);
	DDX_Text(pDX, IDC_MPRUN, m_MPrunStr);
	DDX_Text(pDX, IDC_DDAERROR, m_DDAerrStr);
	DDX_Text(pDX, IDC_BERROR, m_BerrStr);
	DDX_Text(pDX, IDC_MPERROR, m_MPerrStr);
	DDX_Text(pDX, IDC_DDASMOOTH, m_DDAsmoStr);
	DDX_Text(pDX, IDC_BSMOOTH, m_BsmoStr);
	DDX_Text(pDX, IDC_MPSMOOTH, m_MPsmoStr);
}

BEGIN_MESSAGE_MAP(CCgEditControl, CFormView)
	ON_BN_CLICKED(IDD_DDALINE, &CCgEditControl::OnClickedDdaline)
	ON_BN_CLICKED(IDD_BLINE, &CCgEditControl::OnClickedBline)
	ON_BN_CLICKED(IDD_MIDLINE, &CCgEditControl::OnClickedMidline)
	ON_BN_CLICKED(IDD_COMPARISION, &CCgEditControl::OnClickedComparsion)
	ON_BN_CLICKED(IDD_CIRCLE, &CCgEditControl::OnClickedIddCircle)
	ON_BN_CLICKED(IDD_ARC, &CCgEditControl::OnClickedIddArc)
END_MESSAGE_MAP()


// CCgEditControl 诊断

#ifdef _DEBUG
void CCgEditControl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCgEditControl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCgEditControl 消息处理程序


void CCgEditControl::OnClickedDdaline()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYDrawLineDoc* pDoc = (Ccg2019WJYDrawLineDoc*)GetDocument();

	pDoc->m_displayMode = 0;

	pDoc->UpdateAllViews(this);
}


void CCgEditControl::OnClickedBline()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYDrawLineDoc* pDoc = (Ccg2019WJYDrawLineDoc*)GetDocument();

	pDoc->m_displayMode = 1;

	pDoc->UpdateAllViews(this);
}


void CCgEditControl::OnClickedMidline()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYDrawLineDoc* pDoc = (Ccg2019WJYDrawLineDoc*)GetDocument();

	pDoc->m_displayMode = 2;

	pDoc->UpdateAllViews(this);
}


void CCgEditControl::OnClickedComparsion()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYDrawLineDoc* pDoc = (Ccg2019WJYDrawLineDoc*)GetDocument();

	pDoc->m_displayMode = 3;

	pDoc->UpdateAllViews(this);
}


void CCgEditControl::OnClickedIddCircle()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYDrawLineDoc* pDoc = (Ccg2019WJYDrawLineDoc*)GetDocument();

	pDoc->m_displayMode = 4;

	pDoc->UpdateAllViews(this);
}



void CCgEditControl::OnClickedIddArc()
{
	// TODO: 在此添加控件通知处理程序代码
	Ccg2019WJYDrawLineDoc* pDoc = (Ccg2019WJYDrawLineDoc*)GetDocument();

	pDoc->m_displayMode = 5;

	pDoc->UpdateAllViews(this);
}



void CCgEditControl::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	UpdateData(TRUE);

	CString DDArunStr;
	DDArunStr.Format(_T("%.2f"), Ccg2019WJYDrawLineView::getDdaRuntime());
	m_DDArunStr = DDArunStr;

	CString BrunStr;
	BrunStr.Format(_T("%.2f"), Ccg2019WJYDrawLineView::getbRuntime());
	m_BrunStr = BrunStr;

	CString MPrunStr;
	MPrunStr.Format(_T("%.2f"), Ccg2019WJYDrawLineView::getmpRuntime());
	m_MPrunStr = MPrunStr;

	CString DDAerrStr;
	DDAerrStr.Format(_T("%.4f"), Ccg2019WJYDrawLineView::getDdaError());
	m_DDAerrStr = DDAerrStr;

	CString BerrStr;
	BerrStr.Format(_T("%.4f"), Ccg2019WJYDrawLineView::getbError());
	m_BerrStr = BerrStr;

	CString MPerrStr;
	MPerrStr.Format(_T("%.4f"), Ccg2019WJYDrawLineView::getmpError());
	m_MPerrStr = MPerrStr;

	CString DDAsmoStr;
	DDAsmoStr.Format(_T("%.4f"), Ccg2019WJYDrawLineView::getDdaSmooth());
	m_DDAsmoStr = DDAsmoStr;

	CString BsmoStr;
	BsmoStr.Format(_T("%.4f"), Ccg2019WJYDrawLineView::getbSmooth());
	m_BsmoStr = BsmoStr;

	CString MPsmoStr;
	MPsmoStr.Format(_T("%.4f"), Ccg2019WJYDrawLineView::getmpSmooth());
	m_MPsmoStr = MPsmoStr;

	UpdateData(FALSE);
}


