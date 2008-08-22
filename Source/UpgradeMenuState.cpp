#include "UpgradeMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CSGD_DirectInput.h"
#include "CGame.h"
#include "CUnit.h"
#include "CUnitCreationState.h"

CUpgradeMenuState::CUpgradeMenuState(void)
{
	m_nBackgroundID = -1;
	m_nScrollButtonID = -1;
}

CUpgradeMenuState::~CUpgradeMenuState(void)
{
}
void CUpgradeMenuState::Enter()
{
	// Get Our Managers Ready
	m_fTimer = 0.f;

	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();

	m_nGoldTotal = CGame::GetInstance()->GetTotalGold();

	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_nBackgroundID = m_pTM->LoadTexture("Resource/KQ_PageBkg3.png");
	m_nScrollButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");
	m_nCheckBoxID = m_pTM->LoadTexture("Resource/KQ_CheckBox.png");
	m_nCheckMarkID = m_pTM->LoadTexture("Resource/KQ_CheckMark.png");
	m_nClick = m_pWM->LoadWave("Resource/KQ_UpgradePurchase.wav");

	m_pWM->SetVolume(m_nClick,CGame::GetInstance()->GetSFXVolume());

	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);

	m_nUnitCosts[UNIT_INFANTRY] = 65;
	m_nUnitCosts[UNIT_CAVALRY] = 150;
	m_nUnitCosts[UNIT_CAVALRY_ARCHER] = 150;
	m_nUnitCosts[UNIT_AXMEN] = 65;
	m_nUnitCosts[UNIT_ARCHER] = 60;
	m_nUnitCosts[UNIT_WAR_ELEPHANT] = 300;

	m_rAttackButton.left = 415;
	m_rAttackButton.top = 515;
	m_rAttackButton.right = 545;
	m_rAttackButton.bottom = 575;	

	m_rBackButton.left = 615;
	m_rBackButton.top = 515;
	m_rBackButton.right = 745;
	m_rBackButton.bottom = 575;

	for( int i = 0; i < 6; ++i)
		m_bModified[i] = false;

	for(int j = 0; j < 6; ++j)
	{
		for (int i  = 0 ; i < 3; ++i)
		{
			m_rCheckBox[j][i].left = 260 + (i * 215);
			m_rCheckBox[j][i].top = 100 + (j * 64);
			m_rCheckBox[j][i].right = m_rCheckBox[j][i].left + 32;
			m_rCheckBox[j][i].bottom = m_rCheckBox[j][i].top + 32;

			m_bUpgraded[j][i] = CGame::GetInstance()->GetUpgraded(j,i);
		}
	}
	m_bAccept = false;
	m_fJoyTimer = 0;
	CGame::GetInstance()->SetSongPlay(CITYSELECT);
	m_nGoldTotal = CGame::GetInstance()->GetTotalGold();
}

void CUpgradeMenuState::Exit()
{	
	for(int i = 0; i < 6;++i)
	{
		switch(i)
		{
		case UNIT_AXMEN:
			{	
				if(m_bModified[i])
					CGame::GetInstance()->SetPlayerUnitInfo(m_pUnit[UNIT_AXMEN],UNIT_AXMEN);
			}break;
		case UNIT_INFANTRY:
			{
				if(m_bModified[i])
					CGame::GetInstance()->SetPlayerUnitInfo(m_pUnit[UNIT_INFANTRY],UNIT_INFANTRY);
			}break;
		case UNIT_CAVALRY:
			{
				if(m_bModified[i])	
					CGame::GetInstance()->SetPlayerUnitInfo(m_pUnit[UNIT_CAVALRY],UNIT_CAVALRY);
			}break;
		case UNIT_CAVALRY_ARCHER:
			{
				if(m_bModified[i])
					CGame::GetInstance()->SetPlayerUnitInfo(m_pUnit[UNIT_CAVALRY_ARCHER],UNIT_CAVALRY_ARCHER);
			}break;
		case UNIT_WAR_ELEPHANT:
			{
				if(m_bModified[i])
					CGame::GetInstance()->SetPlayerUnitInfo(m_pUnit[UNIT_WAR_ELEPHANT],UNIT_WAR_ELEPHANT);
			}break;
		}
	}
	if(m_bAccept)
	{
	CGame::GetInstance()->SetTotalGold(m_nGoldTotal);
		for(int j = 0; j < 6; ++j)
		{	
			for(int i = 0; i < 3; ++i)
			{
				CGame::GetInstance()->SetUpgraded(j,i,m_bUpgraded[j][i]);
			}
		}
	}

	if(m_pWM->IsWavePlaying(m_nClick))
		m_pWM->Stop(m_nClick);
	m_pWM->UnloadWave(m_nClick);

	
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	m_pTM->ReleaseTexture(m_nBackgroundID);
	m_pTM->ReleaseTexture(m_nScrollButtonID);
}

