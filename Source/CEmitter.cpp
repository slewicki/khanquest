//////////////////////////////////////////////////////////////////////////
// File CEmitter.cpp
//
// Author: Rodney Kite (RK)
//
// Purpose: Contains class definition for CParticle Class, which holds
//			all of the info for a particle
//////////////////////////////////////////////////////////////////////////
#include "CEmitter.h"
#include "irrXML.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace irr;
using namespace io;


CEmitter::CEmitter(void)
{
	PROFILE("CEmitter::CEmitter()");
	m_pD3D	= NULL;
	m_pTM	= NULL;
	m_fPercentAlpha		= 0;
	m_fPercentRed		= 0;
	m_fPercentGreen		= 0;
	m_fPercentBlue		= 0;

	m_bIsRunning =  false;

	m_fPercentXScale = 0;
	m_fPercentYScale = 0;

	m_fPercentGravityX = 0;
	m_fPercentGravityY = 0;

	m_fUpdateFloat		= .25f;
	m_fUpdateQuarter	= .25f;

	m_fPartNumber = 0;

	m_nPartCount = 0;

	m_dElapsedTime = 0.0f;
	m_dwCurrentTime = GetTickCount();
	m_dwPreviousTime = GetTickCount();
	m_dwFrameTimer = GetTickCount();
	m_cCurrentColor = D3DCOLOR_ARGB(0,0,0,0);
	STOP("CEmitter::CEmitter()");
}


//CEmitter::CEmitter(const CEmitter& ref)
//{
//
//}

//CEmitter& CEmitter::operator=(const CEmitter& ref)
//{
//	if(this != &ref)
//	{
//
//	}
//	return *this;
//}

CEmitter::~CEmitter(void)
{

}

void CEmitter::Exit(void)
{

}
void CEmitter::LoadBinaryEmitter(char* emitterFileName, float locX, float locY)
{
	PROFILE("CEmitter::LoadBinaryEmitter(char*, float, float)");
	ifstream fin;

	int nStringSize = 0;;
	char szBuffer[64];
	short nContinue;
	double nDouble;

	fin.open(emitterFileName, std::ios_base::in | std::ios_base::binary);
	if(!fin.is_open())
	{
		STOP("CEmitter::LoadBinaryEmitter(char*, float, float)");
		return;	
	}

#pragma region Read In
	fin.read((char*)&nContinue, sizeof(short));

	if(nContinue != 0)
		particle.m_bContinuous = true;
	else
		particle.m_bContinuous = false;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fMaxParticles = (float)nStringSize;

	m_nMaxParts = (int)particle.m_fMaxParticles;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fMaxLife = (float)nStringSize;
	
	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fMinLife = (float)nStringSize;
	
	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fVelX = (float)nStringSize;		

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fVelY = (float)nStringSize;	

	fin.read((char*)&nStringSize,sizeof(int));
	fin.read(szBuffer,nStringSize);
	szBuffer[nStringSize] = 0;
	particle.m_szSourceBlend = szBuffer;

	fin.read((char*)&nStringSize, sizeof(int));
	fin.read(szBuffer,nStringSize);
	szBuffer[nStringSize] = 0;
	particle.m_szDestBlend = szBuffer;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_nOffsetX = nStringSize;	

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_nOffsetY = nStringSize;	

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fStartRed = (float)nStringSize;	

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fEndRed = (float)nStringSize;	

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fStartGreen = (float)nStringSize;	

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fEndGreen = (float)nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fStartBlue = (float)nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fEndBlue = (float)nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fStartAlpha = (float)nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fEndAlpha = (float)nStringSize;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fStartScaleX = (float)nDouble;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fEndScaleX = (float)nDouble;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fStartScaleY = (float)nDouble;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fEndScaleY = (float)nDouble;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fGravityX = (float)nDouble;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fGravityY = (float)nDouble;

	fin.read((char*)&nStringSize, sizeof(int));
	fin.read(szBuffer,nStringSize);
	szBuffer[nStringSize] = 0;
	particle.m_szTrigger = szBuffer;

	fin.read((char*)&nStringSize, sizeof(int));
	fin.read(szBuffer,nStringSize);
	szBuffer[nStringSize] = 0;
	particle.m_szFileName = szBuffer;
#pragma endregion Read In

	fin.close();

	particle.m_fPartLocX = locX;
	particle.m_fPartLocY = locY;

	particle.m_szFileName = "Resource/" + particle.m_szFileName;

	// load the particle texture into the TM
	m_pTM = CSGD_TextureManager::GetInstance();

	particle.m_nImageID = m_pTM->LoadTexture((char *)particle.m_szFileName.c_str());

	particle.m_cCurrentColor.a = particle.m_fStartAlpha;
	particle.m_cCurrentColor.r = particle.m_fStartRed;
	particle.m_cCurrentColor.g = particle.m_fStartGreen;
	particle.m_cCurrentColor.b = particle.m_fStartBlue;

	particle.m_cStartColor.a = particle.m_fStartAlpha;
	particle.m_cStartColor.r = particle.m_fStartRed;
	particle.m_cStartColor.g = particle.m_fStartGreen;
	particle.m_cStartColor.b = particle.m_fStartBlue;

	particle.m_cEndColor.a = particle.m_fEndAlpha;
	particle.m_cEndColor.r = particle.m_fEndRed;
	particle.m_cEndColor.g = particle.m_fEndGreen;
	particle.m_cEndColor.b = particle.m_fEndBlue;

	for (int i =0; i < particle.m_fMaxParticles; ++i)
	{
		vParticleList.push_back(particle);
	}
	if(vParticleList.size() > 0)
		m_bHasParts = true;

	STOP("CEmitter::LoadBinaryEmitter(char*, float, float)");
}

