
#include "stdafx.h"
#include "myfile.h"
#include "shlwapi.h"
#include "MyFile.h"
#include "Parameter.h"
#ifndef UNDER_CE
#pragma comment(lib, "Shlwapi.lib" )
#endif// UNDER_CE

int	IsFileExit( const CString& _pathname )
{
	int		len = _pathname.GetLength();
	if( len <= 0 )
		return 0;

	CString pathname = _pathname;
	
	if( pathname.Right( 1 ) == '\\' )
	{
		pathname = pathname.Left( len - 1 );
	}

	int		nRes = 0;

	WIN32_FIND_DATA  find_data = {0};

	CString		strTemp = pathname;

	HANDLE handle = FindFirstFile( strTemp, &find_data ); 
	
	if( handle != INVALID_HANDLE_VALUE  )
	{
		if( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			nRes = 2;
		}
		else
		{
			nRes = 1;
		}
		FindClose( handle );
	}

	return nRes;
}

int ComparePath(const CString& path1, const CString& path2 )
{
	int nres = -1;
	
	CString p1 = path1; 
	CString p2 = path2;

	if( p1.Right(1) == "\\" ) p1 = p1.Left(p1.GetLength()-1);
	if( p2.Right(1) == "\\" ) p2 = p2.Left(p2.GetLength()-1);
	
	p1.MakeLower(); p2.MakeLower();

	nres = p1.Compare( p2 );

	return nres;
}

CString pathnameGetname_ext( const CString& pathname )
{
	if( pathname.IsEmpty() ) return _T("");

	CString	str = pathname;
	int n = str.ReverseFind('\\');
	str = str.Right( str.GetLength() - n - 1 );
	return str;
}

// example ".etp", ".doc";
CString nameGetExp( const CString& filename )
{
	if( filename.IsEmpty() ) return _T("");

	int			length = filename.GetLength();
	int			PointPos = filename.ReverseFind('.');

	int			slashPos = filename.ReverseFind('\\');

	CString ext =_T("");

	if( (slashPos != -1) && (slashPos < PointPos) )
		ext = filename.Right( length - PointPos );
	else if( slashPos == -1 )
	{
		if( length > PointPos )
		{
			ext = filename.Right( length - PointPos );
		}
	}

	ext.MakeUpper();
	return ext;
}

CString path_append( const CString& path1, const CString& path2 )
{
	TCHAR	path[MAX_PATH*10] = TEXT("");

#ifdef _UNICODE
	wcscpy( path, path1 );
#else
	strcpy( path, path1 );
#endif // _UNICODE

#ifndef UNDER_CE
	PathAppend( path, path2 );
#endif 

	return path;
}

// example : "file", "file1"..
CString pathnameGetname( const CString& pathname )
{
	if( pathname.IsEmpty() )return _T("");

	CString	str = pathname;
	int n = str.ReverseFind('\\');

	str = str.Right( str.GetLength() - n - 1 );

	n = str.ReverseFind('.');
	
	if( n != -1 )	str = str.Left( n );

	return str;
}

CString pathNameGetPath( const CString& filepathname )
{
	if( filepathname.IsEmpty() )return _T("");

	CString		path = filepathname;
	int pos = path.ReverseFind('\\');
	if( pos != -1 )
		path = path.Left( pos +1 );

	return path;
}

CString NewFileName( const CString& oldName )
{
	CString		strNewFileName = oldName;

	CString		strFileExt = nameGetExp( oldName );
	CString		strFilePath = pathNameGetPath( oldName );
	CString		strFileName = pathnameGetname( oldName );

	if( strFilePath.GetLength() <= 0 )
	{
		ASSERT( 0 );
		//return "";
	}

	if( strFileName.GetLength() <= 0 )
	{
		strFileName = "NoName";
		strNewFileName = strFilePath + strFileName + strFileExt;
	}

	int				nTemp = 1;

	CString temp = strFileName;

	while( IsFileExit( strNewFileName ) )
	{
#ifdef _UNICODE
		strFileName.Format( L"%s%d", temp, nTemp );
#else
		strFileName.Format( "%s%d", temp, nTemp );
#endif
		nTemp ++;

		strNewFileName = strFilePath + strFileName + strFileExt;
	}

	return strNewFileName;
}

