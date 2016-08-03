
// VideoProcessorDlg.cpp : ���� ����
//

#include <stdio.h>
#include "stdafx.h"
#include "VideoProcessor.h"
#include "VideoProcessorDlg.h"
#include "afxdialogex.h"
#include "Features.h"
#include <memory.h>

#include "Segmentation.h"
#include "MyImage.h"
#include "MyImageFunc.h"
#include "ImageFrameWnd.h"
#include "ImageFrameWndManager.h"
#include "LoadImageFromFileDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoProcessorDlg ��ȭ ����



CVideoProcessorDlg::CVideoProcessorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIDEOPROCESSOR_DIALOG, pParent)
	, m_nThreshold(45)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoProcessorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THRES, m_nThreshold);
	DDV_MinMaxInt(pDX, m_nThreshold, 0, 255);
	DDX_Control(pDX, IDC_SLIDER_THRES, m_sliderThres);
}

BEGIN_MESSAGE_MAP(CVideoProcessorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START_CAM, &CVideoProcessorDlg::OnBnClickedButtonStartCam)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CAM, &CVideoProcessorDlg::OnBnClickedButtonStopCam)
	ON_BN_CLICKED(IDC_BUTTON_REC_CAM, &CVideoProcessorDlg::OnBnClickedButtonRecCam)
	ON_BN_CLICKED(IDC_BUTTON_STOP_REC, &CVideoProcessorDlg::OnBnClickedButtonStopRec)
	ON_BN_CLICKED(IDC_BUTTON_BINARIZTION, &CVideoProcessorDlg::OnBnClickedButtonBinariztion)
	ON_BN_CLICKED(IDC_BUTTON_ViewImage, &CVideoProcessorDlg::OnBnClickedButtonViewimage)
	ON_BN_CLICKED(IDC_BUTTON_Close_Image, &CVideoProcessorDlg::OnBnClickedButtonCloseImage)
END_MESSAGE_MAP()


// CVideoProcessorDlg �޽��� ó����

BOOL CVideoProcessorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_sliderThres.SetRange(0, 255);
	m_sliderThres.SetPos(m_nThreshold);







	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CVideoProcessorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CVideoProcessorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CVideoProcessorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ī�޶� ���� �� ������ �����ϱ� ���� ���� ����
BITMAPINFO gBmpInfo;    // *�ݹ��Լ����� ���캸�ƾ� �ϹǷ� ���� ������ �����Ѵ�. 
CByteImage gImageBuf;
CByteImage m_imageOut;
CByteImage d_imageOut;


