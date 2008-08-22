//////////////////////////////////////////////////////////
//	File:	"CProfile.cpp"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: Code Profiler
//////////////////////////////////////////////////////////
#include "CProfile.h"

CProfile::CProfile()
{
	m_nTimesCalled = 1;
	m_bChanged = false;
	Profile.clear();
}

CProfile::~CProfile()
{
}

CProfile* CProfile::GetInstance()
{
	static CProfile Instance;
	return & Instance;
}

void CProfile::Start(LPSTR token)
{

	m_szToken = token;
	QueryPerformanceFrequency(&m_liFreq);
	QueryPerformanceCounter(&m_liStart);

	if(Profile.empty())
	{
		LOGITEM TempProfile;
		TempProfile.Function = m_szToken;
		TempProfile.StartTime = m_liStart;
		TempProfile.NumCalled = m_nTimesCalled;
		TempProfile.Average = 0;
		TempProfile.MinTime = 100;
		TempProfile.MaxTime = 0;

		Profile.push_back(TempProfile);
	}
	else
	{
		for(unsigned int i = 0; i < Profile.size(); i++)
		{
			if(m_szToken == Profile[i].Function)
			{
				Profile[i].StartTime = m_liStart;
				m_bChanged = true;
			}
		}
		if(!m_bChanged)
		{
			LOGITEM TempProfile;
			TempProfile.Function = m_szToken;
			TempProfile.StartTime = m_liStart;
			TempProfile.NumCalled = m_nTimesCalled;
			TempProfile.Average = 0;
			TempProfile.MinTime = 100;
			TempProfile.MaxTime = 0;

			Profile.push_back(TempProfile);
		}
		m_bChanged = false;
	}
}

void CProfile::Stop(LPSTR token)
{
	m_szToken = token;
	QueryPerformanceCounter(&m_liEnd);

	for(unsigned int i = 0; i < Profile.size(); i++)
	{
		if(m_szToken == Profile[i].Function)
		{
			Profile[i].EndTime = m_liEnd;
			m_dTotal = (double)((double)(Profile[i].EndTime.QuadPart - Profile[i].StartTime.QuadPart) / (double)m_liFreq.QuadPart);
			Profile[i].Time.push_back(m_dTotal);
			Profile[i].NumCalled++;

			if(Profile[i].MinTime > m_dTotal)
				Profile[i].MinTime = m_dTotal;

			if(Profile[i].MaxTime < m_dTotal)
				Profile[i].MaxTime = m_dTotal;
		}
	}
}

void CProfile::Process()
{
	fstream fout("Resource\\KQ_Profile.kqp", std::ios_base::out | std::ios_base::app);

	for(unsigned int i = 0; i < Profile.size(); i++)
	{
		fout.write("Function: ", (int)strlen("Function "));
		fout.write(Profile[i].Function, (int)strlen(Profile[i].Function) + 1) << '\n';

		for(unsigned int j = 0; j < Profile[i].Time.size(); j++)
		{
			Profile[i].Average += Profile[i].Time[j];
		}

		fout.write("Total Time: \t", (int)strlen("Total Time: "));
		fout << Profile[i].Average << '\n';

		fout.write("Avg. Time: \t", (int)strlen("Avg. Time: "));
		Profile[i].Average /= Profile[i].Time.size();
		fout << Profile[i].Average << '\n';

		fout.write("Times Called: \t", (int)strlen("Times Called: "));
		fout << Profile[i].NumCalled << '\n' << '\n';
	}

	fout.close();
}

void CProfile::MinMaxOutput()
{
	fstream fout("Resource\\KQ_TimeCompare.kqp", std::ios_base::out | std::ios_base::app);

	for(unsigned int i = 0; i < Profile.size(); i++)
	{
		fout.write("Function: ", (int)strlen("Function "));
		fout.write(Profile[i].Function, (int)strlen(Profile[i].Function) + 1) << '\n';

		fout.write("Min Time: ", (int)strlen("Min Time: "));
		fout << Profile[i].MinTime << '\n';

		fout.write("Max Time: ", (int)strlen("Max Time: "));
		fout << Profile[i].MaxTime << '\n' << '\n';
	}

	fout.close();
}