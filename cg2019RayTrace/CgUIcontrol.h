#pragma once



// CgUIcontrol ������ͼ

class CgUIcontrol : public CFormView
{
	DECLARE_DYNCREATE(CgUIcontrol)

protected:
	CgUIcontrol();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CgUIcontrol();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CgUIcontrol };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeMethod();
	int m_methodSelect;
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnSelchangeMethod();
	afx_msg void OnClickedGenerate();
	afx_msg void OnEnChangeNum();
	int sampleNum;
	int threadNum;
	float runTime;
};


