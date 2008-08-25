#include "AnimationManager.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// File: “AnimationManager.cpp” 
// Author: Dennis Wallace II (DWII) 
// Purpose: This is the file containing the information for the	Animation Manager class.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
#include "CSGD_TextureManager.h"
#include <fstream>
#include <windows.h>
#include "irrXML.h"

using namespace irr;
using namespace io;
CAnimationManager::CAnimationManager(void)
{

}

CAnimationManager::~CAnimationManager(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “BinParse” 
// Last Modified: July 25, 2008
// Purpose: Parse a binary file.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimationManager::BinParse(char* szFileName, char* szPlayerImageName, char* szAIImageName)
{
	PROFILE("CAnimationManager::BinParse(char*, char*, char*)");
	ifstream toRead;
	toRead.open(szFileName,ios::in | ios::binary);

	if(toRead.is_open())
	{
		CSGD_TextureManager* TM = CSGD_TextureManager::GetInstance();

		short nFrameCount;
		short nAccessories;
		short nAnimationCount;

		int nTriggerNameLength;
		int nTriggerTypeLength;
		int nAnimationNameLength;
		int nUnitTypeLength;
		int nRenderX,nRenderY,nRenderWidth,nRenderHeight;
		int nCollX,nCollY,nCollWidth,nCollHeight;
		int nPassX,nPassY,nPassWidth,nPassHeight;
		int nAptX,nAptY;
		int nAccessoryX,nAccessoryY;

		double fDuration;
		double fSpeed;
		
		//find the length of the unit type string
		toRead.read((char*)&nUnitTypeLength,sizeof(nUnitTypeLength));
		char szUnitType[20];
		//read in the unit type
		toRead.read(szUnitType,nUnitTypeLength);
		szUnitType[nUnitTypeLength] = 0;

#pragma region Infantry
		if(!strncmp(szUnitType,"Infantry",nUnitTypeLength))
		{
			//set the unit type
			m_UnitSheets[UNIT_INFANTRY].SetUnitType(UNIT_INFANTRY);
		
			//find the number of animations in the sheet
			toRead.read((char*)&nAnimationCount,sizeof(nAnimationCount));
			for(int i = 0; i < nAnimationCount; i++)
			{
				//make an animation to add
				CAnimation AnimationToAdd;
				//read in the length of the animation name
				toRead.read((char*)&nAnimationNameLength,sizeof(nAnimationNameLength));
				char szAnimationName[128];
				//read in the animation name
				toRead.read((char*)&szAnimationName,nAnimationNameLength);
				szAnimationName[nAnimationNameLength] = 0;

				//set the animations name
				AnimationToAdd.SetName(szAnimationName);
				//read in the animations speed
				toRead.read((char*)&fSpeed,sizeof(fSpeed));
				//set the speed of the animation
				AnimationToAdd.SetSpeed((float)fSpeed);
				//find the number of frames in the animation
				toRead.read((char*)&nFrameCount,sizeof(nFrameCount));
				for(int j = 0; j < nFrameCount; j++)
				{
					Frame FrameToAdd;
#pragma region RenderRect
					toRead.read((char*)&nRenderX,sizeof(nRenderX));
					FrameToAdd.rRender.left = nRenderX;
					toRead.read((char*)&nRenderY,sizeof(nRenderY));
					FrameToAdd.rRender.top = nRenderY;
					toRead.read((char*)&nRenderWidth,sizeof(nRenderWidth));
					FrameToAdd.rRender.right = nRenderWidth + FrameToAdd.rRender.left;
					toRead.read((char*)&nRenderHeight,sizeof(nRenderHeight));
					FrameToAdd.rRender.bottom = nRenderHeight + FrameToAdd.rRender.top;
#pragma endregion
#pragma region ActiveRect
					toRead.read((char*)&nCollX,sizeof(nCollX));
					FrameToAdd.rActive.left = nCollX;
					toRead.read((char*)&nCollY,sizeof(nCollY));
					FrameToAdd.rActive.top = nCollY;
					toRead.read((char*)&nCollWidth,sizeof(nCollWidth));
					FrameToAdd.rActive.right = nCollWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nCollHeight,sizeof(nCollHeight));
					FrameToAdd.rActive.bottom = nCollHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region PassiveRect
					toRead.read((char*)&nPassX,sizeof(nPassX));
					FrameToAdd.rActive.left = nPassX;
					toRead.read((char*)&nPassY,sizeof(nPassY));
					FrameToAdd.rActive.top = nPassY;
					toRead.read((char*)&nPassWidth,sizeof(nPassWidth));
					FrameToAdd.rActive.right = nPassWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nPassHeight,sizeof(nPassHeight));
					FrameToAdd.rActive.bottom = nPassHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region Anchor Point
					toRead.read((char*)&nAptX,sizeof(nAptX));
					FrameToAdd.ptAnchorX = nAptX;
					toRead.read((char*)&nAptY,sizeof(nAptY));
					FrameToAdd.ptAnchorY = nAptY;
#pragma endregion

					//read in the duration of the current frame
					toRead.read((char*)&fDuration,sizeof(fDuration));
					//set the duration for the current frame
					FrameToAdd.fDuration = (float)fDuration;

#pragma region Trigger Information
					//read in the length of the trigger name
					toRead.read((char*)&nTriggerNameLength,sizeof(nTriggerNameLength));
					char szTriggerName[128];
					//read in the trigger name
					toRead.read((char*)&szTriggerName,nTriggerNameLength);
					szTriggerName[nTriggerNameLength] = 0;
					FrameToAdd.szTriggerName = szTriggerName;

					//read in the length of the trigger type
					toRead.read((char*)&nTriggerTypeLength,sizeof(nTriggerTypeLength));
					char szTriggerType[128];
					//read in the trigger type
					toRead.read((char*)&szTriggerType,nTriggerTypeLength);
					szTriggerType[nTriggerTypeLength] = 0;					
					FrameToAdd.szTriggerType = szTriggerType;
					
#pragma endregion

					toRead.read((char*)&nAccessories,sizeof(nAccessories));
					for(int k = 0; k < nAccessories; k++)
					{
						POINT AccessoryToAdd;
						toRead.read((char*)&nAccessoryX,sizeof(nAccessoryX));
						AccessoryToAdd.x = nAccessoryX;
						toRead.read((char*)&nAccessoryY,sizeof(nAccessoryY));
						AccessoryToAdd.y = nAccessoryY;
						FrameToAdd.ptAccessories.push_back(AccessoryToAdd);
					}
					AnimationToAdd.AddFrame(FrameToAdd);
				}
				m_UnitSheets[UNIT_INFANTRY].AddAnimation(AnimationToAdd);
				m_UnitSheets[UNIT_INFANTRY].SetPlayerImageID(TM->LoadTexture(szPlayerImageName));
				m_UnitSheets[UNIT_INFANTRY].SetAIImageID(TM->LoadTexture(szAIImageName));
			}
		}
#pragma endregion

#pragma region Cavalry
		if(!strncmp(szUnitType,"Calvary",nUnitTypeLength))
		{
			//set the unit type
			m_UnitSheets[UNIT_CAVALRY].SetUnitType(UNIT_CAVALRY);

			//find the number of animations in the sheet
			toRead.read((char*)&nAnimationCount,sizeof(nAnimationCount));
			for(int i = 0; i < nAnimationCount; i++)
			{
				//make an animation to add
				CAnimation AnimationToAdd;
				//read in the length of the animation name
				toRead.read((char*)&nAnimationNameLength,sizeof(nAnimationNameLength));
				char szAnimationName[128];
				//read in the animation name
				toRead.read((char*)&szAnimationName,nAnimationNameLength);
				szAnimationName[nAnimationNameLength] = 0;

				//set the animations name
				AnimationToAdd.SetName(szAnimationName);
				//read in the animations speed
				toRead.read((char*)&fSpeed,sizeof(fSpeed));
				//set the speed of the animation
				AnimationToAdd.SetSpeed((float)fSpeed);
				//find the number of frames in the animation
				toRead.read((char*)&nFrameCount,sizeof(nFrameCount));
				for(int j = 0; j < nFrameCount; j++)
				{
					Frame FrameToAdd;

#pragma region RenderRect
					toRead.read((char*)&nRenderX,sizeof(nRenderX));
					FrameToAdd.rRender.left = nRenderX;
					toRead.read((char*)&nRenderY,sizeof(nRenderY));
					FrameToAdd.rRender.top = nRenderY;
					toRead.read((char*)&nRenderWidth,sizeof(nRenderWidth));
					FrameToAdd.rRender.right = nRenderWidth + FrameToAdd.rRender.left;
					toRead.read((char*)&nRenderHeight,sizeof(nRenderHeight));
					FrameToAdd.rRender.bottom = nRenderHeight + FrameToAdd.rRender.top;
#pragma endregion
#pragma region ActiveRect
					toRead.read((char*)&nCollX,sizeof(nCollX));
					FrameToAdd.rActive.left = nCollX;
					toRead.read((char*)&nCollY,sizeof(nCollY));
					FrameToAdd.rActive.top = nCollY;
					toRead.read((char*)&nCollWidth,sizeof(nCollWidth));
					FrameToAdd.rActive.right = nCollWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nCollHeight,sizeof(nCollHeight));
					FrameToAdd.rActive.bottom = nCollHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region PassiveRect
					toRead.read((char*)&nPassX,sizeof(nPassX));
					FrameToAdd.rActive.left = nPassX;
					toRead.read((char*)&nPassY,sizeof(nPassY));
					FrameToAdd.rActive.top = nPassY;
					toRead.read((char*)&nPassWidth,sizeof(nPassWidth));
					FrameToAdd.rActive.right = nPassWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nPassHeight,sizeof(nPassHeight));
					FrameToAdd.rActive.bottom = nPassHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region Anchor Point
					toRead.read((char*)&nAptX,sizeof(nAptX));
					FrameToAdd.ptAnchorX = nAptX;
					toRead.read((char*)&nAptY,sizeof(nAptY));
					FrameToAdd.ptAnchorY = nAptY;
#pragma endregion

					//read in the duration of the current frame
					toRead.read((char*)&fDuration,sizeof(fDuration));
					//set the duration for the current frame
					FrameToAdd.fDuration = (float)fDuration;

#pragma region Trigger Information
					//read in the length of the trigger name
					toRead.read((char*)&nTriggerNameLength,sizeof(nTriggerNameLength));
					char szTriggerName[128];
					//read in the trigger name
					toRead.read((char*)&szTriggerName,nTriggerNameLength);
					szTriggerName[nTriggerNameLength] = 0;
					FrameToAdd.szTriggerName = szTriggerName;

					//read in the length of the trigger type
					toRead.read((char*)&nTriggerTypeLength,sizeof(nTriggerTypeLength));
					char szTriggerType[128];
					//read in the trigger type
					toRead.read((char*)&szTriggerType,nTriggerTypeLength);
					szTriggerType[nTriggerTypeLength] = 0;					
					FrameToAdd.szTriggerType = szTriggerType;
					
#pragma endregion

					toRead.read((char*)&nAccessories,sizeof(nAccessories));
					for(int k = 0; k < nAccessories; k++)
					{
						POINT AccessoryToAdd;
						toRead.read((char*)&nAccessoryX,sizeof(nAccessoryX));
						AccessoryToAdd.x = nAccessoryX;
						toRead.read((char*)&nAccessoryY,sizeof(nAccessoryY));
						AccessoryToAdd.y = nAccessoryY;
						FrameToAdd.ptAccessories.push_back(AccessoryToAdd);
					}
					AnimationToAdd.AddFrame(FrameToAdd);
				}
				m_UnitSheets[UNIT_CAVALRY].AddAnimation(AnimationToAdd);
				m_UnitSheets[UNIT_CAVALRY].SetPlayerImageID(TM->LoadTexture(szPlayerImageName));
				m_UnitSheets[UNIT_CAVALRY].SetAIImageID(TM->LoadTexture(szAIImageName));
			}
		}
