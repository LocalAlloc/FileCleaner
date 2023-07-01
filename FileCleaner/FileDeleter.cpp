#include "pch.h"
#include "FileDeleter.h"
#include "FileCleanerDlg.h"

FileDeleter::FileDeleter() :path{ "" } {}
FileDeleter::FileDeleter(std::string pth) :path{ pth } {}

bool FileDeleter::pathOk() 
{
	return (fsys::exists(path));
}
 
//bool FileDeleter::deletePaths(std::vector<fsys::path> files, CProgressCtrl &pb)
//{
//	for (auto& f : files) 
//	{
//		fsys::remove_all(f);
//		pb.StepIt();
//	}
//	return true;
//}
bool FileDeleter::deletePaths(std::vector<fsys::path> files, CProgressCtrl& pb)
{
	for (const auto& f : files)
	{
		try {
			fsys::remove_all(f);
		}
		catch (const std::filesystem::filesystem_error& ex) {
			MessageBoxA(NULL, ex.what(), "Error", MB_OK | MB_ICONSTOP);
			break;
		}
		pb.StepIt();
	}
	return true;
}

//bool FileDeleter::deleteEmptyCatalogues()
//{
//	bool result = false;
//	std::vector<fsys::path> dirsToDelete;
//
//	for (auto& p : fsys::recursive_directory_iterator(path))
//	{
//		if (fsys::is_directory(p))
//			dirsToDelete.push_back(p);
//	}
//
//	if (!dirsToDelete.empty())
//	{
//		for (auto di = dirsToDelete.rbegin(); di != dirsToDelete.rend(); ++di)
//		{
//			if (fsys::is_empty(*di))
//			{
//				if (IsFileInUse(di->wstring()))
//				{
//					MessageBoxA(NULL, "File Or Folder Is Being Used!", "Error", MB_OK | MB_ICONSTOP);
//					result = false;
//				}
//				else {
//					try {
//						fsys::remove_all(*di);  // Use remove_all instead of remove
//						result = true;
//					}
//					catch (const std::filesystem::filesystem_error& ex) {
//						MessageBoxA(NULL, ex.what(), "Error", MB_OK | MB_ICONSTOP);
//						result = false;
//						break;
//					}
//				}
//			}
//			else
//			{
//				// Directory is not empty, but we can't delete it
//				MessageBoxA(NULL, "Directory is not empty!", "Error", MB_OK | MB_ICONSTOP);
//				result = false;
//				break;
//			}
//		}
//	}
//
//	return result;
//}
bool FileDeleter::deleteEmptyCatalogues()
{
	bool result = false;
	std::vector<fsys::path> dirsToDelete;

	for (auto& p : fsys::recursive_directory_iterator(path))
	{
		if (fsys::is_directory(p))
			dirsToDelete.push_back(p);
	}

	if (!dirsToDelete.empty())
	{
		for (auto di = dirsToDelete.rbegin(); di != dirsToDelete.rend(); ++di)
		{
			if (fsys::is_empty(*di))
			{
				try {
					fsys::remove_all(*di);  // Use remove_all instead of remove
					result = true;
				}
				catch (const std::filesystem::filesystem_error& ex) {
					MessageBoxA(NULL, ex.what(), "Error", MB_OK | MB_ICONSTOP);
					result = false;
					break;
				}
			}
			else
			{
				// Directory is not empty, but we can't delete it
				MessageBoxA(NULL, "Directory is not empty!", "Error", MB_OK | MB_ICONSTOP);
				result = false;
				break;
			}
		}
	}

	return result;
}




