
// maze.h : maze ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CmazeApp:
// �� Ŭ������ ������ ���ؼ��� maze.cpp�� �����Ͻʽÿ�.
//

class CmazeApp : public CWinApp
{
public:
	CmazeApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CmazeApp theApp;
