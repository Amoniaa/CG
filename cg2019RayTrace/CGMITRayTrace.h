
// CGMITRayTrace.h : CGMITRayTrace Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CCGMITRayTraceApp:
// �йش����ʵ�֣������ CGMITRayTrace.cpp
//

class CCGMITRayTraceApp : public CWinAppEx
{
public:
	CCGMITRayTraceApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCGMITRayTraceApp theApp;
