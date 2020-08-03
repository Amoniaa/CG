#pragma once


#include "cg2019WJY3DTransDoc.h"

// CCgTransControl 窗体视图
#define  N 64
#define PRJPLANERETWIDTH  400
#define PRJPLANERETHEIGHT 400

class CCgTransControl : public CFormView
{
	DECLARE_DYNCREATE(CCgTransControl)

protected:
	CCgTransControl();           // 动态创建所使用的受保护的构造函数
	virtual ~CCgTransControl();

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
	int m_transSelect;
	afx_msg void OnClickedXleft();			//当点击+X按钮时，沿X方向向左平移
	afx_msg void OnClickedXright();			//当点击-X按钮时，沿X方向向右平移
	afx_msg void OnClickedYup();			//当点击+Y按钮时，沿Y方向向上平移
	afx_msg void OnClickedYdown();			//当点击-Y按钮时，沿Y方向向下平移
	afx_msg void OnClickedZfront();			//当点击+Z按钮时，沿Z方向向前平移
	afx_msg void OnClickedZback();			//当点击-Z按钮时，沿Z方向向后平移
	afx_msg void OnClickedTransmode();		//当点击模式切换按钮时，切换变换模式
	afx_msg void OnSelchangeTransselect();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

private:
	int      m_objNumber;			// Object number
	Object_p m_whoObject;           // Object pointer
	COLORREF vcObjColor[SPACEOBJECTS];

	void pTransToZbuffer(CRect dcRect);	//视见变换
	void pDrawLineObject(CDC *pDC, CRect dcRect);	
	void pDrawLightObject(CDC *pDC, CRect dcRect, float maxShade, float minShade);		//为物体增加光照

	int polyCount;
	BOOL m_bitmapOutput;
	float ymax[N], ymin[N];
	int ibegin, iend, scan, pdges;
	float Dx[N], Xa[N], Sc[N], Dc[N];

	int   prjPlaneRetSize;       // Z-Buffer Array Size
	float *prjPlaneRetZ;         // Z-Buffer Array Memory

	void InitPrjPlaneData(int width, int height);  //根据prjPlaneRet初始化z缓冲prjPlaneRetZ

	void pDrawShadeLightObject(CDC *pDC, CRect dcRect, float maxShade, float minShade);  //绘制明暗过渡后的物体

	//多边形填充算法
	void FillPolygon(CDC *pDC, int n, int *x, int *y, int *color, CRect dcRect);
	void Loadpolygon(int n, int *x, int *y, int *color);
	void PolyInsert(float x1, float y1, float x2, float y2, int c1, int c2);
	void UpdateXvalue();
	void XSort(int begin, int i);
	void Fillscan(CDC *pDC, CRect dcRect);
	void Include();

	//用于删除隐藏部分的Z-buffer方法。
	float CalculateZValue(int x, int y, CRect dcRect, int i);

	//openMP处理加速
	BOOL m_openMP;
	_VPIXEL* prjPlaneDibImage;
	int m_imageWidth, m_imageHeight;
	BITMAPINFO* DisplayBitMapHeader;

	char DisplayBitMapBuffer[sizeof(BITMAPINFO) + 16];//BitMapBuffer
	void CreateDisplayBitMap(int width, int height); //创建绘制好的bitmap
	void _SetPixel(int x, int y, int red, int green, int blue);
	float DrawSceneRunTime;
public:
	//	afx_msg void OnOnshade();
	//	afx_msg void OnUpdateOnshade(CCmdUI *pCmdUI);
	afx_msg void OnStnClickedDrawsceneruntime();
};


