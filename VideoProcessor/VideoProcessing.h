#pragma once

#include"MyImage.h"
#include<vfw.h>                 //ī�޶� ���� �������� ����ϰ��� ����. 

#pragma comment(lib,"vfw32.lib")  //���� ��ũ. 


// ī�޶� ĸ�� Ŭ����
class CWebCam
{
public:
	CWebCam();
	virtual ~CWebCam();

	bool StartCam(CWnd* wnd, int nWidth, int nHeight, BITMAPINFO& bmpInfo); //ī�޶� �ʱ�ȭ. ȭ�� ��� ����.
	void StopCam();                                                         //ī�޶� ����.
	BOOL StartRecord(const char* filename);                               //AVI ���Ϸ� ���� (��� ���Ұ���.)
	BOOL StopRecord();                                                    
	BOOL SetCallBackOnFrame(LRESULT(*fpProc)(HWND, LPVIDEOHDR));             //������ ĸó�Ͽ� ȭ�鿡 ���. * SetCall�Լ��� �Է��� �ݹ� �Լ��� ���� �Լ�,���������� �����ؾ� �Ѵ�. 

protected:
	HWND		m_hCam;   // ĸó �������� �ڵ�, ĸó�� ������ �ϳ��� ������� �����Ͽ� �� �ڵ��� ���Ͽ� ������ �� �ְ� �ȴ�. 
};

void YUY2ToRGB24(int nWidth, int nHeight, const BYTE* pYUY2, BYTE* pRGB24);