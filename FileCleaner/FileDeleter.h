#pragma once

namespace fsys = std::filesystem;

static std::time_t to_time_t(std::string str);

class FileDeleter
{
protected:
	fsys::path path;
	bool dateTest(const fsys::path&, const std::string&, const std::string&, const int&);

public:
	FileDeleter();
	FileDeleter(std::string);

	bool pathOk();
	void setPath(std::string pth) { path = pth; };
	fsys::path getPath() { return path; };
	
	
	bool deletePaths(std::vector<fsys::path>, CProgressCtrl& pb);
	std::vector<fsys::path> findRequiredFiles(const int& filterType, const std::string& dateLow, const std::string& dateHigh);
	std::vector<fsys::path> findRequiredFiles1(const int& filterType);
	bool deleteEmptyCatalogues();
};

