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
	LARGE_INTEGER StartTime;
	LARGE_INTEGER EndTime;
	//USHORT Line;		//Line number in source file
	vector<double> Time;		//NULL for function start
	//LPSTR  Source;		//Source file path and name
	LPSTR  Function;	//Function that was profiled
	double  Average;
	int NumCalled;
};

//Profiler class
//QueryPerformanceFrequency();
//QueryPerformanceCounter();
//LARGE_INTEGER

class CProfile  
{
private:

	//Location information
	LPSTR		 m_szToken;
	//LPSTR		 m_szFile;
	//USHORT		 m_usLine;
	LARGE_INTEGER m_liFreq;
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liEnd;
	double m_dTotal;
	double m_dAvg;
	int m_nTimesCalled;
	vector<LOGITEM> Profile;
	bool m_bChanged;


public:
	//Constructor
	//CProfile(LPSTR token, LPSTR strFile, USHORT nLine);
	CProfile();
	
	//Destructor
	~CProfile();

	static CProfile* GetInstance();
	void Start(LPSTR token);
	void Stop(LPSTR token);
	void Process();
};

//CProfile useage requires only single declartion at beginning of function

#if _DEBUG
	
#define PROFILE(token) CProfile::GetInstance()->Start(#token)
#define STOP(token) CProfile::GetInstance()->Stop(#token)
#else
	#define PROFILE(token) ((void)0)		//Release
#endif



#endif