#pragma endregion

#pragma region Archer
		if(!strncmp(szUnitType,"Archer",nUnitTypeLength))
		{
			//set the unit type
			m_UnitSheets[UNIT_ARCHER].SetUnitType(UNIT_ARCHER);

			//find the number of animations in the sheet
			toRead.read((char*)&nAnimationCount,sizeof(nAnimationCount));
			for(int i = 0; i < nAnimationCount; i++)
			{
				//make an animation to add
				CAnimation AnimationToAdd;
				//read in the length of the animation name
				toRead.read((char*)&nAnimationNameLength,sizeof(nAnimationNameLength));
				char szAnimationName[128];
				//read in the animation name
				toRead.read((char*)&szAnimationName,nAnimationNameLength);
				szAnimationName[nAnimationNameLength] = 0;

				//set the animations name
				AnimationToAdd.SetName(szAnimationName);
				//read in the animations speed
				toRead.read((char*)&fSpeed,sizeof(fSpeed));
				//set the speed of the animation
				AnimationToAdd.SetSpeed((float)fSpeed);
				//find the number of frames in the animation
				toRead.read((char*)&nFrameCount,sizeof(nFrameCount));
				for(int j = 0; j < nFrameCount; j++)
				{
					Frame FrameToAdd;

#pragma region RenderRect
					toRead.read((char*)&nRenderX,sizeof(nRenderX));
					FrameToAdd.rRender.left = nRenderX;
					toRead.read((char*)&nRenderY,sizeof(nRenderY));
					FrameToAdd.rRender.top = nRenderY;
					toRead.read((char*)&nRenderWidth,sizeof(nRenderWidth));
					FrameToAdd.rRender.right = nRenderWidth + FrameToAdd.rRender.left;
					toRead.read((char*)&nRenderHeight,sizeof(nRenderHeight));
					FrameToAdd.rRender.bottom = nRenderHeight + FrameToAdd.rRender.top;
#pragma endregion
#pragma region ActiveRect
					toRead.read((char*)&nCollX,sizeof(nCollX));
					FrameToAdd.rActive.left = nCollX;
					toRead.read((char*)&nCollY,sizeof(nCollY));
					FrameToAdd.rActive.top = nCollY;
					toRead.read((char*)&nCollWidth,sizeof(nCollWidth));
					FrameToAdd.rActive.right = nCollWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nCollHeight,sizeof(nCollHeight));
					FrameToAdd.rActive.bottom = nCollHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region PassiveRect
					toRead.read((char*)&nPassX,sizeof(nPassX));
					FrameToAdd.rActive.left = nPassX;
					toRead.read((char*)&nPassY,sizeof(nPassY));
					FrameToAdd.rActive.top = nPassY;
					toRead.read((char*)&nPassWidth,sizeof(nPassWidth));
					FrameToAdd.rActive.right = nPassWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nPassHeight,sizeof(nPassHeight));
					FrameToAdd.rActive.bottom = nPassHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region Anchor Point
					toRead.read((char*)&nAptX,sizeof(nAptX));
					FrameToAdd.ptAnchorX = nAptX;
					toRead.read((char*)&nAptY,sizeof(nAptY));
					FrameToAdd.ptAnchorY = nAptY;
#pragma endregion

					//read in the duration of the current frame
					toRead.read((char*)&fDuration,sizeof(fDuration));
					//set the duration for the current frame
					FrameToAdd.fDuration = (float)fDuration;

#pragma region Trigger Information
					//read in the length of the trigger name
					toRead.read((char*)&nTriggerNameLength,sizeof(nTriggerNameLength));
					char szTriggerName[128];
					//read in the trigger name
					toRead.read((char*)&szTriggerName,nTriggerNameLength);
					szTriggerName[nTriggerNameLength] = 0;
					FrameToAdd.szTriggerName = szTriggerName;

					//read in the length of the trigger type
					toRead.read((char*)&nTriggerTypeLength,sizeof(nTriggerTypeLength));
					char szTriggerType[128];
					//read in the trigger type
					toRead.read((char*)&szTriggerType,nTriggerTypeLength);
					szTriggerType[nTriggerTypeLength] = 0;					
					FrameToAdd.szTriggerType = szTriggerType;
					
#pragma endregion

					toRead.read((char*)&nAccessories,sizeof(nAccessories));
					for(int k = 0; k < nAccessories; k++)
					{
						POINT AccessoryToAdd;
						toRead.read((char*)&nAccessoryX,sizeof(nAccessoryX));
						AccessoryToAdd.x = nAccessoryX;
						toRead.read((char*)&nAccessoryY,sizeof(nAccessoryY));
						AccessoryToAdd.y = nAccessoryY;
						FrameToAdd.ptAccessories.push_back(AccessoryToAdd);
					}
					AnimationToAdd.AddFrame(FrameToAdd);
				}
				m_UnitSheets[UNIT_ARCHER].AddAnimation(AnimationToAdd);
				m_UnitSheets[UNIT_ARCHER].SetPlayerImageID(TM->LoadTexture(szPlayerImageName));
				m_UnitSheets[UNIT_ARCHER].SetAIImageID(TM->LoadTexture(szAIImageName));
			}
		}