BOOL MyCopyFile(CString strSource, CString strDest )
{
	if( strSource.CompareNoCase( strDest ) == 0 )
		return TRUE;

#ifdef _WIN32_WCE
	
	return CopyFile( strSource, strDest, FALSE );

#else // _WIN32_WCE

	SHFILEOPSTRUCT			op = {0};

#ifndef _UNICODE
	CHAR			from[MAX_PATH] = "";
	strcpy( from, strSource.GetBuffer(0) );
	CHAR			to[MAX_PATH] = "";
	strcpy( to, strDest.GetBuffer(0) );
#else // _unicode
	WCHAR			from[MAX_PATH] = L"";
	wcscpy( from, strSource.GetBuffer(0) );
	WCHAR			to[MAX_PATH] = L"";
	wcscpy( to, strDest.GetBuffer(0) );
#endif // end define UNICODE
	
	op.hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	op.wFunc = FO_COPY ;
	op.pFrom = from;
	op.pTo = to;
	op.fAnyOperationsAborted = TRUE;

#ifndef _UNICODE
	op.lpszProgressTitle = "Create file ...";
#else // _unicode
	op.lpszProgressTitle = L"Create file...";
#endif // _UNICODE
	op.fFlags = FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR;


	int nRes = SHFileOperation( &op );

	if( nRes != 0 ) return FALSE;

#endif // _WIN32_WCE

	return TRUE;
}

int del_path_and_file( CString strPath )
{
	int		nres = 1;
	if( strPath.IsEmpty() ) return nres;

	if( FILE_EXIT==IsFileExit( strPath ) )
	{
		if( '\\' == strPath.Right(1) )
			strPath = strPath.Left( strPath.GetLength()-1 );
		
		if( DeleteFile( strPath ) )
			return 0;
		return nres;
	}

	TCHAR chEnd = strPath.GetAt( strPath.GetLength()-1 );
	if( chEnd != '\\' )
		strPath += "\\";

	WIN32_FIND_DATA  find_data = {0};

	CString		strTemp = strPath + _T("*.*");

	HANDLE handle = FindFirstFile( strTemp, &find_data ); 
	
	if( handle != INVALID_HANDLE_VALUE  )
	{
		BOOL		bFind = TRUE;

		while( bFind )
		{
			if( find_data.cFileName[0] != '.' )
			{
				strTemp = strPath + find_data.cFileName;
				del_path_and_file( strTemp );
			}
			bFind = FindNextFile( handle, &find_data );
		}

		FindClose( handle );
	}

	RemoveDirectory( strPath );

	return nres;
}

int move_file(  CString oldPath, CString newPath, bool bDelOldPath )
{
	int		nRes = 0;
	TCHAR chEnd = oldPath.GetAt( oldPath.GetLength()-1 );
	if( chEnd != '\\' )
		oldPath += "\\";
	chEnd = newPath.GetAt( newPath.GetLength()-1 );
	if( chEnd != '\\' )
		newPath += "\\";

	CString		oldTemp = oldPath + _T("*.*");

	WIN32_FIND_DATA  find_data = {0};
	HANDLE handle = FindFirstFile( oldTemp, &find_data ); 
	
	if( handle != INVALID_HANDLE_VALUE  )
	{
		BOOL		bFind = TRUE;

		while( bFind )
		{
			if( find_data.cFileName[0] != '.' )
			{
				CString temp1 = oldPath + find_data.cFileName;
				CString temp2 = newPath + find_data.cFileName;
				if( MyCopyFile( temp1, temp2 ) )
				{
					DeleteFile( temp1 );
					nRes ++;
				}
			}
			bFind = FindNextFile( handle, &find_data );
		}

		FindClose( handle );
	}

	if( bDelOldPath )
	{
		RemoveDirectory( oldPath );
	}
	return nRes;
}

