#include "pch.h"
#include "FileCleaner.h"
#include "FileCleanerDlg.h"
#include "RunAS.h"

//1st Pass
int TakeOWNDirectory(const std::wstring& filePath) {
	// Prepare the command to be executed
	std::wstring command = L"cmd.exe /c takeown /f \"" + filePath + L"\" /r /d y & icacls \"" + filePath + L"\" /grant *S-1-3-4:F /t /c /l";

	// Create the process information structures
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	// Create the process with the hidden console window
	//CREATE_NO_WINDOW
	if (CreateProcessW(NULL, &command[0], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		// Wait for the process to exit
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Get the exit code
		DWORD exitCode = 0;
		GetExitCodeProcess(pi.hProcess, &exitCode);

		// Clean up process handles
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		// Check if the process was successful
		if (exitCode != 0) {
			return GetLastError();
			// Handle the error
		}
	}
	else {
		return GetLastError();
		// Handle the error
	}

	return ERROR_SUCCESS; // Ownership and permission changes successful
}

int UseStandard(const std::wstring& filePath) {
	// Prepare the command to be executed
	wchar_t command[512];
	swprintf(command, 512, L"cmd.exe /c takeown /f \"%s\" & icacls \"%s\" /grant *S-1-3-4:F /t /c /l", filePath.c_str(), filePath.c_str());

	// Create the process information structures
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	// Create the process with the hidden console window
	//CREATE_NO_WINDOW
	if (CreateProcessW(NULL, &command[0], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		// Wait for the process to exit
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Get the exit code
		DWORD exitCode = 0;
		GetExitCodeProcess(pi.hProcess, &exitCode);

		// Clean up process handles
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		// Check if the process was successful
		if (exitCode != 0) {
			return GetLastError();
			// Handle the error
		}
	}
	else {
		return GetLastError();
		// Handle the error
	}

	return ERROR_SUCCESS; // Ownership and permission changes successful
}


bool SetFileAttributesSp(const std::wstring& filePath)
{
	// Convert the wide string to a narrow string
	int bufferSize = WideCharToMultiByte(CP_ACP, 0, filePath.c_str(), -1, NULL, 0, NULL, NULL);
	if (bufferSize == 0)
	{
		// Failed to get the buffer size
		DWORD errorCode = GetLastError();
		LPSTR errorMessage = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&errorMessage,
			0,
			NULL);

		std::string errorMsg = "Failed to get the buffer size.\n\nError Code: " + std::to_string(errorCode);
		if (errorMessage != nullptr)
		{
			errorMsg += "\nError Message: " + std::string(errorMessage);
			LocalFree(errorMessage);
		}

		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
		// Handle the error
		return false;
	}

	std::string narrowFilePath(bufferSize, '\0');
	if (WideCharToMultiByte(CP_ACP, 0, filePath.c_str(), -1, &narrowFilePath[0], bufferSize, NULL, NULL) == 0)
	{
		// Failed to convert the wide string to a narrow string
		DWORD errorCode = GetLastError();
		LPSTR errorMessage = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&errorMessage,
			0,
			NULL);

		std::string errorMsg = "Failed to convert the wide string to a narrow string\n\nError Code: " + std::to_string(errorCode);
		if (errorMessage != nullptr)
		{
			errorMsg += "\nError Message: " + std::string(errorMessage);
			LocalFree(errorMessage);
		}

		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
		// Handle the error
		return false;
	}

	// Set the file attributes to FILE_ATTRIBUTE_NORMAL
	if (SetFileAttributesA(narrowFilePath.c_str(), FILE_ATTRIBUTE_NORMAL) == 0)
	{
		// Failed to set the file attributes
		DWORD errorCode = GetLastError();
		LPSTR errorMessage = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&errorMessage,
			0,
			NULL);

		std::string errorMsg = "Failed to set the file attributes\n\nError Code: " + std::to_string(errorCode);
		if (errorMessage != nullptr)
		{
			errorMsg += "\nError Message: " + std::string(errorMessage);
			LocalFree(errorMessage);
		}

		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
		// Handle the error
		return false;
	}

	// File attributes set successfully
	return true;
}
//2nd Pass
int TakeOwn(const std::wstring& filePath) {
	UseStandard(filePath);

	return ERROR_SUCCESS; // Ownership transfer successful
}
int DeleteFileHandle(const std::wstring& filePath) 
{
	TakeOwn(filePath);
	//int result = TakeOwn(filePath);
	//Sleep(4);
	//if (result == ERROR_SUCCESS) {
	//	//File Ownership taken successfully 
	//	//Do Nothing!
	//}
	//else {
	//	DWORD errorCode = GetLastError();
	//	LPSTR errorMessage = nullptr;
	//	FormatMessageA(
	//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	//		NULL,
	//		errorCode,
	//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	//		(LPSTR)&errorMessage,
	//		0,
	//		NULL);

	//	std::string errorMsg = "Failed to Take Ownership Of The File\n\nError Code: " + std::to_string(errorCode);
	//	if (errorMessage != nullptr)
	//	{
	//		errorMsg += "\nError Message: " + std::string(errorMessage);
	//		LocalFree(errorMessage);
	//	}

	//	MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
	//	return 0;
	//}
	HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD errorCode = GetLastError();
		LPSTR errorMessage = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&errorMessage,
			0,
			NULL);

		std::string errorMsg = "Failed to delete the file\nError Code: " + std::to_string(errorCode);
		if (errorMessage != nullptr)
		{
			errorMsg += "\nError Message: " + std::string(errorMessage);
			LocalFree(errorMessage);
		}

		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
	}
	else {
		CloseHandle(hFile);
		if (GetFileAttributesW(filePath.c_str()) == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND) {
			MessageBoxA(NULL, "File deleted successfully!", "", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBoxA(NULL, "Failed to delete the file!", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
		}
	}
}

void HandleButtonClean_() {
	OPENFILENAMEA ofn;
	CHAR szFile[MAX_PATH] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_PATHMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		DWORD dwSession;
		WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
		DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);

		//std::wstringstream startSessionMsg;
		//startSessionMsg << L"RmStartSession returned " << dwError;
		//MessageBox(NULL, startSessionMsg.str().c_str(), L"Session Start", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

		if (dwError == ERROR_SUCCESS) {
			std::wstring wstrFilePath;
			if (strlen(ofn.lpstrFile) > 0) {
				int len = MultiByteToWideChar(CP_ACP, 0, ofn.lpstrFile, -1, NULL, 0);
				wchar_t* wFilePath = new wchar_t[len];
				MultiByteToWideChar(CP_ACP, 0, ofn.lpstrFile, -1, wFilePath, len);
				wstrFilePath = wFilePath;
				delete[] wFilePath;
			}

			const wchar_t* pszFilePath = wstrFilePath.c_str();

			dwError = RmRegisterResources(dwSession, 1, &pszFilePath, 0, NULL, 0, NULL);

			//std::wstringstream registerResourcesMsg;
			//registerResourcesMsg << L"RmRegisterResources(" << wstrFilePath << L") returned " << dwError;
			//MessageBox(NULL, registerResourcesMsg.str().c_str(), L"Register Resources", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
			if (dwError == ERROR_SUCCESS) {
				DWORD dwReason;
				UINT i;
				UINT nProcInfoNeeded;
				UINT nProcInfo = 10;
				RM_PROCESS_INFO rgpi[10];
				dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi, &dwReason);

				//std::wstring getListMsg = L"RmGetList returned " + std::to_wstring(dwError);
				//MessageBox(NULL, getListMsg.c_str(), L"Get List", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

				if (dwError == ERROR_SUCCESS) {
					//std::wstring infoMsg = L"RmGetList returned " + std::to_wstring(nProcInfo) + L" infos (" + std::to_wstring(nProcInfoNeeded) + L" needed)";
					//MessageBox(NULL, infoMsg.c_str(), L"Info", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

					for (i = 0; i < nProcInfo; i++) {
						//std::wstring appTypeMsg = L"ApplicationType: " + std::to_wstring(rgpi[i].ApplicationType);
						//MessageBox(NULL, appTypeMsg.c_str(), L"Application Type", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

						std::wstring appNameMsg = L"AppName: " + std::wstring(rgpi[i].strAppName);
						MessageBox(NULL, appNameMsg.c_str(), L"App Name", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

						std::wstring procIdMsg = L"ProcessId: " + std::to_wstring(rgpi[i].Process.dwProcessId);
						MessageBox(NULL, procIdMsg.c_str(), L"Process ID", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

						HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE, FALSE, rgpi[i].Process.dwProcessId);
						if (hProcess) {
							FILETIME ftCreate, ftExit, ftKernel, ftUser;
							if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser) &&
								CompareFileTime(&rgpi[i].Process.ProcessStartTime, &ftCreate) == 0) {
								WCHAR sz[MAX_PATH];
								DWORD cch = MAX_PATH;
								if (QueryFullProcessImageNameW(hProcess, 0, sz, &cch) && cch <= MAX_PATH) {
									std::wstring pathMsg = L"Process Path: " + std::wstring(sz);
									MessageBox(NULL, pathMsg.c_str(), L"Process Path", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
								}
							}
						}
						TerminateProcess(hProcess, 0);
						CloseHandle(hProcess);
						Sleep(1000);
						DeleteFileHandle(wstrFilePath);
						RmEndSession(dwSession);
						break;
					}
				}
			}
			//DeleteFileHandle(wstrFilePath);
			RmEndSession(dwSession);
		}
	}
}
void HandleButtonClean() {
	OPENFILENAMEA ofn;
	CHAR szFile[MAX_PATH] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//| OFN_FILEMUSTEXIST
	// | OFN_PATHMUSTEXIST
	ofn.Flags = OFN_EXPLORER | OFN_FORCESHOWHIDDEN | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE) {
		DWORD dwSession;
		WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
		DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);

		if (dwError == ERROR_SUCCESS) {
			std::wstring wstrFilePath;
			if (strlen(ofn.lpstrFile) > 0) {
				int len = MultiByteToWideChar(CP_ACP, 0, ofn.lpstrFile, -1, NULL, 0);
				wchar_t* wFilePath = new wchar_t[len];
				MultiByteToWideChar(CP_ACP, 0, ofn.lpstrFile, -1, wFilePath, len);
				wstrFilePath = wFilePath;
				delete[] wFilePath;
			}

			const wchar_t* pszFilePath = wstrFilePath.c_str();

			dwError = RmRegisterResources(dwSession, 1, &pszFilePath, 0, NULL, 0, NULL);

			if (dwError == ERROR_SUCCESS || dwError == ERROR_RESOURCE_NOT_FOUND) {
				DWORD dwReason;
				UINT i;
				UINT nProcInfoNeeded;
				UINT nProcInfo = 10;
				RM_PROCESS_INFO rgpi[10];
				dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi, &dwReason);

				if (dwError == ERROR_SUCCESS) {
					bool isFileUsed = false;

					for (i = 0; i < nProcInfo; i++) {
						//std::wstring appTypeMsg = L"ApplicationType: " + std::to_wstring(rgpi[i].ApplicationType);
						//MessageBox(NULL, appTypeMsg.c_str(), L"Application Type", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

						std::wstring appNameMsg = L"AppName: " + std::wstring(rgpi[i].strAppName);
						MessageBox(NULL, appNameMsg.c_str(), L"App Name", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

						std::wstring procIdMsg = L"ProcessId: " + std::to_wstring(rgpi[i].Process.dwProcessId);
						MessageBox(NULL, procIdMsg.c_str(), L"Process ID", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

						HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE, FALSE, rgpi[i].Process.dwProcessId);
						if (hProcess) {
							FILETIME ftCreate, ftExit, ftKernel, ftUser;
							if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser) &&
								CompareFileTime(&rgpi[i].Process.ProcessStartTime, &ftCreate) == 0) {
								WCHAR sz[MAX_PATH];
								DWORD cch = MAX_PATH;
								if (QueryFullProcessImageNameW(hProcess, 0, sz, &cch) && cch <= MAX_PATH) {
									std::wstring pathMsg = L"Process Path: " + std::wstring(sz);
									MessageBox(NULL, pathMsg.c_str(), L"Process Path", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
								}
							}
						}
						TerminateProcess(hProcess, 0);
						CloseHandle(hProcess);
						Sleep(1000);
						//DeleteFileHandle(wstrFilePath);
						RmEndSession(dwSession);
						break;
					}

					if (!isFileUsed) {
						if (DeleteFileHandle(wstrFilePath)) {
							//MessageBoxW(NULL, L"File deleted successfully!", L"Success", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
						}
						else {
							MessageBoxW(NULL, L"Failed to delete the file!", L"Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
						}
					}
					else {
						if (DeleteFileHandle(wstrFilePath)) {
							//MessageBoxW(NULL, L"File deleted successfully!", L"Success", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
						}
					}
				}
				else {
					// Failed to retrieve process list
					MessageBoxW(NULL, L"Failed to retrieve process list!", L"Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
				}
			}
			else {
				// File is being used by some processes
				MessageBoxW(NULL, L"File is in use by other processes!", L"Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
			}

			RmEndSession(dwSession);
		}
	}
}

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
	DDX_Control(pDX, btnFilesList2, m_btnFilesList2);
}

