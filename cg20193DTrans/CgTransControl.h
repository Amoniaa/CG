#pragma once


#include "cg2019WJY3DTransDoc.h"

// CCgTransControl ������ͼ
#define  N 64
#define PRJPLANERETWIDTH  400
#define PRJPLANERETHEIGHT 400

class CCgTransControl : public CFormView
{
	DECLARE_DYNCREATE(CCgTransControl)

protected:
	CCgTransControl();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_transSelect;
	afx_msg void OnClickedXleft();			//�����+X��ťʱ����X��������ƽ��
	afx_msg void OnClickedXright();			//�����-X��ťʱ����X��������ƽ��
	afx_msg void OnClickedYup();			//�����+Y��ťʱ����Y��������ƽ��
	afx_msg void OnClickedYdown();			//�����-Y��ťʱ����Y��������ƽ��
	afx_msg void OnClickedZfront();			//�����+Z��ťʱ����Z������ǰƽ��
	afx_msg void OnClickedZback();			//�����-Z��ťʱ����Z�������ƽ��
	afx_msg void OnClickedTransmode();		//�����ģʽ�л���ťʱ���л��任ģʽ
	afx_msg void OnSelchangeTransselect();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

private:
	int      m_objNumber;			// Object number
	Object_p m_whoObject;           // Object pointer
	COLORREF vcObjColor[SPACEOBJECTS];

	void pTransToZbuffer(CRect dcRect);	//�Ӽ��任
	void pDrawLineObject(CDC *pDC, CRect dcRect);	
	void pDrawLightObject(CDC *pDC, CRect dcRect, float maxShade, float minShade);		//Ϊ�������ӹ���

	int polyCount;
	BOOL m_bitmapOutput;
	float ymax[N], ymin[N];
	int ibegin, iend, scan, pdges;
	float Dx[N], Xa[N], Sc[N], Dc[N];

	int   prjPlaneRetSize;       // Z-Buffer Array Size
	float *prjPlaneRetZ;         // Z-Buffer Array Memory

	void InitPrjPlaneData(int width, int height);  //����prjPlaneRet��ʼ��z����prjPlaneRetZ

	void pDrawShadeLightObject(CDC *pDC, CRect dcRect, float maxShade, float minShade);  //�����������ɺ������

	//���������㷨
	void FillPolygon(CDC *pDC, int n, int *x, int *y, int *color, CRect dcRect);
	void Loadpolygon(int n, int *x, int *y, int *color);
	void PolyInsert(float x1, float y1, float x2, float y2, int c1, int c2);
	void UpdateXvalue();
	void XSort(int begin, int i);
	void Fillscan(CDC *pDC, CRect dcRect);
	void Include();

	//����ɾ�����ز��ֵ�Z-buffer������
	float CalculateZValue(int x, int y, CRect dcRect, int i);

	//openMP�������
	BOOL m_openMP;
	_VPIXEL* prjPlaneDibImage;
	int m_imageWidth, m_imageHeight;
	BITMAPINFO* DisplayBitMapHeader;

	char DisplayBitMapBuffer[sizeof(BITMAPINFO) + 16];//BitMapBuffer
	void CreateDisplayBitMap(int width, int height); //�������ƺõ�bitmap
	void _SetPixel(int x, int y, int red, int green, int blue);
	float DrawSceneRunTime;
public:
	//	afx_msg void OnOnshade();
	//	afx_msg void OnUpdateOnshade(CCmdUI *pCmdUI);
	afx_msg void OnStnClickedDrawsceneruntime();
};


