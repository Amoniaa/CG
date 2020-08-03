#pragma once

// CCgEditControl 窗体视图

class CCgEditControl : public CFormView
{
	DECLARE_DYNCREATE(CCgEditControl)

protected:
	CCgEditControl();           // 动态创建所使用的受保护的构造函数
	virtual ~CCgEditControl();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITCONTROL };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnClickedDdaline();
	afx_msg void OnClickedBline();
	afx_msg void OnClickedMidline();
	afx_msg void OnClickedComparsion();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	CString m_DDArunStr;
	CString m_BrunStr;
	CString m_MPrunStr;
	CString m_DDAerrStr;
	CString m_BerrStr;
	CString m_MPerrStr;
	CString m_DDAsmoStr;
	CString m_BsmoStr;
	CString m_MPsmoStr;
	float DrawSceneRunTime;

	afx_msg void OnClickedIddCircle();
	afx_msg void OnClickedIddArc();
};


