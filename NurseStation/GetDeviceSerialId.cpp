#include "StdAfx.h"
#include "GetDeviceSerialId.h"

GetDeviceSerialId::GetDeviceSerialId(void)

{
	//m_szSystemInfo[MAX_DEVID_LENGTH] = {0};
	//m_uSystemInfoLen = 0;
}

GetDeviceSerialId::~GetDeviceSerialId(void)
{
}

CString GetDeviceSerialId::GetMACHex()
{
	//return GetDevIDHex(&GetDeviceSerialId::_GetMAC);
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	CString strDevIdHex = _T("");
	if(GetMAC())
	{
		for(UINT i = 0; i < m_uSystemInfoLen; i++)
		{
			if(m_szSystemInfo[i] == 20) continue;
			strDevIdHex.AppendFormat(_T("%X"), m_szSystemInfo[i]);
		}
	}

	return strDevIdHex;
}

CString GetDeviceSerialId::GetHdSnHex()
{
	//return GetDevIDHex(&GetDeviceSerialId::_GetHdSn);
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	CString strDevIdHex = _T("");
	if(GetHdSn())
	{
		for(UINT i = 0; i < m_uSystemInfoLen; i++)
		{
			if(m_szSystemInfo[i] == 20) continue;
			strDevIdHex.AppendFormat(_T("%X"), m_szSystemInfo[i]);
		}
	}

	return strDevIdHex;
}

