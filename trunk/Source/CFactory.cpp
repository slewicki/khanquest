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
	PROFILE("CFactory::CreatePlayerUnit(int)");
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
	unit->SetDirection(SOUTH_WEST);
	unit->SetIsPlayerUnit(true);
	unit->SetAttackSoundID(CGame::GetInstance()->GetAttackSound(unit->GetType()));
	// Register Events
	unit->SetDeathSoundID(CGame::GetInstance()->GetDeathSound(unit->GetType()));


	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();
	STOP("CFactory::CreatePlayerUnit(int)");

}

void CFactory::CreateComputerUnit(int nType)
{
	PROFILE("CFactory::CreateComputerUnit(int)");
	//static float xPos = 100;
	//static float yPos = 50;
	CUnit* unit = new CUnit(nType);

	// Use default shallow copy since no dynamic info in creation
	CUnit temp = CGame::GetInstance()->GetCPUUnitInfo(nType);
	unit->SetAttackPower(temp.GetAttackPower());
	unit->SetAttackSpeed(temp.GetAttackSpeed());
	unit->SetMaxHP(temp.GetMaxHP());
	unit->SetCurrentHP(temp.GetMaxHP());
	unit->SetRange(temp.GetRange());
	unit->SetSpeed(temp.GetSpeed());

	unit->SetState(IDLE);
	unit->SetDirection(NORTH_WEST);
	unit->SetIsPlayerUnit(false);
	// Register Events
	unit->SetAttackSoundID(CGame::GetInstance()->GetAttackSound(unit->GetType()));
	unit->SetDeathSoundID(CGame::GetInstance()->GetDeathSound(unit->GetType()));

	switch(CGame::GetInstance()->GetSelectedCity()->GetID())
	{
		case KCITY1:

			break;
		case KCITY2:
			unit->SetAttackPower(unit->GetAttackPower()+2);
			break;
		case KCITY3:
			unit->SetAttackPower(unit->GetAttackPower()+2);
			unit->SetAttackSpeed(unit->GetAttackSpeed()-(unit->GetAttackSpeed()*.2f));
			unit->SetAttackSpeed(unit->GetSpeed()-(unit->GetSpeed()*.5f));

			break;
		case XCITY1:

			break;
		case XCITY2:
			unit->SetAttackPower(unit->GetAttackPower()+2);

			break;
		case XCITY3:
			unit->SetAttackPower(unit->GetAttackPower()+2);
			unit->SetAttackSpeed(unit->GetAttackSpeed()-(unit->GetAttackSpeed()*.2f));
			unit->SetAttackSpeed(unit->GetSpeed()-(unit->GetSpeed()*.5f));

			break;
		case JCITY1:

			break;
		case JCITY2:
			unit->SetAttackPower(unit->GetAttackPower()+2);

			break;
		case JCITY3:
			unit->SetAttackPower(unit->GetAttackPower()+2);
			unit->SetAttackSpeed(unit->GetAttackSpeed()-(unit->GetAttackSpeed()*.2f));
			unit->SetAttackSpeed(unit->GetSpeed()-(unit->GetSpeed()*.5f));

			break;
	}



	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();
	STOP("CFactory::CreateComputerUnit(int)");


}