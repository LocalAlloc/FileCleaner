#include "pch.h"
#include "FileCleaner.h"
#include "FilesListDlg.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(FilesListDlg, CDialogEx)

FilesListDlg::FilesListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CC_FILES_LIST, pParent)
	, m_txtFilesList(_T(""))
{

}

FilesListDlg::~FilesListDlg()
{
}

void FilesListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, txtFilesList, m_txtFilesList);
}

BEGIN_MESSAGE_MAP(FilesListDlg, CDialogEx)
END_MESSAGE_MAP()

void FilesListDlg::printVector() {

	CString str,tmp;

	for (auto& element : files) 
	{
		tmp = (element.c_str());
		str += tmp + _T("\r\n");
	}
	m_txtFilesList = str;
}

