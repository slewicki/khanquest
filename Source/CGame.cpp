//////////////////////////////////////////////////////////
//	File:	"CGame.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain all outer game functionality.
//////////////////////////////////////////////////////////

#include "CGame.h"
#include "CFactory.h"
#include <fstream>
#include <iostream>
#include <string>
#include "CWorldMapState.h"
#include "../irrXML/irrXML.h"
using namespace std;

CGame::CGame(void)
{
	m_pD3D	= NULL;
	m_pTM	= NULL;
	m_pDS	= NULL;
	m_pWM	= NULL;
	m_pDI	= NULL;
	m_pSelectedCity = NULL;
	m_dwPreviousTime = GetTickCount();
	m_nFrameCounter = 0;
	m_nFPS = 0;	
	m_dwFrameTimer = GetTickCount();
	m_fGameTime = 0;
	m_nSFXVolume = 50;
	m_nMusicVolume = 50;
	*m_pCities = NULL;
	m_nGold = 0;
	
	m_chJinCount = m_chKCount = m_chXiaCount = 0;
}

CGame::~CGame(void)
{
}

CGame* CGame::GetInstance(void)
{
	static CGame instance;	//	Lazy instantiation
							//  Since CGame is running the entire time
	return &instance;
}

bool CGame::Initialize(HWND hWnd, HINSTANCE hInstance,
					   int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	//	Do game initialization here.
	m_hWnd = hWnd;
	m_hInstance = hInstance;
	
	//this->ParseBinaryUnitInfo("Resource/KQ_unitStats.dat");
	this->ParseBinaryUnitInfo("Resource/KQ_unitStats.dat");

	//	Get pointers to singletons:
	m_pD3D	= CSGD_Direct3D::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pDS	= CSGD_DirectSound::GetInstance();
	m_pWM	= CSGD_WaveManager::GetInstance();
	m_pDI	= CSGD_DirectInput::GetInstance();

	//	Call initialize on each wrapper:
	#pragma region WrapperInit
	if (!m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, true))
	{
		MessageBox(0, "InitDirect3D Failed", " Error", MB_OK);
		return false;
	}
	if (!m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite()))
	{
		MessageBox(0, "InitTextureManager Failed", " Error", MB_OK);
		return false;
	}
	if (!m_pDS->InitDirectSound(hWnd))
	{
		MessageBox(0, "InitDirectSound Failed", " Error", MB_OK);
		return false;
	}

	if (!m_pWM->InitWaveManager(hWnd, m_pDS->GetDSObject()))
	{
		MessageBox(0, "InitWaveManager Failed", " Error", MB_OK);
		return false;
	}

	if (!m_pDI->InitDevices(hWnd, hInstance))
	{
		MessageBox(0, "InitDevices Failed", " Error", MB_OK);
		return false;
	} 
#pragma endregion

	// Remember how the window started
	m_bIsWindowed = bIsWindowed;
	
	InitCities();
		
	SetCursorNormal();

	m_pAM = CAnimationManager::GetInstance();
	m_pAM->BinParse("Resource/KQ_Infantry.dat", "Resource/KQ_Infantry.png");
	m_pAM->BinParse("Resource/KQ_Cavalry.dat", "Resource/KQ_Cavalry.png");
	m_pAM->BinParse("Resource/KQ_Cavalry_Archer.dat", "Resource/KQ_CavalryArcher.png");
	m_pAM->BinParse("Resource/KQ_Axmen.dat", "Resource/KQ_Axmen.png");
	m_pAM->BinParse("Resource/KQ_Archer.dat", "Resource/KQ_Archer.png");
	m_pAM->BinParse("Resource/KQ_War_Elephant.dat", "Resource/KQ_WarElephant.png");

	ChangeState(CWorldMapState::GetInstance());
	return false;
}