// ��ü ���� ���� ���� ����
CByteImage	gBgImage;
CByteImage	gBinImage;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ProcessCamFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{

	//�ȼ� ���� ��ȯ----------------------------------------------------------------------------------
	if (gBmpInfo.bmiHeader.biCompression == BI_RGB) // RGB ������ ��쿡��, �״�� ����.
	{
		memcpy(gImageBuf.GetPtr(), lpVHdr->lpData,
			3*gBmpInfo.bmiHeader.biHeight*gBmpInfo.bmiHeader.biWidth);  //*3�� �����־�� ���� �����̴�. 
	}

	

	else if (gBmpInfo.bmiHeader.biCompression == MAKEFOURCC('Y', 'U', 'Y', '2')) // 16��Ʈ ����(YUY2) �ϰ�쿡�� �Լ�����.
	{
		YUY2ToRGB24(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight,
			lpVHdr->lpData, gImageBuf.GetPtr());
	}
	
	else
	{
		return FALSE;
	}

	//gImageBuf.GetChannelImg(2);
    //ShowImage(gImageBuf.PEPSI(), "���_�ݶ�");    //����ó���� ���ؼ� BITINFO ������ BMP �������� ��ȯ�Ͽ���. 
	

	//------------------------------------------------------------------------------------------------
	
	//�Һ�����

	/*
	CByteImage e_imageOut(gImageBuf);
	SobelEdge(RGB2Gray(gImageBuf), e_imageOut);
	*/

	/*
	//������ȯ
	double arrRho[100];
	double arrTheta[100];
	int nLine = HoughLines(e_imageOut,227,100,1,100,arrRho,arrTheta);

	CByteImage	m_imageOut;
	m_imageOut = gImageBuf;

	// RGB���� �׸���
	for (int i = 0; i<nLine; i++)
	{
		if (arrTheta[i] == 90) // ������
		{
			DrawLine(m_imageOut, arrRho[i], 0, arrRho[i], m_imageOut.GetHeight() - 1, 255, 0, 0);
		}
		else
		{
			int x1 = 0;
			int y1 = (int)(arrRho[i] / cos(arrTheta[i] * M_PI / 180) + 0.5);
			int x2 = m_imageOut.GetWidth() - 1;
			int y2 = (int)((arrRho[i] - x2*sin(arrTheta[i] * M_PI / 180)) / cos(arrTheta[i] * M_PI / 180) + 0.5);
			DrawLine(m_imageOut, x1, y1, x2, y2, 255, 0, 0);
		}
	}
	ShowImage(m_imageOut, "���� ���� ���");
	*/
	//---------------------------------------------------------------------------------------------


	


	//ShowImage(e_imageOut, "�Һ�����"); 
	
	//AfxMessageBox("�׽�Ʈ");

	//-----------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------
	

	                                   //����ó�� �˰���//

	
	// ���(����ó���� ����)�� �ȹٷ� ����ϱ����Ͽ� ȸ������ ������ �Ŀ� ���Ϲ��� ���״�.
	
	
	CByteImage c_imageOut(gImageBuf.PEPSI());
	CByteImage d_imageOut(gImageBuf.PEPSI());

	for (int r = 0; r < c_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < c_imageOut.GetWidth(); c++)
		{
			d_imageOut.GetAt(c, r) = c_imageOut.GetAt(c, c_imageOut.GetHeight() - 1 - r);
		}

	}

	//����ȭ�� ������ (0.0)�� ���� (640.480)�� �ȼ����� �����ϸ鼭 �ݶ��� Ư¡�� ã�Ƴ�
	//���η� 100���� �ȼ��� 1 �̸� �ݶ�� �ν�
	int _y=0;
	int _pepsi_Y_line_count=0;
	int _p_flag = 0;
	for (int r = 0; r < c_imageOut.GetHeight()-101 && _p_flag == 0; r++)
	{
		for (int c = 0; c < c_imageOut.GetWidth()-11 && _p_flag == 0; c++)
		{
			if (d_imageOut.GetAt(c, r) == 0)
			{
				_y = r;
				while (d_imageOut.GetAt(c, _y) == 0&& _pepsi_Y_line_count < 100&& _p_flag == 0)
				{
					_y++;
					_pepsi_Y_line_count++;
					if (_pepsi_Y_line_count == 100)
					{
						AfxMessageBox("����ݶ� ���� �Ǿ����ϴ�.(By Line)");
						_p_flag = 1;
						break;
					}

		
				}
				_pepsi_Y_line_count = 0;
			}

		
		}

	}
	
	ShowImage(d_imageOut, "����ݶ� ����");

	
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	// ���̴�(����ó���� ����)�� �ȹٷ� ����ϱ����Ͽ� ȸ������ ������ �Ŀ� ���Ϲ��� ���״�.

	
	CByteImage j_imageOut(gImageBuf.CIDER());
	CByteImage k_imageOut(gImageBuf.CIDER());

	for (int r = 0; r < j_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < j_imageOut.GetWidth(); c++)
		{
			k_imageOut.GetAt(c, r) = j_imageOut.GetAt(c, j_imageOut.GetHeight() - 1 - r);
		}

	}

	int _CIDER_Area_count = 0;
	int _c_flag = 0;
	int x__;
	int y__;

	for (int r = 0; r < j_imageOut.GetHeight()-136 && _c_flag == 0; r++)
	{
		for (int c = 49; c < j_imageOut.GetWidth()-49  && _c_flag == 0; c++)
		{

			if (k_imageOut.GetAt(c, r) == 0&& k_imageOut.GetAt(c-1, r) == 0&& k_imageOut.GetAt(c-2, r) == 0&& k_imageOut.GetAt(c-3, r) == 0&& k_imageOut.GetAt(c-4, r) == 0)
			{

				x__ = c;
				y__ = r;
				_CIDER_Area_count = 0;

				for (int k = x__-48; k < x__+48  && _c_flag == 0; k++)
				{
					for (int j = y__; j < y__+135 && _c_flag == 0; j++)
					{
						if (k_imageOut.GetAt(k, j) == 0 && _c_flag == 0)
						{
							_CIDER_Area_count++;
							if (_CIDER_Area_count > 400 && _c_flag == 0)
							{
								AfxMessageBox("���̴ٰ� ���� �Ǿ����ϴ�.(By Area)");
								_c_flag = 1;
								break;
							}

						}
					}

				}


			}

		}

	}


	ShowImage(k_imageOut, "���̴� ����");
	
	
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	// ������(����ó���� ����)�� �ȹٷ� ����ϱ����Ͽ� ȸ������ ������ �Ŀ� ���Ϲ��� ���״�.

   
	
	CByteImage l_imageOut(gImageBuf.JOLLY_PONG());
	CByteImage m_imageOut(gImageBuf.JOLLY_PONG());

	for (int r = 0; r < l_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < l_imageOut.GetWidth(); c++)
		{
			m_imageOut.GetAt(c, r) = l_imageOut.GetAt(c, l_imageOut.GetHeight() - 1 - r);
		}

	}
	
	
	int _zolly_Area_count = 0;
	int _z_flag = 0;
	int x_;
	int y_;

	for (int r = 0; r < l_imageOut.GetHeight()-229 && _z_flag == 0; r++)
	{
		for (int c = 0; c < l_imageOut.GetWidth()-175 && _z_flag == 0; c=c+5)
		{

		if (   m_imageOut.GetAt(c, r) == 0     && m_imageOut.GetAt(c + 1, r) == 0 && m_imageOut.GetAt(c + 2, r) == 0 && m_imageOut.GetAt(c + 3, r) == 0
		    && m_imageOut.GetAt(c + 4, r) == 0 && m_imageOut.GetAt(c + 5, r) == 0 && m_imageOut.GetAt(c + 6, r) == 0 && m_imageOut.GetAt(c + 7, r) == 0 
		    && m_imageOut.GetAt(c + 8, r) == 0 && m_imageOut.GetAt(c + 9, r) == 0&& m_imageOut.GetAt(c+10, r) == 0 && m_imageOut.GetAt(c + 11, r) == 0 
			&& m_imageOut.GetAt(c + 12, r) == 0 && m_imageOut.GetAt(c + 13, r) == 0&& m_imageOut.GetAt(c + 14, r) == 0 && m_imageOut.GetAt(c + 15, r) == 0
			&& m_imageOut.GetAt(c + 16, r) == 0 && m_imageOut.GetAt(c + 17, r) == 0&& m_imageOut.GetAt(c + 18, r) == 0 && m_imageOut.GetAt(c + 19, r) == 0
			
			&& m_imageOut.GetAt(c, r+1) == 0 && m_imageOut.GetAt(c + 1, r+1) == 0 && m_imageOut.GetAt(c + 2, r+1) == 0 && m_imageOut.GetAt(c + 3, r+1) == 0
			&& m_imageOut.GetAt(c + 4, r+1) == 0 && m_imageOut.GetAt(c + 5, r+1) == 0 && m_imageOut.GetAt(c + 6, r+1) == 0 && m_imageOut.GetAt(c + 7, r+1) == 0
			&& m_imageOut.GetAt(c + 8, r+1) == 0 && m_imageOut.GetAt(c + 9, r+1) == 0 && m_imageOut.GetAt(c + 10, r+1) == 0 && m_imageOut.GetAt(c + 11, r+1) == 0
			&& m_imageOut.GetAt(c + 12, r+1) == 0 && m_imageOut.GetAt(c + 13, r+1) == 0 && m_imageOut.GetAt(c + 14, r+1) == 0 && m_imageOut.GetAt(c + 15, r+1) == 0
			&& m_imageOut.GetAt(c + 16, r+1) == 0 && m_imageOut.GetAt(c + 17, r+1) == 0 && m_imageOut.GetAt(c + 18, r+1) == 0 && m_imageOut.GetAt(c + 19, r+1) == 0

			&&m_imageOut.GetAt(c, r+2) == 0 && m_imageOut.GetAt(c + 1, r+2) == 0 && m_imageOut.GetAt(c + 2, r+2) == 0 && m_imageOut.GetAt(c + 3, r+2) == 0
			&& m_imageOut.GetAt(c + 4, r+2) == 0 && m_imageOut.GetAt(c + 5, r+2) == 0 && m_imageOut.GetAt(c + 6, r+2) == 0 && m_imageOut.GetAt(c + 7, r+2) == 0
			&& m_imageOut.GetAt(c + 8, r+2) == 0 && m_imageOut.GetAt(c + 9, r+2) == 0 && m_imageOut.GetAt(c + 10, r+2) == 0 && m_imageOut.GetAt(c + 11, r+2) == 0
			&& m_imageOut.GetAt(c + 12, r+2) == 0 && m_imageOut.GetAt(c + 13, r+2) == 0 && m_imageOut.GetAt(c + 14, r+2) == 0 && m_imageOut.GetAt(c + 15, r+2) == 0
			&& m_imageOut.GetAt(c + 16, r+2) == 0 && m_imageOut.GetAt(c + 17, r+2) == 0 && m_imageOut.GetAt(c + 18, r+2) == 0 && m_imageOut.GetAt(c + 19, r+2) == 0

			&& m_imageOut.GetAt(c, r + 3) == 0 && m_imageOut.GetAt(c + 1, r + 3) == 0 && m_imageOut.GetAt(c + 2, r + 3) == 0 && m_imageOut.GetAt(c + 3, r + 3) == 0
			&& m_imageOut.GetAt(c + 4, r + 3) == 0 && m_imageOut.GetAt(c + 5, r + 3) == 0 && m_imageOut.GetAt(c + 6, r + 3) == 0 && m_imageOut.GetAt(c + 7, r + 3) == 0
			&& m_imageOut.GetAt(c + 8, r + 3) == 0 && m_imageOut.GetAt(c + 9, r + 3) == 0 && m_imageOut.GetAt(c + 10, r + 3) == 0 && m_imageOut.GetAt(c + 11, r + 3) == 0
			&& m_imageOut.GetAt(c + 12, r + 3) == 0 && m_imageOut.GetAt(c + 13, r + 3) == 0 && m_imageOut.GetAt(c + 14, r + 3) == 0 && m_imageOut.GetAt(c + 15, r + 3) == 0
			&& m_imageOut.GetAt(c + 16, r + 3) == 0 && m_imageOut.GetAt(c + 17, r + 3) == 0 && m_imageOut.GetAt(c + 18, r + 3) == 0 && m_imageOut.GetAt(c + 19, r + 3) == 0


		)
		   {
				x_ = c;
				y_ = r;
				_zolly_Area_count = 0;

				for (int k = x_; k < x_ + 174 && _z_flag == 0; k++)
				{
					for (int j = y_; j < y_ + 228 && _z_flag == 0; j++)
					{
						if (m_imageOut.GetAt(k, j) == 0&& _z_flag == 0)
						{
							_zolly_Area_count++;
							if (_zolly_Area_count > 8000&& _z_flag == 0)
							{
								AfxMessageBox("�������� ���� �Ǿ����ϴ�.(By Area)");
								_z_flag = 1;
								break;
							}

						}
					}

				}

		
			}
			
		}

	}
	
	


	ShowImage(m_imageOut, "������ ����");
	
	

	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	// ��¡���(����ó���� ����)�� �ȹٷ� ����ϱ����Ͽ� ȸ������ ������ �Ŀ� ���Ϲ��� ���״�.
	
	
	CByteImage n_imageOut(gImageBuf.CUTTLE_PEANUT());
	CByteImage o_imageOut(gImageBuf.CUTTLE_PEANUT());

	for (int r = 0; r < n_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < n_imageOut.GetWidth(); c++)
		{
			o_imageOut.GetAt(c, r) = n_imageOut.GetAt(c, n_imageOut.GetHeight() - 1 - r);
		}

	}
	
	int _O_zinger_Area_count = 0;
	int Y_O_zinger_Area_count = 0;
	int _O_flag = 0;
	int _x_;
	int _y_;

	for (int r = 0; r < n_imageOut.GetHeight() - 114 && _O_flag == 0; r++)
	{
		for (int c = 0; c < n_imageOut.GetWidth() - 100 && _O_flag == 0; c++)
		{

			if (o_imageOut.GetAt(c, r) ==0 && o_imageOut.GetAt(c+1, r) == 0 && o_imageOut.GetAt(c + 2, r) == 0 && o_imageOut.GetAt(c + 3, r) == 0
				&& o_imageOut.GetAt(c + 4, r) == 0 

				&& o_imageOut.GetAt(c, r+10) == 127&&o_imageOut.GetAt(c+1, r + 10) == 127 && o_imageOut.GetAt(c+2, r + 10) == 127 && o_imageOut.GetAt(c+3, r + 10) == 127
				&& o_imageOut.GetAt(c+4, r + 10) == 127 
				)
				
			{
				_x_ = c;
				_y_ = r;
				_O_zinger_Area_count = 0;
				Y_O_zinger_Area_count = 0;

				for (int k = _x_; k < _x_ + 99 && _O_flag == 0; k++)
				{
					for (int j = _y_; j < _y_ + 114 && _O_flag == 0; j++)
					{
						if (o_imageOut.GetAt(k, j) == 127 && _O_flag == 0)
						{
							Y_O_zinger_Area_count++;
						}

						if (o_imageOut.GetAt(k, j) == 0 && _O_flag == 0)
						{
							_O_zinger_Area_count++;
							if (_O_zinger_Area_count > 2500 && Y_O_zinger_Area_count > 1500 && _O_flag == 0)
							{
								AfxMessageBox("��¡����� ���� �Ǿ����ϴ�.(By Area)");
								_O_flag = 1;
								break;
							}
						}
					}

				}


			}

		}

	}
	


	ShowImage(o_imageOut, "��¡��� ����");
    
	
	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	//�Һ����� ������

	/*
	CByteImage f_imageOut(e_imageOut);
	CByteImage g_imageOut(e_imageOut);

	for (int r = 0; r < f_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < f_imageOut.GetWidth(); c++)
		{
			g_imageOut.GetAt(c, r) = f_imageOut.GetAt(c, f_imageOut.GetHeight() - 1 - r);
		}

	}
	*/
	//ShowImage(g_imageOut, "�Һ� ����");

	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	/*
	//��������ϱ�

	CByteImage h_imageOut(RGB2Gray(gImageBuf));
	CByteImage i_imageOut(RGB2Gray(gImageBuf));

	for (int r = 0; r < h_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < h_imageOut.GetWidth(); c++)
		{
			i_imageOut.GetAt(c, r) = h_imageOut.GetAt(c, h_imageOut.GetHeight() - 1 - r);
		}

	}

	for (int r = 0; r < h_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < h_imageOut.GetWidth(); c++)
		{
			if (r < 126 || r>416 || c < 120 || c>522)
			{
				i_imageOut.GetAt(c, r) = 255;
			}
		}

	}
	ShowImage(i_imageOut, "��踦 ����");
	*/
	
	return TRUE;
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//ī�޶� ���� 
// ī�޶� �ػ󵵿� �°� ����, StartCam() ȣ��, 
void CVideoProcessorDlg::OnBnClickedButtonStartCam()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOZORDER | SWP_NOMOVE);
	m_webCam.StartCam(GetDlgItem(IDC_STATIC_PREVIEW), 640, 480, gBmpInfo);


	gImageBuf = CByteImage(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight, 3);
	m_webCam.SetCallBackOnFrame(ProcessCamFrame);


}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//�������Ϸ� ����. 
void CVideoProcessorDlg::OnBnClickedButtonRecCam()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	char szFilter[] = "Descriptor File (*.AVI) | *.AVI; | All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) // ���� ��ȭ ���� ����
	{
		CString strPathName = dlg.GetPathName();
		m_webCam.StartRecord(strPathName);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//�����Լ� -ȣ�� 
void CVideoProcessorDlg::OnBnClickedButtonStopCam()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_webCam.StopCam();
}

void CVideoProcessorDlg::OnBnClickedButtonStopRec()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_webCam.StopRecord();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//����ȭ �Լ�.


void CVideoProcessorDlg::OnBnClickedButtonBinariztion()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_imageIn.IsEmpty())
	{
		AfxMessageBox("�Է� ������ �����ϴ�.");
		return;
	}



	m_imageOut = CByteImage(m_imageIn.GetWidth(), m_imageIn.GetHeight());

	if (m_imageIn.GetChannel() != 1)	// �÷� ����
		Binarization(RGB2Gray(m_imageIn), m_imageOut, m_nThreshold);
	else							// ȸ���� ����
		Binarization(m_imageIn,m_imageOut, m_nThreshold);

	ShowImage(m_imageOut, "��� ����");
}


void CVideoProcessorDlg::OnBnClickedButtonViewimage()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_imageIn = LoadImageFromDialog();
	ShowImage(m_imageIn, "Image");
}


void CVideoProcessorDlg::OnBnClickedButtonCloseImage()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CloseImage("Image");
}
