#pragma once

#include "Controller.h"


////////////////////////////////////////////////////////////////////////////////
//////////////////////////	其他数据定义	////////////////////////////////////
/*在线医生的状态*/
//typedef enum _tagDoctorStatusEnum
//{
//	dsWorking		= 0,		//工作状态
//	dsPause			= 1,		//暂停状态
//} DoctorStatusEnum;

//医生退出码
#define EXITCODE_OFFLINE			-1			//因网络断线而退出
#define EXITCODE_UPDATE_NONE		0			//退出时保留就诊和准备患者
#define EXITCODE_UPDATE_CALL		1			//退出时不保留就诊患者
#define EXITCODE_UPDATE_WAIT		2			//退出时不保留准备患者
#define EXITCODE_UPDATE_ALL			3			//退出时不保留就诊和准备患者

/////////////////////////	End 其他数据定义	//////////////////////////////////


class CUser : public CObject
{
	DECLARE_DYNAMIC(CUser)
public:
	CUser()
		: strId(_T(""))
		, strName(_T(""))
		, bOnline(TRUE)
		, bOffline(FALSE)
	{}
	virtual ~CUser(){}
	CUser(const CUser& user)
	{
		this->strId = user.strId;
		this->strName = user.strName;
		this->bOnline = user.bOnline;
		this->bOffline = user.bOffline;
		this->sSockAddrIn = user.sSockAddrIn;
	}
	CUser& operator=(const CUser& user)
	{
		if(this != &user)
		{
			this->strId = user.strId;
			this->strName = user.strName;
			this->bOnline = user.bOnline;
			this->bOffline = user.bOffline;
			this->sSockAddrIn = user.sSockAddrIn;
		}
		return *this;
	}

	void SetId(CString strId)
	{
		this->strId = strId;
	}
	CString GetId() const
	{
		return strId;
	}
	void SetName(CString strName)
	{
		this->strName = strName;
	}
	CString GetName() const
	{
		return strName;
	}
	void SetOnline(BOOL bOnline)
	{
		this->bOnline = bOnline;
	}
	BOOL GetOnline() const
	{
		return bOnline;
	}
	void SetOffline(BOOL bOffline)
	{
		this->bOffline = bOffline;
	}
	BOOL GetOffline() const
	{
		return bOffline;
	}
	void SetSockAddrIn(LPSOCKADDR_IN psSockAddrIn)
	{
		this->sSockAddrIn = *psSockAddrIn;
	}
	const SOCKADDR_IN& GetSockAddrIn() const
	{
		return sSockAddrIn;
	}

private:
	CString strId;
	CString strName;
	BOOL bOnline;//心跳包判断是否在线依据
	BOOL bOffline;//是否已离线
	SOCKADDR_IN sSockAddrIn;
	//UserTypeEnum eUserType;
	//CStringArray arrstrOfficeList;
};//CUser

//IMPLEMENT_DYNAMIC(CUser, CObject)

class Office
{
public:
	void SetOfficeId(CString strOfficeId)
	{
		this->strOfficeId = strOfficeId;
	}
	CString GetOfficeId() const
	{
		return strOfficeId;
	}
	void SetOfficeName(CString strOfficeName)
	{
		this->strOfficeName = strOfficeName;
	}
	CString GetOfficeName() const
	{
		return strOfficeName;
	}
private:
	CString strOfficeId;
	CString strOfficeName;
};

class CNurse : public CUser
{
	DECLARE_DYNAMIC(CNurse)
public:
	CNurse(){}
	//CNurse(const CNurse& nurse)
	//	:CUser(nurse)
	//{
	//	this->arrstrOfficeList.Copy(nurse.arrstrOfficeList);
	//}
	//void operator=(const CNurse& nurse)
	//	:CUser(nurse)
	//{
	//	this->arrstrOfficeList.Copy(nurse.arrstrOfficeList);
	//}

	//void SetOfficeList(CStringArray& arrstr)
	//{
	//	this->arrstrOfficeList.Copy(arrstr);
	//}
	//CStringArray& GetOfficeList()
	//{
	//	return arrstrOfficeList;
	//}
	//void AddOffice(CString strOffice)
	//{
	//	arrstrOfficeList.Add(strOffice);
	//}
	virtual ~CNurse(){}
	CNurse(const CNurse& nurse)
		:CUser(nurse)
	{
		this->arrOfficeList.Copy(nurse.arrOfficeList);
	}
	CNurse& operator=(const CNurse& nurse)
		//:CUser(nurse) //Error
	{
		if(this != &nurse)
		{
			CUser::operator=(nurse);
			this->arrOfficeList.Copy(nurse.arrOfficeList);
		}
		return *this;
	}

	typedef CArray<Office> OfficeArray;