bool CUpgradeMenuState::Input(float fElapsedTime)
{
		m_fJoyTimer = fElapsedTime;
#pragma region Controller to Mouse
	
	if(m_pDI->GetJoystickDir(JOYSTICK_UP) && m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_UP) && m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN) && m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN) && m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_UP))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y);
			m_fJoyTimer = 0;
		}
	}


#pragma endregion

	if(CGame::GetInstance()->IsMouseInRect(m_rAttackButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
		{
			m_pWM->Play(m_nClick);
			m_bAccept = true;
			CUnitCreationState::GetInstance()->SetPause(false);
			CGame::GetInstance()->PopCurrentState();
		}
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rBackButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X) )
		{
			m_pWM->Play(m_nClick);
			for( int i = 0; i < 6; ++i)
				m_bModified[i] = false;
			m_bAccept = false;
			CUnitCreationState::GetInstance()->SetPause(false);
			CGame::GetInstance()->PopCurrentState();
		}
	}

		for (int j = 0; j < 6; j++)
		{
			for( int i = 0; i < 3;i++)
			{
				if(CGame::GetInstance()->IsMouseInRect(m_rCheckBox[j][i]))
				{
					CGame::GetInstance()->SetCursorClick();
					if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
					{
						m_pWM->Play(m_nClick);
						switch(j)
						{
						case UNIT_INFANTRY:
							{
								m_pUnit[UNIT_INFANTRY] = CGame::GetInstance()->GetUnitInfo(UNIT_INFANTRY);
								switch(i)
								{
								case 0:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= SPEED)
										{
											m_pUnit[UNIT_INFANTRY].SetSpeed(m_pUnit[UNIT_INFANTRY].GetSpeed() + (m_pUnit[UNIT_INFANTRY].GetSpeed() * .5f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= SPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_INFANTRY].SetSpeed(m_pUnit[UNIT_INFANTRY].GetSpeed() - (m_pUnit[UNIT_INFANTRY].GetSpeed() * .5f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal +=SPEED;
											}
										}
									}break;
								case 1:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= ATTACKSPEED)
										{
											m_pUnit[UNIT_INFANTRY].SetAttackSpeed(m_pUnit[UNIT_INFANTRY].GetAttackSpeed() - (m_pUnit[UNIT_INFANTRY].GetAttackSpeed() * .2f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACKSPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_INFANTRY].SetAttackSpeed(m_pUnit[UNIT_INFANTRY].GetAttackSpeed() + (m_pUnit[UNIT_INFANTRY].GetAttackSpeed() * .2f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += ATTACKSPEED;
											}
										}
									}break;
								case 2:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= ATTACK)
										{
											m_pUnit[UNIT_INFANTRY].SetAttackPower(m_pUnit[UNIT_INFANTRY].GetAttackPower() + 2);
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACK;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_INFANTRY].SetAttackPower(m_pUnit[UNIT_INFANTRY].GetAttackPower() - 2);
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += ATTACK;
											}
										}
									}break;
								}
								m_bModified[UNIT_INFANTRY] = true;
							}break;
						case UNIT_CAVALRY:
							{
								m_pUnit[UNIT_CAVALRY] = CGame::GetInstance()->GetUnitInfo(UNIT_CAVALRY);
								switch(i)
								{
								case 0:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= SPEED)
										{
											m_pUnit[UNIT_CAVALRY].SetSpeed(m_pUnit[UNIT_CAVALRY].GetSpeed() + (m_pUnit[UNIT_CAVALRY].GetSpeed() * .5f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= SPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_CAVALRY].SetSpeed(m_pUnit[UNIT_CAVALRY].GetSpeed() - (m_pUnit[UNIT_CAVALRY].GetSpeed() * .5f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += SPEED;
											}
										}
									}break;
								case 1:
									{
										if(!m_bUpgraded[j][i]&& m_nGoldTotal >= ATTACKSPEED)
										{
											m_pUnit[UNIT_CAVALRY].SetAttackSpeed(m_pUnit[UNIT_CAVALRY].GetAttackSpeed() - (m_pUnit[UNIT_CAVALRY].GetAttackSpeed() * .2f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACKSPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_CAVALRY].SetAttackSpeed(m_pUnit[UNIT_CAVALRY].GetAttackSpeed() + (m_pUnit[UNIT_CAVALRY].GetAttackSpeed() * .2f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal+=ATTACKSPEED;
											}
										}
									}break;
								case 2:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= ATTACK)
										{
											m_pUnit[UNIT_CAVALRY].SetAttackPower(m_pUnit[UNIT_CAVALRY].GetAttackPower() + 2);
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACK;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_CAVALRY].SetAttackPower(m_pUnit[UNIT_CAVALRY].GetAttackPower() - 2);
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += ATTACK;
											}
										}
									}break;
								}
								m_bModified[UNIT_CAVALRY] = true;
							}break;
						case UNIT_CAVALRY_ARCHER:
							{
								m_pUnit[UNIT_CAVALRY_ARCHER] = CGame::GetInstance()->GetUnitInfo(UNIT_CAVALRY_ARCHER);
								switch(i)
								{
								case 0:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= SPEED)
										{
											m_pUnit[UNIT_CAVALRY_ARCHER].SetSpeed(m_pUnit[UNIT_CAVALRY_ARCHER].GetSpeed() + (m_pUnit[UNIT_CAVALRY_ARCHER].GetSpeed() * .5f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -=SPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_CAVALRY_ARCHER].SetSpeed(m_pUnit[UNIT_CAVALRY_ARCHER].GetSpeed() - (m_pUnit[UNIT_CAVALRY_ARCHER].GetSpeed() * .5f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += SPEED;
											}
										}
									}break;
								case 1:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= ATTACKSPEED)
										{
											m_pUnit[UNIT_CAVALRY_ARCHER].SetAttackSpeed(m_pUnit[UNIT_CAVALRY_ARCHER].GetAttackSpeed() - (m_pUnit[UNIT_CAVALRY_ARCHER].GetAttackSpeed() * .2f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACKSPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_CAVALRY_ARCHER].SetAttackSpeed(m_pUnit[UNIT_CAVALRY_ARCHER].GetAttackSpeed() + (m_pUnit[UNIT_CAVALRY_ARCHER].GetAttackSpeed() * .2f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal +=ATTACKSPEED;
											}
										}
									}break;
								case 2:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= ATTACK)
										{
											m_pUnit[UNIT_CAVALRY_ARCHER].SetAttackPower(m_pUnit[UNIT_CAVALRY_ARCHER].GetAttackPower() + 2);
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACK;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_CAVALRY_ARCHER].SetAttackPower(m_pUnit[UNIT_CAVALRY_ARCHER].GetAttackPower() - 2);
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += ATTACK;
											}
										}
									}break;
								}
								m_bModified[UNIT_CAVALRY_ARCHER] = true;
							}break;
						case UNIT_AXMEN:
							{
								m_pUnit[UNIT_AXMEN] = CGame::GetInstance()->GetUnitInfo(UNIT_AXMEN);
								switch(i)
								{
								case 0:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal>=SPEED)
										{
											m_pUnit[UNIT_AXMEN].SetSpeed(m_pUnit[UNIT_AXMEN].GetSpeed() + (m_pUnit[UNIT_AXMEN].GetSpeed() * .5f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -=SPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_AXMEN].SetSpeed(m_pUnit->GetSpeed() - (m_pUnit[UNIT_AXMEN].GetSpeed() * .5f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += SPEED;
											}
										}
									}break;
								case 1:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >=ATTACKSPEED)
										{
											m_pUnit[UNIT_AXMEN].SetAttackSpeed(m_pUnit[UNIT_AXMEN].GetAttackSpeed() - (m_pUnit[UNIT_AXMEN].GetAttackSpeed() * .2f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACKSPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_AXMEN].SetAttackSpeed(m_pUnit[UNIT_AXMEN].GetAttackSpeed() + (m_pUnit[UNIT_AXMEN].GetAttackSpeed() * .2f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal+=ATTACKSPEED;
											}
										}
									}break;
								case 2:
									{
										if(!m_bUpgraded[j][i] &&m_nGoldTotal >= ATTACK)
										{
											m_pUnit[UNIT_AXMEN].SetAttackPower(m_pUnit[UNIT_AXMEN].GetAttackPower() + 2);
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACK;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_AXMEN].SetAttackPower(m_pUnit[UNIT_AXMEN].GetAttackPower() - 2);
												m_bUpgraded[j][i] = false;
												m_nGoldTotal+=ATTACK;
											}
										}
									}break;
								}
								m_bModified[UNIT_AXMEN] = true;
							}break;
						case UNIT_ARCHER:
							{
								m_pUnit[UNIT_ARCHER] = CGame::GetInstance()->GetUnitInfo(UNIT_ARCHER);
								switch(i)
								{
								case 0:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >=SPEED)
										{
											m_pUnit[UNIT_ARCHER].SetSpeed(m_pUnit[UNIT_ARCHER].GetSpeed() + (m_pUnit[UNIT_ARCHER].GetSpeed() * .5f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= SPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_ARCHER].SetSpeed(m_pUnit[UNIT_ARCHER].GetSpeed() - (m_pUnit[UNIT_ARCHER].GetSpeed() * .5f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += SPEED;
											}
										}
									}break;
								case 1:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal>=ATTACKSPEED)
										{
											m_pUnit[UNIT_ARCHER].SetAttackSpeed(m_pUnit[UNIT_ARCHER].GetAttackSpeed() - (m_pUnit[UNIT_ARCHER].GetAttackSpeed() * .2f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal-=ATTACKSPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_ARCHER].SetAttackSpeed(m_pUnit[UNIT_ARCHER].GetAttackSpeed() + (m_pUnit[UNIT_ARCHER].GetAttackSpeed() * .2f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal+=ATTACKSPEED;
											}
										}
									}break;
								case 2:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal>=ATTACK)
										{
											m_pUnit[UNIT_ARCHER].SetAttackPower(m_pUnit[UNIT_ARCHER].GetAttackPower() + 2);
											m_bUpgraded[j][i] = true;
											m_nGoldTotal-=ATTACK;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_ARCHER].SetAttackPower(m_pUnit[UNIT_ARCHER].GetAttackPower() - 2);
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += ATTACK;
											}
										}
									}break;
								}
								m_bModified[UNIT_ARCHER] = true;
							}break;
						case UNIT_WAR_ELEPHANT:
							{
								m_pUnit[UNIT_WAR_ELEPHANT] = CGame::GetInstance()->GetUnitInfo(UNIT_WAR_ELEPHANT);
								switch(i)
								{
								case 0:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal >= SPEED)
										{
											m_pUnit[UNIT_WAR_ELEPHANT].SetSpeed(m_pUnit[UNIT_WAR_ELEPHANT].GetSpeed() + (m_pUnit[UNIT_WAR_ELEPHANT].GetSpeed() * .5f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal-= SPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_WAR_ELEPHANT].SetSpeed(m_pUnit[UNIT_WAR_ELEPHANT].GetSpeed() - (m_pUnit[UNIT_WAR_ELEPHANT].GetSpeed() * .5f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal +=SPEED;
											}
										}
									}break;
								case 1:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal>=ATTACKSPEED)
										{
											m_pUnit[UNIT_WAR_ELEPHANT].SetAttackSpeed(m_pUnit[UNIT_WAR_ELEPHANT].GetAttackSpeed() - (m_pUnit[UNIT_WAR_ELEPHANT].GetAttackSpeed() * .2f));
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACKSPEED;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_WAR_ELEPHANT].SetAttackSpeed(m_pUnit[UNIT_WAR_ELEPHANT].GetAttackSpeed() + (m_pUnit[UNIT_WAR_ELEPHANT].GetAttackSpeed() * .2f));
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += ATTACKSPEED;
											}
										}
									}break;
								case 2:
									{
										if(!m_bUpgraded[j][i] && m_nGoldTotal>=ATTACK)
										{
											m_pUnit[UNIT_WAR_ELEPHANT].SetAttackPower(m_pUnit[UNIT_WAR_ELEPHANT].GetAttackPower() + 2);
											m_bUpgraded[j][i] = true;
											m_nGoldTotal -= ATTACK;
										}	
										else
										{
											if(!CGame::GetInstance()->GetUpgraded(j,i) && m_bUpgraded[j][i])
											{
												m_pUnit[UNIT_WAR_ELEPHANT].SetAttackPower(m_pUnit[UNIT_WAR_ELEPHANT].GetAttackPower() - 2);
												m_bUpgraded[j][i] = false;
												m_nGoldTotal += ATTACK;
											}
										}
									}break;
								}	
								m_bModified[UNIT_WAR_ELEPHANT] = true;
							}break;
						}	
					}
			}
		}
	}
	return true;
}

