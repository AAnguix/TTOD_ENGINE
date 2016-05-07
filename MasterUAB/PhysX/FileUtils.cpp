#include "FileUtils.h"
#include <sstream>
#include <fstream>

bool CFileUtils::CookedMeshFileExists(const std::string &Name, const unsigned int &CurrentLevel)
{
	std::string l_CookedMeshFileName = GetCookedMeshFileName(Name, CurrentLevel);
	/*int l_Return = PathFileExistsA(l_CookedMeshFileName.c_str());

	return (l_Return == 1);*/
	
	std::ifstream infile(l_CookedMeshFileName.c_str());
	bool l_Return = infile.good();
	return l_Return;
}

bool CFileUtils::MeshFileModified(const std::string &Name, const unsigned int &CurrentLevel)
{
	std::string l_CookedMeshFileName = GetCookedMeshFileName(Name, CurrentLevel);
	std::string l_MeshFileName = GetMeshFileName(Name, CurrentLevel);

	SYSTEMTIME l_MeshTime = GetFileCreationDate(l_MeshFileName);
	SYSTEMTIME l_CookedMeshTime = GetFileCreationDate(l_CookedMeshFileName);

	_int64 l_Difference = Delta(l_MeshTime, l_CookedMeshTime);

	if (l_Difference < 0) /*Mesh is newest than cooked mesh*/
	{
		return true;
	}

	return false;
}

std::string CFileUtils::GetCookedMeshFileName(const std::string &Name, unsigned int CurrentLevel)
{
	std::stringstream l_StringStream;
	l_StringStream << "Data\\Level" << CurrentLevel << "\\cooked\\" << Name;
	return l_StringStream.str();
}

std::string CFileUtils::GetMeshFileName(const std::string &Name, unsigned int CurrentLevel)
{
	std::string l_Result;

	std::stringstream l_StringStream;
	l_StringStream << "Data\\Level" << CurrentLevel << "\\meshes\\" << Name << ".mesh";
	return l_StringStream.str();
}

SYSTEMTIME CFileUtils::GetFileCreationDate(const std::string &FileName)
{
	//std::stringstream ss;
	HANDLE l_FileHandle = CreateFile(FileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME l_FileTime;
	SYSTEMTIME l_SystemTime;

	GetFileTime(l_FileHandle, &l_FileTime, NULL, NULL);
	FileTimeToSystemTime(&l_FileTime, &l_SystemTime);

	//ss << l_SystemTime.wMonth << '/' << l_SystemTime.wDay << '/' << l_SystemTime.wYear;
	CloseHandle(l_FileHandle);

	//return ss.str();
	return l_SystemTime;
}

_int64 CFileUtils::Delta(const SYSTEMTIME st1, const SYSTEMTIME st2)
{
	union timeunion
	{
		FILETIME fileTime;
		ULARGE_INTEGER ul;
	};

	timeunion ft1;
	timeunion ft2;

	SystemTimeToFileTime(&st1, &ft1.fileTime);
	SystemTimeToFileTime(&st2, &ft2.fileTime);

	return ft2.ul.QuadPart - ft1.ul.QuadPart;
}
