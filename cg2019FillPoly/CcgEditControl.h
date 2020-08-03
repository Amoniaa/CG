#pragma once



// CcgEditControl 窗体视图

class CcgEditControl : public CFormView
{
	DECLARE_DYNCREATE(CcgEditControl)

protected:
	CcgEditControl();           // 动态创建所使用的受保护的构造函数
	virtual ~CcgEditControl();

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
	afx_msg void OnClickedScanlineseedfill();
	afx_msg void OnClickedScantransferaet();
	afx_msg void OnClickedPointfill();
	afx_msg void OnClickedTriangulation();
	afx_msg void OnBnClickedPatternfill();
};


