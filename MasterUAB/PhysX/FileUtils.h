#ifndef _FILEUTILS_H
#define _FILEUTILS_H

#include <windows.h>
#include <string>

class CFileUtils
{

private:
	static std::string GetMeshFileName(const std::string &Name, unsigned int CurrentLevel);
	static _int64 Delta(const SYSTEMTIME st1, const SYSTEMTIME st2);
	static SYSTEMTIME GetFileCreationDate(const std::string &FileName);

	static bool FolderExists(const char* FolderName);
	static bool CreateFolder(const char* FolderName);

	static bool FileExists(const std::string &Filename);

public:
	static void CheckPhysxFolders(const std::string &Level);
	static bool CookedMeshExists(bool TriangleMesh, const std::string &Name, unsigned int CurrentLevel, std::string &_CookedMeshFileName);
	static bool MeshFileModified(bool TriangleMesh, const std::string &Name, const unsigned int &CurrentLevel);

};

#endif