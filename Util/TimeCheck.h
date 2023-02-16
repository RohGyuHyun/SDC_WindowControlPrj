#ifndef	_CTIMER_CHECK
#define	_CTIMER_CHECK

class CTimerCheck
{
public:
	CTimerCheck(long checktime = 100); // 단위는 msec...
	~CTimerCheck();

public:
	long TimePassed();	// 현재 까지의 시간을 리턴....
	long GetCheckTime();
	void SetCheckTime(long ctime);
	BOOL IsTimeOver();
	BOOL IsMoreThan(long ctime);	// 단위는 msec..
	BOOL IsLessThan(long ctime);	// 단위는 msec..
	void StartTimer();


	void SetStartTime();
	long SetEndTime();
private:
	long m_CheckTime;
	long StartTime;
	long EndTime;
	//long m_ElapsedTime 
};

#endif