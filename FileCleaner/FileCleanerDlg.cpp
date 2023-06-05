#include "pch.h"
#include "FileCleaner.h"
#include "FileCleanerDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFileCleanerDlg::CFileCleanerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILECLEANER_DIALOG1, pParent),
	iFileCounter(0),
	iFileNumber(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileCleanerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, brwsPath, m_brwsPath);
	DDX_Control(pDX, cbbCleaningPattern, cComboBoxSelection);
	//DDX_Control(pDX, dtmStartDate, cDtmStartDate);
	//DDX_Control(pDX, dtmEndDate, cDtmEndDate);
	DDX_Control(pDX, pbarCleaning, cpBar);
	DDX_Control(pDX, checkboxCatalogueClear, cCatalogueClear);
	DDX_Control(pDX, btnFilesList, m_btnFilesList);
}

BEGIN_MESSAGE_MAP(CFileCleanerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(btnClean, &CFileCleanerDlg::OnBnClickedbtnclean)
	ON_BN_CLICKED(btnFilesList, &CFileCleanerDlg::OnBnClickedbtnfileslist)
END_MESSAGE_MAP()


BOOL CFileCleanerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);	

	m_btnFilesList.ShowWindow(false);
	m_btnFilesList.EnableWindow(false);
	m_brwsPath.EnableFolderBrowseButton();

	cpBar.SetPos(0);
	cpBar.SetRange(0, 100);

	return TRUE;  
}

void CFileCleanerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileCleanerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CFileCleanerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileCleanerDlg::OnBnClickedbtnclean()
{
	CString path;
	m_brwsPath.GetWindowTextW(path);

	std::string stringPath(CW2A(path.GetString()));

	FD.setPath(stringPath);        

	cpBar.SetPos(0);

	int userSelection = cComboBoxSelection.GetCurSel();

	if (!path.IsEmpty())
	{
		if (userSelection >= 0)
		{
			auto iClearCatalogues = static_cast<bool>(cCatalogueClear.GetState());

			CTime timeStartDate{ 0 };
			CTime timeEndDate{ 0 };
			CString scStartDate, scEndDate;

			//DWORD dwResult;
			//dwResult = cDtmStartDate.GetTime(timeStartDate);
			//dwResult = cDtmEndDate.GetTime(timeEndDate);

			//scStartDate = timeStartDate.Format(_T("%Y-%m-%d")); 
			//scEndDate = timeEndDate.Format(_T("%Y-%m-%d"));	

			//std::string sStartDate(CW2A(scStartDate.GetString()));
			//std::string sEndDate(CW2A(scEndDate.GetString()));

			//files = FD.findRequiredFiles(userSelection, sStartDate, sEndDate);
			files = FD.findRequiredFiles1(userSelection);
			
			if (files.size() == 0)
			{
				AfxMessageBox(L"No files found.", MB_ICONINFORMATION);
				return;
			}

			iFileNumber = files.size();
			iFileCounter = 0;

			cpBar.SetRange(0,static_cast<short>(iFileNumber));

			FD.deletePaths(files, cpBar);

			if (iClearCatalogues == 1) {
				FD.deleteEmptyCatalogues();
			}
			
			m_btnFilesList.ShowWindow(true);
			m_btnFilesList.EnableWindow(true);
			AfxMessageBox(L"Files deleted", MB_ICONINFORMATION);
		}
		else
		{
			m_btnFilesList.ShowWindow(false);
			m_btnFilesList.EnableWindow(false);
			AfxMessageBox(L"Wrong searching type selection");
		}
	}
	else
	{
		m_btnFilesList.ShowWindow(false);
		m_btnFilesList.EnableWindow(false);
		AfxMessageBox(L"Path is not selected");
	}
}


void CFileCleanerDlg::OnBnClickedbtnfileslist()
{
	FilesListDlg FLDlg;
	FLDlg.files = files;
	FLDlg.printVector();
	FLDlg.DoModal();
}

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()