BEGIN_MESSAGE_MAP(CFileCleanerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(btnClean, &CFileCleanerDlg::OnBnClickedbtnclean)
	ON_BN_CLICKED(btnClean2, &CFileCleanerDlg::OnBnClickedbtnclean2)
	ON_BN_CLICKED(btnFilesList, &CFileCleanerDlg::OnBnClickedbtnfileslist)
	ON_BN_CLICKED(btnFilesList2, &CFileCleanerDlg::OnBnClickedbtnfileslist2)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL IsElevated()
{
	BOOL isElevated = FALSE;
	HANDLE hToken;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		TOKEN_ELEVATION elevation;
		DWORD dwSize;

		if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
		{
			isElevated = elevation.TokenIsElevated;
		}

		CloseHandle(hToken);
	}

	return isElevated;
}

BOOL CFileCleanerDlg::OnInitDialog()
{
	if (!IsElevated()) {
		wchar_t szPath[MAX_PATH];
		GetModuleFileName(NULL, szPath, MAX_PATH);
		SHELLEXECUTEINFO sei = { sizeof(sei) };
		sei.lpVerb = L"runas"; // Run the program as administrator
		sei.lpFile = szPath;
		sei.hwnd = NULL;
		sei.nShow = SW_NORMAL;
		ShellExecuteEx(&sei);
		ExitProcess(0);
	}
	else {
		//Do Nothing!
	}
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
	m_btnFilesList2.ShowWindow(false);
	m_btnFilesList2.EnableWindow(false);
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
	HandleButtonClean();
}
void CFileCleanerDlg::OnBnClickedbtnclean2() {
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

			//CTime timeStartDate{ 0 };
			//CTime timeEndDate{ 0 };
			//CString scStartDate, scEndDate;

			//DWORD dwResult;
			//dwResult = cDtmStartDate.GetTime(timeStartDate);
			//dwResult = cDtmEndDate.GetTime(timeEndDate);

			//scStartDate = timeStartDate.Format(_T("%Y-%m-%d")); 
			//scEndDate = timeEndDate.Format(_T("%Y-%m-%d"));	

			//std::string sStartDate(CW2A(scStartDate.GetString()));
			//std::string sEndDate(CW2A(scEndDate.GetString()));

			//files = FD.findRequiredFiles(userSelection, sStartDate, sEndDate);
			//files = FD.findRequiredFiles1(userSelection);
			files = FD.findRequiredFilesWatchDog(userSelection);

			if (files.size() == 0)
			{
				AfxMessageBox(L"No files found.", MB_ICONINFORMATION);
				return;
			}

			iFileNumber = files.size();
			iFileCounter = 0;

			cpBar.SetRange(0, static_cast<short>(iFileNumber));
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
			m_btnFilesList2.ShowWindow(false);
			m_btnFilesList2.EnableWindow(false);
			AfxMessageBox(L"Wrong searching type selection");
		}
	}
	else
	{
		m_btnFilesList.ShowWindow(false);
		m_btnFilesList.EnableWindow(false);
		m_btnFilesList2.ShowWindow(false);
		m_btnFilesList2.EnableWindow(false);
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



void CFileCleanerDlg::OnBnClickedbtnfileslist2()
{
	MessageBoxA(NULL, "Dangerous Mode Activated!", "INFO", MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
	TCHAR pathtofile[MAX_PATH];
	HMODULE GetModH = GetModuleHandleA(NULL);
	GetModuleFileName(GetModH, pathtofile, sizeof(pathtofile));
	auto pid = StartTrustedInstallerService();
	CreateProcessAsTrustedInstaller(pid, pathtofile);
	MessageBoxA(NULL, "Please USE THIS MODE RESPONSILY YOU COULD CORRUPT WINDOWS WITH IT!\n\nNote : Some Of The Functionality Won't Work Properly", "DANGEROUS THINGS AHEAD", MB_OK | MB_ICONHAND | MB_TOPMOST);
	ExitProcess(0);
}
void CFileCleanerDlg::EnableButtonForDuration(int duration)
{
	m_btnFilesList2.EnableWindow(TRUE);
	m_btnFilesList2.ShowWindow(TRUE);

	SetTimer(1, duration * 1000, NULL); // Start a timer for the specified duration in milliseconds
}
void CFileCleanerDlg::DisableButton()
{
	m_btnFilesList2.EnableWindow(FALSE);
	m_btnFilesList2.ShowWindow(FALSE);

	KillTimer(1); // Stop the timer if it's still running
}

int f5Counter = 0;
int f9Counter = 0;
BOOL CFileCleanerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F5)
	{
		f5Counter++;
		if (f5Counter == 5)
		{
			f5Counter = 0; // Reset the counter
			CAboutDlg aboutDlg;
			aboutDlg.DoModal();
			return TRUE; // Consumes the message
		}
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9) {
		f9Counter++;
		if (f9Counter == 9)
		{
			f9Counter = 0; // Reset the counter
			if (MessageBoxA(NULL, "This Is A Really Dangerous Mode.\n\nOnly Experts Should Use This Mode!\nIf You Agree To The Terms Click Yes If You Don't Click No.", "INFO", MB_YESNO | MB_ICONHAND | MB_TOPMOST) == IDYES) {
				EnableButtonForDuration(5); // Enable the button for 5 seconds
			}
			else {
				//Do Nothing!
			}
			return TRUE; // Consumes the message
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
void CFileCleanerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		DisableButton(); // Disable the button when the timer expires
	}

	CDialogEx::OnTimer(nIDEvent);
}