void CGame::Shutdown(void)
{
	//	Clean up current state
	ChangeState(NULL);

	//	Unload assets
	for (int i = 0; i < 10; i++)
	{
		m_pTM->ReleaseTexture(m_pCities[i]->GetImageID());
		delete m_pCities[i];
	}
	//	Safe Release
	if (m_pDI)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = NULL;
	}
	if (m_pWM)
	{
		m_pWM->ShutdownWaveManager();
		m_pWM = NULL;
	}
	if (m_pDS)
	{
		m_pDS->ShutdownDirectSound();
		m_pDS = NULL;
	}
	if (m_pTM)
	{
		m_pTM->ShutdownTextureManager();
		m_pTM = NULL;
	}
	if (m_pD3D)
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D = NULL;
	}
}

bool CGame::Main(void)
{
	// Calculating Elapsed Time
	DWORD dwCurrentTime = GetTickCount();
	
	float fElapsedTime = (float)(dwCurrentTime - m_dwPreviousTime) / 1000.0f;
	m_fGameTime += fElapsedTime;

	// Remember previous time for next frame
	m_dwPreviousTime = dwCurrentTime;
	
	//	1.	Input
	m_pDI->ReadDevices();	//	usually only want to call this ONCE per frame

	// Get input from all since during battle the HUD still needs input
	// Because of this use paused bools to stop input to other states
	// when pushing on states such as CCityInfoState over CWorldMapState
	for(unsigned int i = 0; i < m_vStates.size(); i++)
	{
		// If false is returned, exit the game
		if(!m_vStates[i]->Input(fElapsedTime))
			return false;
	}


	// Fullscreen Toggling
	if(m_pDI->GetBufferedKey(DIK_RETURN) && (m_pDI->GetKey(DIK_LMENU) || m_pDI->GetKey(DIK_RMENU)))
	{
		m_bIsWindowed = !m_bIsWindowed;
		m_pD3D->ChangeDisplayParam(800, 600, m_bIsWindowed);
		ShowCursor(true);
		
	}

	//	2.	Update	-	move objects, check collisions, do physics, do AI

	m_pWM->Update();	//	clean up sound copies that aren't being used.

	// Update all, read Input comment for reason why
	for(unsigned int i = 0; i < m_vStates.size(); i++)
	{
		m_vStates[i]->Update(fElapsedTime);
	}
	
	//	3.	Draw
	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->LineBegin();
	m_pD3D->SpriteBegin();

	// Print Frames Per Second
	char buffer[128];
	sprintf_s(buffer, _countof(buffer), "FPS: %i", m_nFPS);
	CSGD_Direct3D::GetInstance()->DrawTextA(buffer, 0,20,255,255,255);

	char buffer2[128];
	sprintf_s(buffer2, _countof(buffer2), "Position: %i %i", m_ptMousePos.x, m_ptMousePos.y);
	CSGD_Direct3D::GetInstance()->DrawTextA(buffer2, 120,20,255,255,255);

	// Render all states on the stack
	for(unsigned int i = 0; i < m_vStates.size(); i++)
	{
		m_vStates[i]->Render(fElapsedTime);
	}
	POINT ptMouse = GetCursorPosition();
	//m_pTM->Draw(m_nCursorID, ptMouse.x, ptMouse.y);

	m_pD3D->SpriteEnd();
	m_pD3D->LineEnd();
	m_pD3D->DeviceEnd();

	// Count number of frames
	m_nFrameCounter++;

	// Check if one second has passed
	if (GetTickCount() - m_dwFrameTimer > 1000)
	{
		m_nFPS = m_nFrameCounter;	// remember fps
		m_nFrameCounter = 0;		// reset frame counter
		m_dwFrameTimer = GetTickCount();
	}
	m_pD3D->Present();
	
	return true;
}

void CGame::ChangeState(IGameState* pNewState)
{
	// Exit out of all states on the stack
	for (unsigned int i = 0; i < m_vStates.size(); i++)
	{
		m_vStates[i]->Exit();
	}
	
	// Clear all states from the stack
	m_vStates.clear();

	// Add on the new state and Enter it
	if(pNewState)
	{
		m_vStates.push_back(pNewState);
		m_vStates[0]->Enter();
	}
}

