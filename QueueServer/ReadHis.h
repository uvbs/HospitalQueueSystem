#pragma once

#include "../NurseStation/ado2.h"
#include "../NurseStation/MyTimer.h"

// CReadHis 命令目标

class CReadHis : public CObject
{
public:
	CReadHis();
	virtual ~CReadHis();
	BOOL Start();
	void Close();

private:
	BOOL StartPatientThread();
	void EndPatientThread();
	static UINT ReadPatientThreadProc(LPVOID pParam);//读HIS排队病人数据线程函数
	void ReadOtherDataTimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);//读HIS部门等基本数据定时器函数
	void AutoDeletePatientTimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);//自动删除本系统病人数据定时器函数
	static UINT ReadOtherDataThreadProc(LPVOID pParam);//读HIS部门等基本数据线程函数
	static UINT AutoDeleteThreadProc(LPVOID pParam);//自动删除本系统病人数据线程函数

	BOOL OpenHisDb(CADODatabase& hisDb);
	BOOL OpenMyDb(CADODatabase& myDb);

	BOOL ReadPatient();
	BOOL ReadDepart();
	BOOL ReadItem();
	BOOL ReadClinic();
	BOOL ReadDoctor();
	BOOL AutoDeletePatient();
	//int GetHisDays(int nDef);
	BOOL StringExistInStringArray(CString str, CStringArray& arr);

private:

	CString m_strMyDbConnStr;//本系统数据库连接字符串
	//CString m_strHisDbConnStr;//HIS数据库连接字符串
	int m_nHisDays;//HIS提供的排队数据在该整数的天数之内
	CWinThread* m_pReadPatientThread;//读HIS排队病人数据线程
	BOOL m_bReadPatientThread;
	MyTimer<CReadHis> m_timerReadOtherData;//读HIS部门等基本数据定时器，触发时创建一个 读HIS部门等基本数据线程
	MyTimer<CReadHis> m_timerAutoDeletePatient;//自动删除本系统病人数据定时器，触发时创建一个 自动删除本系统病人数据线程
	CWinThread* m_pReadOtherDataThread;//读HIS部门等基本数据线程
	CWinThread* m_pAutoDeleteThread;//自动删除本系统病人数据线程

	BOOL m_bHisDbErr;
	BOOL m_bMyDbErr;

	CString m_strHisDbConn;

public:
	void LoadConf();

	CMap<CString, LPCTSTR, CString, LPCTSTR> m_mapItemDepart;
	void LoadMap();

	int m_nDataReadMode;

	int m_nDbType;

	CString m_strQueryPatient;

};

