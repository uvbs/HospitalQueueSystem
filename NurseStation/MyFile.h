
#ifndef _MyFile_h_
#define _MyFile_h_


// judge this is this file exit in disk, 

class Cfileliststr
{
public:
	CString		 filepath;
	CString		 filename;
	CTime		 m_fileaccesstime;
	CTime		 m_filewritetime;
	DWORD		 m_size;
};


#define FILE_EXIT		1
#define FOLDER_EXIT		2
#define FILE_EMPTY		0

int		IsFileExit( const CString& pathname );

int		ComparePath( const CString& path1, const CString& path2 );

// 为了防止同名，NewFileName 返回肯定是一个没有同名的文件名字
CString NewFileName( const CString& oldName );

// 文件路径、文件名字、文件扩展名操作

// example : "file.etp", "file1.doc"..
CString pathnameGetname_ext( const CString &pathname );

// example : "file", "file1"..
CString pathnameGetname( const CString& pathname );

// example ".ETP", ".DOC";
CString nameGetExp( const CString& filename );

// example c:\ 
CString pathNameGetPath( const CString& filepathname );

// 组合路径
CString path_append( const CString& path1, const CString& path2 );

// 
CString parentDirectory(const CString& path );
// 0: 成功， 其他的失败
int		createDirectory( const CString& path );

// 文件拷贝
BOOL MyCopyFile(CString strSource, CString strDest );

int		del_path_and_file( CString strPath );
int		move_file( CString oldPath, CString newPath, bool bDelOldPath=TRUE );

//  szFile
//   [in]: 要执行的文件的路径
//  bWait:
//   [in]: 是否要等待该程序运行结束
void ExecFile( const CString&  szFile, BOOL bWait = FALSE );
//Cfileliststr of CPtrList& listfile			
void	SearchDirectory( const CString& strfolder,CPtrList& listfile,const CString& filter,const CString& filterfolder);
void	RealseDirectory(CPtrList & listfile);
// 文件夹拷贝
void MyCopyDirectory(CString strSource, CString strDest );
//判断 文件大小const DWORD& filesize, 时间是否大于相前时间 const CString& acesstime 
BOOL	isNewFile(const CString& filename,const DWORD& filesize,const CString& acesstime);
////////////////////////////////////////////////////////////////////////// 
//   DelDirectory 
//   删除一个文件夹 
//   lpszPath   -   要删除的文件夹路径 
//   返回值：成功返回TRUE，否则返回FALSE 
//   备注：亦可用来删除单个文件 
////////////////////////////////////////////////////////////////////////// 
BOOL   DeleteDirectory(   LPCTSTR   lpszPath   );
BOOL BrowseForFolder(CString& strpath,HWND  hWnd);

//////////////////////////////////////////////////////////////////////////

class CMyStdioFile : public CStdioFile
{
public:
	CMyStdioFile();
public:
	
	BOOL	ReadString( CString& str );
	void	WriteString( const CString& str );
};
bool HSelectDir(CString &strPath, HWND hWnd=0);
int  HSelectFile(CStringArray &array, 
							BOOL bOpen=TRUE,
							const CString &filter=_T(""), 
							BOOL bMulti=FALSE, 
							DWORD dwFlags=0, 
							CWnd* pParent=NULL );

int  HSelectFileMany(CStringArray &array, 
				 BOOL bOpen=TRUE,
				 const CString &filter=_T(""), 
				 BOOL bMulti=FALSE, 
				 DWORD dwFlags=0, 
							CWnd* pParent=NULL );
#endif // _MyFile_h_