void CGame::PushState(IGameState* pNewState)
{
	// Add on the new state and Enter it
	if(pNewState)
	{
		pNewState->Enter();
		m_vStates.push_back(pNewState);
	}
}

void CGame::PopCurrentState()
{
	// Exit the top state and remove it from the stack
	m_vStates[m_vStates.size()-1]->Exit();
	m_vStates.pop_back();
}

bool CGame::ParseXMLUnitInfo (const char* szFile)
{
	int nUnit, nHP, nAttack, nRange;
	nUnit = 0;
	float fAttackSpeed, fMovement;
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(szFile);
	string szName;
	// parse the file until end reached
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case irr::io::EXN_ELEMENT:
			{
				szName = xml->getNodeName();
			}
			break;
		case irr::io::EXN_TEXT:
			{
				if (!strcmp("HP", szName.c_str()))
					nHP = atoi(xml->getNodeName());
				if (!strcmp("Attack", szName.c_str()))
					nAttack = atoi(xml->getNodeName());
				if (!strcmp("Range", szName.c_str()))
					nRange = atoi(xml->getNodeName());
				if (!strcmp("AttackSpeed", szName.c_str()))
					fAttackSpeed = (float)atof(xml->getNodeName());
				if (!strcmp("Movement", szName.c_str()))
				{
					fMovement = (float)atof(xml->getNodeName());

					// Player unit info
					m_pPlayerUnitInfo[nUnit].SetType(nUnit);
					m_pPlayerUnitInfo[nUnit].SetHP(nHP);
					m_pPlayerUnitInfo[nUnit].SetAttackPower(nAttack);
					m_pPlayerUnitInfo[nUnit].SetRange(nRange);

					m_pPlayerUnitInfo[nUnit].SetAttackSpeed(fAttackSpeed);
					m_pPlayerUnitInfo[nUnit].SetSpeed(fMovement);
					
					// CPU unit info
					m_pCPUUnitInfo[nUnit].SetType(nUnit);
					m_pCPUUnitInfo[nUnit].SetHP(nHP);
					m_pCPUUnitInfo[nUnit].SetAttackPower(nAttack);
					m_pCPUUnitInfo[nUnit].SetRange(nRange);

					m_pCPUUnitInfo[nUnit].SetAttackSpeed(fAttackSpeed);
					m_pCPUUnitInfo[nUnit].SetSpeed(fMovement);
					nUnit++;		
				}
				szName = "none";
		}
		break;
		}
	}

	// delete the xml parser after usage
	delete xml;

	return true;
}

bool CGame::ParseBinaryUnitInfo (const char* szFile)
{
	int nType, nHP, nAttack, nRange;
	double dAttackSpeed, dMovement;
	try 
	{	 
		ifstream input(szFile,ios_base::binary);
		short length = 0;
		input.read((char*)&length, sizeof(length));
		for (int i = 0; i < length; i++)
		{
			input.read((char*)&nType,sizeof(nType));
			input.read((char*)&nHP,sizeof(nHP));
			input.read((char*)&nAttack,sizeof(nAttack));
			input.read((char*)&nRange,sizeof(nRange));

			input.read((char*)&dAttackSpeed,sizeof(dAttackSpeed));
			input.read((char*)&dMovement,sizeof(dMovement));
			
			// Player unit info
			m_pPlayerUnitInfo[i].SetType(nType);
			m_pPlayerUnitInfo[i].SetHP(nHP);
			m_pPlayerUnitInfo[i].SetAttackPower(nAttack);
			m_pPlayerUnitInfo[i].SetRange(nRange);

			m_pPlayerUnitInfo[i].SetAttackSpeed((float)dAttackSpeed);
			m_pPlayerUnitInfo[i].SetSpeed((float)dMovement);
			
			// CPU unit info
			m_pCPUUnitInfo[i].SetType(nType);
			m_pCPUUnitInfo[i].SetHP(nHP);
			m_pCPUUnitInfo[i].SetAttackPower(nAttack);
			m_pCPUUnitInfo[i].SetRange(nRange);

			m_pCPUUnitInfo[i].SetAttackSpeed((float)dAttackSpeed);
			m_pCPUUnitInfo[i].SetSpeed((float)dMovement);
		}

		input.close();
	}
	catch (std::exception e)
	{
		MessageBox(m_hWnd, "Error reading data. Reinstalling may fix the problem", "Error", MB_OK);
		return false;

	}
	return true;
}