void	RealseDirectory(CPtrList & listfile)
{
	POSITION pos=listfile.GetHeadPosition();
	while(pos)
	{
		Cfileliststr *pfile=(Cfileliststr*)listfile.GetNext(pos);
		if(pfile)
		{
			delete pfile;
		}
	}
	listfile.RemoveAll();
}

void MyCopyDirectory(CString strSource, CString strDest )
{
	if(strSource==strDest) return;
	CFileFind finder;
	CString strFolderPath;
	CString	strFolderDest=strDest;
	
	CString str = strSource;
	if(str != "" && str.Right(1) != '\\')//为全目录名添加'\'
		str += '\\';
	
	if(strFolderDest!="" && strFolderDest.Right(1)!='\\')
		strFolderDest+='\\';

    strFolderPath = str;
	BOOL bWorking = finder.FindFile(strFolderPath + _T("*.*"));	
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		
		if(finder.IsDirectory() && !finder.IsDots())
		{
			CString  strPath = finder.GetFilePath();
			strPath.Replace(strSource,_T(""));
			
			CString strSourceD=strSource;
			if(strSourceD!="" && strSourceD.Right(1)!='\\')
			strSourceD+='\\';
			if(strPath!="" && strPath.Left(1)=='\\')
			strPath=strPath.Mid(1);

			CString strDestD=strDest;
			if(strDestD!="" && strDestD.Right(1)!='\\')
				strDestD+='\\';
			if(strPath!="" && strPath.Left(1)=='\\')
			strPath=strPath.Mid(1);

			
			MyCopyDirectory(strSourceD+strPath,strDestD+strPath);
			//查找下一个目录
			
		}
		else if(finder.GetFileName()!="." && finder.GetFileName()!="..")
		{
			MyCopyFile(finder.GetFilePath(),strFolderDest+finder.GetFileName());
		}
		
	}//end of while
	finder.Close();		
}

BOOL   DeleteDirectory(   LPCTSTR   strDirname   ) 
{ 
	CFileFind   tempFind;   //声明一个CFileFind类变量，以用来搜索 
	char   tempFileFind[200];   //用于定义搜索格式 
	CString DirName= strDirname;
	if(DirName != "" && DirName.Right(1) == '\\')//为全目录名添加'\'
		DirName=DirName.Left(DirName.GetLength()-1);
	sprintf(tempFileFind, "%s\\*.*",DirName); 
	//匹配格式为*.*,即该目录下的所有文件   
	wchar_t ptempFileFind[400]={0};
	MultiByteToWideChar(CP_ACP,0,tempFileFind,-1,ptempFileFind,0);
	BOOL   IsFinded=(BOOL)tempFind.FindFile(ptempFileFind);

	//查找第一个文件 
	while(IsFinded) 
	{ 
		IsFinded=(BOOL)tempFind.FindNextFile();   //递归搜索其他的文件   
        if(!tempFind.IsDots())   //如果不是 ". "目录 
        { 
			wchar_t   foundFileName[200]; 
			_tcscpy(foundFileName,tempFind.GetFileName().GetBuffer(200)); 
			if(tempFind.IsDirectory())   //如果是目录，则递归地调用 
			{   //DeleteDirectory 
                char   tempDir[200]; 
                sprintf(tempDir, "%s\\%s",DirName,foundFileName); 
				wchar_t ptempDir[400]={0};
				MultiByteToWideChar(CP_ACP,0,tempDir,-1,ptempDir,0);
                DeleteDirectory(ptempDir); 
			} 
			else 
			{   //如果是文件则直接删除之 
				char   tempFileName[200]; 
				sprintf(tempFileName, "%s\\%s ",DirName,foundFileName);
				wchar_t ptempFileName[400]={0};
				MultiByteToWideChar(CP_ACP,0,tempFileName,-1,ptempFileName,0);
				DeleteFile(ptempFileName); 
			} 
        } 
	} 
	tempFind.Close(); 
	if(!RemoveDirectory(DirName))   //删除目录 
	{ 
	//	AfxMessageBox( "删除目录失败！ ",MB_OK); 
		return   FALSE; 
	} 
        return   TRUE; 
}

