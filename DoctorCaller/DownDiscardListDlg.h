#pragma once
#include "../NurseStation/ListCtrlCl.h"
#include "../NurseStation/ado2.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgDoctorHistory 对话框

class CDlgDoctorHistory : public CDialog
{
	DECLARE_DYNAMIC(CDlgDoctorHistory)

public:
	CDlgDoctorHistory(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDoctorHistory();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOCTOR_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlCl m_lstPatientList;
	CStatic m_stOptStatus;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonReflush();

	void OnUpdatePatient();

	void OnShowPatients(CADORecordset& rset);

	typedef struct {
		CString strQueSerialId;
		QueueStatusEnum eStatus;

	} PATIENTINFO, *LPPATIENTINFO;

	//CMap<int, int, PATIENTINFO, PATIENTINFO&> m_mapPatientList;

	CString GetStatus(QueueStatusEnum qs);
	afx_msg void OnReinline();
	afx_msg void OnNMRClickListPatient(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_bOptStatusSuccess;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonReinline();
};