CUnit CGame::GetPlayerUnitInfo (int nType)
{
	if(nType >=0 && nType <=5)
		return this->m_pPlayerUnitInfo[nType];
	else
	{
		CUnit unit;
		return unit;
	}
}

CUnit CGame::GetCPUUnitInfo (int nType)
{
	if(nType >=0 && nType <=5)
		return this->m_pCPUUnitInfo[nType];
	else
	{
		CUnit unit;
		return unit;
	}
}

POINT CGame::GetCursorPosition()
	{
		GetCursorPos(&m_ptMousePos);
		POINT pos = GetWindowPosition();
		m_ptMousePos.x -= pos.x;
		m_ptMousePos.y -= pos.y;
		return m_ptMousePos;
	}

POINT CGame::GetWindowPosition()
{
	RECT rWindow;
	GetWindowRect(m_hWnd,&rWindow);
	POINT pos = {rWindow.left, rWindow.top};
	if(this->m_bIsWindowed)
	{
		pos.x += m_ptWindOffset.x/2;
		pos.y += m_ptWindOffset.y-m_ptWindOffset.x/2;
	}
	return pos;
}

void CGame::InitCities()
{
	for (int i = 0; i < 10; i++)
	{
		m_pCities[i] = new CCity();
		m_pCities[i]->SetID(i);

	}

	// Player City
	m_pCities[PCITY]->SetDefaultColor(PCITY);
	m_pCities[PCITY]->SetGoldTribute(0);
	m_pCities[PCITY]->SetOriginalOwner(PLAYER_CITY);
	m_pCities[PCITY]->SetImageID(m_pTM->LoadTexture("Resource/KQ_Mongolia.png"));

	m_pCities[PCITY]->SetClickRect(0, 0, 0, 0);
	m_pCities[PCITY]->AddAdjacent(KCITY1);
	m_pCities[PCITY]->AddAdjacent(XCITY1);
	m_pCities[PCITY]->AddAdjacent(JCITY1);
	m_pCities[PCITY]->AddAdjacent(JCITY2);
	m_pCities[PCITY]->AddAdjacent(KCITY2);


	// Khwarezmian Cities
	//-------------------------------------------------------------------
	// Click Rects
	m_pCities[KCITY1]->SetClickRect(109, 225, 56, 56);
	m_pCities[KCITY2]->SetClickRect(229, 250, 56, 56);
	m_pCities[KCITY3]->SetClickRect(194, 371, 56, 56);
	// Attack point symbol
	m_pCities[KCITY1]->SetAttackPoint(109, 225);
	m_pCities[KCITY2]->SetAttackPoint(229, 250);
	m_pCities[KCITY3]->SetAttackPoint(194, 371);



	m_pCities[KCITY1]->SetDefaultColor(KCITY1);
	m_pCities[KCITY1]->SetGoldTribute(300);
	m_pCities[KCITY1]->SetOriginalOwner(K_CITY);
	m_pCities[KCITY1]->SetImageID(m_pTM->LoadTexture("Resource/KQ_kEasy.png"));
	// Adjacent
	m_pCities[KCITY1]->AddAdjacent(KCITY2);
	m_pCities[KCITY1]->AddAdjacent(KCITY3);


	m_pCities[KCITY2]->SetDefaultColor(KCITY2);
	m_pCities[KCITY2]->SetGoldTribute(525);
	m_pCities[KCITY2]->SetOriginalOwner(K_CITY);
	m_pCities[KCITY2]->SetImageID(m_pTM->LoadTexture("Resource/KQ_kMedium.png"));
	// Adjacent
	m_pCities[KCITY2]->AddAdjacent(KCITY1);
	m_pCities[KCITY2]->AddAdjacent(KCITY3);
	m_pCities[KCITY2]->AddAdjacent(XCITY1);
	m_pCities[KCITY2]->AddAdjacent(XCITY2);


	m_pCities[KCITY3]->SetDefaultColor(KCITY3);
	m_pCities[KCITY3]->SetGoldTribute(850);
	m_pCities[KCITY3]->SetOriginalOwner(K_CITY);
	m_pCities[KCITY3]->SetImageID(m_pTM->LoadTexture("Resource/KQ_kHard.png"));
	// Adjacent
	m_pCities[KCITY3]->AddAdjacent(KCITY1);
	m_pCities[KCITY3]->AddAdjacent(KCITY2);
	m_pCities[KCITY3]->AddAdjacent(XCITY2);
	//-------------------------------------------------------------------

	// Xia Cities
	//-------------------------------------------------------------------
	// Click Rects
	m_pCities[XCITY1]->SetClickRect(376, 285, 56, 56);
	m_pCities[XCITY2]->SetClickRect(469, 407, 56, 56);
	m_pCities[XCITY3]->SetClickRect(642, 413, 56, 56);
	// Attack point symbol
	m_pCities[XCITY1]->SetAttackPoint(376, 285);
	m_pCities[XCITY2]->SetAttackPoint(469, 407);
	m_pCities[XCITY3]->SetAttackPoint(642, 413);


	m_pCities[XCITY1]->SetDefaultColor(XCITY1);
	m_pCities[XCITY1]->SetGoldTribute(275);
	m_pCities[XCITY1]->SetOriginalOwner(XIA_CITY);
	m_pCities[XCITY1]->SetImageID(m_pTM->LoadTexture("Resource/KQ_xiaEasy.png"));
	// Adjacent
	m_pCities[XCITY1]->AddAdjacent(XCITY2);
	m_pCities[XCITY1]->AddAdjacent(KCITY2);
	m_pCities[XCITY1]->AddAdjacent(JCITY2);


	m_pCities[XCITY2]->SetDefaultColor(XCITY2);
	m_pCities[XCITY2]->SetGoldTribute(525);
	m_pCities[XCITY2]->SetOriginalOwner(XIA_CITY);
	m_pCities[XCITY2]->SetImageID(m_pTM->LoadTexture("Resource/KQ_xiaMedium.png"));
	// Adjacent
	m_pCities[XCITY2]->AddAdjacent(KCITY2);
	m_pCities[XCITY2]->AddAdjacent(KCITY3);
	m_pCities[XCITY2]->AddAdjacent(XCITY1);
	m_pCities[XCITY2]->AddAdjacent(JCITY2);
	m_pCities[XCITY2]->AddAdjacent(XCITY3);


	m_pCities[XCITY3]->SetDefaultColor(XCITY3);
	m_pCities[XCITY3]->SetGoldTribute(850);
	m_pCities[XCITY3]->SetOriginalOwner(XIA_CITY);
	m_pCities[XCITY3]->SetImageID(m_pTM->LoadTexture("Resource/KQ_xiaHard.png"));
	// Adjacent
	m_pCities[XCITY3]->AddAdjacent(XCITY2);
	m_pCities[XCITY3]->AddAdjacent(JCITY2);
	//-------------------------------------------------------------------

	// Jin Cities
	//-------------------------------------------------------------------
	// Click Rects
	m_pCities[JCITY1]->SetClickRect(534, 53, 56, 56);
	m_pCities[JCITY2]->SetClickRect(547, 207, 56, 56);
	m_pCities[JCITY3]->SetClickRect(661, 123, 56, 56);
	// Attack point symbol
	m_pCities[JCITY1]->SetAttackPoint(534, 53);
	m_pCities[JCITY2]->SetAttackPoint(547, 207);
	m_pCities[JCITY3]->SetAttackPoint(661, 123);



	m_pCities[JCITY1]->SetDefaultColor(JCITY1);
	m_pCities[JCITY1]->SetGoldTribute(300);
	m_pCities[JCITY1]->SetOriginalOwner(JIN_CITY);
	m_pCities[JCITY1]->SetImageID(m_pTM->LoadTexture("Resource/KQ_jinEasy.png"));
	// Adjacent
	m_pCities[JCITY1]->AddAdjacent(JCITY2);
	m_pCities[JCITY1]->AddAdjacent(JCITY3);


	m_pCities[JCITY2]->SetDefaultColor(JCITY2);
	m_pCities[JCITY2]->SetGoldTribute(525);
	m_pCities[JCITY2]->SetOriginalOwner(JIN_CITY);
	m_pCities[JCITY2]->SetImageID(m_pTM->LoadTexture("Resource/KQ_jinMedium.png"));
	// Adjacent
	m_pCities[JCITY2]->AddAdjacent(JCITY1);
	m_pCities[JCITY2]->AddAdjacent(JCITY3);
	m_pCities[JCITY2]->AddAdjacent(XCITY1);
	m_pCities[JCITY2]->AddAdjacent(XCITY2);
	m_pCities[JCITY2]->AddAdjacent(XCITY3);


	m_pCities[JCITY3]->SetDefaultColor(JCITY3);
	m_pCities[JCITY3]->SetGoldTribute(850);
	m_pCities[JCITY3]->SetOriginalOwner(JIN_CITY);
	m_pCities[JCITY3]->SetImageID(m_pTM->LoadTexture("Resource/KQ_jinHard.png"));
	// Adjacent
	m_pCities[JCITY3]->AddAdjacent(JCITY1);
	m_pCities[JCITY3]->AddAdjacent(JCITY2);
	//-------------------------------------------------------------------

}

