
// VideoProcessor.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CVideoProcessorApp:
// �� Ŭ������ ������ ���ؼ��� VideoProcessor.cpp�� �����Ͻʽÿ�.
//

class CVideoProcessorApp : public CWinApp
{
public:
	CVideoProcessorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CVideoProcessorApp theApp;