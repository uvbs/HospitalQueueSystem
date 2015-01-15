#pragma once

class SLZData
{
public:
	SLZData(void);
	~SLZData(void);
	//////	定义 Get/Set方法 //////
	//获取业务流水号
	CString GetSerialId() const
	{
		return m_strSerialId;
	}
	//设置业务流水号
	void SetSerialId(const CString& strSerialId)
	{
		m_strSerialId = strSerialId;
	}
	// 医生
	CString GetDoctorName(){return m_strDoctorName;}
	void SetDoctorName(const CString& doctorName){m_strDoctorName=doctorName;}
	//科室名称
	CString GetOfficeName(){return m_strOfficeName;}
	void SetOfficeName(const CString& officeName){m_strOfficeName = officeName;}
	//诊室名称
	CString GetConsultName(){return m_strConsultName;}
	void SetConsultName(const CString& consultName){m_strConsultName = consultName;}
	//排队号码
	CString GetQueNum(){return m_strQueNum;}
	void SetQueNum(const CString& queNum){m_strQueNum=queNum;}
	//病人姓名
	CString GetPatientName(){return m_strPatientName;}
	void SetPatientName(const CString& patientName){m_strPatientName = patientName;}
	//星级
	UINT GetCusLevel(){return m_iCusLevel;}
	void SetCusLevel(UINT level){m_iCusLevel = level;}
	//Ctime
	CTime GetTakingNumTime(){return m_timeTakingNum;}
	void SetTakingNumTime(const CTime& time){m_timeTakingNum = time;}
private:
	CString m_strSerialId;	//业务流水号
	CString m_strDoctorName;//医生姓名
	CString m_strOfficeName;//科室名称
	CString m_strConsultName;//诊室名称
	CString m_strQueNum;//排队号码
	CString m_strPatientName;//病人姓名
	//CString m_CustGender;	//客户性别
	UINT m_iCusLevel;		//客户等级
	CTime m_timeTakingNum;	//取号时间
};