void	SearchDirectory(const CString& strfolder,CPtrList& listfile,const CString& filter,const CString& filterfolder)
{
	CFileFind finder;
	CString strFolderPath;
	
	CString str = strfolder;
	if(str != "" && str.Right(1) != '\\')//为全目录名添加'\'
		str += '\\';
    strFolderPath = str;
	BOOL bWorking = finder.FindFile(strFolderPath + _T("*.*"));	
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDirectory() && !finder.IsDots() && finder.GetFileName()==filterfolder)
			continue;
		if(finder.IsDirectory() && !finder.IsDots())
		{
			strFolderPath = finder.GetFilePath();
			//查找下一个目录
				SearchDirectory(strFolderPath, listfile,filter,filterfolder);
		}
		else if(finder.GetFileName()!="." && finder.GetFileName()!="..")
		{
		//	array1.Add(finder.GetFilePath());
			CString strfilter=filter;
			strfilter.MakeUpper();
			CString strExtension=finder.GetFileName();
			strExtension.MakeUpper();			
			strExtension=strExtension.Mid(strExtension.ReverseFind('.')+1);						
			if((strfilter.Find(strExtension,0)>=0 && !strExtension.IsEmpty()) || filter=="*.*" || filter=="")
			{			
				Cfileliststr *pfilestr=new Cfileliststr();
				if(pfilestr)
				{
					pfilestr->filepath=finder.GetFilePath();
					pfilestr->filename=finder.GetFileName();
					finder.GetLastAccessTime(pfilestr->m_fileaccesstime);
					finder.GetLastWriteTime(pfilestr->m_filewritetime);
					pfilestr->m_size=finder.GetLength();
					listfile.AddTail(pfilestr);
				}
			}
		}
		
	}//end of while
	finder.Close();		
}

BOOL BrowseForFolder(CString& strpath,HWND  hWnd)
{
	BOOL bres=FALSE;
	LPMALLOC   pMalloc; 
	if   (::SHGetMalloc(&pMalloc)   ==   NOERROR) 
	{ 
		BROWSEINFO   bi; 
		char   pszBuffer[MAX_PATH]; 
		LPITEMIDLIST   pidl; 
		bi.hwndOwner=hWnd;
		//bi.hwndOwner   =   GetSafeHwnd(); 
		bi.pidlRoot   =   NULL;
		wchar_t wpszBuffer[520]={0};
		MultiByteToWideChar(CP_ACP,0,pszBuffer,-1,wpszBuffer,0);
		bi.pszDisplayName   =   wpszBuffer; 
		bi.lpszTitle   =   _T( "Select   a   Starting   Directory "); 
		bi.ulFlags   =   BIF_RETURNFSANCESTORS   |   BIF_RETURNONLYFSDIRS; 
		bi.lpfn   =   NULL; 
		bi.lParam   =   0; 
		if   ((pidl   =   ::SHBrowseForFolder(&bi))   !=   NULL) 
		{ 
			if   (::SHGetPathFromIDList(pidl,   wpszBuffer)) 
			{   
				bres=TRUE;
				//MessageBox(pszBuffer); 
				strpath.Format(_T("%s"),pszBuffer);
			} 
			pMalloc-> Free(pidl); 
		} 
		pMalloc-> Release();
	}
	return bres;
}

BOOL	isNewFile(const CString& filename,const DWORD& filesize,const CString& acesstime)
{
	BOOL bres=FALSE;
		WIN32_FIND_DATA fileInfo;
				HANDLE	hFind = FindFirstFile(filename,&fileInfo);
				if(hFind != INVALID_HANDLE_VALUE)
				{
					DWORD	fileSize1 = fileInfo.nFileSizeLow;
					FILETIME ft=fileInfo.ftLastAccessTime; 
					CTime time(ft);
					CString strtime=DayTimeToString(time);
					if(filesize!=fileSize1 ||  acesstime>strtime)
					{
						//下载
						//	AfxMessageBox("真");
						bres=TRUE;
					}
					
				}
				else
				{
					bres=TRUE;
				}
				FindClose(hFind);
	return bres;
}


