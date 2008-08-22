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
#include "IntroState.h"
#include "MainMenuState.h"
#include "irrXML.h"
#include "WinGameState.h"
#include "LoseGameState.h"
#include "DirectShow.h"
#include "CWorldMapState.h"
#include "KeyBindState.h"
using namespace std;

CGame::CGame(void)
{
	PROFILE("CGame::CGame()");
	m_pD3D	= NULL;
	m_pTM	= NULL;
	m_pDS	= NULL;
	m_pWM	= NULL;
	m_pDI	= NULL;
//	m_pMap = NULL;
	m_nTerrorLevel = 0;
	m_dwPreviousTime = GetTickCount();
	m_nFrameCounter = 0;
	m_nFPS = 0;	
	m_dwFrameTimer = GetTickCount();
	m_fGameTime = 0;
	m_nSFXVolume = 100;
	m_nMusicVolume = 50;
	*m_pCities = NULL;
	m_nGold = 0;
	m_nWins = 1;
	m_nLoses = 0;
	m_nFood = 0;
	m_pSelectedCity = NULL;
	m_chJinCount = m_chKCount = m_chXiaCount = 0;
	m_bFPS = true;
	m_szPlayerName[0] = 'E';
	m_szPlayerName[1] = 'M';
	m_szPlayerName[2] = 'P';
	m_szPlayerName[3] = 'T';
	m_szPlayerName[4] = 'Y';
	m_szPlayerName[5] = '\0';
	m_nCurrentSaveSlot = SLOT1;
}

CGame::~CGame(void)
{
	delete m_nPlayList;
}

CGame* CGame::GetInstance(void)
{
	PROFILE("CGame::GetInstance()");
	static CGame instance;	//	Lazy instantiation
							//  Since CGame is running the entire time
	return &instance;
}

bool CGame::Initialize(HWND hWnd, HINSTANCE hInstance,
					   int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	PROFILE("CGame::Initialize(HWND, HINSTANCE, int, int, bool)");
	srand(GetTickCount());
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
	if (!m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, false))
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

	// Init the Pixel Shader
	if (!m_PixelShader.Create("Resource/KQ_FoW.fx", m_pD3D->GetDirect3DDevice()))
		MessageBox(hWnd, "Failed to load Pixel Shader", "Error", MB_OK);
	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;

	// Remember how the window started
	m_bIsWindowed = bIsWindowed;

	//m_pMap	= CTileEngine::GetInstance();

	InitCities();
		