void CEmitter::LoadXMLEmitter(char* emitterFileName, float locX, float locY)
{
	PROFILE("CEmitter::LoadXMLEmitter(char*, float, float)");
	string filename = emitterFileName;

	//filename = "Resource/" + filename; 
	IrrXMLReader* xml = createIrrXMLReader(".\\Resource\\KQ_torch.xml");

	// parse the file until end reached
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT:
			{
				if (!strcmp("Emitter", xml->getNodeName()))
				{
				particle.m_bContinuous = reinterpret_cast<bool>(xml->getAttributeValue("Continuous"));
				particle.m_fMaxParticles =	xml->getAttributeValueAsFloat("MaxParticles");
				particle.m_fMaxLife = xml->getAttributeValueAsFloat("MaxLife");
				particle.m_fMinLife = xml->getAttributeValueAsFloat("MinLife");
				
				particle.m_fVelX =	xml->getAttributeValueAsFloat("VelX");
				particle.m_fVelY =	xml->getAttributeValueAsFloat("VelY");
				particle.m_szSourceBlend = xml->getAttributeValue("SourceBlend");
				particle.m_szDestBlend =	xml->getAttributeValue("DestBlend");
				particle.m_nOffsetX =	xml->getAttributeValueAsInt("OffsetX");
				particle.m_nOffsetY =	xml->getAttributeValueAsInt("OffsetY");

				particle.m_fStartRed =	xml->getAttributeValueAsFloat("StartRed");
				particle.m_fEndRed	 = xml->getAttributeValueAsFloat("EndRed");
				particle.m_fStartGreen	 = xml->getAttributeValueAsFloat("StartGreen");
				particle.m_fEndGreen	 = xml->getAttributeValueAsFloat("EndGreen");
				particle.m_fStartBlue	 = xml->getAttributeValueAsFloat("StartBlue");
				particle.m_fEndBlue	 = xml->getAttributeValueAsFloat("EndBlue");
				particle.m_fStartAlpha	 = xml->getAttributeValueAsFloat("StartAlpha");
				particle.m_fEndAlpha	 = xml->getAttributeValueAsFloat("EndAlpah");

				particle.m_fStartScaleX	 = xml->getAttributeValueAsFloat("StartScaleX");
				particle.m_fEndScaleX	 = xml->getAttributeValueAsFloat("EndScaleX");
				particle.m_fStartScaleY	 = xml->getAttributeValueAsFloat("StartScaleY");
				particle.m_fEndScaleY	 = xml->getAttributeValueAsFloat("EndScaleY");

				particle.m_fGravityX	= xml->getAttributeValueAsFloat("GravityX");
				particle.m_fGravityY	= xml->getAttributeValueAsFloat("GravityY");

				particle.m_szTrigger	 = xml->getAttributeValue("Trigger");
				particle.m_szFileName = xml->getAttributeValue("FileName");
				}
			}
			break;
		}
	}
	// delete the xml parser after usage
	delete xml;


	particle.m_fPartLocX = locX;
	particle.m_fPartLocY = locY;

	particle.m_szFileName = "Resource/" + particle.m_szFileName;

	// load the particle texture into the TM
	m_pTM = CSGD_TextureManager::GetInstance();

	particle.m_nImageID = m_pTM->LoadTexture((char *)particle.m_szFileName.c_str());

	particle.m_cCurrentColor.a = particle.m_fStartAlpha;
	particle.m_cCurrentColor.r = particle.m_fStartRed;
	particle.m_cCurrentColor.g = particle.m_fStartGreen;
	particle.m_cCurrentColor.b = particle.m_fStartBlue;

	particle.m_cStartColor.a = particle.m_fStartAlpha;
	particle.m_cStartColor.r = particle.m_fStartRed;
	particle.m_cStartColor.g = particle.m_fStartGreen;
	particle.m_cStartColor.b = particle.m_fStartBlue;

	particle.m_cEndColor.a = particle.m_fEndAlpha;
	particle.m_cEndColor.r = particle.m_fEndRed;
	particle.m_cEndColor.g = particle.m_fEndGreen;
	particle.m_cEndColor.b = particle.m_fEndBlue;

	for (int i =0; i < particle.m_fMaxParticles; ++i)
	{
		vParticleList.push_back(particle);
	}
	STOP("CEmitter::LoadXMLEmitter(char*, float, float)");

}


