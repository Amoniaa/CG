#pragma once



// CCgTransControl 窗体视图

class CCgTransControl : public CFormView
{
	DECLARE_DYNCREATE(CCgTransControl)

protected:
	CCgTransControl();           // 动态创建所使用的受保护的构造函数
	virtual ~CCgTransControl();

	void ViewTransLine(CDC* pDC, CRect dcRect);
	void ViewTransPolygon(CDC* pDC, CRect dcRect);
	void LB_ViewTransLine(CDC* pDC, CRect dcRect);
	void WA_ViewTransPolygon(CDC* pDC, CRect dcRect);

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSCONTROL };
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

	afx_msg void OnClickedLeft();
	afx_msg void OnClickedRight();
	afx_msg void OnClickedUp();
	afx_msg void OnClickedDown();
//	afx_msg void OnCbnSelchangeTransselect();
	int m_transSelect;
	afx_msg void OnSelchangeTransselect();
	afx_msg void OnClickedTransmode();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