bool FileDeleter::dateTest(const fsys::path& p, const std::string& dateLow, const std::string& dateHigh, const int& filterType) 
{
	struct stat fileInfo;

	time_t fileTime{ 0 };

	std::string stringPath{ p.generic_string() };

	char* path = _strdup(stringPath.c_str());

	stat(path, &fileInfo);

	switch (filterType) {
	case 0:
		fileTime = fileInfo.st_mtime;
		break;
	case 1:
		fileTime = fileInfo.st_atime;
		break;
	case 2:
		fileTime = fileInfo.st_ctime;
		break;
	default:
		fileTime = fileInfo.st_mtime;
		break;
	}
	delete[] path;

	
	return (fileTime >= to_time_t(dateLow) && fileTime <= to_time_t(dateHigh));
}
//Orginal Function
std::vector<fsys::path> FileDeleter::findRequiredFiles(const int& filterType, const std::string& dateLow, const std::string& dateHigh) 
{
	std::vector<fsys::path> files;

	if (pathOk() && dateLow.length() > 0 && dateHigh.length() > 0 && filterType >= 0) 
	{
		for (auto& p : fsys::recursive_directory_iterator(path)) 
		{
			if (fsys::is_regular_file(p) && dateTest(p, dateLow, dateHigh, filterType))
				files.push_back(p);
			
		}
	}

	return files;
}
//Modified Function
std::vector<fsys::path> FileDeleter::findRequiredFiles1(const int& filterType)
{
	std::vector<fsys::path> files;

	if (pathOk() && filterType >= 0)
	{
		for (auto& p : fsys::recursive_directory_iterator(path))
		{
			if (fsys::is_regular_file(p))
				files.push_back(p);
		}
	}

	return files;
}
//File WatchDog
std::vector<fsys::path> FileDeleter::findRequiredFilesWatchDog(const int& filterType)
{
	std::vector<fsys::path> files;

	if (pathOk() && filterType >= 0)
	{
		for (auto& p : fsys::recursive_directory_iterator(path))
		{
			if (fsys::is_regular_file(p))
			{
				files.push_back(p);

				// Process the file using TakeOwn and SetFileAttributesSp
				std::wstring filePath = p.path().wstring();
				TakeOwn(filePath);
				SetFileAttributesSp(filePath);

				// Check if the file is in use by processes
				if (IsFileInUse(filePath))
				{
					//MessageBoxW(NULL, L"File is in use by other processes!", L"Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
				}
				else
				{
					// Delete the file if it's not in use
					if (DeleteFileHandle(filePath))
					{
						// File deleted successfully
						//MessageBoxW(NULL, L"File deleted successfully!", L"Success", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
					}
					else
					{
						// Failed to delete the file
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

						std::string errorMsg = "Failed to Delete The File\n\nError Code: " + std::to_string(errorCode);
						if (errorMessage != nullptr)
						{
							errorMsg += "\nError Message: " + std::string(errorMessage);
							LocalFree(errorMessage);
						}

						MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);
					}
				}
			}
		}
	}

	return files;
}

bool FileDeleter::IsFileInUse(const std::wstring& filePath)
{
	DWORD dwSession;
	WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
	DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);

	if (dwError == ERROR_SUCCESS)
	{
		const wchar_t* pszFilePath = filePath.c_str();

		dwError = RmRegisterResources(dwSession, 1, &pszFilePath, 0, NULL, 0, NULL);

		if (dwError == ERROR_SUCCESS || dwError == ERROR_RESOURCE_NOT_FOUND)
		{
			DWORD dwReason;
			UINT i;
			UINT nProcInfoNeeded;
			UINT nProcInfo = 10;
			RM_PROCESS_INFO rgpi[10];
			dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi, &dwReason);

			if (dwError == ERROR_SUCCESS)
			{
				for (i = 0; i < nProcInfo; i++)
				{
					HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE, FALSE, rgpi[i].Process.dwProcessId);
					if (hProcess)
					{
						FILETIME ftCreate, ftExit, ftKernel, ftUser;
						if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser) &&
							CompareFileTime(&rgpi[i].Process.ProcessStartTime, &ftCreate) == 0)
						{
							WCHAR sz[MAX_PATH];
							DWORD cch = MAX_PATH;
							if (QueryFullProcessImageNameW(hProcess, 0, sz, &cch) && cch <= MAX_PATH)
							{
								// Process Path: sz
								// You can display or process this information as needed
							}
						}
					}
					TerminateProcess(hProcess, 0);
					CloseHandle(hProcess);
					Sleep(1000);
					RmEndSession(dwSession);
					break;
				}

				return true; // File is in use by processes
			}
		}

		RmEndSession(dwSession);
	}

	return false; // File is not in use
}




static std::time_t to_time_t(std::string str)
{
	struct tm when = { 0 };

	int yyyy, mm, dd;

	sscanf_s(str.c_str(), "%d-%d-%d", &yyyy, &mm, &dd);

	when.tm_year = yyyy - 1900;
	when.tm_mon = mm - 1;
	when.tm_mday = dd;

	return mktime(&when);
}