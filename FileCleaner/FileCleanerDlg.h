#pragma once

#include "FileDeleter.h"
#include "FilesListDlg.h"
#include <Aclapi.h>
#include <windows.h>
#include <RestartManager.h>
#include <commdlg.h>
#pragma comment(lib, "RstrtMgr.lib")
class CFileCleanerDlg : public CDialogEx
{

public:
	CFileCleanerDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILECLEANER_DIALOG1 };
#endif


protected:

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedbtnclean();
	afx_msg void OnBnClickedbtnclean2();
	afx_msg void OnBnClickedbtnfileslist();
	

	DECLARE_MESSAGE_MAP()

private:
	std::vector<std::filesystem::path> files;

	FileDeleter FD;

	HICON m_hIcon;
	size_t iFileCounter;
	size_t iFileNumber;

	CComboBox cComboBoxSelection;
	CButton m_btnFilesList;
	CButton m_btnFilesList2;
	CButton cCatalogueClear;
	CMFCEditBrowseCtrl m_brwsPath;

	CProgressCtrl cpBar;	
	//int f5Count = 0;
public:
	afx_msg void OnBnClickedbtnfileslist2();
	void EnableButtonForDuration(int duration);
	void DisableButton();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

class ThreadData
{
public:
	int iFileCounterPtr;
	int iFileNumberPtr;

	CProgressCtrl* progressBarPointer;
};

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
};
void HandleButtonClean();
bool SetFileAttributesSp(const std::wstring& filePath);
int TakeOwn(const std::wstring& filePath);
int DeleteFileHandle(const std::wstring& filePath);
BOOL IsElevated();