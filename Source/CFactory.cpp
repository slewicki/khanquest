//////////////////////////////////////////////////////////
//	File:	"CFactory.h"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To create all of our game objects
//////////////////////////////////////////////////////////

#include "CFactory.h"
#include "CGame.h"


CFactory::CFactory(void)
{
}
CFactory::~CFactory(void)
{
}

void CFactory::CreatePlayerUnit(int nType)
{
	static float xPos = 100;
	static float yPos = 50;
	CUnit* unit = new CUnit(nType);

	// Use default shallow copy since no dynamic info in creation
	CUnit temp = CGame::GetInstance()->GetPlayerUnitInfo(nType);
	unit->SetAttackPower(temp.GetAttackPower());
	unit->SetAttackSpeed(temp.GetAttackSpeed());
	unit->SetHP(temp.GetHP());
	unit->SetRange(temp.GetRange());
	unit->SetSpeed(temp.GetSpeed());
	// For now give it a default position
	unit->SetPosX(xPos);
	unit->SetPosY(yPos);
	// Row by Row
	if(xPos >= 700.f)
	{
		xPos = 60.f;
		yPos += 60.f;
	}
	xPos+=100.f;
	if(yPos >= 500)
	{
		yPos = 50;
		xPos = 0;
	}
	
	
	//unit->SetDirection(SOUTH_WEST);
	//unit->SetState(MOVEMENT);
	// Register Events


	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();

}

void CFactory::CreateComputerUnit(int nType)
{
	CUnit* unit = new CUnit(nType);
	*(unit) = CGame::GetInstance()->GetCPUUnitInfo(nType);
	
	// Register Events
	
	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();

}