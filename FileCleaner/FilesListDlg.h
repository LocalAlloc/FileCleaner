#pragma once

class FilesListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FilesListDlg)

public:
	FilesListDlg(CWnd* pParent = nullptr);   
	virtual ~FilesListDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CC_FILES_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	DECLARE_MESSAGE_MAP()
public:
	CString m_txtFilesList;
	std::vector<std::filesystem::path> files;
	void printVector();
};
