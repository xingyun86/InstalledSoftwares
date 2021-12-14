
// InstalledSoftwaresDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "InstalledSoftwares.h"
#include "InstalledSoftwaresDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CInstalledSoftwaresDlg dialog



CInstalledSoftwaresDlg::CInstalledSoftwaresDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INSTALLEDSOFTWARES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInstalledSoftwaresDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInstalledSoftwaresDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CInstalledSoftwaresDlg::OnBnClickedOk)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CInstalledSoftwaresDlg message handlers

BOOL CInstalledSoftwaresDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetWindowText(TEXT("已安装软件列表"));
	SetDlgItemText(IDOK, TEXT("刷新获取"));
	SetDlgItemText(IDCANCEL, TEXT("退出软件"));
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SOFTWARE);
	if (pListCtrl != NULL)
	{
		int nColumnIdx = 0;
		pListCtrl->InsertColumn(nColumnIdx++, TEXT("软件名称"), LVCFMT_LEFT, 100);
		pListCtrl->InsertColumn(nColumnIdx++, TEXT("序号"), LVCFMT_LEFT, 80);
		pListCtrl->InsertColumn(nColumnIdx++, TEXT("显示名称"), LVCFMT_LEFT, 100);
		pListCtrl->InsertColumn(nColumnIdx++, TEXT("发布者"), LVCFMT_LEFT, 100);
		pListCtrl->InsertColumn(nColumnIdx++, TEXT("安装日期"), LVCFMT_LEFT, 80);
		pListCtrl->InsertColumn(nColumnIdx++, TEXT("大小"), LVCFMT_LEFT, 80);
		pListCtrl->InsertColumn(nColumnIdx++, TEXT("版本号"), LVCFMT_LEFT, 80);
	}
	SendMessage(WM_SIZE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInstalledSoftwaresDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInstalledSoftwaresDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInstalledSoftwaresDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CInstalledSoftwaresDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SOFTWARE);
	pListCtrl->DeleteAllItems();
	AllRegKeyListUninstall(pListCtrl, HKEY_LOCAL_MACHINE, TEXT("SoftWare\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), KEY_WOW64_64KEY);
	AllRegKeyListUninstall(pListCtrl, HKEY_LOCAL_MACHINE, TEXT("SoftWare\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), KEY_WOW64_32KEY);
	AllRegKeyListUninstall(pListCtrl, HKEY_CURRENT_USER, TEXT("SoftWare\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0);
	AllRegKeyListUninstall(pListCtrl, HKEY_CURRENT_USER, TEXT("SoftWare\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0);
}


void CInstalledSoftwaresDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SOFTWARE);
	if (pListCtrl != NULL)
	{
		RECT rc = { 0 };
		GetClientRect(&rc);
		rc.left += 8;
		rc.top = 36;
		rc.right -= 8;
		rc.bottom -= 8;
		pListCtrl->MoveWindow(&rc);
	}
}
