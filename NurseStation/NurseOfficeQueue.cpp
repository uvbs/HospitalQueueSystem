#include "stdafx.h"
#include "NurseStation.h"
#include "NurseOfficeQueue.h"

CNurseOfficeQueue::CNurseOfficeQueue() : m_bHisFlag(FALSE)
{
	m_queNumber=0;
	m_stdId= _T("");
	m_serial_id=0;
}

CNurseOfficeQueue::~CNurseOfficeQueue()
{

}

CNurseOfficeQueue::CNurseOfficeQueue(const CNurseOfficeQueue& obj)
{
	m_queName=obj.m_queName;
	m_queCallName=obj.m_queCallName;
	m_queID=obj.m_queID;
	m_queIDCallName=obj.m_queIDCallName;
	m_queNumStart=obj.m_queNumStart;
	m_queNumEnd=obj.m_queNumEnd;
	m_AmLimitPatient=obj.m_AmLimitPatient;
	m_PmLimitPatient=obj.m_PmLimitPatient;
	m_DayLimitPatient=obj.m_DayLimitPatient;
	m_queNumber=obj.m_queNumber;
	m_stdId=obj.m_stdId;
	m_serial_id=obj.m_serial_id;
	m_bHisFlag = obj.m_bHisFlag;

	m_list_officeConsult.RemoveAll();
	POSITION pos=obj.m_list_officeConsult.GetHeadPosition();
	while(pos)
	{
		CConsult consult=obj.m_list_officeConsult.GetNext(pos);
		m_list_officeConsult.AddTail(consult);
	}
}
CNurseOfficeQueue& CNurseOfficeQueue::operator=(CNurseOfficeQueue& obj)
{
	m_queName=obj.m_queName;
	m_queCallName=obj.m_queCallName;
	m_queID=obj.m_queID;
	m_queIDCallName=obj.m_queIDCallName;
	m_queNumStart=obj.m_queNumStart;
	m_queNumEnd=obj.m_queNumEnd;
	m_AmLimitPatient=obj.m_AmLimitPatient;
	m_PmLimitPatient=obj.m_PmLimitPatient;
	m_DayLimitPatient=obj.m_DayLimitPatient;
	m_queNumber=obj.m_queNumber;
	m_stdId=obj.m_stdId;
	m_serial_id=obj.m_serial_id;
	m_bHisFlag = obj.m_bHisFlag;

	m_list_officeConsult.RemoveAll();
	POSITION pos=obj.m_list_officeConsult.GetHeadPosition();
	while(pos)
	{
		CConsult consult=obj.m_list_officeConsult.GetNext(pos);
		m_list_officeConsult.AddTail(consult);
	}
	return *this;
}
void CNurseOfficeQueue::SetAmLimitPatient(int patient)
{
	m_AmLimitPatient=patient;
}

void CNurseOfficeQueue::SetPmLimitPatient(int patient)
{
	m_PmLimitPatient=patient;
}

void CNurseOfficeQueue::SetDayLimitPatient(int patient)
{
	if(m_AmLimitPatient<=0 || m_PmLimitPatient<=0)
	{
		m_DayLimitPatient=patient;
	}
	else
	{
		m_DayLimitPatient=m_AmLimitPatient+m_PmLimitPatient;
	}
}

void CNurseOfficeQueue::SetQueNumber(int patient)
{
	m_queNumber=patient;
}