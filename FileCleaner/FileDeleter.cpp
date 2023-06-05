#include "pch.h"
#include "FileDeleter.h"

FileDeleter::FileDeleter() :path{ "" } {}
FileDeleter::FileDeleter(std::string pth) :path{ pth } {}

bool FileDeleter::pathOk() 
{
	return (fsys::exists(path));
}

bool FileDeleter::deletePaths(std::vector<fsys::path> files, CProgressCtrl &pb)
{
	for (auto& f : files) 
	{
		fsys::remove_all(f);
		pb.StepIt();
	}
	return true;
}

bool FileDeleter::deleteEmptyCatalogues() 
{

	bool result{ false };
	std::vector<fsys::path> dirs;

		for (auto& p : fsys::recursive_directory_iterator(path)) 
		{
			if (fsys::is_directory(p)) 
				dirs.push_back(p);
		}
		if (dirs.size() > 0) {
			for (auto di = dirs.rbegin(); di != dirs.rend(); ++di) 
			{
				if (fsys::is_empty(*di)) 
				{
					fsys::remove(*di);
					result = true;
				}
				else  
					break; 
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