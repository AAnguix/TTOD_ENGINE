#include "FileUtils.h"
#include <sstream>
#include <fstream>
#include <assert.h>
#include <direct.h>
#include "Engine\Engine.h"
#include "Input\InputManagerImplementation.h"
#include "Log\Log.h"

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

void CFileUtils::CheckEffectsFolders()
{
	bool l_Success = true;
	std::stringstream l_Compiled;
	l_Compiled << "Data\\Effects\\compiled\\";
	if (!FolderExists(l_Compiled.str().c_str()))
		l_Success = CreateFolder(l_Compiled.str().c_str());
	assert(l_Success != false);
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

	_int64 l_Difference = TimeDifference(l_MeshTime, l_CookedMeshTime);

	if (l_Difference < 0) /*Mesh is newest than cooked mesh*/
	{
		return true;
	}

	return false;
}

void CFileUtils::RemoveFileExtension(std::string &Filename)
{
	std::size_t l_Pos = Filename.find(".");
	Filename = Filename.substr(0, l_Pos);
}

/*Returns a code that tells if the effects.xml file has been modified */
size_t CFileUtils::GetEffectsFileStateCode()
{
	std::string l_Effects = "Data\\effects.xml";
	std::string l_EffectsData = "Data\\EffectsDate";
	
	SYSTEMTIME l_EffectsDate;
	SYSTEMTIME l_EffectsDataDate;
	l_EffectsDate = GetFileLastModificationDate(l_Effects);


	if (FileExists(l_EffectsData))
	{
		l_EffectsDataDate = GetFileLastModificationDate(l_EffectsData);

		_int64 l_Difference = TimeDifference(l_EffectsDate, l_EffectsDataDate);

		if (l_Difference < 0) /*Effects is newest than EffectsData file*/
		{
			WriteEffectsFileStateCode(l_EffectsData, &l_EffectsDate);
			return 2;
		}
		return 1;
	}
	else
	{
		WriteEffectsFileStateCode(l_EffectsData, &l_EffectsDate);
		return 0;
	}
}

bool CFileUtils::WriteEffectsFileStateCode(const std::string &EffectsDateFile, SYSTEMTIME* Date)
{
	FILE *l_File;
	errno_t l_Error;

	l_Error = fopen_s(&l_File, EffectsDateFile.c_str(), "wb");
	assert(l_Error == 0);

	unsigned int l_DataSize = sizeof(Date);

	if (l_Error == 0)
	{
		fwrite(&l_DataSize, sizeof(unsigned int), 1, l_File);
	}

	fclose(l_File);

	return 0;
}

bool CFileUtils::CompiledShaderExists(const std::string &ShaderName)
{
	std::string l_CompiledShadersFolder = "Data\\Effects\\compiled";
	bool l_Folder = FolderExists(l_CompiledShadersFolder.c_str());
	assert(l_Folder);
	if (l_Folder)
	{
		l_CompiledShadersFolder = l_CompiledShadersFolder + "\\" + ShaderName;
		if (!FileExists(l_CompiledShadersFolder))
			return false;
		return true;
	}
	return false;
}

bool CFileUtils::ShaderFileModified(const std::string &ShaderFilename, const std::string &CompiledShaderFilename)
{
	SYSTEMTIME l_ShaderLastDate = GetFileLastModificationDate(ShaderFilename);
	SYSTEMTIME l_CompiledShaderLastDate = GetFileLastModificationDate(CompiledShaderFilename);

	_int64 l_Difference = TimeDifference(l_ShaderLastDate, l_CompiledShaderLastDate);

	if (l_Difference < 0) /*Shader is newest than pre-compiled shader*/
	{
		return true;
	}

	return false;
}

bool CFileUtils::CookedMeshExists(bool TriangleMesh, const std::string &Name, unsigned int CurrentLevel, std::string &_CookedMeshFileName)
{
	
	std::stringstream l_StringStream;
	l_StringStream << "Data\\Level" << CurrentLevel << "\\cooked\\";

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
	HANDLE l_FileHandle = CreateFile(FileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	FILETIME l_CreationTime;
	SYSTEMTIME l_StUTC, l_StLocal;

	GetFileTime(l_FileHandle, &l_CreationTime, NULL, NULL);
	FileTimeToSystemTime(&l_CreationTime, &l_StUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &l_StUTC, &l_StLocal);

	CloseHandle(l_FileHandle);

	return l_StLocal;
}

SYSTEMTIME CFileUtils::GetFileLastModificationDate(const std::string &FileName)
{
	HANDLE l_FileHandle = CreateFile(FileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	FILETIME l_WriteTime;
	SYSTEMTIME l_StUTC, l_StLocal;

	GetFileTime(l_FileHandle, NULL, NULL, &l_WriteTime);
	FileTimeToSystemTime(&l_WriteTime, &l_StUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &l_StUTC, &l_StLocal);

	CloseHandle(l_FileHandle);

	return l_StLocal;
}

_int64 CFileUtils::TimeDifference(const SYSTEMTIME st1, const SYSTEMTIME st2)
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

std::string CFileUtils::SelectTextureFile(HWND Hwnd, const std::string  &InFile)
{
	char* l_CurrentWorkingDirectory;
	l_CurrentWorkingDirectory = _getcwd(NULL, 0);
	OPENFILENAME of;
	ZeroMemory(&of, sizeof(of));

	of.lStructSize = sizeof(of);
	of.hwndOwner = CEngine::GetSingleton().GetInputManager()->GetHwnd();
	//of.lpstrFilter = "Text Files (*.txt)\0*.txt\0";
	of.lpstrFilter = "PNG Files(.png)\0*.png\0DDS Files(.dds)\0*.dds\0All Files(*.*)\0*.*\0"; //"PNG Files(.png)\0*.ASM\0DDS Files(.dds)\0*.dds\0All Files(*.*)\0*.*\0";
	char l_File[MAX_PATH];
	strcpy_s(l_File, MAX_PATH, InFile.c_str());
	of.lpstrFile = l_File;
	of.nMaxFile = MAX_PATH;
	of.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	of.lpstrDefExt = "dds";

	std::string l_Result = "";

	if (GetOpenFileName(&of))
	{
		_chdir(l_CurrentWorkingDirectory);
		l_Result = of.lpstrFile;
		size_t l_Position = l_Result.find("Data");

		if (l_Position != std::string::npos)
		{
			l_Result = l_Result.substr(l_Position);
		}
		else
		{	
			CEngine::GetSingleton().GetLogManager()->Log("The texture should be at the Data\\LevelX\\meshes folder");
			l_Result = "";
		}
		free(l_CurrentWorkingDirectory);
	}
	return l_Result;
}

bool CFileUtils::ShowDialog()
{
	int l_MessageboxId= MessageBox(NULL,"Hola artista! :).\n¿Estás segura de que quiere exportar?(Quizas peta)","Confirm",MB_ICONEXCLAMATION | MB_YESNO);
	if (l_MessageboxId == IDYES)
	{
		return true;
	}
	return false;
}
