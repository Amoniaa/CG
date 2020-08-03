
// cg2019WJY2DTrans.h: cg2019WJY2DTrans 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// Ccg2019WJY2DTransApp:
// 有关此类的实现，请参阅 cg2019WJY2DTrans.cpp
//

class Ccg2019WJY2DTransApp : public CWinApp
{
public:
	Ccg2019WJY2DTransApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Ccg2019WJY2DTransApp theApp;
