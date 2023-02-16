#ifndef	_CTIMER_CHECK
#define	_CTIMER_CHECK

class CTimerCheck
{
public:
	CTimerCheck(long checktime = 100); // ������ msec...
	~CTimerCheck();

public:
	long TimePassed();	// ���� ������ �ð��� ����....
	long GetCheckTime();
	void SetCheckTime(long ctime);
	BOOL IsTimeOver();
	BOOL IsMoreThan(long ctime);	// ������ msec..
	BOOL IsLessThan(long ctime);	// ������ msec..
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