#pragma endregion

#pragma region Axmen
		if(!strncmp(szUnitType,"Axmen",nUnitTypeLength))
		{
			//set the unit type
			m_UnitSheets[UNIT_AXMEN].SetUnitType(UNIT_AXMEN);

			//find the number of animations in the sheet
			toRead.read((char*)&nAnimationCount,sizeof(nAnimationCount));
			for(int i = 0; i < nAnimationCount; i++)
			{
				//make an animation to add
				CAnimation AnimationToAdd;
				//read in the length of the animation name
				toRead.read((char*)&nAnimationNameLength,sizeof(nAnimationNameLength));
				char szAnimationName[128];
				//read in the animation name
				toRead.read((char*)&szAnimationName,nAnimationNameLength);
				szAnimationName[nAnimationNameLength] = 0;

				//set the animations name
				AnimationToAdd.SetName(szAnimationName);
				//read in the animations speed
				toRead.read((char*)&fSpeed,sizeof(fSpeed));
				//set the speed of the animation
				AnimationToAdd.SetSpeed((float)fSpeed);
				//find the number of frames in the animation
				toRead.read((char*)&nFrameCount,sizeof(nFrameCount));
				for(int j = 0; j < nFrameCount; j++)
				{
					Frame FrameToAdd;

#pragma region RenderRect
					toRead.read((char*)&nRenderX,sizeof(nRenderX));
					FrameToAdd.rRender.left = nRenderX;
					toRead.read((char*)&nRenderY,sizeof(nRenderY));
					FrameToAdd.rRender.top = nRenderY;
					toRead.read((char*)&nRenderWidth,sizeof(nRenderWidth));
					FrameToAdd.rRender.right = nRenderWidth + FrameToAdd.rRender.left;
					toRead.read((char*)&nRenderHeight,sizeof(nRenderHeight));
					FrameToAdd.rRender.bottom = nRenderHeight + FrameToAdd.rRender.top;
#pragma endregion
#pragma region ActiveRect
					toRead.read((char*)&nCollX,sizeof(nCollX));
					FrameToAdd.rActive.left = nCollX;
					toRead.read((char*)&nCollY,sizeof(nCollY));
					FrameToAdd.rActive.top = nCollY;
					toRead.read((char*)&nCollWidth,sizeof(nCollWidth));
					FrameToAdd.rActive.right = nCollWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nCollHeight,sizeof(nCollHeight));
					FrameToAdd.rActive.bottom = nCollHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region PassiveRect
					toRead.read((char*)&nPassX,sizeof(nPassX));
					FrameToAdd.rActive.left = nPassX;
					toRead.read((char*)&nPassY,sizeof(nPassY));
					FrameToAdd.rActive.top = nPassY;
					toRead.read((char*)&nPassWidth,sizeof(nPassWidth));
					FrameToAdd.rActive.right = nPassWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nPassHeight,sizeof(nPassHeight));
					FrameToAdd.rActive.bottom = nPassHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region Anchor Point
					toRead.read((char*)&nAptX,sizeof(nAptX));
					FrameToAdd.ptAnchorX = nAptX;
					toRead.read((char*)&nAptY,sizeof(nAptY));
					FrameToAdd.ptAnchorY = nAptY;
#pragma endregion

					//read in the duration of the current frame
					toRead.read((char*)&fDuration,sizeof(fDuration));
					//set the duration for the current frame
					FrameToAdd.fDuration = (float)fDuration;

#pragma region Trigger Information
					//read in the length of the trigger name
					toRead.read((char*)&nTriggerNameLength,sizeof(nTriggerNameLength));
					char szTriggerName[128];
					//read in the trigger name
					toRead.read((char*)&szTriggerName,nTriggerNameLength);
					szTriggerName[nTriggerNameLength] = 0;
					FrameToAdd.szTriggerName = szTriggerName;

					//read in the length of the trigger type
					toRead.read((char*)&nTriggerTypeLength,sizeof(nTriggerTypeLength));
					char szTriggerType[128];
					//read in the trigger type
					toRead.read((char*)&szTriggerType,nTriggerTypeLength);
					szTriggerType[nTriggerTypeLength] = 0;					
					FrameToAdd.szTriggerType = szTriggerType;
					
#pragma endregion

					toRead.read((char*)&nAccessories,sizeof(nAccessories));
					for(int k = 0; k < nAccessories; k++)
					{
						POINT AccessoryToAdd;
						toRead.read((char*)&nAccessoryX,sizeof(nAccessoryX));
						AccessoryToAdd.x = nAccessoryX;
						toRead.read((char*)&nAccessoryY,sizeof(nAccessoryY));
						AccessoryToAdd.y = nAccessoryY;
						FrameToAdd.ptAccessories.push_back(AccessoryToAdd);
					}
					AnimationToAdd.AddFrame(FrameToAdd);
				}
				m_UnitSheets[UNIT_AXMEN].AddAnimation(AnimationToAdd);
				m_UnitSheets[UNIT_AXMEN].SetPlayerImageID(TM->LoadTexture(szPlayerImageName));
				m_UnitSheets[UNIT_AXMEN].SetAIImageID(TM->LoadTexture(szAIImageName));
			}
		}
