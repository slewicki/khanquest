#ifndef _CPROFILE_H_
#define _CPROFILE_H_

//////////////////////////////////////////////////////////
//	File:	"CProfile.h"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: Code Profiler
//////////////////////////////////////////////////////////
#include <windows.h>
#include <fstream>
#include <vector>

using std::vector;

using std::fstream;

//log file item struct
struct LOGITEM {
	LARGE_INTEGER StartTime;	//Function's Start Time
	LARGE_INTEGER EndTime;		//Function's End Time
	vector<double> Time;		//Vector storing all the times
	LPSTR  Function;			//Function that was profiled
	double  Average;			//Average of all the times together
	int NumCalled;				//Number of times the function has been called
	double MinTime;				//Minimum time a function ran
	double MaxTime;				//Maximum time a function ran
};

class CProfile  
{
private:
	LPSTR m_szToken;			//Function name
	LARGE_INTEGER m_liFreq;		//Frequency
	LARGE_INTEGER m_liStart;	//Timer Start
	LARGE_INTEGER m_liEnd;		//Timer End
	double m_dTotal;			//Total time
	int m_nTimesCalled;
	vector<LOGITEM> Profile;
	bool m_bChanged;


public:
	//Constructor
	CProfile();
	
	//Destructor
	~CProfile();

	static CProfile* GetInstance();
	void Start(LPSTR token);
	void Stop(LPSTR token);
	void Process();
	void MinMaxOutput();
};

//CProfile useage requires only single declartion at beginning of function
#if _DEBUG
	
#define PROFILE(token) CProfile::GetInstance()->Start(#token)
#define STOP(token) CProfile::GetInstance()->Stop(#token)
#else
	#define PROFILE(token) ((void)0)		//Release
	#define STOP(token) ((void)0)
#endif



#endif