CString GetDeviceSerialId::GetHdSnHex2()
{

	//  GETVERSIONOUTPARAMS contains the data returned from the   
	//  Get Driver Version function.  
	CString strSerialID;
	CString csPhysical;  
	csPhysical.Format(_T("\\\\.\\PhysicalDrive%d"), PHYSICAL_DRIVE_NUMBER);  
	HANDLE hPhysical = ::CreateFile(csPhysical,  
		GENERIC_READ | GENERIC_WRITE,  
		FILE_SHARE_READ | FILE_SHARE_WRITE,  
		NULL, OPEN_EXISTING, 0, NULL);  
	if (hPhysical != INVALID_HANDLE_VALUE)  
	{  
		//第一种方法  
		GETVERSIONOUTPARAMS VersionParams;  
		DWORD dwRet = sizeof(VersionParams);  
		if (::DeviceIoControl(hPhysical,  
			DFP_GET_VERSION,  
			NULL, 0,  
			&VersionParams, sizeof(VersionParams),  
			&dwRet, NULL))  
		{  
			if (VersionParams.bIDEDeviceMap > 0)  
			{  
				BYTE bIDCmd = 0; // IDE or ATAPI IDENTIFY cmd  
				SENDCMDINPARAMS scip;  
				BYTE IdOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];  

				// Now, get the ID sector for all IDE devices in the system.  
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,  
				// otherwise use the IDE_ATA_IDENTIFY command  
				bIDCmd = (VersionParams.bIDEDeviceMap >> PHYSICAL_DRIVE_NUMBER & 0x10) ? 
IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;  

				memset(IdOutCmd, 0, sizeof(IdOutCmd));  
				memset(&scip, 0, sizeof(scip));  
				scip.cBufferSize = IDENTIFY_BUFFER_SIZE;  
				scip.irDriveRegs.bFeaturesReg = 0;  
				scip.irDriveRegs.bSectorCountReg = 1;  
				scip.irDriveRegs.bSectorNumberReg = 1;  
				scip.irDriveRegs.bCylLowReg = 0;  
				scip.irDriveRegs.bCylHighReg = 0;  

				// Compute the drive number.  
				scip.irDriveRegs.bDriveHeadReg = 0xA0 | ((PHYSICAL_DRIVE_NUMBER & 1) << 4);  

				// The command can either be IDE identify or ATAPI identify.  
				scip.irDriveRegs.bCommandReg = bIDCmd;  
				scip.bDriveNumber = PHYSICAL_DRIVE_NUMBER;  
				scip.cBufferSize = IDENTIFY_BUFFER_SIZE;  

				if (DeviceIoControl(hPhysical,  
					DFP_RECEIVE_DRIVE_DATA,  
					&scip,  
					sizeof(SENDCMDINPARAMS) - 1,  
					IdOutCmd,  
					sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,  
					&dwRet, NULL))  
				{
					//printf("Serial Number = ");
					for (DWORD i = 0; i < 20; i += 2)  
					{
						//putchar(((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer[i + 10 * sizeof(TCHAR) + 1]);  
						//putchar(((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer[i + 10 * sizeof(TCHAR)]);
						//strSerialID.AppendFormat(_T("%02X%02X"), 
						//	((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer[i + 10 * sizeof(TCHAR) + 1],
						//	((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer[i + 10 * sizeof(TCHAR)]);

						BYTE ch1 = ((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer[i + 10 * sizeof(TCHAR) + 1];
						BYTE ch2 = ((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer[i + 10 * sizeof(TCHAR)];
						if(ch1 != 0x20)
						{
							strSerialID.AppendFormat(_T("%02X"), ch1);
						}
						if(ch2 != 0x20)
						{
							strSerialID.AppendFormat(_T("%02X"), ch2);
						}
					}  
					//putchar('\n');  
					//AfxMessageBox(strSerialID);
				}  
				else  
				{  
					//printf("DeviceIoControl DFP_RECEIVE_DRIVE_DATA Fail, Error Code: %d\n", ::GetLastError());
					//return FALSE;
				}  
			}  
		}  
		else  
		{  
			//printf("DeviceIoControl DFP_GET_VERSION Fail, Error Code: %d\n", ::GetLastError());
			//return FALSE;
		}
	

		//第二种方法  
		//BYTE pBuffer[8192] = { 0 };  
		//STORAGE_PROPERTY_QUERY spq;  
		//memset(&spq, 0, sizeof(STORAGE_PROPERTY_QUERY));  
		//spq.PropertyId = StorageDeviceProperty;  
		//spq.QueryType = PropertyStandardQuery;  

		//if (::DeviceIoControl(hPhysical,  
		//	IOCTL_STORAGE_QUERY_PROPERTY,  
		//	&spq, sizeof(STORAGE_PROPERTY_QUERY),  
		//	pBuffer, sizeof(pBuffer),  
		//	&dwRet, NULL))  
		//{  
		//	DWORD dwStart = ((PSTORAGE_DEVICE_DESCRIPTOR)pBuffer)->SerialNumberOffset;  
		//	//printf("Serial Number = ");  
		//	while (TRUE)  
		//	{  
		//		CHAR cSN = (CHAR)pBuffer[dwStart++];  
		//		if (cSN == '\0') break;  
		//		//putchar(cSN);  
		//	}  
		//	//putchar('\n');  
		//}  
		//else  
		//{  
		//	//printf("DeviceIoControl IOCTL_STORAGE_QUERY_PROPERTY Fail, Error Code: %d\n", ::GetLastError());  
		//}  
	}
	//return TRUE;
	return strSerialID;
}

CString GetDeviceSerialId::GetCpuIdHex()
{
	//return GetDevIDHex(&GetDeviceSerialId::_GetCpuId);
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	CString strDevIdHex = _T("");
	if(GetCpuId())
	{
		for(UINT i = 0; i < m_uSystemInfoLen; i++)
		{
			if(m_szSystemInfo[i] == 20) continue;
			strDevIdHex.AppendFormat(_T("%X"), m_szSystemInfo[i]);
		}
	}

	return strDevIdHex;
}

CString GetDeviceSerialId::GetBiosSnHex()
{
	//return GetDevIDHex(&GetDeviceSerialId::_GetBiosSn);
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	CString strDevIdHex = _T("");
	if(GetBiosSn())
	{
		for(UINT i = 0; i < m_uSystemInfoLen; i++)
		{
			if(m_szSystemInfo[i] == 20) continue;
			strDevIdHex.AppendFormat(_T("%X"), m_szSystemInfo[i]);
		}
	}

	return strDevIdHex;
}


BOOL GetDeviceSerialId::GetMAC(BYTE* szData, UINT* puLength)
{
	//return GetDevID(szData, puLength, &GetDeviceSerialId::_GetMAC);
	if(szData == NULL) return FALSE;
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	if(GetMAC())
	{
		if(*puLength > m_uSystemInfoLen)
		{
			*puLength = m_uSystemInfoLen;
		}
		_memccpy(szData, m_szSystemInfo, 0, *puLength);

		return TRUE;
	}

	return FALSE;
}

BOOL GetDeviceSerialId::GetHdSn(BYTE* szData,  UINT* puLength)
{
	//return GetDevID(szData, puLength, &GetDeviceSerialId::_GetHdSn);
	if(szData == NULL) return FALSE;
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	if(GetHdSn())
	{
		if(*puLength > m_uSystemInfoLen)
		{
			*puLength = m_uSystemInfoLen;
		}
		_memccpy(szData, m_szSystemInfo, 0, *puLength);

		return TRUE;
	}

	return FALSE;
}

BOOL GetDeviceSerialId::GetCpuId(BYTE* szData, UINT* puLength)
{
	//return GetDevID(szData, puLength, &GetDeviceSerialId::_GetCpuId);
	if(szData == NULL) return FALSE;
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	if(GetCpuId())
	{
		if(*puLength > m_uSystemInfoLen)
		{
			*puLength = m_uSystemInfoLen;
		}
		_memccpy(szData, m_szSystemInfo, 0, *puLength);

		return TRUE;
	}

	return FALSE;
}

BOOL GetDeviceSerialId::GetBiosSn(BYTE* szData,  UINT* puLength)
{
	//return GetDevID(szData, puLength, &GetDeviceSerialId::_GetBiosSn);
	if(szData == NULL) return FALSE;
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	if(GetBiosSn())
	{
		if(*puLength > m_uSystemInfoLen)
		{
			*puLength = m_uSystemInfoLen;
		}
		_memccpy(szData, m_szSystemInfo, 0, *puLength);

		return TRUE;
	}

	return FALSE;
}



/*
BOOL GetDeviceSerialId::GetDevID(BYTE* szData,  UINT* puLength, PGETDEVIDFUNC pGetDevIdFunc)
{
	if(szData == NULL) return FALSE;
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	if(pGetDevIdFunc())
	{
		if(*puLength > m_uSystemInfoLen)
		{
			*puLength = m_uSystemInfoLen;
		}
		memccpy(szData, m_szSystemInfo, 0, *puLength);

		return TRUE;
	}

	return FALSE;
}

CString GetDeviceSerialId::GetDevIDHex(PGETDEVIDFUNC pGetDevIdFunc)
{
	memset(m_szSystemInfo, 0, MAX_DEVID_LENGTH);
	m_uSystemInfoLen = 0;
	CString strDevIdHex = _T("");
	if(pGetDevIdFunc())
	{
		for(int i = 0; i < m_uSystemInfoLen; i++)
			strDevIdHex.AppendFormat(_T("%x"), m_szSystemInfo[i]);
	}

	return strDevIdHex;
}
*/

// 网卡 MAC 地址，注意: MAC 地址是可以在注册表中修改的
BOOL GetDeviceSerialId::GetMAC()
{
	UINT uErrorCode = 0;
	IP_ADAPTER_INFO iai;
	ULONG uSize = 0;
	DWORD dwResult = GetAdaptersInfo( &iai, &uSize );
	if( dwResult == ERROR_BUFFER_OVERFLOW )
	{
		IP_ADAPTER_INFO* piai = ( IP_ADAPTER_INFO* )HeapAlloc( GetProcessHeap( ), 0, uSize );
		if( piai != NULL )
		{
			dwResult = GetAdaptersInfo( piai, &uSize );
			if( ERROR_SUCCESS == dwResult )
			{
				IP_ADAPTER_INFO* piai2 = piai;
				while( piai2 != NULL && ( m_uSystemInfoLen + piai2->AddressLength ) < MAX_DEVID_LENGTH )
				{
					CopyMemory( m_szSystemInfo + m_uSystemInfoLen, piai2->Address, piai2->AddressLength );
					m_uSystemInfoLen += piai2->AddressLength;
					piai2 = piai2->Next;                        
				}
			}
			else
			{
				uErrorCode = 0xF0000000U + dwResult;
			}
			VERIFY( HeapFree( GetProcessHeap( ), 0, piai ) );
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		uErrorCode = 0xE0000000U + dwResult;
	}

	return uErrorCode != 0U ? FALSE : TRUE;
}

// 硬盘序列号，注意：有的硬盘没有序列号
BOOL GetDeviceSerialId::GetHdSn()
{
	OSVERSIONINFO ovi = { 0 };
	ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &ovi );

	if( ovi.dwPlatformId != VER_PLATFORM_WIN32_NT )
	{
		// Only Windows 2000, Windows XP, Windows Server 2003...
		return FALSE;
	}
	else
	{
		if( !WinNTHDSerialNumAsPhysicalRead( m_szSystemInfo, &m_uSystemInfoLen, 1024 ) )
		{
			return WinNTHDSerialNumAsScsiRead( m_szSystemInfo, &m_uSystemInfoLen, 1024 );
		}
		else
			return TRUE;
	}
}

// CPU ID
BOOL GetDeviceSerialId::GetCpuId()
{
	BOOL bException = FALSE;
	BYTE szCpu[16]  = { 0 };
	UINT uCpuID     = 0U;

	__try 
	{
		_asm 
		{
			mov eax, 0
				cpuid
				mov dword ptr szCpu[0], ebx
				mov dword ptr szCpu[4], edx
				mov dword ptr szCpu[8], ecx
				mov eax, 1
				cpuid
				mov uCpuID, edx
		}
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		bException = TRUE;
	}

	if( !bException )
	{
		CopyMemory( m_szSystemInfo + m_uSystemInfoLen, &uCpuID, sizeof( UINT ) );
		m_uSystemInfoLen += sizeof( UINT );

		uCpuID = strlen( ( char* )szCpu );
		CopyMemory( m_szSystemInfo + m_uSystemInfoLen, szCpu, uCpuID );
		m_uSystemInfoLen += uCpuID;

		return TRUE;
	}
	else
		return FALSE;
}

// BIOS 编号，支持 AMI, AWARD, PHOENIX
BOOL GetDeviceSerialId::GetBiosSn()
{
	BOOL ret = FALSE;

	//BYTE m_szSystemInfo[MAX_DEVID_LENGTH];
	//UINT m_uSystemInfoLen;

	SIZE_T ssize; 

	LARGE_INTEGER so; 
	so.LowPart=0x000f0000;
	so.HighPart=0x00000000; 
	ssize=0xffff; 
	wchar_t strPH[30]=L"\\device\\physicalmemory"; 

	DWORD ba=0;

	UNICODE_STRING struniph; 
	struniph.Buffer=strPH; 
	struniph.Length=0x2c; 
	struniph.MaximumLength =0x2e; 

	OBJECT_ATTRIBUTES obj_ar; 
	obj_ar.Attributes =64;
	obj_ar.Length =24;
	obj_ar.ObjectName=&struniph;
	obj_ar.RootDirectory=0; 
	obj_ar.SecurityDescriptor=0; 
	obj_ar.SecurityQualityOfService =0; 

	HMODULE hinstLib = LoadLibrary(_T("ntdll.dll")); 
	ZWOS ZWopenS=(ZWOS)GetProcAddress(hinstLib,"ZwOpenSection"); 
	ZWMV ZWmapV=(ZWMV)GetProcAddress(hinstLib,"ZwMapViewOfSection"); 
	ZWUMV ZWunmapV=(ZWUMV)GetProcAddress(hinstLib,"ZwUnmapViewOfSection"); 

	//调用函数，对物理内存进行映射 
	HANDLE hSection; 
	if( 0 == ZWopenS(&hSection,4,&obj_ar) && 
		0 == ZWmapV( 
		( HANDLE )hSection,   //打开Section时得到的句柄 
		( HANDLE )0xFFFFFFFF, //将要映射进程的句柄， 
		&ba,                  //映射的基址 
		0,
		0xFFFF,               //分配的大小 
		&so,                  //物理内存的地址 
		&ssize,               //指向读取内存块大小的指针 
		1,                    //子进程的可继承性设定 
		0,                    //分配类型 
		2                     //保护类型 
		) )
		//执行后会在当前进程的空间开辟一段64k的空间，并把f000:0000到f000:ffff处的内容映射到这里 
		//映射的基址由ba返回,如果映射不再有用,应该用ZwUnmapViewOfSection断开映射 
	{
		BYTE* pBiosSerial = ( BYTE* )ba;
		UINT uBiosSerialLen = FindAwardBios( &pBiosSerial );
		if( uBiosSerialLen == 0U )
		{
			uBiosSerialLen = FindAmiBios( &pBiosSerial );
			if( uBiosSerialLen == 0U )
			{
				uBiosSerialLen = FindPhoenixBios( &pBiosSerial );
			}
		}

		if( uBiosSerialLen != 0U )
		{
			CopyMemory( m_szSystemInfo + m_uSystemInfoLen, pBiosSerial, uBiosSerialLen );
			m_uSystemInfoLen += uBiosSerialLen;

			ret = TRUE;
		}

		ZWunmapV( ( HANDLE )0xFFFFFFFF, ( void* )ba );
	}

	return ret;
}


BOOL GetDeviceSerialId::WinNTHDSerialNumAsScsiRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen )
{
	BOOL bInfoLoaded = FALSE;

	for( int iController = 0; iController < 4; ++ iController )
	{
		HANDLE hScsiDriveIOCTL = 0;
		char   szDriveName[256];

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		sprintf_s( szDriveName, 256, "\\\\.\\Scsi%d:", iController );

		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFile( (WCHAR*)szDriveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);

		// if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
		//            controller, GetLastError ());

		if( hScsiDriveIOCTL != INVALID_HANDLE_VALUE )
		{
			int iDrive = 0;
			for( iDrive = 0; iDrive < 16; ++ iDrive )
			{
				char szBuffer[sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH] = { 0 };

				SRB_IO_CONTROL* p = ( SRB_IO_CONTROL* )szBuffer;
				SENDCMDINPARAMS* pin = ( SENDCMDINPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
				DWORD dwResult;

				p->HeaderLength = sizeof( SRB_IO_CONTROL );
				p->Timeout = 10000;
				p->Length = SENDIDLENGTH;
				p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy_s( ( char* )p->Signature,8,  "SCSIDISK", 8 );

				pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin->bDriveNumber = iDrive;

				if( DeviceIoControl( hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
					szBuffer,
					sizeof( SRB_IO_CONTROL ) + sizeof( SENDCMDINPARAMS ) - 1,
					szBuffer,
					sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH,
					&dwResult, NULL ) )
				{
					SENDCMDOUTPARAMS* pOut = ( SENDCMDOUTPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
					IDSECTOR* pId = ( IDSECTOR* )( pOut->bBuffer );
					if( pId->sModelNumber[0] )
					{
						if( * puSerialLen + 20U <= uMaxSerialLen )
						{
							// 序列号
							CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 10, 20 );

							// Cut off the trailing blanks
							UINT i;
							for( i = 20; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )
							{}
							* puSerialLen += i;

							// 型号
							CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 27, 40 );
							// Cut off the trailing blanks
							for( i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )
							{}
							* puSerialLen += i;

							bInfoLoaded = TRUE;
						}
						else
						{
							::CloseHandle( hScsiDriveIOCTL );
							return bInfoLoaded;
						}
					}
				}
			}
			::CloseHandle( hScsiDriveIOCTL );
		}
	}
	return bInfoLoaded;
}


BOOL GetDeviceSerialId::DoIdentify( HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
				PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
				PDWORD lpcbBytesReturned )
{
	// Set up data structures for IDENTIFY command.
	pSCIP->cBufferSize                  = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg     = 0;
	pSCIP->irDriveRegs.bSectorCountReg  = 1;
	pSCIP->irDriveRegs.bSectorNumberReg = 1;
	pSCIP->irDriveRegs.bCylLowReg       = 0;
	pSCIP->irDriveRegs.bCylHighReg      = 0;

	// calc the drive number.
	pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ( ( bDriveNum & 1 ) << 4 );

	// The command can either be IDE identify or ATAPI identify.
	pSCIP->irDriveRegs.bCommandReg = bIDCmd;
	pSCIP->bDriveNumber = bDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	return DeviceIoControl( hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		( LPVOID ) pSCIP,
		sizeof( SENDCMDINPARAMS ) - 1,
		( LPVOID ) pSCOP,
		sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL );
}

BOOL GetDeviceSerialId::WinNTHDSerialNumAsPhysicalRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen )
{
#define  DFP_GET_VERSION          0x00074080
	BOOL bInfoLoaded = FALSE;

	for( UINT uDrive = 0; uDrive < 4; ++ uDrive )
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		char szDriveName [256];
		sprintf_s( szDriveName, 256, "\\\\.\\PhysicalDrive%d", uDrive );

		//  Windows NT, Windows 2000, must have admin rights
		hPhysicalDriveIOCTL = CreateFile( (WCHAR*)szDriveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);

		if( hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE )
		{
			GETVERSIONOUTPARAMS VersionParams = { 0 };
			DWORD               cbBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL
			if( DeviceIoControl( hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL, 
				0,
				&VersionParams,
				sizeof( GETVERSIONOUTPARAMS ),
				&cbBytesReturned, NULL ) )
			{
				// If there is a IDE device at number "i" issue commands
				// to the device
				if( VersionParams.bIDEDeviceMap != 0 )
				{
					BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
					SENDCMDINPARAMS  scip = { 0 };

					// Now, get the ID sector for all IDE devices in the system.
					// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
					// otherwise use the IDE_ATA_IDENTIFY command
					bIDCmd = ( VersionParams.bIDEDeviceMap >> uDrive & 0x10 ) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
					BYTE IdOutCmd[sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1] = { 0 };

					if( DoIdentify( hPhysicalDriveIOCTL, 
						&scip, 
						( PSENDCMDOUTPARAMS )&IdOutCmd, 
						( BYTE )bIDCmd,
						( BYTE )uDrive,
						&cbBytesReturned ) )
					{
						if( * puSerialLen + 20U <= uMaxSerialLen )
						{
							CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 10, 20 );  // 序列号

							// Cut off the trailing blanks
							UINT i;
							for(i = 20; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )  {}
							* puSerialLen += i;

							CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 27, 40 ); // 型号

							// Cut off the trailing blanks
							for( i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )  {}
							* puSerialLen += i;

							bInfoLoaded = TRUE;
						}
						else
						{
							::CloseHandle( hPhysicalDriveIOCTL );
							return bInfoLoaded;
						}
					}
				}
			}
			CloseHandle( hPhysicalDriveIOCTL );
		}
	}
	return bInfoLoaded;
}

