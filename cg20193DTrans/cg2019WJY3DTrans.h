
// cg2019WJY3DTrans.h : cg2019WJY3DTrans Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Ccg2019WJY3DTransApp:
// �йش����ʵ�֣������ cg2019WJY3DTrans.cpp
//

class Ccg2019WJY3DTransApp : public CWinAppEx
{
public:
	Ccg2019WJY3DTransApp();


	// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Ccg2019WJY3DTransApp theApp;
