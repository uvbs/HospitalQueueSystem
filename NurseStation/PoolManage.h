#pragma once


class CPoolManage : public CObject
{
public:
	CPoolManage(void);
	~CPoolManage(void);
	
	class CPatientQue
	{
		public:
			CPatientQue(){}
			~CPatientQue(){}
			int GetQueueID(){return m_queueID;}
			void SetQueueID(int id){m_queueID=id;}

			CString GetPatientName(){return m_patientName;}
			void SetPatientName(CString name){m_patientName=name;}

			int GetOfficeID(){return m_officeID;}
			void SetOfficeID(int id){m_officeID=id;}

			CString GetDoctorName(){return m_doctorName;}
			void SetDoctorName(CString name){m_doctorName=name;}

			int GetStatus(){return m_status;}
			void SetStatus(int status){m_status=status;}

			int GetPirorty(){return m_priorty;}
			void SetPirorty(int priorpty){m_priorty=priorpty;}

			BOOL GetGender_flag(){return m_gender_flag;}
			void SetGender_flag(BOOL gender_flag){m_gender_flag=gender_flag;}

			CString GetRegtime(){return m_regTime;}
			void SetRegtime(CString time){m_regTime=time;}
			
			CString GetWaitTime(){return m_waitTime;}
			void SetWaitTime(CString time){m_waitTime=time;}
		protected:
			int m_queueID;//排队号码
			CString m_patientName;//患者姓名
			int m_officeID;//科室ID
			CString m_doctorName;//患者ID
			CString m_regTime;//挂号时间
			int m_status;//当前状态0-准备1-以呼叫2-过号3-挂起
			int m_priorty;//优先级0-普通1-中等2-较高3-最高
			BOOL m_gender_flag;//指定性别
			CString m_waitTime;
	};
	
	class CMyInformation
	{
	public:
		void SetPaitentName(CString name){m_paitentname=name;}
		CString GetPaitentName(){return m_paitentname;}
		void SetOfficeName(CString name){m_officename=name;}
		CString GetOfficeName(){return m_officename;}
		void SetLogid(CString logid){m_logid=logid;}
		CString GetLogid(){return m_logid;}
	private:
		CString m_paitentname;
		CString m_officename;
		CString m_logid;
	};
	class CDoctorInfo
	{
	public:
		void SetDocId(CString docid){m_docid=docid;}
		CString GetDocId(){return m_docid;}

		void SetDocName(CString docname){m_docname=docname;}
		CString GetDocName(){return m_docname;}

		void SetOnPaitname(CString paitname){m_onPaitname=paitname;}
		CString GetOnPaitname(){return m_onPaitname;}

		void SetDoctorStatus(CString status){m_docstatus=status;}
		CString GetDoctorStatus(){return m_docstatus;}
	private:
		CString m_docid;
		CString m_docname;
		CString m_onPaitname;
		CString m_docstatus;
	};
/*	
	class CSuspendTime
	{
	public:
		void SetPatLogid(CString logid){m_patLogid=logid;}
		CString GetPatLogid(){return m_patLogid;}

		void SetSuspendTime(CTime time){m_suspendTime=time;}
		CTime GetSuspendTime(){return m_suspendTime;}
	private:
		CString m_patLogid;
		CTime m_suspendTime;
	};
*/
	int CStringToChar(CString str,char c_str[]);
	int CharToCstring(CString &str,char c_str[]);
	void intToCString(int i,CString& str);
	void CStringToint(int& i,CString str);
	CString GetComputerIP();
	
	CString RequestBracket(const CString &str);
	CString DeleRequest(const CString &str);
};

