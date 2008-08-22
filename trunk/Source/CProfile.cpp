//////////////////////////////////////////////////////////
//	File:	"CProfile.cpp"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: Code Profiler
//////////////////////////////////////////////////////////
#include "CProfile.h"

//CProfile::CProfile(LPSTR token, LPSTR strFile, USHORT nLine)
CProfile::CProfile()
{
	////Intialize members
	//m_Token = token;	//Function name
	//m_Line  = nLine;    //Current line
	//m_File  = strFile;	//Current source
	//static USHORT m_CurrLine = -1;
	////LARGE_INTEGER m_liStart;
	////LARGE_INTEGER m_liEnd;

	//if(m_CurrLine != m_Line)
	//{
	//	m_CurrLine = m_Line;
	//}

	//QueryPerformanceFrequency(&m_liFreq);
	//QueryPerformanceCounter(&m_liStart);
	m_nTimesCalled = 0;
}

CProfile::~CProfile()
{
	/*QueryPerformanceCounter(&m_liEnd);
	m_Total = (double)((m_liEnd.QuadPart - m_liStart.QuadPart) / m_liFreq.QuadPart);

	LOGITEM logItem = {
		m_Line,
		m_Total,
		m_File,			  
		m_Token};

		fstream fout("Resource\\KQ_Profile.kqp", std::ios_base::out | std::ios_base::app);
		fout.write(logItem.Function, strlen(logItem.Function) + 1) << '\n';
		fout << logItem.Line << '\n';
		fout << logItem.Time << '\n';
		fout.write(logItem.Source, strlen(logItem.Source) + 1) << '\n' << '\n';
		fout.close();*/
}

CProfile* CProfile::GetInstance()
{
	static CProfile Instance;
	return & Instance;
}

void CProfile::Start(LPSTR token, LPSTR strFile, USHORT nLine)
{
	//Intialize members
	//m_Token = token;	//Function name
	m_usLine  = nLine;    //Current line
	m_szFile  = strFile;	//Current source
	//m_nTimesCalled = 0;
	//LARGE_INTEGER m_liStart;
	//LARGE_INTEGER m_liEnd;

	if(m_szToken != token)
	{
		if(m_szToken != NULL)
		{
			Stop();
			m_nTimesCalled = 0;
		}

		m_szToken = token;
		QueryPerformanceFrequency(&m_liFreq);
		QueryPerformanceCounter(&m_liStart);
		m_nTimesCalled++;
	}
}

void CProfile::Stop()
{
	QueryPerformanceCounter(&m_liEnd);
	m_dTotal = (double)((m_liEnd.QuadPart - m_liStart.QuadPart) / m_liFreq.QuadPart);

	for(int i = 0; i < m_nTimesCalled; i++)
		m_dTotal += m_dTotal;

	m_fAvg = (float)m_dTotal / m_nTimesCalled;

	LOGITEM logItem = {
		m_usLine,
		m_dTotal,
		m_szFile,			  
		m_szToken,
		m_fAvg};

		fstream fout("Resource\\KQ_Profile.kqp", std::ios_base::out | std::ios_base::app);
		fout.write(logItem.Function, strlen(logItem.Function) + 1) << '\n';
		fout << logItem.Time << '\n';
		fout << logItem.Average << '\n';
		fout.close();
}