#include "stdafx.h"
#include "VideoProcessing.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//������ �Ҹ���
CWebCam::CWebCam() 	
{
	m_hCam = NULL;
}

CWebCam::~CWebCam()
{
	if (m_hCam)
		StopCam();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//ī�޶� �ʱ�ȭ.
bool CWebCam::StartCam(CWnd* wnd, int nWidth, int nHeight, BITMAPINFO& bmpInfo) //wnd :ĸó�ϴ� ������ ��Ÿ��, �������� �����͸� �Ѱܹ޴´�. 
                                                                                // bmpInfo : ������ �԰��� ��°� , ũ���� �ܺο��� �۾��� ������ �� �ִ�.
{
	// ĸ�� ������ ����� �������� ũ�� �� ��ġ ����
	m_hCam = capCreateCaptureWindow("Cam Capture", WS_CHILD | WS_VISIBLE,
		0, 0, nWidth, nHeight, wnd->m_hWnd, NULL);   // �����ڵ� ���� ���ο� ���� ���� �Լ� ����. �Կ��ϴ� ������ �԰ݰ��� ����� ����. 

	if (!capDriverConnect(m_hCam, 0))                //ĸó �����츦 ī�޶�� �����Ѵ�. 
	{
		AfxMessageBox("����� ī�޶� ã�� �� �����ϴ�.");
		return false;
	}

	//	capDlgVideoFormat(m_hCam);
	capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));     //ĸó������ ����Ϸ��� ������ �԰������� �˾ƾ��Ѵ�. 

	// ĸ�� ������ ũ�⸦ ��� ������ ����
	bmpInfo.bmiHeader.biWidth = nWidth;
	bmpInfo.bmiHeader.biHeight = nHeight;
	bmpInfo.bmiHeader.biSizeImage = nWidth*nHeight*bmpInfo.bmiHeader.biBitCount / 8;

	if (!capSetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO)))  //ī�޶��� ���� �԰�(ũ��� �� ���� ������)�� �����Ѵ�. 
	{
		// ������ �����ϸ� ���� ������ �ǵ���
		capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));
	}

	// ĸ�� ������ 24��Ʈ RGB�� �� �� �ֵ��� ����
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	if (!capSetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO)))
	{
		// ������ �����ϸ� ���� ������ �ǵ���
		capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));
	}

	capOverlay(m_hCam, TRUE); //�׷��� �ϵ��� �̿��� ������ ����.   *�������̸� Ȱ��ȭ�ϸ� �̸����Ⱑ �ڵ����� ��Ȱ��ȭ��.
	capPreviewRate(m_hCam,30); //�̸����⸦ �����ϴ� �ð� ������ ����.
	capPreview(m_hCam, TRUE);   //�̸����⸦ Ȱ��ȭ �� ������ ����.

	return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
//ī�޶� �����ϴ� �Լ�.
void CWebCam::StopCam() 
{
	capCaptureAbort(m_hCam);  //ĸó���� ����.
	capDriverDisconnect(m_hCam);  //ī�޶�� ĸó������ ������ ������ ���� �޽����� �߻���Ų��.
	m_hCam = NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
// AVI���Ϸ� �����ϴ� �Լ�.

BOOL CWebCam::StartRecord(const char* filename)
{
	capFileSetCaptureFile(m_hCam, filename);
	return capCaptureSequence(m_hCam);
}

BOOL CWebCam::StopRecord()
{
	capCaptureStop(m_hCam); //ĸó���� ����. ĸó�� ������ ���Ͽ� ����. 

	return capPreview(m_hCam, TRUE);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//�̸����⿡ ��Ÿ�� ������. ȣ��� ����� �ݹ��Լ��� ����� �� �ִ�, 
BOOL CWebCam::SetCallBackOnFrame(LRESULT(*fpProc)(HWND, LPVIDEOHDR)) //�Ű������� ���� �Լ� �����͸� �״��  �ѱ��.
{
	return capSetCallbackOnFrame(m_hCam, fpProc);  
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//YUY2�κ��� RGB�������� ��ȯ�ϴ� �Լ�.
void YUY2ToRGB24(int nWidth, int nHeight, const BYTE* pYUY2, BYTE* pRGB24)
{
	int nSize = nHeight * (nWidth / 2);

	int idxSrc = 0;
	int idxDst = 0;

	BYTE y1, u, y2, v;
	BYTE r, g, b;

	for (int i = 0; i<nSize; i++)
	{
		y1 = pYUY2[idxSrc];
		u = pYUY2[idxSrc + 1];
		y2 = pYUY2[idxSrc + 2];
		v = pYUY2[idxSrc + 3];

		// ù ��° �ȼ�
		b = CLIP((76284 * (y1 - 16) + 132252 * (u - 128)) >> 16);
		g = CLIP((76284 * (y1 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
		r = CLIP((76284 * (y1 - 16) + 104595 * (v - 128)) >> 16);

		pRGB24[idxDst] = b;
		pRGB24[idxDst + 1] = g;
		pRGB24[idxDst + 2] = r;

		idxDst += 3;

		// �� ��° �ȼ�
		b = CLIP((76284 * (y2 - 16) + 132252 * (u - 128)) >> 16);
		g = CLIP((76284 * (y2 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
		r = CLIP((76284 * (y2 - 16) + 104595 * (v - 128)) >> 16);

		pRGB24[idxDst] = b;
		pRGB24[idxDst + 1] = g;
		pRGB24[idxDst + 2] = r;

		idxDst += 3;
		idxSrc += 4;
	}
}