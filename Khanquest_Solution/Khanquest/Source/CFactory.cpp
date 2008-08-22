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
	//static float xPos = 100;
	//static float yPos = 50;
	CUnit* unit = new CUnit(nType);

	// Use default shallow copy since no dynamic info in creation
	CUnit temp = CGame::GetInstance()->GetUnitInfo(nType);
	unit->SetAttackPower(temp.GetAttackPower());
	unit->SetAttackSpeed(temp.GetAttackSpeed());
	unit->SetMaxHP(temp.GetMaxHP());
	unit->SetCurrentHP(temp.GetMaxHP());
	unit->SetRange(temp.GetRange());
	unit->SetSpeed(temp.GetSpeed());
		
	unit->SetState(IDLE);
	unit->SetDirection(NORTH);
	unit->SetIsPlayerUnit(true);
	// Register Events


	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();

}

void CFactory::CreateComputerUnit(int nType)
{
	//static float xPos = 100;
	//static float yPos = 50;
	CUnit* unit = new CUnit(nType);

	// Use default shallow copy since no dynamic info in creation
	CUnit temp = CGame::GetInstance()->GetUnitInfo(nType);
	unit->SetAttackPower(temp.GetAttackPower());
	unit->SetAttackSpeed(temp.GetAttackSpeed());
	unit->SetMaxHP(temp.GetMaxHP());
	unit->SetCurrentHP(temp.GetMaxHP());
	unit->SetRange(temp.GetRange());
	unit->SetSpeed(temp.GetSpeed());

	unit->SetState(IDLE);
	unit->SetDirection(NORTH);
	unit->SetIsPlayerUnit(false);
	// Register Events


	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();


}