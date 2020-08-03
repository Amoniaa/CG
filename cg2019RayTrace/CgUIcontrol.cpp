// CgUIcontrol.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CGMITRayTrace.h"
#include "CgUIcontrol.h"
#include "CGMITRayTraceDoc.h"


// CgUIcontrol

IMPLEMENT_DYNCREATE(CgUIcontrol, CFormView)

CgUIcontrol::CgUIcontrol()
	: CFormView(IDD_CgUIcontrol)
	, m_methodSelect(0)
	, sampleNum(0)
	, threadNum(0)
	, runTime(0)
{

}

CgUIcontrol::~CgUIcontrol()
{
}

void CgUIcontrol::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_METHOD, m_methodSelect);
	DDX_Text(pDX, IDC_SAMPLENUM, sampleNum);
	DDX_Text(pDX, IDC_THREADNUM, threadNum);
	DDX_Text(pDX, IDC_RUNTIME, runTime);
}

void CgUIcontrol::OnCbnSelchangeMethod()
{
	
}

BEGIN_MESSAGE_MAP(CgUIcontrol, CFormView)
	ON_CBN_SELCHANGE(IDC_METHOD, &CgUIcontrol::OnSelchangeMethod)
	ON_BN_CLICKED(IDC_GENERATE, &CgUIcontrol::OnClickedGenerate)
	ON_EN_CHANGE(IDC_SAMPLENUM, &CgUIcontrol::OnEnChangeNum)
	ON_STN_CLICKED(IDC_RUNTIME, &CgUIcontrol::OnStnClickedRuntime)
END_MESSAGE_MAP()


// CgUIcontrol ���

#ifdef _DEBUG
void CgUIcontrol::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CgUIcontrol::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CgUIcontrol ��Ϣ�������


void CgUIcontrol::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CCGMITRayTraceDoc * p_Doc = (CCGMITRayTraceDoc *)GetDocument();
	runTime = p_Doc->m_runTime;
	UpdateData(FALSE);
	
}


void CgUIcontrol::OnSelchangeMethod()
{
	CCGMITRayTraceDoc * p_Doc = (CCGMITRayTraceDoc *)GetDocument();

	UpdateData(TRUE);
	p_Doc->m_methodSelect = m_methodSelect;
	UpdateData(FALSE);
}


void CgUIcontrol::OnClickedGenerate()
{
	CCGMITRayTraceDoc * p_Doc = (CCGMITRayTraceDoc *)GetDocument();
	UpdateData(TRUE);
	p_Doc->sampleNum = sampleNum;
	p_Doc->threadNum = threadNum;
	UpdateData(FALSE);
	p_Doc->UpdateAllViews(this);
}

void CgUIcontrol::OnEnChangeNum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CgUIcontrol::OnStnClickedRuntime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
