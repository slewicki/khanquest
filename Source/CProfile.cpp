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
	m_nTimesCalled = 1;
	m_bChanged = false;
	Profile.clear();
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

void CProfile::Start(LPSTR token)
{
	//Intialize members
	//m_Token = token;	//Function name
	//m_usLine  = nLine;    //Current line
	//m_szFile  = strFile;	//Current source
	//m_nTimesCalled = 0;
	//LARGE_INTEGER m_liStart;
	//LARGE_INTEGER m_liEnd;
		if(m_szToken != NULL)
		{
			Stop();
		}

		m_szToken = token;
		QueryPerformanceFrequency(&m_liFreq);
		QueryPerformanceCounter(&m_liStart);
}

void CProfile::Stop()
{
	QueryPerformanceCounter(&m_liEnd);
	m_dTotal = (double)((m_liEnd.QuadPart - m_liStart.QuadPart) / m_liFreq.QuadPart);

	m_dAvg = 0;


	if(Profile.empty())
	{
		LOGITEM TempProfile;
		TempProfile.Time.push_back(m_dTotal);
		TempProfile.Average = m_dAvg;
		TempProfile.Function = m_szToken;
		TempProfile.NumCalled = m_nTimesCalled;

		Profile.push_back(TempProfile);
	}
	else
	{
		for(int i = 0; i < Profile.size(); i++)
		{
			if(m_szToken == Profile[i].Function)
			{
				Profile[i].Time.push_back(m_dTotal);
				Profile[i].NumCalled++;
				m_bChanged = true;
			}
		}
		if(!m_bChanged)
		{
			LOGITEM TempProfile;
			TempProfile.Time.push_back(m_dTotal);
			TempProfile.Average = m_dAvg;
			TempProfile.Function = m_szToken;
			TempProfile.NumCalled = m_nTimesCalled;

			Profile.push_back(TempProfile);
		}
	}

	//if(m_dTotal < 1)
	//	m_dTotal = 1;

	//for(int i = 0; i < m_nTimesCalled; i++)
		//m_dTotal += m_dTotal;

	//m_fAvg = (float)m_dTotal / (m_nTimesCalled);

	/*LOGITEM logItem = {
		m_usLine,
		m_dTotal,
		m_szFile,			  
		m_szToken,
		m_fAvg};*/

		/*fstream fout("Resource\\KQ_Profile.kqp", std::ios_base::out | std::ios_base::app);
		fout.write(logItem.Function, strlen(logItem.Function) + 1) << '\n';
		fout << logItem.Time << '\n';
		fout << logItem.Average << '\n' << '\n';
		fout.close();*/

	m_bChanged = false;
}

void CProfile::Process()
{
	fstream fout("Resource\\KQ_Profile.kqp", std::ios_base::out | std::ios_base::app);

	for(int i = 0; i < Profile.size(); i++)
	{
		fout.write("Function: ", strlen("Function "));
		fout.write(Profile[i].Function, strlen(Profile[i].Function) + 1) << '\n';

		for(int j = 0; j < Profile[i].Time.size(); j++)
		{
			//fout << Profile[i].Time[j] << ", ";

			Profile[i].Average += Profile[i].Time[j];
			//fout << Profile[i].Average << '\n';
		}

		fout.write("Total Time: \t", strlen("Total Time: "));
		fout << Profile[i].Average << '\n';

		fout.write("Avg. Time: \t", strlen("Avg. Time: "));
		Profile[i].Average /= Profile[i].Time.size();
		fout << Profile[i].Average << '\n';

		fout.write("Times Called: \t", strlen("Times Called: "));
		fout << Profile[i].NumCalled << '\n' << '\n';
	}

	fout.close();
}