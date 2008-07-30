//////////////////////////////////////////////////////////////////////////
// File CEmitter.cpp
//
// Author: Rodney Kite (RK)
//
// Purpose: Contains class definition for CParticle Class, which holds
//			all of the info for a particle
//////////////////////////////////////////////////////////////////////////
#include "CEmitter.h"
#include "../xml/irrXML.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace irr;
using namespace io;


CEmitter::CEmitter(void)
{
	m_pD3D	= NULL;
	m_pTM	= NULL;
	m_fPercentAlpha		= 0;
	m_fPercentRed		= 0;
	m_fPercentGreen		= 0;
	m_fPercentBlue		= 0;


	m_fPercentXScale = 0;
	m_fPercentYScale = 0;

	m_fUpdateFloat		= .25f;
	m_fUpdateQuarter	= .25f;

	m_dElapsedTime = 0.0f;
	m_dwCurrentTime = GetTickCount();
	m_dwPreviousTime = GetTickCount();
	m_dwFrameTimer = GetTickCount();
	m_cCurrentColor = D3DXCOLOR(0,0,0,0);
}


CEmitter::CEmitter(const CEmitter& ref)
{

}

CEmitter& CEmitter::operator=(const CEmitter& ref)
{
	if(this != &ref)
	{

	}
	return *this;
}

CEmitter::~CEmitter(void)
{

}

void CEmitter::Exit(void)
{

}
void CEmitter::LoadBinaryEmitter(char* emitterFileName, float locX, float locY, vector<CParticle> &vParticleList)
{
	ifstream fin;

	int nStringSize = 0;;
	char szBuffer[64];
	short nContinue;
	double nDouble;

	fin.open(emitterFileName, std::ios_base::in | std::ios_base::binary);
	if(!fin.is_open())
		return;	

	//szBuffer = new char[nStringSize];

	fin.read((char*)&nContinue, sizeof(short));

	if(nContinue != 0)
		particle.m_bContinuous = true;
	else
		particle.m_bContinuous = false;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fMaxParticles = nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fMaxLife = nStringSize;
	
	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fMinLife = nStringSize;
	
	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fVelX = nStringSize;		

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fVelY = nStringSize;	

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
	particle.m_fStartRed = nStringSize;	

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fEndRed = nStringSize;	

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fStartGreen = nStringSize;	

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fEndGreen = nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fStartBlue = nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fEndBlue = nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fStartAlpha = nStringSize;

	fin.read((char*)&nStringSize, sizeof(int));
	particle.m_fEndAlpha = nStringSize;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fStartScaleX = nDouble;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fEndScaleX = nDouble;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fStartScaleY = nDouble;

	fin.read((char*)&nDouble, sizeof(nDouble));
	particle.m_fEndScaleY = nDouble;

	fin.read((char*)&nStringSize, sizeof(int));
	fin.read(szBuffer,nStringSize);
	szBuffer[nStringSize] = 0;
	particle.m_szTrigger = szBuffer;

	fin.read((char*)&nStringSize, sizeof(int));
	fin.read(szBuffer,nStringSize);
	szBuffer[nStringSize] = 0;
	particle.m_szFileName = szBuffer;

	fin.close();

	//m_fLocX  = locX;
	//m_fLocY  = locY;

	particle.m_fLocX = locX;
	particle.m_fLocY = locY;

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
}

void CEmitter::LoadXMLEmitter(char* emitterFileName, float locX, float locY, vector<CParticle> &vParticleList)
{
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
				particle.m_bContinuous = (bool)xml->getAttributeValue("Continuous");
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
				particle.m_szTrigger	 = xml->getAttributeValue("Trigger");
				particle.m_szFileName = xml->getAttributeValue("FileName");
				}
			}
			break;
		}
	}
	// delete the xml parser after usage
	delete xml;


	particle.m_fLocX = locX;
	particle.m_fLocY = locY;

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

}