#pragma endregion

#pragma region Cavalry_Archer
		if(!strncmp(szUnitType,"Cavalry_Archer",nUnitTypeLength))
		{
			//set the unit type
			m_UnitSheets[UNIT_CAVALRY_ARCHER].SetUnitType(UNIT_CAVALRY_ARCHER);

			//find the number of animations in the sheet
			toRead.read((char*)&nAnimationCount,sizeof(nAnimationCount));
			for(int i = 0; i < nAnimationCount; i++)
			{
				//make an animation to add
				CAnimation AnimationToAdd;
				//read in the length of the animation name
				toRead.read((char*)&nAnimationNameLength,sizeof(nAnimationNameLength));
				char szAnimationName[128];
				//read in the animation name
				toRead.read((char*)&szAnimationName,nAnimationNameLength);
				szAnimationName[nAnimationNameLength] = 0;

				//set the animations name
				AnimationToAdd.SetName(szAnimationName);
				//read in the animations speed
				toRead.read((char*)&fSpeed,sizeof(fSpeed));
				//set the speed of the animation
				AnimationToAdd.SetSpeed((float)fSpeed);
				//find the number of frames in the animation
				toRead.read((char*)&nFrameCount,sizeof(nFrameCount));
				for(int j = 0; j < nFrameCount; j++)
				{
					Frame FrameToAdd;

#pragma region RenderRect
					toRead.read((char*)&nRenderX,sizeof(nRenderX));
					FrameToAdd.rRender.left = nRenderX;
					toRead.read((char*)&nRenderY,sizeof(nRenderY));
					FrameToAdd.rRender.top = nRenderY;
					toRead.read((char*)&nRenderWidth,sizeof(nRenderWidth));
					FrameToAdd.rRender.right = nRenderWidth + FrameToAdd.rRender.left;
					toRead.read((char*)&nRenderHeight,sizeof(nRenderHeight));
					FrameToAdd.rRender.bottom = nRenderHeight + FrameToAdd.rRender.top;
#pragma endregion
#pragma region ActiveRect
					toRead.read((char*)&nCollX,sizeof(nCollX));
					FrameToAdd.rActive.left = nCollX;
					toRead.read((char*)&nCollY,sizeof(nCollY));
					FrameToAdd.rActive.top = nCollY;
					toRead.read((char*)&nCollWidth,sizeof(nCollWidth));
					FrameToAdd.rActive.right = nCollWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nCollHeight,sizeof(nCollHeight));
					FrameToAdd.rActive.bottom = nCollHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region PassiveRect
					toRead.read((char*)&nPassX,sizeof(nPassX));
					FrameToAdd.rActive.left = nPassX;
					toRead.read((char*)&nPassY,sizeof(nPassY));
					FrameToAdd.rActive.top = nPassY;
					toRead.read((char*)&nPassWidth,sizeof(nPassWidth));
					FrameToAdd.rActive.right = nPassWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nPassHeight,sizeof(nPassHeight));
					FrameToAdd.rActive.bottom = nPassHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region Anchor Point
					toRead.read((char*)&nAptX,sizeof(nAptX));
					FrameToAdd.ptAnchorX = nAptX;
					toRead.read((char*)&nAptY,sizeof(nAptY));
					FrameToAdd.ptAnchorY = nAptY;
#pragma endregion

					//read in the duration of the current frame
					toRead.read((char*)&fDuration,sizeof(fDuration));
					//set the duration for the current frame
					FrameToAdd.fDuration = (float)fDuration;

#pragma region Trigger Information
					//read in the length of the trigger name
					toRead.read((char*)&nTriggerNameLength,sizeof(nTriggerNameLength));
					char szTriggerName[128];
					//read in the trigger name
					toRead.read((char*)&szTriggerName,nTriggerNameLength);
					szTriggerName[nTriggerNameLength] = 0;
					FrameToAdd.szTriggerName = szTriggerName;

					//read in the length of the trigger type
					toRead.read((char*)&nTriggerTypeLength,sizeof(nTriggerTypeLength));
					char szTriggerType[128];
					//read in the trigger type
					toRead.read((char*)&szTriggerType,nTriggerTypeLength);
					szTriggerType[nTriggerTypeLength] = 0;					
					FrameToAdd.szTriggerType = szTriggerType;
					
#pragma endregion

					toRead.read((char*)&nAccessories,sizeof(nAccessories));
					for(int k = 0; k < nAccessories; k++)
					{
						POINT AccessoryToAdd;
						toRead.read((char*)&nAccessoryX,sizeof(nAccessoryX));
						AccessoryToAdd.x = nAccessoryX;
						toRead.read((char*)&nAccessoryY,sizeof(nAccessoryY));
						AccessoryToAdd.y = nAccessoryY;
						FrameToAdd.ptAccessories.push_back(AccessoryToAdd);
					}
					AnimationToAdd.AddFrame(FrameToAdd);
				}
				m_UnitSheets[UNIT_CAVALRY_ARCHER].AddAnimation(AnimationToAdd);
				m_UnitSheets[UNIT_CAVALRY_ARCHER].SetPlayerImageID(TM->LoadTexture(szPlayerImageName));
				m_UnitSheets[UNIT_CAVALRY_ARCHER].SetAIImageID(TM->LoadTexture(szAIImageName));
			}
		}
