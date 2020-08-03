
// cg2019WJY3DTransView.h : Ccg2019WJY3DTransView 类的接口
//

#pragma once

class CDrawScene;

class Ccg2019WJY3DTransView : public CView
{
protected: // 仅从序列化创建
	Ccg2019WJY3DTransView();
	DECLARE_DYNCREATE(Ccg2019WJY3DTransView)

	// 特性
public:
	Ccg2019WJY3DTransDoc* GetDocument() const;

	CClientDC  *m_pDC;
	BOOL       m_autoPlay;
	CRect      m_viewRect;

	CDrawScene *m_drawScene;

	// 操作
public:
	BOOL bSetupPixelFormat();

	void DrawScene();				//使用openGL初始化场景

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~Ccg2019WJY3DTransView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	//窗口分割
	afx_msg void OnSize(UINT nType, int cx, int cy);		//窗口建立
	afx_msg void OnAutoplay();								//点击动画按钮，实现对已选择的对象进行动画播放
	afx_msg void OnUpdateAutoplay(CCmdUI *pCmdUI);			//动画播放界面绘制刷新
	afx_msg void OnTimer(UINT_PTR nIDEvent);				//计时器
	afx_msg void OnOnlight();								//点击光照按钮，实现对全体对象添加光照
	afx_msg void OnUpdateOnlight(CCmdUI *pCmdUI);			//光照显示绘制刷新
	afx_msg void OnOnshade();								//点击明暗过渡按钮，对全体对象增加明暗过渡效果
	afx_msg void OnUpdateOnshade(CCmdUI *pCmdUI);			//明暗过渡绘制刷新
};

#ifndef _DEBUG  // cg2019WJY3DTransView.cpp 中的调试版本
inline Ccg2019WJY3DTransDoc* Ccg2019WJY3DTransView::GetDocument() const
{
	return reinterpret_cast<Ccg2019WJY3DTransDoc*>(m_pDocument);
}
#endif