void ExecFile( const CString&  szFile, BOOL bWait ) 
{
 SHELLEXECUTEINFO ExecInfo;
 ::ZeroMemory( &ExecInfo, sizeof( SHELLEXECUTEINFO ) );
 ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
 ExecInfo.cbSize = sizeof( SHELLEXECUTEINFO );
 ExecInfo.hwnd = NULL;
 ExecInfo.lpVerb = _T("open");
 ExecInfo.lpFile = szFile;
 ExecInfo.nShow = SW_SHOW;
 ::ShellExecuteEx( &ExecInfo ); 
 
 if(bWait)
	 ::WaitForSingleObject( ExecInfo.hProcess, INFINITE );
 
 ::CloseHandle( ExecInfo.hProcess );
}

CString parentDirectory(const CString& path )
{
	if( path.GetLength() < 2 ) return _T("");
	
	CString		minPath = path;
	int pos = minPath.ReverseFind('\\');
	if( pos != -1 )
		minPath = minPath.Left( pos );
	
	return minPath;
}

// c:\temp3\dir
int createDirectory(const CString& path )
{
	int		nerr = -1;

	CString	_path = path;

	if( _path.Right(1) == '\\' )
		_path = _path.Left( _path.GetLength() - 1);

	if( IsFileExit(_path) == 2 ) 
		return 0;

	CString parentPath = parentDirectory( _path );

	if( !parentPath.IsEmpty() )
	{
		nerr = createDirectory( parentPath );
		if( 0 != nerr )
			return nerr;
	}

	if( CreateDirectory( _path, NULL ) )
		nerr = 0;

	return nerr;
}

//////////////////////////////////////////////////////////////////////////
//

CMyStdioFile::CMyStdioFile() : CStdioFile()
{
}

BOOL CMyStdioFile::ReadString(CString& str)
{
	BOOL bres = FALSE;

#ifdef UNDER_CE

	str.Empty();

	char	buf[2048] = "";

	while( Read( buf, 1 ) > 0 )
	{
		if( buf[0] == '\n' || buf[0] == '\r' )
		{
			if( str.IsEmpty() )
				continue;
			break;
		}

		str += CString(buf);
		bres = TRUE;
		memset(buf, 0, 2048 );
	}

#else

	 bres = CStdioFile::ReadString( str );

#endif //UNDER_CE

	return bres;
}

void CMyStdioFile::WriteString(const CString& str )
{
#ifdef UNDER_CE
	CString t = str;
	CFile::Write( t.GetBuffer(0), t.GetLength() );
#else
	 CStdioFile::WriteString( str );
#endif //UNDER_CE
	return;
}

#ifndef UNDER_CE

bool HSelectDir( CString &strPath, HWND hWnd )
{
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	
	bi.hwndOwner = hWnd;
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;
	bi.lpszTitle = TEXT("请选择存放目录");
	
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	
	bool bRet = FALSE;
	
	TCHAR szFolder[MAX_PATH*2];
	szFolder[0] = _T('\0');
	
	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
		{
			bRet = TRUE;
		}
		
		IMalloc *pMalloc = NULL; 
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc) 
		{  
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
		}
	}
	
	strPath = szFolder;
	return bRet;
}

#include "parameter.h"

