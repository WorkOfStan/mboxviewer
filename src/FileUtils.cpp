//
//////////////////////////////////////////////////////////////////
//
//  Windows Mbox Viewer is a free tool to view, search and print mbox mail archives..
//
// Source code and executable can be downloaded from
//  https://sourceforge.net/projects/mbox-viewer/  and
//  https://github.com/eneam/mboxviewer
//
//  Copyright(C) 2019  Enea Mansutti, Zbigniew Minciel
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the version 3 of GNU Affero General Public License
//  as published by the Free Software Foundation; 
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the
//  Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
//  Boston, MA  02110 - 1301, USA.
//
//////////////////////////////////////////////////////////////////
//


#include "stdafx.h"
#include "FileUtils.h"
#include "TextUtilsEx.h"
#include "SimpleString.h"


bool FileUtils::PathDirExists(LPCSTR path)
{
	DWORD dwFileAttributes = GetFileAttributes(path);
	if (dwFileAttributes != (DWORD)0xFFFFFFFF)
	{
		bool bRes = ((dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
		return bRes;
	}
	else
		return false;
}

bool FileUtils::PathDirExistsW(LPCWSTR path)
{
	DWORD dwFileAttributes = GetFileAttributesW(path);
	if (dwFileAttributes != (DWORD)0xFFFFFFFF)
	{
		bool bRes = ((dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
		return bRes;
	}
	else
		return false;
}

BOOL FileUtils::PathFileExist(LPCSTR path)
{
	DWORD dwFileAttributes = GetFileAttributes(path);
	if (dwFileAttributes != (DWORD)0xFFFFFFFF)
	{
		BOOL bRes = ((dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY);
		return bRes;
	}
	else
		return FALSE;
}

BOOL FileUtils::PathFileExistW(LPCWSTR path)
{
	DWORD dwFileAttributes = GetFileAttributesW(path);
	if (dwFileAttributes != (DWORD)0xFFFFFFFF)
	{
		BOOL bRes = ((dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY);
		return bRes;
	}
	else
		return FALSE;
}

int FileUtils::fileExists(LPCSTR file)
{
	DWORD dwFileAttributes = GetFileAttributes(file);
	if (dwFileAttributes != (DWORD)0xFFFFFFFF)
	{
		BOOL bRes = ((dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY);
		return bRes;
}
	else
		return FALSE;
}


__int64 FileUtils::FileSeek(HANDLE hf, __int64 distance, DWORD MoveMethod)
{
	LARGE_INTEGER li;

	li.QuadPart = distance;

	li.LowPart = SetFilePointer(hf,
		li.LowPart,
		&li.HighPart,
		MoveMethod);

	if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError()
		!= NO_ERROR)
	{
		li.QuadPart = -1;
	}

	return li.QuadPart;
}

CString FileUtils::GetmboxviewTempPath(const char *name)
{
	char	buf[_MAX_PATH + 1];
	buf[0] = 0;
	DWORD gtp = ::GetTempPath(_MAX_PATH, buf);

	CString localTempPath = CString(buf);
	localTempPath.TrimRight("\\");
	localTempPath.Append("\\mboxview\\");
	if (name) 
	{
		localTempPath.Append(name);
		localTempPath.Append("\\");
	}
	BOOL ret = TRUE;
	if (!FileUtils::PathDirExists(localTempPath))
		ret = FileUtils::CreateDirectory(localTempPath);
	return localTempPath;
}

CStringW FileUtils::GetmboxviewTempPathW(const wchar_t *name)
{
	wchar_t	buf[_MAX_PATH + 1];
	buf[0] = 0;
	DWORD gtp = ::GetTempPathW(_MAX_PATH, buf);
	//
	CStringW localTempPath = CStringW(buf);
	localTempPath.TrimRight(L"\\");
	localTempPath.Append(L"\\mboxview\\");
	if (name)
	{
		localTempPath.Append(name);
		localTempPath.Append(L"\\");
	}
	BOOL ret = TRUE;
	if (!FileUtils::PathDirExistsW(localTempPath))
		ret = FileUtils::CreateDirectoryW(localTempPath);
	return localTempPath;
}

CString FileUtils::GetMboxviewLocalAppDataPath(const char *name)
{
	char	buf[_MAX_PATH + 1];
	buf[0] = 0;
	DWORD gtp = ::GetTempPath(_MAX_PATH, buf);

	CString folderPath;
	CString fileName;

	CString localTempPath = CString(buf);
	localTempPath.TrimRight("\\");
	FileUtils::GetFolderPathAndFileName(localTempPath, folderPath, fileName);
	folderPath.Append("MBoxViewer\\");
	if (name) {
		folderPath.Append(name);
		folderPath.Append("\\");
	}
	BOOL ret = TRUE;
	if (!FileUtils::PathDirExists(folderPath))
	{
		ret = FileUtils::CreateDirectory(folderPath);
		if (ret == FALSE)
			folderPath.Empty();
	}
	return folderPath;
}

CStringW FileUtils::GetMboxviewLocalAppDataPathW(const wchar_t *name)
{
	wchar_t	buf[_MAX_PATH + 1];
	buf[0] = 0;
	DWORD gtp = ::GetTempPathW(_MAX_PATH, buf);

	CStringW folderPath;
	CStringW fileName;

	CStringW localTempPath = CStringW(buf);
	localTempPath.TrimRight(L"\\");
	FileUtils::GetFolderPathAndFileNameW(localTempPath, folderPath, fileName);
	folderPath.Append(L"MBoxViewer\\");
	if (name) {
		folderPath.Append(name);
		folderPath.Append(L"\\");
	}
	BOOL ret = TRUE;
	if (!FileUtils::PathDirExistsW(folderPath))
		ret = FileUtils::CreateDirectoryW(folderPath);

	return folderPath;
}

CString FileUtils::CreateMboxviewLocalAppDataPath(const char *name)
{
	CString folderPath = FileUtils::GetMboxviewLocalAppDataPath(name);
	BOOL ret = TRUE;
	if (!FileUtils::PathDirExists(folderPath))
	{
		ret = FileUtils::CreateDirectory(folderPath);
		if (ret == FALSE)
			folderPath.Empty();
	}
	return folderPath;
}

CStringW FileUtils::CreateMboxviewLocalAppDataPathW(const wchar_t *name)
{
	CStringW folderPath = FileUtils::GetMboxviewLocalAppDataPathW(name);
	BOOL ret = TRUE;
	if (!FileUtils::PathDirExistsW(folderPath))
		ret = FileUtils::CreateDirectoryW(folderPath);

	return folderPath;
}

BOOL FileUtils::RemoveDirectory(CString &dir, DWORD &error)
{
	error = 0;

	WIN32_FIND_DATA FileData;
	HANDLE hSearch;
	BOOL	bFinished = FALSE;
	// Start searching for all files in the current directory.
	CString searchPath = dir + "\\*.*";
	hSearch = FindFirstFile(searchPath, &FileData);
	if (hSearch == INVALID_HANDLE_VALUE) {
		TRACE("No files found.");
		return FALSE;
	}
	while (!bFinished)
	{
		if (!(strcmp(FileData.cFileName, ".") == 0 || strcmp(FileData.cFileName, "..") == 0))
		{
			CString fileFound = dir + "\\" + CString(FileData.cFileName);
			if (FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				BOOL retR = FileUtils::RemoveDirectory(fileFound, error);
				if (retR == FALSE) {
					bFinished = TRUE;
					break;
				}
			}
			else
			{
				BOOL retF = ::DeleteFile(fileFound);
			}
		}
		if (!FindNextFile(hSearch, &FileData)) {
			bFinished = TRUE;
			break;
		}
	}
	FindClose(hSearch);
	// dir must be empty
	BOOL retRD = ::RemoveDirectory(dir);
	if (!retRD)
		return FALSE;
	else
		return TRUE;
}

BOOL FileUtils::RemoveDirectoryW(CStringW &dir, DWORD &error)
{
	error = 0;

	WIN32_FIND_DATAW FileData;
	HANDLE hSearch;
	BOOL bFinished = FALSE;
	// Start searching for all files in the current directory.
	CStringW searchPath = dir + L"\\*.*";
	hSearch = FindFirstFileW(searchPath, &FileData);
	if (hSearch == INVALID_HANDLE_VALUE) {
		TRACE("No files found.");
		return FALSE;
	}
	while (!bFinished)
	{
		if (!(wcscmp(FileData.cFileName, L".") == 0 || wcscmp(FileData.cFileName, L"..") == 0))
		{
			CStringW	fileFound = dir + L"\\" + CStringW(FileData.cFileName);
			if (FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				BOOL retR = RemoveDirectoryW(fileFound, error);
				if (retR == FALSE)
					bFinished = TRUE;
			}
		}
		if (!FindNextFileW(hSearch, &FileData))
			bFinished = TRUE;
	}
	FindClose(hSearch);
	// dir must be empty
	BOOL retRD = ::RemoveDirectoryW( dir );
	if (!retRD)
		return FALSE;
	else
		return TRUE;
}

// Removes files only
BOOL FileUtils::RemoveDir(CString & dir, bool recursive, bool removeFolders)
{
	WIN32_FIND_DATA FileData;
	HANDLE hSearch;
	BOOL	bFinished = FALSE;
	// Start searching for all files in the current directory.
	CString searchPath = dir + "\\*.*";
	hSearch = FindFirstFile(searchPath, &FileData);
	if (hSearch == INVALID_HANDLE_VALUE) {
		TRACE("No files found.");
		return FALSE;
	}
	while (!bFinished) 
	{
		if (!(strcmp(FileData.cFileName, ".") == 0 || strcmp(FileData.cFileName, "..") == 0)) 
		{
			CString	fileFound = dir + "\\" + FileData.cFileName;
			if (FileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				BOOL retF = DeleteFile((LPCSTR)fileFound);
				// handle error
			}
			else if (recursive)
			{
				BOOL retB = FileUtils::RemoveDir(fileFound, recursive, removeFolders);
				if (removeFolders) {
					BOOL retD = ::RemoveDirectory(dir);
					// handle error
				}
			}
		}
		if (!FindNextFile(hSearch, &FileData))
		{
			bFinished = TRUE;
		}
	}
	FindClose(hSearch);
	if (removeFolders)
		:: RemoveDirectory(dir);
	return TRUE;
}

BOOL FileUtils::RemoveDirW(CString & dir, bool recursive)
{
	CStringW dirW;
	DWORD error;
	BOOL retA2W = TextUtilsEx::Ansi2Wide(dir, dirW, error);
	BOOL ret = RemoveDirW(dirW, recursive);
	return ret;
}

// Removes files only
BOOL FileUtils::RemoveDirW(CStringW & dir, bool recursive)
{
	WIN32_FIND_DATAW FileData;
	HANDLE hSearch;
	BOOL	bFinished = FALSE;
	// Start searching for all files in the current directory.
	CStringW searchPath = dir + L"\\*.*";
	hSearch = FindFirstFileW(searchPath, &FileData);
	if (hSearch == INVALID_HANDLE_VALUE) {
		TRACE("No files found.");
		return FALSE;
	}
	while (!bFinished) 
	{
		if (!(wcscmp(FileData.cFileName, L".") == 0 || wcscmp(FileData.cFileName, L"..") == 0)) 
		{
			CStringW	fileFound = dir + L"\\" + CStringW(FileData.cFileName);
			if (FileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				DeleteFileW((LPCWSTR)fileFound);
			else if (recursive)
				RemoveDirW(fileFound, recursive);
		}
		if (!FindNextFileW(hSearch, &FileData))
			bFinished = TRUE;
	}
	FindClose(hSearch);
	//RemoveDirectoryW( dir );
	return TRUE;
}

CString FileUtils::CreateTempFileName(CString ext)
{
	CString fmt = GetmboxviewTempPath() + "PTT%05d." + ext;
	CString fileName;
ripeti:
	fileName.Format(fmt, abs((int)(1 + (int)(100000.0*rand() / (RAND_MAX + 1.0)))));
	if (FileUtils::PathFileExist(fileName))
		goto ripeti;

	return fileName; //+_T(".HTM");
}

void FileUtils::CPathStripPath(const char *path, CString &fileName)
{
	int pathlen = strlen(path);
	char *pathbuff = new char[pathlen + 1];
	strcpy(pathbuff, path);
	PathStripPath(pathbuff);
	fileName.Empty();
	fileName.Append(pathbuff);
	delete[] pathbuff;
}

void FileUtils::CPathStripPathW(const wchar_t *path, CStringW &fileName)
{
	int pathlen = wcslen(path);
	wchar_t *pathbuff = new wchar_t[pathlen + 1];
	wcscpy(pathbuff, path);
	PathStripPathW(pathbuff);
	fileName.Empty();
	fileName.Append(pathbuff);
	delete[] pathbuff;
}

BOOL FileUtils::CPathGetPath(const char *path, CString &filePath)
{
	int pathlen = strlen(path);
	char *pathbuff = new char[2 * pathlen + 1];  // to avoid overrun ?? see microsoft docs
	strcpy(pathbuff, path);
	BOOL ret = PathRemoveFileSpec(pathbuff);
	//HRESULT  ret = PathCchRemoveFileSpec(pathbuff, pathlen);  //   pathbuff must be of PWSTR type

	filePath.Empty();
	filePath.Append(pathbuff);
	delete[] pathbuff;
	return ret;  // ret = 0; microsoft doc says if something goes wrong -:) 
}

void FileUtils::SplitFilePath(CString &fileName, CString &driveName, CString &directory, CString &fileNameBase, CString &fileNameExtention)
{
	TCHAR ext[_MAX_EXT + 1]; ext[0] = 0;
	TCHAR drive[_MAX_DRIVE + 1]; drive[0] = 0;
	TCHAR dir[_MAX_DIR + 1]; dir[0] = 0;
	TCHAR fname[_MAX_FNAME + 1]; fname[0] = 0;

	driveName.Empty();
	directory.Empty();
	fileNameBase.Empty();
	fileNameExtention.Empty();

	_tsplitpath_s(fileName,
		drive, _MAX_DRIVE + 1,
		dir, _MAX_DIR + 1,
		fname, _MAX_FNAME + 1,
		ext, _MAX_EXT + 1);

	driveName.Append(drive);
	directory.Append(dir);
	fileNameBase.Append(fname);
	fileNameExtention.Append(ext);
}

void FileUtils::SplitFilePathW(CStringW &fileName, CStringW &driveName, CStringW &directory, CStringW &fileNameBase, CStringW &fileNameExtention)
{
	wchar_t ext[_MAX_EXT + 1]; ext[0] = 0;
	wchar_t drive[_MAX_DRIVE + 1]; drive[0] = 0;
	wchar_t dir[_MAX_DIR + 1]; dir[0] = 0;
	wchar_t fname[_MAX_FNAME + 1]; fname[0] = 0;

	driveName.Empty();
	directory.Empty();
	fileNameBase.Empty();
	fileNameExtention.Empty();

	_wsplitpath_s(fileName,
		drive, _MAX_DRIVE + 1,
		dir, _MAX_DIR + 1,
		fname, _MAX_FNAME + 1,
		ext, _MAX_EXT + 1);

	driveName.Append(drive);
	directory.Append(dir);
	fileNameBase.Append(fname);
	fileNameExtention.Append(ext);
}

void FileUtils::GetFolderPathAndFileName(CString &fileNamePath, CString &folderPath, CString &fileName)
{
	CString driveName;
	CString directory;
	CString fileNameBase;
	CString fileNameExtention;
	SplitFilePath(fileNamePath, driveName, directory, fileNameBase, fileNameExtention);
	folderPath = driveName + directory;
	fileName = fileNameBase + fileNameExtention;
}

void FileUtils::GetFolderPathAndFileNameW(CStringW &fileNamePath, CStringW &folderPath, CStringW &fileName)
{
	CStringW driveName;
	CStringW directory;
	CStringW fileNameBase;
	CStringW fileNameExtention;
	SplitFilePathW(fileNamePath, driveName, directory, fileNameBase, fileNameExtention);
	folderPath = driveName + directory;
	fileName = fileNameBase + fileNameExtention;
}

void FileUtils::GetFileBaseNameAndExtension(CString &fileName, CString &fileNameBase, CString &fileNameExtention)
{
	CString driveName;
	CString directory;
	SplitFilePath(fileName, driveName, directory, fileNameBase, fileNameExtention);
}

void FileUtils::GetFileBaseName(CString &fileName, CString &fileBaseName)
{
	CString driveName;
	CString directory;
	//CString fileNameBase;
	CString fileNameExtention;
	SplitFilePath(fileName, driveName, directory, fileBaseName, fileNameExtention);
}

void FileUtils::GetFileExtension(CString &fileName, CString &fileNameExtention)
{
	CString driveName;
	CString directory;
	CString fileNameBase;
	SplitFilePath(fileName, driveName, directory, fileNameBase, fileNameExtention);
}


void FileUtils::UpdateFileExtension(CString &fileName, CString &newExtension)
{
	CString driveName;
	CString directory;
	CString fileNameBase;
	CString fileNameExtention;
	SplitFilePath(fileName, driveName, directory, fileNameBase, fileNameExtention);

	fileName.Empty();
	fileName.Append(driveName);
	fileName.Append("\\");
	fileName.Append(directory);
	fileName.Append("\\");
	fileName.Append(fileNameBase);
	fileName.Append(newExtension);

}

_int64 FileUtils::FileSize(LPCSTR fileName)
{
	LARGE_INTEGER li = { 0 };
	HANDLE hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		TRACE(_T("(FileSize)INVALID_HANDLE_VALUE error=%ld file=%s\n"), err, fileName);
		return li.QuadPart;
	}
	else {
		BOOL retval = GetFileSizeEx(hFile, &li);
		CloseHandle(hFile);
		if (retval != TRUE) {
			DWORD err = GetLastError();
			TRACE(_T("(GetFileSizeEx)Failed with error=%ld file=%s\n"), err, fileName);
		}
		else {
			long long fsize = li.QuadPart;
			return li.QuadPart;
		}
	}
	return li.QuadPart;
}

BOOL FileUtils::BrowseToFile(LPCTSTR filename)
{
	BOOL retval = TRUE;

	// It appears m_ie.Create  in CBrowser::OnCreate calls CoInitilize
	//Com_Initialize();

	ITEMIDLIST *pidl = ILCreateFromPath(filename);
	if (pidl) {
		HRESULT  ret = SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		if (ret != S_OK)
			retval = FALSE;
		ILFree(pidl);
	}
	else
		retval = FALSE;
	return retval;
}

BOOL FileUtils::BrowseToFileW(LPCWSTR filename)
{
	BOOL retval = TRUE;

	// It appears m_ie.Create  in CBrowser::OnCreate calls CoInitilize
	//Com_Initialize();

	ITEMIDLIST *pidl = ILCreateFromPathW(filename);
	if (pidl) {
		HRESULT  ret = SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		if (ret != S_OK)
			retval = FALSE;
		ILFree(pidl);
	}
	else
		retval = FALSE;
	return retval;
}

void  FileUtils::MakeValidFilePath(CString &name, BOOL bReplaceWhiteWithUnderscore)
{
	SimpleString validName(name.GetLength());
	validName.Append((LPCSTR)name, name.GetLength());
	FileUtils::MakeValidFilePath(validName, bReplaceWhiteWithUnderscore);
	name.Empty();
	name.Append(validName.Data(), validName.Count());
}

void  FileUtils::MakeValidFilePath(SimpleString &name, BOOL bReplaceWhiteWithUnderscore)
{
	// Always merge consecutive underscore characters
	int csLen = name.Count();
	char* str = name.Data();
	int i;
	int j = 0;
	char c;
	BOOL allowUnderscore = TRUE;
	for (i = 0; i < csLen; i++)
	{
		c = str[i];
		if (
			(c == '?') || (c == '<') || (c == '>') || (c == ':') ||  // not valid file name characters
			(c == '*') || (c == '|') || (c == '"') || (c == '\\')    // not valid file name characters
			// I seem to remember that browser would not open a file with name with these chars, 
			// but now I can't recreate the case. One more reminder I need to keep track of all test cases
			// 1.0.3.7 || (c == ',') || (c == ';') || (c == '%')
			|| (c == '%')
			)
		{
			if (allowUnderscore)
			{
				allowUnderscore = FALSE;
				str[j++] = '_';
			}
		}
		else if (bReplaceWhiteWithUnderscore && ((c == ' ') || (c == '\t')))
		{
			if (allowUnderscore)
			{
				str[j++] = '_';
				allowUnderscore = FALSE;
			}
		}
		else if ((c < 32) || (c > 126))
		{
			if (allowUnderscore)
			{
				str[j++] = '_';
				allowUnderscore = FALSE;
			}
		}
		else
		{
			str[j++] = c;
			allowUnderscore = TRUE;
		}
	}
	name.SetCount(j);
}

void  FileUtils::MakeValidFileName(CString &name, BOOL bReplaceWhiteWithUnderscore)
{
	SimpleString validName(name.GetLength());
	validName.Append((LPCSTR)name, name.GetLength());
	FileUtils::MakeValidFileName(validName, bReplaceWhiteWithUnderscore);
	name.Empty();
	name.Append(validName.Data(), validName.Count());
}

void  FileUtils::MakeValidFileName(SimpleString &name, BOOL bReplaceWhiteWithUnderscore)
{
	// Always merge consecutive underscore characters
	int csLen = name.Count();
	char* str = name.Data();
	int i;
	int j = 0;
	char c;
	BOOL allowUnderscore = TRUE;
	for (i = 0; i < csLen; i++)
	{
		c = str[i];
		if (
			(c == '?') || (c == '/') || (c == '<') || (c == '>') || (c == ':') ||  // not valid file name characters
			(c == '*') || (c == '|') || (c == '"') || (c == '\\')                  // not valid file name characters
			// I seem to remember that browser would not open a file with name with these chars, 
			// but now I can't recreate the case. One more reminder I need to keep track of all test cases
			// 1.0.3.7 || (c == ',') || (c == ';') || (c == '%')
			|| (c == '%')
			)
		{
			if (allowUnderscore)
			{
				allowUnderscore = FALSE;
				str[j++] = '_';
			}
		}
		else if (bReplaceWhiteWithUnderscore && ((c == ' ') || (c == '\t')))
		{
			if (allowUnderscore)
			{
				str[j++] = '_';
				allowUnderscore = FALSE;
			}
		}
		else if ((c < 32) || (c > 126))
		{
			if (allowUnderscore)
			{
				str[j++] = '_';
				allowUnderscore = FALSE;
			}
		}
		else
		{
			str[j++] = c;
			allowUnderscore = TRUE;
		}
	}
	name.SetCount(j);
}

void  FileUtils::MakeValidFileNameW(CStringW &name, CStringW &result, BOOL bReplaceWhiteWithUnderscore)
{
	// Always merge consecutive underscore characters
	int csLen = name.GetLength();
	int i;

	wchar_t c;
	BOOL allowUnderscore = TRUE;
	for (i = 0; i < csLen; i++)
	{
		c = name[i];
		if (
			(c == L'?') || (c == L'/') || (c == L'<') || (c == L'>') || (c == L':') ||  // not valid file name characters
			(c == L'*') || (c == L'|') || (c == L'"') || (c == L'\\')                  // not valid file name characters
			// I seem to remember that browser would not open a file with name with these chars, 
			// but now I can't recreate the case. One more reminder I need to keep track of all test cases
			// 1.0.3.7 || (c == ',') || (c == ';') || (c == '%')
			|| (c == L'%')
			)
		{
			if (allowUnderscore)
			{
				allowUnderscore = FALSE;
				result.AppendChar(L'_');
			}
		}
		else if (bReplaceWhiteWithUnderscore && ((c == L' ') || (c == L'\t')))
		{
			if (allowUnderscore)
			{
				result.AppendChar(L'_');
				allowUnderscore = FALSE;
			}
		}
		else if (iswcntrl(c))
		{
			if (allowUnderscore)
			{
				result.AppendChar(L'_');
				allowUnderscore = FALSE;
			}
		}
		else
		{
			result.AppendChar(c);
			allowUnderscore = TRUE;
		}
	}
}

BOOL FileUtils::Write2File(CString &cStrNamePath, const unsigned char *data, int dataLength)
{
	DWORD dwAccess = GENERIC_WRITE;
	DWORD dwCreationDisposition = CREATE_ALWAYS;

	HANDLE hFile = CreateFile(cStrNamePath, dwAccess, FILE_SHARE_READ, NULL, dwCreationDisposition, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		int deb = 1;
		return FALSE;
	}
	else
	{
		const unsigned char* pszData = data;
		int nLeft = dataLength;

		for (;;)
		{
			DWORD nWritten = 0;

			BOOL retWrite = WriteFile(hFile, pszData, nLeft, &nWritten, 0);
			if (nWritten != nLeft)
				int deb = 1;

			if (nWritten < 0)
				break;

			pszData += nWritten;
			nLeft -= nWritten;
			if (nLeft <= 0)
				break;
		}

		BOOL retClose = CloseHandle(hFile);
		if (retClose == FALSE)
			int deb = 1;
	}
	return TRUE;
}

BOOL FileUtils::Write2File(CStringW &cStrNamePath, const unsigned char *data, int dataLength)
{
	DWORD dwAccess = GENERIC_WRITE;
	DWORD dwCreationDisposition = CREATE_ALWAYS;

	HANDLE hFile = CreateFileW(cStrNamePath, dwAccess, FILE_SHARE_READ, NULL, dwCreationDisposition, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		int deb = 1;
		return FALSE;
	}
	else
	{
		const unsigned char* pszData = data;
		int nLeft = dataLength;

		for (;;)
		{
			DWORD nWritten = 0;

			BOOL retWrite = WriteFile(hFile, pszData, nLeft, &nWritten, 0);
			if (nWritten != nLeft)
				int deb = 1;

			if (nWritten < 0)
				break;

			pszData += nWritten;
			nLeft -= nWritten;
			if (nLeft <= 0)
				break;
		}

		BOOL retClose = CloseHandle(hFile);
		if (retClose == FALSE)
			int deb = 1;
	}
	return TRUE;
}

int FileUtils::Write2File(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
{
	DWORD bytesToWrite = nNumberOfBytesToWrite;
	const unsigned char* pszData = (unsigned char*)lpBuffer;
	DWORD nwritten = 0;
	while (bytesToWrite > 0)
	{
		nwritten = 0;
		if (!WriteFile(hFile, pszData, bytesToWrite, &nwritten, NULL)) {
			DWORD retval = GetLastError();
			break;
		}
		pszData += nwritten;
		bytesToWrite -= nwritten;
	}
	*lpNumberOfBytesWritten = nNumberOfBytesToWrite - bytesToWrite;
	if (*lpNumberOfBytesWritten != nNumberOfBytesToWrite)
		return FALSE;
	else
		return TRUE;
}

BOOL FileUtils::ReadEntireFile(CString &cStrNamePath, SimpleString &txt)
{
	DWORD dwAccess = GENERIC_READ;
	DWORD dwCreationDisposition = OPEN_EXISTING;

	HANDLE hFile = CreateFile(cStrNamePath, dwAccess, FILE_SHARE_READ, NULL, dwCreationDisposition, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		int deb = 1;
		return FALSE;
	}
	else
	{
		LPOVERLAPPED lpOverlapped = 0;
		LPVOID       lpBuffer;
		DWORD        nNumberOfBytesToRead;
		DWORD      nNumberOfBytesRead;

		_int64 fsize = FileUtils::FileSize(cStrNamePath);
		_int64 bytesLeft = fsize;
		txt.ClearAndResize((int)fsize+1);
		lpBuffer = txt.Data();
		while (bytesLeft > 0)
		{
			if (bytesLeft > 0xffffffff)
				nNumberOfBytesToRead = 0xffffffff;
			else
				nNumberOfBytesToRead = (DWORD)bytesLeft;

			BOOL retval = ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, &nNumberOfBytesRead, lpOverlapped);
			if (retval == FALSE)
				break;

			int newCount = txt.Count() + nNumberOfBytesRead;
			txt.SetCount(newCount);
			lpBuffer = txt.Data(newCount);
			bytesLeft -= nNumberOfBytesRead;
		}
		BOOL retClose = CloseHandle(hFile);
	}

	return TRUE;
}

BOOL FileUtils::NormalizeFilePath(CString &filePath)
{
	CString outFilePath;
	filePath.Replace("/", "\\");
	filePath.TrimRight("\\");
	int i, k;
	char c;
	k = 0;
	for (i = 0; i < filePath.GetLength(); )
	{
		c = filePath.GetAt(i);
		outFilePath.AppendChar(c);
		i++;
		if (c == '\\')
		{
			for ( ; i < filePath.GetLength(); i++)
			{
				c = filePath.GetAt(i);
				if (c != '\\')
					break;
			}
		}
	}

	char buffer[MAX_PATH + 1];
	BOOL ret = PathCanonicalizeA(buffer, (LPCSTR)outFilePath);
	if (ret)
	{
		filePath.Empty();
		filePath.Append(buffer);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL FileUtils::CreateDirectory(const char *path)
{
	CList<CString, CString &> folderList;

	CString folderPath(path);
	CString subFolderPath(path);
	CString folderName;

	int i;
	BOOL foundValidSubFolder = FALSE;
	for (i = 0; i < 100; i++)
	{
		if (FileUtils::PathDirExists(subFolderPath))
		{
			foundValidSubFolder = TRUE;
			break;
		}

		folderPath = subFolderPath;
		folderPath.TrimRight("\\");
		FileUtils::GetFolderPathAndFileName(folderPath, subFolderPath, folderName);
		if (subFolderPath.IsEmpty())
			break;
		folderList.AddHead(folderName);
	}
	if (!foundValidSubFolder)
		return FALSE;
	if (folderList.IsEmpty())
		return TRUE;

	folderPath = subFolderPath;
	folderPath.TrimRight("\\");
	folderPath.Append("\\");
	while (folderList.GetCount() > 0)
	{
		folderName = folderList.RemoveHead();
		folderPath.Append(folderName);
		folderPath.Append("\\");
		if (!::CreateDirectory(folderPath, NULL))
			return FALSE;
		if (!FileUtils::PathDirExists(folderPath))
			return FALSE;
	}

	return TRUE;
}

DWORD FileUtils::CreateDirectoryW(const wchar_t *path)
{
	CList<CStringW, CStringW &> folderList;

	CStringW folderPath(path);
	CStringW subFolderPath(path);
	CStringW folderName;

	int i;
	BOOL foundValidSubFolder = FALSE;
	for (i = 0; i < 100; i++)
	{
		if (FileUtils::PathDirExistsW(subFolderPath))
		{
			foundValidSubFolder = TRUE;
			break;
		}

		folderPath = subFolderPath;
		folderPath.TrimRight(L"\\");
		FileUtils::GetFolderPathAndFileNameW(folderPath, subFolderPath, folderName);
		if (subFolderPath.IsEmpty())
			break;
		folderList.AddHead(folderName);
	}
	if (!foundValidSubFolder)
		return FALSE;
	if (folderList.IsEmpty())
		return TRUE;

	folderPath = subFolderPath;
	folderPath.TrimRight(L"\\");
	folderPath.Append(L"\\");
	while (folderList.GetCount() > 0)
	{
		folderName = folderList.RemoveHead();
		folderPath.Append(folderName);
		folderPath.Append(L"\\");
		if (!::CreateDirectoryW(folderPath, NULL))
			return FALSE;
		if (!FileUtils::PathDirExistsW(folderPath))
			return FALSE;
	}

	return TRUE;
}

BOOL FileUtils::DeleteFile(const char *path)
{
	return ::DeleteFile(path);
}

BOOL FileUtils::DeleteFileW(const wchar_t *path)
{
	return ::DeleteFileW(path);
}

void FileUtils::UnitTest()
{
	DWORD error;
	bool retPathExists;
	BOOL retRemoveDirectory;
	BOOL retRemoveFiles;

	CString pathA;
	CString subpathA;
	CStringW pathW;
	CStringW subpathW;

	CString safeDir = "AppData\\Local\\Temp";
	CStringW safeDirW = L"AppData\\Local\\Temp";

	/////////////////////////////////////////
	pathW = GetmboxviewTempPathW();
	if (pathW.Find(safeDirW) < 0)
		ASSERT(false);
	retRemoveFiles = RemoveDirW(pathW, true);
	retRemoveDirectory = RemoveDirectoryW(pathW, error);

	pathW = GetmboxviewTempPathW();
	retPathExists = PathDirExistsW(pathW);
	ASSERT(retPathExists);
	retRemoveFiles = RemoveDirW(pathW, true);
	retRemoveDirectory = RemoveDirectoryW(pathW, error);
	ASSERT(retRemoveDirectory);

	pathW = GetmboxviewTempPathW();
	retPathExists = PathDirExistsW(pathW);
	subpathW = GetmboxviewTempPathW(L"Inline");
	retPathExists = PathDirExistsW(subpathW);
	ASSERT(retPathExists);
	retRemoveFiles = RemoveDirW(subpathW, true);
	//retRemoveDirectory = RemoveDirectoryW(subpathW, error);
	retRemoveDirectory = RemoveDirectoryW(pathW, error);
	ASSERT(retRemoveDirectory);

	/////////////////////////////////////////

	/////////////////////////////////
	pathA = GetmboxviewTempPath();
	if (pathA.Find(safeDir) < 0)
		ASSERT(false);
	retRemoveFiles = RemoveDir(pathA, true);
	retRemoveDirectory = RemoveDirectory(pathA, error);

	pathA = GetmboxviewTempPath();
	retPathExists = PathDirExists(pathA);
	ASSERT(retPathExists);
	retRemoveFiles = RemoveDir(pathA, true);
	retRemoveDirectory = RemoveDirectory(pathA, error);
	ASSERT(retRemoveDirectory);


	pathA = GetmboxviewTempPath();
	retPathExists = PathDirExists(pathA);
	subpathA = GetmboxviewTempPath("Inline");
	retPathExists = PathDirExists(subpathA);
	ASSERT(retPathExists);
	//retRemoveDirectory = RemoveDirectory(subpathA, error);
	retRemoveDirectory = RemoveDirectoryA(pathA, error);
	ASSERT(retRemoveDirectory);


}