//	SetCursorNormal();
	m_nCurrentSong = 0;
	ParseOptions("Resource/KQ_Options.dat");

	m_pAM = CAnimationManager::GetInstance();
	m_pAM->BinParse("Resource/KQ_Infantry.dat", "Resource/KQ_Player_Infantry.png","Resource/Enemies/KQ_AI_Infantry.png");
	m_pAM->BinParse("Resource/KQ_Axmen.dat", "Resource/KQ_Player_Axmen.png","Resource/Enemies/KQ_AI_Axmen.png");
	m_pAM->BinParse("Resource/KQ_Cavalry.dat", "Resource/KQ_Player_Cavalry.png","Resource/Enemies/KQ_AI_Cavalry.png");
	m_pAM->BinParse("Resource/KQ_Cavalry_Archer.dat", "Resource/KQ_Player_CavalryArcher.png","Resource/Enemies/KQ_AI_CavalryArcher.png");
	m_pAM->BinParse("Resource/KQ_Archer.dat", "Resource/KQ_Player_Archer.png","Resource/Enemies/KQ_AI_Archer.png");
	m_pAM->BinParse("Resource/KQ_War_Elephant.dat", "Resource/KQ_Player_War_Elephant.png","Resource/Enemies/KQ_AI_War_Elephant.png");

	ChangeState(CIntroState::GetInstance());
	//	Create the render target.
	// D3DUSAGE_RENDERTARGET can only be used with D3DPOOL_DEFAULT
	m_pD3D->GetDirect3DDevice()->CreateTexture(m_nScreenWidth, m_nScreenHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pRenderTarget, NULL);
	//m_pD3D->GetDirect3DDevice()->CreateTexture(m_nScreenWidth, m_nScreenHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pVisibleTarget, NULL);
	
	m_nFontID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_BF.InitBitmapFont(m_nFontID,' ',16,128,128);
	
	this->ParsePlayList("Resource/KQ_PlayList.xml");
	m_nAttackSoundID[UNIT_AXMEN] = m_nAttackSoundID[UNIT_INFANTRY] = m_pWM->LoadWave("Resource/KQ_SwordAttack.wav");
	m_nAttackSoundID[UNIT_CAVALRY] = m_nAttackSoundID[UNIT_CAVALRY_ARCHER] = m_nAttackSoundID[UNIT_ARCHER] = m_pWM->LoadWave("Resource/KQ_ArrowAttack.wav");
	m_nAttackSoundID[UNIT_WAR_ELEPHANT] = m_pWM->LoadWave("Resource/KQ_AttackElephant.wav");
	m_nDeathSoundID[UNIT_AXMEN] = m_nDeathSoundID[UNIT_INFANTRY] = m_nDeathSoundID[UNIT_ARCHER] = m_pWM->LoadWave("Resource/KQ_DeathMen.wav");
    m_nDeathSoundID[UNIT_CAVALRY] = m_nDeathSoundID[UNIT_CAVALRY_ARCHER] = m_pWM->LoadWave("Resource/KQ_DeathHorse.wav");
	m_nDeathSoundID[UNIT_WAR_ELEPHANT] = m_pWM->LoadWave("Resource/KQ_DeathElephant.wav");
	
	return true;
}
void CGame::Shutdown(void)
{
	PROFILE("CGame::ShutDown()");
	//	Clean up current state
	ChangeState(NULL);

	//	Unload assets
	for (int i = 0; i < 10; i++)
	{
		m_pTM->ReleaseTexture(m_nFontID);
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
	PROFILE("CGame::Main()");
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
	// Fullscreen Toggling
	if( (m_pDI->GetBufferedKey(DIK_RETURN)  || m_pDI->GetBufferedKey(DIK_NUMPADENTER) ) && (m_pDI->GetKey(DIK_LMENU) || m_pDI->GetKey(DIK_RMENU)))
	{
		m_bIsWindowed = !m_bIsWindowed;
		m_pD3D->ChangeDisplayParam(800, 600, m_bIsWindowed);
		ShowCursor(true);

		
	}

	for(unsigned int i = 0; i < m_vStates.size(); i++)
	{
		// If false is returned, exit the game
		if(!m_vStates[i]->Input(fElapsedTime))
			return false;
	}


	

	//	2.	Update	-	move objects, check collisions, do physics, do AI

	m_pWM->Update();	//	clean up sound copies that aren't being used.

	// Update all, read Input comment for reason why
	for(unsigned int i = 0; i < m_vStates.size(); i++)
	{
		m_vStates[i]->Update(fElapsedTime);
	}

	if(!m_pWM->IsWavePlaying(m_nPlayList[m_nCurrentSong]))
	{
		m_pWM->SetVolume(m_nPlayList[m_nCurrentSong],m_nMusicVolume);
		m_pWM->Stop(m_nPlayList[m_nPreviousSong]);
		m_pWM->Play(m_nPlayList[m_nCurrentSong],DSBPLAY_LOOPING);
		m_nPreviousSong = m_nCurrentSong;
	}
	//	3.	Draw
	// Check for Device
	//bool bDevice = m_pD3D->GetDirect3DDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET;
	//if(bDevice)
	//{
	//	m_pRenderTarget->Release();
	//	//m_pVisibleTarget->Release();

	//	HRESULT hr = m_pD3D->GetDirect3DDevice()->CreateTexture(m_nScreenWidth, m_nScreenHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pRenderTarget, NULL);
	//	if(hr != D3D_OK)
	//	{
	//		if(hr == D3DERR_INVALIDCALL)
	//			int i = 1;
	//		else if(hr == D3DERR_OUTOFVIDEOMEMORY)
	//			int i = 1;
	//		else if(hr == D3DERR_NOTAVAILABLE)
	//			int i = 1;
	//		else if (hr == E_OUTOFMEMORY)
	//			int i = 1;

	//		MessageBox(m_hWnd, "Failed to load texture", "Error", MB_OK);
	//	}
	//}

	//	/*hr = m_pD3D->GetDirect3DDevice()->CreateTexture(m_nScreenWidth, m_nScreenHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pVisibleTarget, NULL);
	//	if(hr != D3D_OK)
	//	{
	//		if(hr == D3DERR_INVALIDCALL)
	//			int i = 1;
	//		else if(hr == D3DERR_OUTOFVIDEOMEMORY)
	//			int i = 1;
	//		else if(hr == D3DERR_NOTAVAILABLE)
	//			int i = 1;
	//		else if (hr == E_OUTOFMEMORY)
	//			int i = 1;

	//		MessageBox(m_hWnd, "Failed to load texture", "Error", MB_OK);
	//	}*/
	//}

	if(/*m_bIsWindowed && */CDirectShow::GetInstance()->IsPlaying() && InvalidateRect(m_hWnd, 0, 0))
		return true;

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->LineBegin();
	m_pD3D->SpriteBegin();

	//grab the backbuffer
	m_pD3D->GetDirect3DDevice()->GetRenderTarget(0, &m_pBackBuffer);

	if(m_vStates[0] == CGamePlayState::GetInstance())
	{

		//Set Render Target
		LPDIRECT3DSURFACE9 pSurface = NULL;
		m_pRenderTarget->GetSurfaceLevel(0, &pSurface);
		m_pD3D->GetDirect3DDevice()->SetRenderTarget(0, pSurface);
		pSurface->Release();

		//LPDIRECT3DTEXTURE9 pVisible = NULL;
		//D3DXCreateTextureFromFile(m_pD3D->GetDirect3DDevice(), "Resource/KQ_Circle.bmp", &pVisible);
		//m_pVisibleTarget = pVisible;
		////m_pD3D->GetDirect3DDevice()->SetRenderTarget(0, pVisible);
		//pVisible->Release();
	}

	m_pD3D->Clear(0, 0, 0);

	////Print mouse position
	//char buffer2[128];
	//sprintf_s(buffer2, _countof(buffer2), "Position: %i %i", m_ptMousePos.x, m_ptMousePos.y);
	//CSGD_Direct3D::GetInstance()->DrawTextA(buffer2, 120,20,255,255,255);

	// Render all states on the stack
	for(unsigned int i = 0; i < m_vStates.size(); i++)
	{
		m_vStates[i]->Render(fElapsedTime);
	}
	POINT ptMouse = GetCursorPosition();
	//m_pTM->Draw(m_nCursorID, ptMouse.x, ptMouse.y);

	//Print Frames Per Second
	if (m_bFPS)
	{
		char buffer[128];
		sprintf_s(buffer, _countof(buffer), "FPS: %i", m_nFPS);

		m_BF.DrawTextA(buffer,0,20,.3f,.3f,D3DCOLOR_ARGB(255,255,255,255));//CSGD_Direct3D::GetInstance()->DrawTextA(buffer, 0,20,255,255,255);
	}

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
	if(m_vStates[0] != CGamePlayState::GetInstance())
		m_pD3D->Present();
	else if (ObjectManager::GetInstance()->GetUnits().size() > 0)
	{
		//Setup Pixel shader for second loop
		m_pD3D->GetDirect3DDevice()->SetRenderTarget(0, m_pBackBuffer);
		m_pBackBuffer->Release();

		

		//Pixel Shader Render
		m_pD3D->Clear(0, 0, 0);
		m_pD3D->DeviceBegin();
		m_pD3D->SpriteBegin();

		//process shader
		m_PixelShader.SetConstantFloat("Pos.x",ObjectManager::GetInstance()->GetUnits()[0]->GetPosX());
		m_PixelShader.SetConstantFloat("Pos.Y", ObjectManager::GetInstance()->GetUnits()[0]->GetPosX());
		//m_PixelShader.SetConstantFloat("texture1", m_pTM->LoadTexture("Resource/KQ_Circle.bmp"));
		m_PixelShader.Begin();

		//Draw to the backbuffer
		m_pD3D->GetSprite()->Draw(m_pRenderTarget, 0, 0, 0, D3DCOLOR_XRGB(255, 255, 255));

		m_pD3D->SpriteEnd();
		m_PixelShader.End();
		m_pD3D->DeviceEnd();
		m_pD3D->Present();
	}
	
	return true;
}

void CGame::ChangeState(IGameState* pNewState)
{
	PROFILE("CGame::ChangeState(IGameState*)");
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
	PROFILE("CGame::PushState(IGameState*)");
	// Add on the new state and Enter it
	if(pNewState)
	{
		pNewState->Enter();
		m_vStates.push_back(pNewState);
	}
}

void CGame::PopCurrentState()
{
	PROFILE("CGame::PopCurrentState()");
	// Exit the top state and remove it from the stack
	m_vStates[m_vStates.size()-1]->Exit();
	m_vStates.pop_back();
}

bool CGame::ParseXMLUnitInfo (const char* szFile)
{
	PROFILE("CGame::ParseXMLUnitInfo(const char*)");
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
				else if (!strcmp("Attack", szName.c_str()))
					nAttack = atoi(xml->getNodeName());
				else if (!strcmp("Range", szName.c_str()))
					nRange = atoi(xml->getNodeName());
				else if (!strcmp("AttackSpeed", szName.c_str()))
					fAttackSpeed = (float)atof(xml->getNodeName());
				else if (!strcmp("Cost", szName.c_str()))
					fAttackSpeed = (float)atof(xml->getNodeName());
				else if (!strcmp("Movement", szName.c_str()))
				{
					fMovement = (float)atof(xml->getNodeName());

					// Player unit info
					m_pPlayerUnitInfo[nUnit].SetType(nUnit);
					m_pPlayerUnitInfo[nUnit].SetMaxHP(nHP);
					m_pPlayerUnitInfo[nUnit].SetAttackPower(nAttack);
					m_pPlayerUnitInfo[nUnit].SetRange(nRange);

					m_pPlayerUnitInfo[nUnit].SetAttackSpeed(fAttackSpeed);
					m_pPlayerUnitInfo[nUnit].SetSpeed(fMovement);
					
					// CPU unit info
					m_pCPUUnitInfo[nUnit].SetType(nUnit);
					m_pCPUUnitInfo[nUnit].SetMaxHP(nHP);
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
	PROFILE("CGame::ParseBinaryUnitInfo(const char*)");
	int nType, nHP, nAttack, nRange, nCost, nBuffer;
	double dAttackSpeed, dMovement, dBuffer;
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
			input.read((char*)&nCost,sizeof(nCost));

			// Unneeded info
			//-------------
			input.read((char*)&nBuffer,sizeof(nBuffer));
			input.read((char*)&nBuffer,sizeof(nBuffer));
			input.read((char*)&nBuffer,sizeof(nBuffer));
			input.read((char*)&nBuffer,sizeof(nBuffer));
			input.read((char*)&nBuffer,sizeof(nBuffer));
			input.read((char*)&nBuffer,sizeof(nBuffer));

			input.read((char*)&dBuffer,sizeof(dBuffer));
			input.read((char*)&dBuffer,sizeof(dBuffer));
			input.read((char*)&dBuffer,sizeof(dBuffer));
			input.read((char*)&dBuffer,sizeof(dBuffer));

			input.read((char*)&nBuffer,sizeof(nBuffer));
			input.read((char*)&nBuffer,sizeof(nBuffer));

			
			//-------------

			
			// Player unit info
			m_pPlayerUnitInfo[i].SetType(nType);
			m_pPlayerUnitInfo[i].SetMaxHP(nHP);
			m_pPlayerUnitInfo[i].SetAttackPower(nAttack);
			m_pPlayerUnitInfo[i].SetRange(nRange);
			m_pPlayerUnitInfo[i].SetCost(nCost);

			m_pPlayerUnitInfo[i].SetAttackSpeed((float)dAttackSpeed);
			m_pPlayerUnitInfo[i].SetSpeed((float)dMovement);
			
			// CPU unit info
			m_pCPUUnitInfo[i].SetType(nType);
			m_pCPUUnitInfo[i].SetMaxHP(nHP);
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


bool CGame::ParsePlayList(const char* szFileName)
{
	PROFILE("CGame::ParsePlayList(const char*)");
	//Function variables - do not change
	string szName, szTempBool;
	IrrXMLReader* xml = createIrrXMLReader(szFileName);
	int nCounter = 0;
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT:
			{
				//Grabs the attribute name
 				szName = xml->getNodeName();
			}
			break;
		case EXN_TEXT:
			{
				//Checks the Attribute name and sets the approiate value
				if(!strcmp("NumSongs",szName.c_str()))
				{
					m_nNumSongs = atoi(xml->getNodeName());
					m_nPlayList = new int[m_nNumSongs];
				}
				if (!strcmp("Song", szName.c_str()))
				{
					string idk = xml->getNodeName();
					char temp[128];
					strncpy(temp,idk.c_str(),idk.length());
					temp[idk.length()] = 0;
					m_nPlayList[nCounter] = m_pWM->LoadWave(temp);
					nCounter++;
				}				
				szName = "none";
			}
			break;
		}
	}
	//Delete the parser
	delete xml;
	return true;
}

CUnit CGame::GetUnitInfo (int nType)
{
	PROFILE("CGame::GetUnitInfo(int)");
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
	PROFILE("CGame::GetCPUInfo(int)");
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
		PROFILE("CGame::GetCursorPosition()");
		GetCursorPos(&m_ptMousePos);
		POINT pos = GetWindowPosition();
		m_ptMousePos.x -= pos.x;
		m_ptMousePos.y -= pos.y;
		return m_ptMousePos;
	}

POINT CGame::GetWindowPosition()
{
	PROFILE("CGame::getWindowPosition()");
	RECT rWindow;
	GetWindowRect(m_hWnd,&rWindow);
	POINT pos = {rWindow.left, rWindow.top};
	if(this->m_bIsWindowed)
	{
		pos.x += (int)(m_ptWindOffset.x*.5f);
		pos.y += (int)(m_ptWindOffset.y-m_ptWindOffset.x*.5f);
	}
	return pos;
}

void CGame::InitCities()
{
	PROFILE("CGame::InitCities()");
	for (int i = 0; i < 10; i++)
	{
		if(m_pCities[i])
		{
			delete m_pCities[i];
		}
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
	PROFILE("CGame::getNumConquered()");
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
	PROFILE("CGame::GetNextFoodTribute()");
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
	PROFILE("CGame::getTotalFoodTribute()");
	switch(GetNumConquered())
	{
	case 0:
		m_nFood = 1000;
		break;
	case 1:
		m_nFood = 1250;
		break;
	case 2:
		m_nFood = 1550;
		break;
	case 3:
		m_nFood = 1850;
		break;
	case 4:
		m_nFood = 2200;
		break;
	case 5:
		m_nFood = 2600;
		break;
	default:
		m_nFood = 3000;
		break;
	}
	return m_nFood;
}

void CGame::SetCityConquered(CCity* pCity)
{
	PROFILE("CGame::SetCityConquered(CCity*)");

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
	GetTotalFoodTribute();
	//AddWins();
}

void CGame::LoseLastCity()
{
	PROFILE("CGame::LoseLastCity()");
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
	if(m_nWins > 1)
		--m_nWins;
	
}
void CGame::AddWins()
{
	PROFILE("CGame::AddWins()");
	++m_nWins;
	if(m_nLoses > 0)
		--m_nLoses;
	if(GetTerrorLevel() < 100)
	{
		SetTerrorLevel(GetTerrorLevel() + 25);
	}
	
	if(m_nWins == TOTAL_CITIES)
	{
		while(m_vStates.size() > 0)
			PopCurrentState();
		PushState(CMainMenuState::GetInstance());
		PushState(CWinGameState::GetInstance());
		InitCities();
		m_nGold = 0;
		m_nWins = 1;
	}
	else
		Save(false);

}
void CGame::AddLoses()
{
	PROFILE("CGame::AddLoses()");
	++m_nLoses;
	
	
	SetTerrorLevel(GetTerrorLevel() - 25);
	if(m_nLoses >= 2 && this->GetNumConquered() == 0)
	{	
		while(m_vStates.size() > 0)
			PopCurrentState();
	
		PushState(CMainMenuState::GetInstance());
		PushState(CLoseGameState::GetInstance());
		
	}
	else if(m_nLoses >= 2)
	{
		LoseLastCity();
		m_nLoses = 0;

		
	}
	else
	{
		Save(false);
	}
}


bool CGame::LoadSlot(int nSlot)
{
	PROFILE("CGame::LoadSlot(int)");
	NewGame(nSlot);
	string szSaveSlot;
	switch (nSlot)
	{
	case SLOT1:
		m_nCurrentSaveSlot = SLOT1;
		szSaveSlot = "Resource/KQ_Save1.dat";
		break;
	case SLOT2:
		m_nCurrentSaveSlot = SLOT2;
		szSaveSlot = "Resource/KQ_Save2.dat";
		break;
	case SLOT3:
		m_nCurrentSaveSlot = SLOT3;
		szSaveSlot = "Resource/KQ_Save3.dat";
		break;
	default:
		m_nCurrentSaveSlot = -1;
		return false;
	}
	try 
	{	 
		ifstream input;
		input.open(szSaveSlot.c_str(),ios_base::binary);
		if(!input.is_open())
		{
			MessageBox(m_hWnd, "Save file not found.  Try making a new one." , "Error", MB_OK);

			return false;
		}
		int nLength = 0;
		input.read((char*)&nLength, sizeof(nLength));
		input.read(m_szPlayerName, nLength);

		// Food total
		input.read((char*)&m_nFood  , sizeof(m_nFood));
		// Gold total
		input.read((char*)&m_nGold  , sizeof(m_nGold));

		int tempGold = m_nGold;

		//Conquered Cities
		int nConquered = 0;
		int nCity = 0;
		
		input.read((char*) &nConquered  , sizeof( nConquered));
		for ( int i = 0; i < nConquered; i++)
		{
			input.read((char*)&nCity  , sizeof(nCity ));
			SetCityConquered(m_pCities[nCity]);
		}

		input.read((char*)&m_nTerrorLevel, sizeof(m_nTerrorLevel));
		int nType, nMaxHP, nAttack, nRange, nCost;
		float fAttack, fSpeed;
		for (int i = 0; i < 6; i++)
		{
			// read out player unit stats (upgrades inside)
			
	
			input.read((char*)&nType,			sizeof(nType));
			input.read((char*)&nMaxHP,			sizeof(nMaxHP));
			input.read((char*)&nAttack,			sizeof(nAttack));
			input.read((char*)&nRange,			sizeof(nRange));
			input.read((char*)&fAttack,			sizeof(fAttack));
			input.read((char*)&fSpeed,			sizeof(fSpeed));
			input.read((char*)&nCost,			sizeof(nCost));
			
			m_pPlayerUnitInfo[i].SetType(nType);
			m_pPlayerUnitInfo[i].SetMaxHP(nMaxHP);
			m_pPlayerUnitInfo[i].SetAttackPower(nAttack);
			m_pPlayerUnitInfo[i].SetRange(nRange);
			m_pPlayerUnitInfo[i].SetAttackSpeed(fAttack);
			m_pPlayerUnitInfo[i].SetSpeed(fSpeed);
			m_pPlayerUnitInfo[i].SetCost(nCost);

		}
		// read out which upgrades are active

		
		// Wins total
		input.read((char*)&m_nWins , sizeof(m_nWins));
		// Loses total
		input.read((char*)&m_nLoses  , sizeof(m_nLoses));
		
		
		// Jin Count (num jin conquered)
		input.read((char*)&m_chJinCount  , sizeof(m_chJinCount));
		input.read((char*)&m_chXiaCount  , sizeof(m_chXiaCount));
		input.read((char*)&m_chKCount  , sizeof(m_chKCount));


		input.close();
		
		// Reset gold since setcityconquered will add on to it
		m_nGold = tempGold;
	}
	
	
	catch (std::exception e)
	{
		MessageBox(m_hWnd, "Error loading data. My bad...", "Error", MB_OK);
		return false;

	}
	PushState(CWorldMapState::GetInstance());
	return true;

}
void CGame::NewGame(int nSlot)
{
	PROFILE("CGame::NewGame(int)");
	m_nCurrentSaveSlot = nSlot;
	*m_pCities = NULL;
	m_nGold = 0;
	m_nTerrorLevel = 0;
	m_nWins = 1;
	m_nLoses = 0;
	m_nFood = 0;
	m_pSelectedCity = NULL;
	m_vConqueredCities.clear();
	m_chJinCount = m_chKCount = m_chXiaCount = 0;
	this->ParseBinaryUnitInfo("Resource/KQ_unitStats.dat");
	InitCities();
}

bool CGame::Save(bool bNewSave)
{
	PROFILE("CGame::Save(bool)");
	string szSaveSlot;
	switch (m_nCurrentSaveSlot)
	{
	case SLOT1:
		m_nCurrentSaveSlot = SLOT1;
		szSaveSlot = "Resource/KQ_Save1.dat";
		m_szPlayerName[0] = 'G';
		m_szPlayerName[1] = 'A';
		m_szPlayerName[2] = 'M';
		m_szPlayerName[3] = 'E';
		m_szPlayerName[4] = '1';
		m_szPlayerName[5] = '\0';
		break;
	case SLOT2:
		m_nCurrentSaveSlot = SLOT2;
		szSaveSlot = "Resource/KQ_Save2.dat";
		m_szPlayerName[0] = 'G';
		m_szPlayerName[1] = 'A';
		m_szPlayerName[2] = 'M';
		m_szPlayerName[3] = 'E';
		m_szPlayerName[4] = '2';
		m_szPlayerName[5] = '\0';
		break;
	case SLOT3:
		m_nCurrentSaveSlot = SLOT3;
		szSaveSlot = "Resource/KQ_Save3.dat";
		m_szPlayerName[0] = 'G';
		m_szPlayerName[1] = 'A';
		m_szPlayerName[2] = 'M';
		m_szPlayerName[3] = 'E';
		m_szPlayerName[4] = '3';
		m_szPlayerName[5] = '\0';
		break;
	default:
		m_nCurrentSaveSlot = -1;
		return false;
	}
	if(bNewSave)
	{
		m_szPlayerName[0] = 'E';
		m_szPlayerName[1] = 'M';
		m_szPlayerName[2] = 'P';
		m_szPlayerName[3] = 'T';
		m_szPlayerName[4] = 'Y';
		m_szPlayerName[5] = '\0';
	}

	try 
	{	 
		ofstream output(szSaveSlot.c_str(),ios_base::binary);
		int length = sizeof(m_szPlayerName);
		output.write((char*)&length, sizeof(length));
		output.write(m_szPlayerName, sizeof(m_szPlayerName));



		// Food total
		output.write((char*)&m_nFood  , sizeof(m_nFood));
		// Gold total
		output.write((char*)&m_nGold  , sizeof(m_nGold));

		//Conquered Cities
		int nConquered = (int)m_vConqueredCities.size();
		output.write((char*)&nConquered   , sizeof( nConquered));
		for (unsigned int i = 0; i < m_vConqueredCities.size(); i++)
		{
			output.write((char*)&m_vConqueredCities[i]  , sizeof(m_vConqueredCities[i] ));
		}


		output.write((char*)&m_nTerrorLevel, sizeof(m_nTerrorLevel));
		int nType, nMaxHP, nAttack, nRange, nCost;
		float fAttack, fSpeed;
		for (int i = 0; i < 6; i++)
		{
			// Write out player unit stats (upgrades inside)
			//output.write(reinterpret_cast<char *>(&m_pPlayerUnitInfo[i])  , sizeof(m_pPlayerUnitInfo[i]));
			nType	= m_pPlayerUnitInfo[i].GetType();		
			nMaxHP	=  m_pPlayerUnitInfo[i].GetMaxHP();		
			nAttack	=  m_pPlayerUnitInfo[i].GetAttackPower();
			nRange	=  m_pPlayerUnitInfo[i].GetRange();		
			fAttack	=  m_pPlayerUnitInfo[i].GetAttackSpeed();
			fSpeed	=  m_pPlayerUnitInfo[i].GetSpeed();	
			nCost	=  m_pPlayerUnitInfo[i].GetCost();		
			output.write((char *)(&nType	),			sizeof(nType));
			output.write((char *)(&nMaxHP	),		sizeof(nMaxHP));
			output.write((char *)(&nAttack	),	sizeof(nAttack));
			output.write((char *)(&nRange	),		sizeof(nRange));
			output.write((char *)(&fAttack	),	sizeof(fAttack));
			output.write((char *)(&fSpeed	),		sizeof(fSpeed));
			output.write((char *)(&nCost	),			sizeof(nCost));

		}
		// Write out which upgrades are active

		
		// Wins total
		output.write((char*)&m_nWins , sizeof(m_nWins));
		// Loses total
		output.write((char*)&m_nLoses  , sizeof(m_nLoses));
		
		
		// Jin Count (num jin conquered)
		output.write((char*)&m_chJinCount  , sizeof(m_chJinCount));
		output.write((char*)&m_chXiaCount  , sizeof(m_chXiaCount));
		output.write((char*)&m_chKCount  , sizeof(m_chKCount));


		output.close();
	}
	catch (std::exception e)
	{
		MessageBox(m_hWnd, "Error saving data. Reinstalling may fix the problem (yeah right).", "Error", MB_OK);
		return false;

	}
	return true;
}
string CGame::GetSaveName(int nSlot, bool bTitle)
{
	PROFILE("CGame::GetSaveName(int, bool)");
	string szSaveSlot;
	char	szName[6];
	int nFood, nGold, nConquered;
	switch (nSlot)
	{
	case SLOT1:
		szSaveSlot = "Resource/KQ_Save1.dat";
		break;
	case SLOT2:
		szSaveSlot = "Resource/KQ_Save2.dat";
		break;
	case SLOT3:
		szSaveSlot = "Resource/KQ_Save3.dat";
		break;
	default:
		m_nCurrentSaveSlot = -1;
		return false;
	}
	try 
	{	 
		ifstream input;
		input.open(szSaveSlot.c_str(),ios_base::binary);

		if(!input.is_open())
		{
			//MessageBox(m_hWnd, "Save file not found.  Try making a new one." , "Error", MB_OK);
			if(bTitle)
				return "EMPTY";
			else
				" ";
		}
		int nLength = 0;
		input.read((char*)&nLength, sizeof(nLength));
		input.read(szName, nLength);

		// Food total
		input.read((char*)&nFood  , sizeof(nFood));
		// Gold total
		input.read((char*)&nGold  , sizeof(nGold));

		
		
		input.read((char*) &nConquered  , sizeof( nConquered));

	}
	catch (std::exception e)
	{
		MessageBox(m_hWnd, "Error reading files.", "Error", MB_OK);
		return false;
	}
	
	string szInfo;
	char buffer[128];
	sprintf_s(buffer, 128, "/Conquered: %i  Food: %i  Gold: %i", nConquered, nFood, nGold);

	szInfo = buffer;
	if(bTitle)
		return szName;
	else
		return szInfo;
}
void CGame::ParseOptions(char* szFileName)
{
	PROFILE("CGame::ParseOptions(char*)");
	ifstream toRead;
	toRead.open(szFileName,ios::in | ios::binary);
	for( int i = 0; i < 4 ;++i)
	{
		DWORD temp;
		toRead.read((char*)&temp,sizeof(temp));
		CKeyBindState::GetInstance()->BindKey(temp,i);
	}
	toRead.read((char*)&m_bIsWindowed,sizeof(m_bIsWindowed));
	toRead.read((char*)&m_bFPS,sizeof(m_bFPS));	
	toRead.read((char*)&m_nMusicVolume,sizeof(m_nMusicVolume));
	toRead.read((char*)&m_nSFXVolume,sizeof(m_nSFXVolume));
}