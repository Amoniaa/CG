
// cg2019WJY3DTransView.h : Ccg2019WJY3DTransView ��Ľӿ�
//

#pragma once

class CDrawScene;

class Ccg2019WJY3DTransView : public CView
{
protected: // �������л�����
	Ccg2019WJY3DTransView();
	DECLARE_DYNCREATE(Ccg2019WJY3DTransView)

	// ����
public:
	Ccg2019WJY3DTransDoc* GetDocument() const;

	CClientDC  *m_pDC;
	BOOL       m_autoPlay;
	CRect      m_viewRect;

	CDrawScene *m_drawScene;

	// ����
public:
	BOOL bSetupPixelFormat();

	void DrawScene();				//ʹ��openGL��ʼ������

	// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// ʵ��
public:
	virtual ~Ccg2019WJY3DTransView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	//���ڷָ�
	afx_msg void OnSize(UINT nType, int cx, int cy);		//���ڽ���
	afx_msg void OnAutoplay();								//���������ť��ʵ�ֶ���ѡ��Ķ�����ж�������
	afx_msg void OnUpdateAutoplay(CCmdUI *pCmdUI);			//�������Ž������ˢ��
	afx_msg void OnTimer(UINT_PTR nIDEvent);				//��ʱ��
	afx_msg void OnOnlight();								//������հ�ť��ʵ�ֶ�ȫ�������ӹ���
	afx_msg void OnUpdateOnlight(CCmdUI *pCmdUI);			//������ʾ����ˢ��
	afx_msg void OnOnshade();								//����������ɰ�ť����ȫ�����������������Ч��
	afx_msg void OnUpdateOnshade(CCmdUI *pCmdUI);			//�������ɻ���ˢ��
};

#ifndef _DEBUG  // cg2019WJY3DTransView.cpp �еĵ��԰汾
inline Ccg2019WJY3DTransDoc* Ccg2019WJY3DTransView::GetDocument() const
{
	return reinterpret_cast<Ccg2019WJY3DTransDoc*>(m_pDocument);
}
#endif

