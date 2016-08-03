
// VideoProcessorDlg.h : ��� ����
//

#pragma once

#include "VideoProcessing.h"
#include "afxcmn.h"

// CVideoProcessorDlg ��ȭ ����
class CVideoProcessorDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CVideoProcessorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOPROCESSOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	


	
protected:
	//ī�޶� ĸ�� ���� ��� ����
	CWebCam m_webCam; 

protected:
	CByteImage		m_imageIn;
	CByteImage		m_imageOut;



public:
	afx_msg void OnBnClickedButtonStartCam();
	afx_msg void OnBnClickedButtonStopCam();
	afx_msg void OnBnClickedButtonRecCam();
	afx_msg void OnBnClickedButtonStopRec();
	afx_msg void OnBnClickedButtonBinariztion();

	int m_nThreshold;
	CSliderCtrl m_sliderThres;

	afx_msg void OnBnClickedButtonViewimage();
	afx_msg void OnBnClickedButtonCloseImage();
};