#pragma endregion

#pragma region War_Elephant
		if(!strncmp(szUnitType,"War_Elephant",nUnitTypeLength))
		{
			//set the unit type
			m_UnitSheets[UNIT_WAR_ELEPHANT].SetUnitType(UNIT_WAR_ELEPHANT);

			//find the number of animations in the sheet
			toRead.read((char*)&nAnimationCount,sizeof(nAnimationCount));
			for(int i = 0; i < nAnimationCount; i++)
			{
				//make an animation to add
				CAnimation AnimationToAdd;
				//read in the length of the animation name
				toRead.read((char*)&nAnimationNameLength,sizeof(nAnimationNameLength));
				char szAnimationName[128];
				//read in the animation name
				toRead.read((char*)&szAnimationName,nAnimationNameLength);
				szAnimationName[nAnimationNameLength] = 0;

				//set the animations name
				AnimationToAdd.SetName(szAnimationName);
				//read in the animations speed
				toRead.read((char*)&fSpeed,sizeof(fSpeed));
				//set the speed of the animation
				AnimationToAdd.SetSpeed((float)fSpeed);
				//find the number of frames in the animation
				toRead.read((char*)&nFrameCount,sizeof(nFrameCount));
				for(int j = 0; j < nFrameCount; j++)
				{
					Frame FrameToAdd;

#pragma region RenderRect
					toRead.read((char*)&nRenderX,sizeof(nRenderX));
					FrameToAdd.rRender.left = nRenderX;
					toRead.read((char*)&nRenderY,sizeof(nRenderY));
					FrameToAdd.rRender.top = nRenderY;
					toRead.read((char*)&nRenderWidth,sizeof(nRenderWidth));
					FrameToAdd.rRender.right = nRenderWidth + FrameToAdd.rRender.left;
					toRead.read((char*)&nRenderHeight,sizeof(nRenderHeight));
					FrameToAdd.rRender.bottom = nRenderHeight + FrameToAdd.rRender.top;
#pragma endregion
#pragma region ActiveRect
					toRead.read((char*)&nCollX,sizeof(nCollX));
					FrameToAdd.rActive.left = nCollX;
					toRead.read((char*)&nCollY,sizeof(nCollY));
					FrameToAdd.rActive.top = nCollY;
					toRead.read((char*)&nCollWidth,sizeof(nCollWidth));
					FrameToAdd.rActive.right = nCollWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nCollHeight,sizeof(nCollHeight));
					FrameToAdd.rActive.bottom = nCollHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region PassiveRect
					toRead.read((char*)&nPassX,sizeof(nPassX));
					FrameToAdd.rActive.left = nPassX;
					toRead.read((char*)&nPassY,sizeof(nPassY));
					FrameToAdd.rActive.top = nPassY;
					toRead.read((char*)&nPassWidth,sizeof(nPassWidth));
					FrameToAdd.rActive.right = nPassWidth + FrameToAdd.rActive.left;
					toRead.read((char*)&nPassHeight,sizeof(nPassHeight));
					FrameToAdd.rActive.bottom = nPassHeight + FrameToAdd.rActive.top;
#pragma endregion
#pragma region Anchor Point
					toRead.read((char*)&nAptX,sizeof(nAptX));
					FrameToAdd.ptAnchorX = nAptX;
					toRead.read((char*)&nAptY,sizeof(nAptY));
					FrameToAdd.ptAnchorY = nAptY;
#pragma endregion

					//read in the duration of the current frame
					toRead.read((char*)&fDuration,sizeof(fDuration));
					//set the duration for the current frame
					FrameToAdd.fDuration = (float)fDuration;

#pragma region Trigger Information
					//read in the length of the trigger name
					toRead.read((char*)&nTriggerNameLength,sizeof(nTriggerNameLength));
					char szTriggerName[128];
					//read in the trigger name
					toRead.read((char*)&szTriggerName,nTriggerNameLength);
					szTriggerName[nTriggerNameLength] = 0;
					FrameToAdd.szTriggerName = szTriggerName;

					//read in the length of the trigger type
					toRead.read((char*)&nTriggerTypeLength,sizeof(nTriggerTypeLength));
					char szTriggerType[128];
					//read in the trigger type
					toRead.read((char*)&szTriggerType,nTriggerTypeLength);
					szTriggerType[nTriggerTypeLength] = 0;					
					FrameToAdd.szTriggerType = szTriggerType;
					
#pragma endregion

					toRead.read((char*)&nAccessories,sizeof(nAccessories));
					for(int k = 0; k < nAccessories; k++)
					{
						POINT AccessoryToAdd;
						toRead.read((char*)&nAccessoryX,sizeof(nAccessoryX));
						AccessoryToAdd.x = nAccessoryX;
						toRead.read((char*)&nAccessoryY,sizeof(nAccessoryY));
						AccessoryToAdd.y = nAccessoryY;
						FrameToAdd.ptAccessories.push_back(AccessoryToAdd);
					}
					AnimationToAdd.AddFrame(FrameToAdd);
				}
				m_UnitSheets[UNIT_WAR_ELEPHANT].AddAnimation(AnimationToAdd);
				m_UnitSheets[UNIT_WAR_ELEPHANT].SetPlayerImageID(TM->LoadTexture(szPlayerImageName));
				m_UnitSheets[UNIT_WAR_ELEPHANT].SetAIImageID(TM->LoadTexture(szAIImageName));
			}
		}
#pragma endregion
	}
	STOP("CAnimationManager::BinParse(char*, char*, char*)");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “LookUp” 
// Last Modified: July 25, 2008
// Purpose: Looks up the specified sheet using the unit type.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
CSheet CAnimationManager::LookUp(int UnitType)
{
	PROFILE("CAnimationManager::LookUp(int)");
	for(int i = 0; i < 6; i++)
	{
		if(UnitType == m_UnitSheets[i].GetUnitType())
			return m_UnitSheets[i];
	}
	STOP("CAnimationManager::LookUp(int)");
	return CSheet();
}

void CAnimationManager::ReleaseImages()
{
	CSGD_TextureManager* m_pTM = CSGD_TextureManager::GetInstance();

	for(int i = 0; i < 6; ++i)
	{
		for(unsigned int j = 0; j < m_UnitSheets[i].GetAIImageID().size(); ++j)
		{
			m_pTM->ReleaseTexture(m_UnitSheets[i].GetAIImageID()[j]);
		}
		for(unsigned int j = 0; j < m_UnitSheets[i].GetPlayerImageID().size(); ++j)
		{
			m_pTM->ReleaseTexture(m_UnitSheets[i].GetPlayerImageID()[j]);
		}
	}
}
