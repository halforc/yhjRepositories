
// MyWebDlg2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyWebDlg2App: 
// �йش����ʵ�֣������ MyWebDlg2.cpp
//

class CMyWebDlg2App : public CWinApp
{
public:
	CMyWebDlg2App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyWebDlg2App theApp;