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

using std::fstream;

//log file item struct
struct LOGITEM {
	USHORT Line;		//Line number in source file
	double Time;		//NULL for function start
	LPSTR  Source;		//Source file path and name
	LPSTR  Function;	//Function that was profiled
	float  Average;
};

static char THIS_FILE[] = __FILE__;

//Profiler class
//QueryPerformanceFrequency();
//QueryPerformanceCounter();
//LARGE_INTEGER

class CProfile  
{
private:

	//Location information
	LPSTR		 m_szToken;
	LPSTR		 m_szFile;
	USHORT		 m_usLine;
	LARGE_INTEGER m_liFreq;
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liEnd;
	double m_dTotal;
	float m_fAvg;
	int m_nTimesCalled;

public:
	//Constructor
	//CProfile(LPSTR token, LPSTR strFile, USHORT nLine);
	CProfile();
	
	//Destructor
	~CProfile();

	static CProfile* GetInstance();
	void Start(LPSTR token, LPSTR strFile, USHORT nLine);
	void Stop();
};

//CProfile useage requires only single declartion at beginning of function

#if _DEBUG
	
#define PROFILE(token) CProfile::GetInstance()->Start(#token, /*__FILE__*/THIS_FILE, __LINE__)
#else
	#define PROFILE(token) ((void)0)		//Release
#endif



#endif