void CUpgradeMenuState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nBackgroundID, -20, -10);
	m_cFont.DrawTextA("Upgrades",225,0,.5,.5,D3DCOLOR_ARGB(255,255,0,0));

	for( int j = 0; j < 6; ++j)
	{
		for(int i = 0; i < 3; ++i)
		{
			m_pTM->Draw(m_nCheckBoxID,m_rCheckBox[j][i].left,m_rCheckBox[j][i].top);
			if(m_bUpgraded[j][i])
				m_pTM->Draw(m_nCheckMarkID,m_rCheckBox[j][i].left,m_rCheckBox[j][i].top);
		}

		m_cFont.DrawTextA("Speed:",185,100 + (j*64),.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));
		m_cFont.DrawTextA("Attack Speed:",330,100+j*64,.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));
		m_cFont.DrawTextA("Attack Power:",560,100+j*64,.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));

		switch(j)
		{	
		case 0:
			{
				m_cFont.DrawTextA("Infantry:",		20,100 + (j*64),.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case 1:
			{
				m_cFont.DrawTextA("Cavalry:",		20,100 + (j*64),.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case 2:
			{
				m_cFont.DrawTextA("Cavalry Archer:",20,100 + (j*64),.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case 3:
			{
				m_cFont.DrawTextA("Axmen:",			20,100 + (j*64),.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case 4:
			{
				m_cFont.DrawTextA("Archer:",		20,100 + (j*64),.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case 5:
			{
				m_cFont.DrawTextA("War Elephant:",	20,100 + (j*64),.15f,.15f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		}
	}	
	m_cFont.DrawTextA("Speed Cost: "+IntToString(SPEED),50,475,.15f,.15f,D3DCOLOR_ARGB(255,255,0,0));
	m_cFont.DrawTextA("Attack Speed Cost: "+IntToString(ATTACKSPEED),50,500,.15,.15,D3DCOLOR_ARGB(255,255,0,0));
	m_cFont.DrawTextA("Attack Power Cost: "+IntToString(ATTACK),50,525,.15,.15,D3DCOLOR_ARGB(255,255,0,0));


	m_cFont.DrawTextA("Gold: " + IntToString(m_nGoldTotal), 50, 550, .25f, .25f, D3DCOLOR_ARGB(255, 255, 255, 0));
	m_pTM->Draw(m_nScrollButtonID, m_rAttackButton.left, m_rAttackButton.top, .4f, .3f);
	m_cFont.DrawTextA("Accept", m_rAttackButton.left+30, m_rAttackButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

	m_pTM->Draw(m_nScrollButtonID, m_rBackButton.left, m_rBackButton.top, .4f, .3f);
	m_cFont.DrawTextA("Cancel", m_rBackButton.left+40, m_rBackButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

}

string CUpgradeMenuState::IntToString(int nNum)
{
	char szNumVal[10];
	itoa(nNum, szNumVal, 10);
	string szNum = szNumVal;
	return szNum;
}

string CUpgradeMenuState::FloatToString(float fNum)
{
	char szNumVal[10];
	sprintf_s(szNumVal, 10, "%.2f", fNum);
	string szNum = szNumVal;
	return szNum;
}

void CUpgradeMenuState::Update(float fElapsedTime)
{

}