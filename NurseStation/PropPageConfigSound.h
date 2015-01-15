#pragma once


// CPropPageConfigSound 对话框

class CPropPageConfigSound : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigSound)

public:
	CPropPageConfigSound();
	virtual ~CPropPageConfigSound();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CONFIG_SOUND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strTimes;
	CSliderCtrl m_slSpeed;
	CSliderCtrl m_slVolume;

	void Load();
	void Save();
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnNMReleasedcaptureSliderSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderVolume(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditTimes();

	CString m_strFile;
};
