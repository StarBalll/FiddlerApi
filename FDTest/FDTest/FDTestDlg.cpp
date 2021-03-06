
// FDTestDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "FDTest.h"
#include "FDTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFDTestDlg 对话框



CFDTestDlg::CFDTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FDTEST_DIALOG, pParent)
	, m_szSrc(_T(""))
	, m_szDest(_T(""))
	, m_bStartFd(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFDTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC, m_szSrc);
	DDX_Text(pDX, IDC_EDIT_DEST, m_szDest);
}

BEGIN_MESSAGE_MAP(CFDTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SATAR, &CFDTestDlg::OnBnClickedButtonSatar)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CFDTestDlg::OnBnClickedButtonClose)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_WINDOW, &CFDTestDlg::OnBnClickedButtonCloseWindow)
END_MESSAGE_MAP()


// CFDTestDlg 消息处理程序
HWND CFDTestDlg::m_HWND = 0;

BOOL CFDTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_HWND = m_hWnd;
	CreateAndTrustRootCert();

	SetWindowText(_T("https://www.baidu.com/s?wd=a"));

	m_szSrc = "百度一下";

	m_szDest = "Fiddler";
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFDTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFDTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#include <string>
using namespace std;

// 测试回调
bool CallBack(FiddlerData& data)
{
	string m = string(data.httpMethod);
	if (m._Equal("CONNECT"))
	{
		return true;
	}
	string h = string(data.header);
	if (h.find("image") != -1 || h.find("jpe") != -1 ||
		h.find("Accept-Ranges: bytes") != -1)
	{
		return true;
	}

	string b = string(data.body);

	if (RECV == data.event && b.length() > 0)
	{
		

		data.modifyFlags = MODIFY_BODY;
		WCHAR szSrc[10240] = { 0 };
		GetDlgItemText(CFDTestDlg::m_HWND, IDC_EDIT_SRC, szSrc, 10240);

		WCHAR szDest[10240] = { 0 };
		GetDlgItemText(CFDTestDlg::m_HWND, IDC_EDIT_DEST, szDest, 10240);


		CString body;
		body = b.c_str();
		body.Replace(szSrc, szDest);


		USES_CONVERSION;
		data.body = W2A(body.GetBuffer());

	}
	else if (SEND == data.event)
	{
		data.bModifyResponseBody = true;
	}
 
	ModifyFiddlerData(&data);
	return true;
}


 
void CFDTestDlg::OnBnClickedButtonSatar()
{
	if (!m_bStartFd)
	{
		 FiddlerConfig fd;
		 GetFiddlerConfig(&fd);

		 fd.logLevel = DEBUG;
		 fd.notifyType = NOTIFY_ALL;
		 fd.callBack = CallBack;

		 SetFiddlerConfig(&fd);
		 FreeFiddlerConfig(&fd);

		// USES_CONVERSION;
		// AfxMessageBox(A2W(fd->szLogFileName));
		Start();
		m_bStartFd = true;
	}
}
 

void CFDTestDlg::OnBnClickedButtonClose()
{
	Close();
	m_bStartFd = false;
}


void CFDTestDlg::PostNcDestroy()
{
	if (m_bStartFd)
	{
		Close();
	}
	// TerminateProcess
	
	TerminateProcess(AfxGetInstanceHandle(), 0);
	CDialog::PostNcDestroy();
}


void CFDTestDlg::OnClose()
{
	 CDialog::OnClose();
}


BOOL CFDTestDlg::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE: //Esc按键事件  
			return true;
		case VK_RETURN: //Enter按键事件  
			return true;
		default:
			;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CFDTestDlg::OnBnClickedButtonCloseWindow()
{
	PostQuitMessage(0);
}