int CGame::GetNumConquered()
{
	int nConquered = 0;
	// Skip the Mongolia (i=0)
	for (int i = 1; i < 10; i++)
	{
		if(m_pCities[i]->GetOwner() == PLAYER_CITY)
			nConquered++;
	}
	return nConquered;
}

int CGame::GetNextFoodTribute()
{
	int nConquered = GetNumConquered();
	if(nConquered >= 6)
		return 0;
	switch(GetNumConquered()+1)
	{
	case 1:
		return 250;
		break;
	case 2:
		return 300;
		break;
	case 3:
		return 300;
		break;
	case 4:
		return 350;
		break;
	case 5:
		return 400;
		break;
	case 6:
		return 400;
		break;
	default:
		return 0;
	}
}
int CGame::GetTotalFoodTribute()
{
	switch(GetNumConquered())
	{
	case 0:
		return 1000;
		break;
	case 1:
		return 1250;
		break;
	case 2:
		return 1550;
		break;
	case 3:
		return 1850;
		break;
	case 4:
		return 2200;
		break;
	case 5:
		return 2600;
		break;
	default:
		return 3000;
		break;
	}
}

void CGame::SetCityConquered(CCity* pCity)
{

	switch(pCity->GetOwner())
	{
	case XIA_CITY:
		m_chXiaCount++;
		break;
	case JIN_CITY:
		m_chJinCount++;
		break;
	case K_CITY:
		m_chKCount++;
		break;
	};

	pCity->SetOwner(PLAYER_CITY);

	// Add it to our conquered ID list
	m_vConqueredCities.push_back(pCity->GetID());

	// Give our player the gold loot!
	CGame::GetInstance()->SetTotalGold(CGame::GetInstance()->GetTotalGold() + pCity->GetGoldTribute());

	// This city can no longer be sacked for gold
	pCity->SetGoldTribute(0);

}

void CGame::LoseLastCity()
{
	if(m_vConqueredCities.size() < 1)
		return;
	int nCityID = m_vConqueredCities[m_vConqueredCities.size()-1];
	m_vConqueredCities.pop_back();
	m_pCities[nCityID]->Revolt();

	switch(m_pCities[nCityID]->GetOwner())
	{
	case XIA_CITY:
		if(m_chXiaCount < 3)
			m_chXiaCount--;
		break;
	case JIN_CITY:
		if(m_chJinCount < 3)
			m_chJinCount--;
		break;
	case K_CITY:
		if(m_chKCount < 3)
			m_chKCount--;
		break;
	};
}