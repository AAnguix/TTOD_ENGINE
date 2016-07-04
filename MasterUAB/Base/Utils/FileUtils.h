#ifndef _FILEUTILS_H
#define _FILEUTILS_H

#include <windows.h>
#include <string>

class CFileUtils
{

private:
	static std::string GetMeshFileName(const std::string &Name, unsigned int CurrentLevel);
	static _int64 TimeDifference(const SYSTEMTIME st1, const SYSTEMTIME st2);
	static SYSTEMTIME GetFileCreationDate(const std::string &FileName);
	static SYSTEMTIME GetFileLastModificationDate(const std::string &FileName);

	static bool FolderExists(const char* FolderName);
	static bool CreateFolder(const char* FolderName);

	static bool FileExists(const std::string &Filename);

	static bool WriteEffectsFileStateCode(const std::string &EffectsDateFile, SYSTEMTIME* Date);

public:
	static void CheckEffectsFolders();
	static void RemoveFileExtension(std::string &Filename);
	
	static bool CompiledShaderExists(const std::string &ShaderName);
	static size_t GetEffectsFileStateCode();

	static void CheckPhysxFolders(const std::string &Level);
	
	static bool CookedMeshExists(bool TriangleMesh, const std::string &Name, unsigned int CurrentLevel, std::string &_CookedMeshFileName);
	static bool MeshFileModified(bool TriangleMesh, const std::string &Name, const unsigned int &CurrentLevel);

	static bool ShaderFileModified(const std::string &ShaderFilename, const std::string &CompiledShaderFilename);

	static std::string SelectTextureFile(HWND Hwnd, const std::string  &InFile);
	static bool ShowDialog();

};

#endif