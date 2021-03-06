// File CAISystem.h
//
// Author: Rodney Kite (RK)
// Last Modified: August 20, 2008
// Purpose: Contains class definition for CAISystem Class, which handles
//			all of the unit AI
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <windows.h>
#include "CTileEngine.h"
#include "CTileEngine.h"
#include "CTile.h"
#include <list>
#include "CProfile.h"
using std::list;
 
//names of tiles
enum { TILEEMPTY, TILEBLOCK, TILESTART, TILEEND, TILEPATH, TILESELECT, };
 //const int m_nMapWidth=20;
 //const int m_nMapHeight=20;

class CAISystem
{
	CTileEngine* m_pTE;

	//dimensions of map
	int m_nMapWidth;
	int m_nMapHeight;

	int **Map;		//contains tile data about the map
	int **MapPath;	//contains path finding info about the map marks
	bool **MapMark;	//contains marked tiles for path finding
	

	////dimensions of map
	//int Map[m_nMapWidth][m_nMapHeight];//contains tile data about the map
	//int MapPath[m_nMapWidth][m_nMapHeight];//contains pathfinding info about the map
	////map marks
	//bool MapMark[m_nMapWidth][m_nMapHeight];//contains marked tiles for pathfinding


	list<POINT>  m_vPath;


private:
	//////////////////////////////////////////////////////////////////////////
	//  Function: CEmitter
	//	Last Modified: August 04, 2008
	//  Purpose : Default constructor
	//////////////////////////////////////////////////////////////////////////
	CAISystem(void);

	//////////////////////////////////////////////////////////////////////////
	//  Function: CEmitter
	//	Last Modified: August 04, 2008
	//  Purpose : Default constructor
	//////////////////////////////////////////////////////////////////////////
	~CAISystem(void);

public:

	//////////////////////////////////////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: August 04, 2008
	//  Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Function: Modifiers
	//	Last Modified: August 04, 2008
	//  Purpose: Modifies the specified type.
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// Function: �GetInstance�
	//	Last Modified: August 04, 2008
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CAISystem* GetInstance(void)
	{
		static CAISystem instance;

		return &instance;
	}

	//////////////////////////////////////////////////////
	//	Function: �Exit�
	//	Last Modified: August 04, 2008
	//	Purpose: To unload any needed information
	//////////////////////////////////////////////////////
	void Exit(void);

	//////////////////////////////////////////////////////
	//	Function: �UpdateState�
	//	Last Modified: August 04, 2008
	//	Purpose: To change the units state as needed
	//////////////////////////////////////////////////////
	void UpdateState(void);

	//////////////////////////////////////////////////////
	//	Function: �FindPath�
	//	Last Modified: August 06, 2008
	//	Purpose: To find the path to the dest
	//////////////////////////////////////////////////////
	list<POINT> FindPath(CTile* current, CTile* destination);

};