	void SetOfficeList(const CArray<Office>& arrOfficeList)
	{
		this->arrOfficeList.Copy(arrOfficeList);
		//this->arrOfficeList = arrOfficeList;
	}
	//CArray<Office>& GetOfficeList()
	//{
	//	return arrOfficeList;
	//}
	const OfficeArray& GetOfficeList() const
	{
		return arrOfficeList;
	}
	void AddOffice(Office& office)
	{
		arrOfficeList.Add(office);
	}
	void AddOffice(CString strOfficeId, CString strOfficeName)
	{
		Office office;
		office.SetOfficeId(strOfficeId);
		office.SetOfficeName(strOfficeName);
		arrOfficeList.Add(office);
	}
	CString GetOfficeNames() const
	{
		CString str = _T("");
		if(!arrOfficeList.IsEmpty())
		{
			for(int i = 0; i < arrOfficeList.GetCount(); i++)
			{
				if(i != 0)
				{
					str += _T("|");
				}
				str += arrOfficeList[i].GetOfficeName();
			}
		}
		return str;
	}

private:
	//CStringArray arrstrOfficeList;
	OfficeArray arrOfficeList;
};



//IMPLEMENT_DYNAMIC(CNurse, CUser)

class CDoctor : public CUser
{
	DECLARE_DYNAMIC(CDoctor)
public:
	//CDoctor() : m_pDoctorCallInfo(NULL){}
	CDoctor(){}
	virtual ~CDoctor(){}
	CDoctor(const CDoctor& doctor)
		:CUser(doctor)
	{
		this->office = doctor.office;
	}
	CDoctor& operator=(const CDoctor& doctor)
		//:CUser(nurse) //Error
	{
		if(this != &doctor)
		{
			CUser::operator=(doctor);
			this->office = doctor.office;
		}
		return *this;
	}

	void SetOffice(const Office& office)
	{
		this->office = office;
	}
	const Office& GetOffice() const
	{
		return office;
	}
	void SetOfficeId(CString strOfficeId)
	{
		office.SetOfficeId(strOfficeId);
	}
	CString GetOfficeId() const
	{
		return office.GetOfficeId();
	}
	void SetOfficeName(CString strOfficeName)
	{
		office.SetOfficeName(strOfficeName);
	}
	CString GetOfficeName() const
	{
		return office.GetOfficeName();
	}
	void SetOffice(CString strOfficeId, CString strOfficeName)
	{
		office.SetOfficeId(strOfficeId);
		office.SetOfficeName(strOfficeName);
	}
	//DoctorCallInfo* GetPDoctorCallInfo() 
	//{
	//	return m_pDoctorCallInfo;
	//}

private:
	//CString	strOfficeId;
	//CString	strOfficeName;
	Office office;

	//DoctorCallInfo* m_pDoctorCallInfo;
};

//IMPLEMENT_DYNAMIC(CDoctor, CUser)


// CController 命令目标

class CServiceController : public CController
{
public:
	CServiceController(void);
	virtual ~CServiceController(void);

protected:

	virtual BOOL OnStartControl();
	virtual BOOL OnEndControl();

	///////		CMD dispatch function	/////////////////////////////
	//virtual void OnRecvReqLogin(LPREQLOGIN pReqLogin, LPSOCKADDR_IN psockddr) = 0;
	virtual void OnRecvLogin(LPBROADCAST pLogin, LPSOCKADDR_IN psockddr);
	virtual void OnRecvQuit(LPQUIT pQuit, LPSOCKADDR_IN psockddr);
	virtual void OnRecvHeartBeat(LPBROADCAST pHeartbeat, LPSOCKADDR_IN psockddr);
	//virtual void OnRecvDoctorCmd(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr) = 0;
	//virtual void OnRecvCmdResult(LPCMDRESULT pCmdResult, LPSOCKADDR_IN psockddr) = 0;
	//virtual void OnRecvChatMsg(LPCHATMSG pChatMsg, LPSOCKADDR_IN psockddr) = 0;
	///////		End	CMD dispatch function	/////////////////////////

	virtual void OnNurseOutLine(CNurse* pNurse) = 0;
	virtual void OnDoctorOutLine(CDoctor* pDoctor, int ExitCode) = 0;
	virtual void OnNurseLogin(CNurse* pNurse, BOOL bLogin) = 0;
	virtual void OnDoctorLogin(CDoctor* pDoctor, BOOL bLogin) = 0;

	virtual void AddNurse(LPBROADCAST pLogin, LPSOCKADDR_IN psockddr, BOOL bLogin);
	virtual void AddDoctor(LPBROADCAST pLogin, LPSOCKADDR_IN psockddr, BOOL bLogin);

	CDoctor* GetDoctor(CString strDoctorId);
	CNurse* GetNurse(CString strNurseId);
	CUser* GetUser(CString strUserId, UserTypeEnum eUserType);

private:
	virtual void OnRecvReqLogin(LPREQLOGIN pReqLogin, LPSOCKADDR_IN psockddr){};
	//virtual void OnUpdateUserList(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	virtual void UpdateUserList();

	static UINT UpdateUserThreadProc(LPVOID pParam);
	CWinThread* m_pUpdateUserThread;

protected:
	typedef CMap<CString, LPCTSTR, CNurse*, CNurse*&> NurseMap;
	typedef CMap<CString, LPCTSTR, CDoctor*, CDoctor*&> DoctorMap;
	//UserMap m_mapUserList;
	NurseMap m_mapNurseList;
	DoctorMap m_mapDoctorList;

private:
	//MyTimer<CServiceController> m_mytimerUpdateUserList;
	virtual void OnRecvData();
};
