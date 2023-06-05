#pragma once

#include "FileDeleter.h"
#include "FilesListDlg.h"
#include <Aclapi.h>

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
	afx_msg void OnBnClickedbtnfileslist();
	

	DECLARE_MESSAGE_MAP()

private:
	std::vector<std::filesystem::path> files;

	FileDeleter FD;

	HICON m_hIcon;
	size_t iFileCounter;
	size_t iFileNumber;

	CComboBox cComboBoxSelection;
	CDateTimeCtrl cDtmStartDate;
	CDateTimeCtrl cDtmEndDate;
	CButton m_btnFilesList;
	CButton cCatalogueClear;
	CMFCEditBrowseCtrl m_brwsPath;

	CProgressCtrl cpBar;	
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