UINT GetDeviceSerialId::FindAwardBios( BYTE** ppBiosAddr )
{
	BYTE* pBiosAddr = * ppBiosAddr + 0xEC71;

	BYTE szBiosData[128];
	CopyMemory( szBiosData, pBiosAddr, 127 );
	szBiosData[127] = 0;

	int iLen = lstrlen( (WCHAR*)szBiosData );
	if( iLen > 0 && iLen < 128 )
	{
		//AWard:         07/08/2002-i845G-ITE8712-JF69VD0CC-00 
		//Phoenix-Award: 03/12/2002-sis645-p4s333
		if( szBiosData[2] == '/' && szBiosData[5] == '/' )
		{
			BYTE* p = szBiosData;
			while( * p )
			{
				if( * p < ' ' || * p >= 127 )
				{
					break;
				}
				++ p;
			}
			if( * p == 0 )
			{
				* ppBiosAddr = pBiosAddr;
				return ( UINT )iLen;
			}
		}
	}
	return 0;
}

UINT GetDeviceSerialId::FindAmiBios( BYTE** ppBiosAddr )
{
	BYTE* pBiosAddr = * ppBiosAddr + 0xF478;

	BYTE szBiosData[128];
	CopyMemory( szBiosData, pBiosAddr, 127 );
	szBiosData[127] = 0;

	int iLen = lstrlen( (WCHAR*)szBiosData );
	if( iLen > 0 && iLen < 128 )
	{
		// Example: "AMI: 51-2300-000000-00101111-030199-"
		if( szBiosData[2] == '-' && szBiosData[7] == '-' )
		{
			BYTE* p = szBiosData;
			while( * p )
			{
				if( * p < ' ' || * p >= 127 )
				{
					break;
				}
				++ p;
			}
			if( * p == 0 )
			{
				* ppBiosAddr = pBiosAddr;
				return ( UINT )iLen;
			}
		}
	}
	return 0;
}

UINT GetDeviceSerialId::FindPhoenixBios( BYTE** ppBiosAddr )
{
	UINT uOffset[3] = { 0x6577, 0x7196, 0x7550 };
	for( UINT i = 0; i < 3; ++ i )
	{
		BYTE* pBiosAddr = * ppBiosAddr + uOffset[i];

		BYTE szBiosData[128];
		CopyMemory( szBiosData, pBiosAddr, 127 );
		szBiosData[127] = 0;

		int iLen = lstrlen( (WCHAR*)szBiosData );
		if( iLen > 0 && iLen < 128 )
		{
			// Example: Phoenix "NITELT0.86B.0044.P11.9910111055"
			if( szBiosData[7] == '.' && szBiosData[11] == '.' )
			{
				BYTE* p = szBiosData;
				while( * p )
				{
					if( * p < ' ' || * p >= 127 )
					{
						break;
					}
					++ p;
				}
				if( * p == 0 )
				{
					* ppBiosAddr = pBiosAddr;
					return ( UINT )iLen;
				}
			}
		}
	}
	return 0;
}


