#include "FileUtils.h"
#include <sstream>
#include <fstream>
#include <assert.h>

bool CFileUtils::FolderExists(const char* FolderName)
{
	DWORD l_Attributes = GetFileAttributesA(FolderName);
	if (l_Attributes == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	return true;
}

bool CFileUtils::CreateFolder(const char* FolderName)
{
	return (CreateDirectory(FolderName, NULL)!=0);
}

void CFileUtils::CheckPhysxFolders(const std::string& Level)
{
	bool l_Success = true;
	std::stringstream l_Cooked;
	l_Cooked << "Data\\Level" + Level + "\\cooked\\";
	if (!FolderExists(l_Cooked.str().c_str()))
		l_Success = CreateFolder(l_Cooked.str().c_str());
	assert(l_Success != false);

	std::stringstream l_Triangle;
	l_Triangle << "Data\\Level" + Level + "\\cooked\\triangle";
	if (!FolderExists(l_Triangle.str().c_str()))
		CreateFolder(l_Triangle.str().c_str());
	assert(l_Success != false);

	std::stringstream l_Convex;
	l_Convex << "Data\\Level" + Level + "\\cooked\\convex";
	if (!FolderExists(l_Convex.str().c_str()))
		CreateFolder(l_Convex.str().c_str());
	assert(l_Success != false);
}

bool CFileUtils::FileExists(const std::string &Filename)
{
	std::ifstream infile(Filename.c_str());
	bool l_Return = infile.good();
	return l_Return;
}

bool CFileUtils::MeshFileModified(bool TriangleMesh, const std::string &Name, const unsigned int &CurrentLevel)
{
	std::string l_CookedMeshFileName;
	CookedMeshExists(TriangleMesh, Name, CurrentLevel,l_CookedMeshFileName);
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

bool CFileUtils::CookedMeshExists(bool TriangleMesh, const std::string &Name, unsigned int CurrentLevel, std::string &_CookedMeshFileName)
{
	
	std::stringstream l_StringStream;
	l_StringStream << "Data\\Level" << CurrentLevel << "\\cooked\\";
	bool b = FolderExists("Data\\Level1\\cooked\\");

	if (TriangleMesh)
		l_StringStream << "triangle";
	else
		l_StringStream << "convex";
	l_StringStream << "\\" + Name;

	_CookedMeshFileName = l_StringStream.str();

	if (!FileExists(_CookedMeshFileName))
		return false; /*Cooked mesh doesn't exists*/
		
	return true;
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
