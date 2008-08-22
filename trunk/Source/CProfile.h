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
	int m_nTimesCalled;			//Initial number of times called
	vector<LOGITEM> Profile;	//Vector of profiles
	bool m_bChanged;			//Check to see if profile is in the struct


public:
	/////////////////////////////////
	//	Function:	"CProfile"
	//
	//	Purpose: Default Constructor
	/////////////////////////////////
	CProfile();
	
	/////////////////////////////////
	//	Function:	"~CProfile"
	//
	//	Purpose: Destructor
	/////////////////////////////////
	~CProfile();

	/////////////////////////////////
	//	Function:	"GetInstance"
	//
	//	Purpose: Gets the instance to the class
	/////////////////////////////////
	static CProfile* GetInstance();

	/////////////////////////////////
	//	Function:	"Start"
	//
	//	Purpose: Starts the timer
	/////////////////////////////////
	void Start(LPSTR token);

	/////////////////////////////////
	//	Function:	"Stop"
	//
	//	Purpose: Stops the timer
	/////////////////////////////////
	void Stop(LPSTR token);

	/////////////////////////////////
	//	Function:	"Process"
	//
	//	Purpose: Calculates all variables
	/////////////////////////////////
	void Process();

	/////////////////////////////////
	//	Function:	"MinMaxOutput"
	//
	//	Purpose: Displays the min and max times of each function
	/////////////////////////////////
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