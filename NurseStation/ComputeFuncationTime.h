#pragma once

class CComputeFuncationTime
{
public:
	CComputeFuncationTime(void);
	~CComputeFuncationTime(void);
	void SetStartTime(clock_t start){m_start=start;}
	void SetFinshTime(clock_t finsh){m_finsh=finsh;}
	double GetDuration()
	{
		m_duration=(double)(m_finsh-m_start)/CLOCKS_PER_SEC;
		return m_duration;
	}
private:
	clock_t m_start;
	clock_t m_finsh;
	double m_duration;//持续时间
};
