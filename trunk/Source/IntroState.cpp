#include "IntroState.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CGame.h"
#include "MainMenuState.h"

CIntroState::CIntroState(void)
{
}

CIntroState::~CIntroState(void)
{
}

void CIntroState::Enter()
{

	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_BF.InitBitmapFont(m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png"),' ',16,128,128);
	m_nSongID = m_pWM->LoadWave("Resource/KQ_Intro.wav");
	m_nImageID = m_pTM->LoadTexture("Resource/KQ_iCodeGames.png");
	m_nTitleID = m_pTM->LoadTexture("Resource/KQ_Title.png");
	
	m_bAlpha = false;
	m_bTitle = false;

	m_nTitle = 0;
	m_fTimer = 0;
	m_nAlpha = 0;
	m_nVolume = 0;
	m_nMaxVolume = CGame::GetInstance()->GetMusicVolume();

	m_bPaused = false;
	
	m_pWM->SetVolume(m_nSongID,m_nVolume);

	m_pWM->Play(m_nSongID);

}
void CIntroState::Exit()
{
	m_pWM->Stop(m_nSongID);
	m_pWM->UnloadWave(m_nSongID);
}

bool CIntroState::Input(float fElapsedTime)
{
	m_fEscTimer += fElapsedTime;
	
	if(m_bAlpha)
		FadeOut(fElapsedTime);
	if(m_bTitle)
		FadeOut(fElapsedTime);
	
	if(m_bTitle == true && m_nTitle == 0)
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());

	if(m_pDI->GetBufferedKey(DIK_RETURN))
	{	
		m_bTitle = true;
	}
	return true;
}
void CIntroState::Render(float fElapsedTime)
{
	if(!m_bAlpha)
		CSGD_Direct3D::GetInstance()->Clear(255,255,255);
	else
		CSGD_Direct3D::GetInstance()->Clear(m_nAlpha,m_nAlpha,m_nAlpha);

	m_pTM->Draw(m_nImageID,175,0,1.f,1.f, 0,0,0,0,D3DCOLOR_ARGB(m_nAlpha,255,255,255));
	m_BF.DrawTextA("Presents: ",250,250,.5,.5,D3DCOLOR_ARGB(m_nAlpha,0,0,0));
	m_pTM->Draw(m_nTitleID,225,250,1,1,0,0,0,0,D3DCOLOR_ARGB(m_nTitle,255,255,255));
	m_BF.DrawTextA("Press Enter To Begin",100,300,.5,.5,D3DCOLOR_ARGB(m_nTitle,255,255,255));

} 
void CIntroState::Update(float fElapsedTime)
{
	if(m_nAlpha >= 255)
		m_bAlpha = true;

	m_fTimer += fElapsedTime;
	if(!m_bAlpha)
	{		
		if(m_fTimer > .002f) 
		{
			m_fTimer = 0;
			m_nAlpha++;
			if(m_nVolume < m_nMaxVolume)
				m_pWM->SetVolume(m_nSongID,m_nVolume++);
			else
				m_pWM->SetVolume(m_nSongID,m_nMaxVolume);
		}
	}
	else if(!m_bTitle)
		if(m_fTimer > .002f)
		{
			m_fTimer = 0;
			if(m_nTitle <=255)
				m_nTitle++;
			else
				m_nTitle = 255;
		}
}
void CIntroState::FadeOut(float fElapsedTime)
{
	m_fEscTimer += fElapsedTime;
	if(m_fEscTimer > .001)
	{
		m_fEscTimer = 0;
		
		if(m_bTitle)
			if(m_nTitle>0)
				m_nTitle--;
			else
				m_nTitle = 0;
		else
			m_nAlpha--;

		if(m_nAlpha < 0)
			m_nAlpha = 0;

		if(m_bTitle)
			if(m_nVolume >= 0)
				m_pWM->SetVolume(m_nSongID,m_nVolume--);
			else 
				m_pWM->SetVolume(m_nSongID,0);
	}
}
