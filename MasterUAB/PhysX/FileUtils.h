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

public:
	/*Returns the full path of the cooked mesh*/
	static std::string GetCookedMeshFileName(const std::string &Name, unsigned int CurrentLevel);

	/*Checks if the mesh had been cooked yet*/
	static bool CookedMeshFileExists(const std::string &Name, const unsigned int &CurrentLevel);

	/*Checks if the .mesh file has been updated*/
	static bool MeshFileModified(const std::string &Name, const unsigned int &CurrentLevel);

};

#endif