int HSelectFile(CStringArray &array, BOOL bOpen, const CString &filter, BOOL bMulti, DWORD dwFlags, CWnd* pParent )
{
	array.RemoveAll();

	if( dwFlags == 0 )
	{
		dwFlags = OFN_ENABLESIZING|OFN_EXPLORER;
	}

	if( bMulti )
	{
		dwFlags |= OFN_ALLOWMULTISELECT;

	}
	if( bOpen )
	{
		dwFlags |= OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	}
	
	////////////////// 

	CString		str;

	CStringArray	extArray;
	RequestParamsStr( filter, extArray,_T("|"));

	for(int i=0; i<extArray.GetSize(); i++ )
	{
		CString ext = extArray.GetAt(i);
		ext.Format( TEXT("%s Files(*.%s)|*.%s|"), ext, ext, ext );
		str += ext;
	}

	str += _T("|");

	///////////////////

	CFileDialog	dlg( bOpen, NULL, NULL, dwFlags, str, pParent );

	if( bMulti )
	{
		TCHAR*	pBuf = new TCHAR[MAX_PATH * 516];
		memset( pBuf, 0, sizeof(TCHAR) * MAX_PATH * 516 );
		dlg.m_ofn.lpstrFile = pBuf;
		dlg.m_ofn.nMaxFile = 516*MAX_PATH; 
	}
	
	//////////////

	if( IDOK == dlg.DoModal() )
	{
		POSITION	pos = dlg.GetStartPosition();
		while( pos )
		{
			array.Add( dlg.GetNextPathName(pos) );
		}
	}

	if( bMulti )
	{
		delete dlg.m_ofn.lpstrFile;
	}

	return array.GetSize();
}

int HSelectFileMany(CStringArray &array, BOOL bOpen, const CString &filter, BOOL bMulti, DWORD dwFlags, CWnd* pParent )
{
	array.RemoveAll();
	
	if( dwFlags == 0 )
	{
		dwFlags = OFN_ENABLESIZING|OFN_EXPLORER;
	}
	
	if( bMulti )
	{
		dwFlags |= OFN_ALLOWMULTISELECT;
		
	}
	if( bOpen )
	{
		dwFlags |= OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	}
	
	////////////////// 
	
	CString		str;
	
	CStringArray	extArray;
	RequestParamsStr( filter, extArray, _T("|") );
	
	for(int i=0; i<extArray.GetSize(); i++ )
	{
		CString ext = extArray.GetAt(i);
		ext.Format( TEXT("%s Files(*.%s)|*.%s|"), ext, ext, ext );
		str += ext;
	}
	
	str += "|";
	
	///////////////////
	
	CFileDialog	dlg( bOpen, NULL, NULL, dwFlags, str, pParent );
	
	if( bMulti )
	{
		TCHAR*	pBuf = new TCHAR[MAX_PATH * 516];
		memset( pBuf, 0, sizeof(TCHAR) * MAX_PATH * 516 );
		dlg.m_ofn.lpstrFile = pBuf;
		dlg.m_ofn.nMaxFile = 516*MAX_PATH; 
	}
	
	//////////////
	
	if( IDOK == dlg.DoModal() )
	{
		POSITION	pos = dlg.GetStartPosition();
		while( pos )
		{
			
			/////////////////////////////xj add 
			WIN32_FIND_DATA   fileInfo;   
			HANDLE   hFind;   
			DWORD   fileSize;   
			CString fileName,filesizestr;
			//	fileName   =dlg.GetPathName();   
			fileName   =dlg.GetNextPathName(pos);
			hFind   =   FindFirstFile(fileName.GetBuffer(0)   ,&fileInfo);   
			if(hFind   !=   INVALID_HANDLE_VALUE)   
				fileSize   =   fileInfo.nFileSizeLow;   			
			FindClose(hFind);   
			filesizestr.Format(_T("%d"),fileSize);
			int a=fileName.ReverseFind(_T('\\'));
			fileName=fileName.Right(fileName.GetLength()-a-1);
			array.Add(fileName + _T("'^'")+filesizestr);
			//////////////xj///////////////////////
		//	CString strname=	dlg.GetNextPathName(pos);
		}
	}
	
	if( bMulti )
	{
		delete dlg.m_ofn.lpstrFile;
	}
	
	return array.GetSize();
}
#endif //UNDER_CE