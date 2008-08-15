//////////////////////////////////////////////////////////////////////////
// File CAISystem.h
//
// Author: Rodney Kite (RK)
//
// Purpose: Contains class definition for CAISystem Class, which handles
//			all of the unit AI
//////////////////////////////////////////////////////////////////////////
#include "CAISystem.h"
#include "SGD_Math.h"

CAISystem::CAISystem(void)
{
	m_pTE = CTileEngine::GetInstance();


	//dimensions of tiles
	TileSelected=0;	//currently selected tile



}

CAISystem::~CAISystem(void)
{
}

void CAISystem::Exit(void)
{
	//for (int i = 0; i < m_nMapWidth; ++i)
	//{
	//	delete Map[i];
	//}
	//delete [] Map;

	//for (int i = 0; i < m_nMapWidth; ++i)
	//{
	//	delete MapMark[i];
	//}
	//delete [] MapMark;

	//for (int i = 0; i < m_nMapWidth; ++i)
	//{
	//	delete MapPath[i];
	//}
	//delete [] MapPath;
}
void CAISystem::UpdateState(void)
{

}

list<POINT> CAISystem::FindPath(CTile* current, CTile* destination)//find the path
{
	m_vPath.clear();

	if (destination->bIsCollision || destination->bIsOccupied)
		return m_vPath;

	POINT ptStart	= m_pTE->IsoMouse(current->ptLocalAnchor.x, current->ptLocalAnchor.y, 0);
	POINT ptEnd		= m_pTE->IsoMouse(destination->ptLocalAnchor.x, destination->ptLocalAnchor.y, 0);
	POINT ptPath;
	//set initial start and end points
	for (int i = 0; i < MAPWIDTH; ++i)
	{

		// Stock the row with -1 for open tile.
		for (int j = 0; j < MAPHEIGHT ; ++j)
			if (!m_pTE->GetTile(0,i,j)->bIsCollision)
			{
				Map[i][j] = -1; 
				MapPath[i][j] = -1;
			}
			// set closed tiles to high number
			else
			{
				Map[i][j] = 999; 
				MapPath[i][j] = 999;
			}
	}
	Map[ptStart.x][ptStart.y]=TILESTART;
	MapPath[ptStart.x][ptStart.y] =0;
	Map[ptEnd.x][ptEnd.y]=TILEEND;


	ptStart.x=-1;
	ptEnd.x=-1;
	int x;
	int y;
	int nx;
	int ny;
	bool found;
	int lowvalue;
	//find the start
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			//check for the start
			if(Map[x][y]==TILESTART)
			{
				ptStart.x=x;
				ptStart.y=y;
			}
		}
	}
	//find the end
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			//check for the end
			if(Map[x][y]==TILEEND)
			{
				ptEnd.x=x;
				ptEnd.y=y;
			}
		}
	}
	//if no start or end, exit function
	if(ptStart.x==-1 || ptEnd.x==-1) return m_vPath;


	//scan for path-able tiles
	do
	{
		//havent found one yet
		found=false;
		//scan the map
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				MapMark[x][y]=false;
				//make sure this is a "-1" square
				if(MapPath[x][y]==-1)
				{
					//scan the neighbors
					for(nx=x-1;nx<=x+1;nx++)
					{
						for(ny=y-1;ny<=y+1;ny++)
						{
							//make sure the neighbor is on the map
							if(nx>=0 && ny>=0 && nx<MAPWIDTH && ny<MAPHEIGHT && !(nx==x && ny==y))
							{
								//check against negatives and 999
								if(MapPath[nx][ny]>=0 && MapPath[nx][ny]!=999)
								{
									//mark the map
									MapMark[x][y]=true;
									//mark it as found
									found=true;
								}
							}
						}
					}
				}
			}
		}
		//now scan the marks
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				//if this square is marked
				if(MapMark[x][y])
				{
					//set low value very high
					lowvalue=999;
					//loop through neighbors
					for(nx=x-1;nx<=x+1;nx++)
					{
						for(ny=y-1;ny<=y+1;ny++)
						{
							//make sure the neighbor is on the map
							if(nx>=0 && ny>=0 && nx<MAPWIDTH && ny<MAPHEIGHT)
							{
								if(MapPath[nx][ny]>=0)//must be a non-negative value
								{
									//assign the value if it is lower
									if(MapPath[nx][ny]<lowvalue) lowvalue=MapPath[nx][ny];
								}
							}
						}
					}
					//assign the value to the path map
					MapPath[x][y]=lowvalue+1;
				}
			}
		}
	}
	while(found);
	//done with path finding

	if(MapPath[ptEnd.x][ptEnd.y]!=-1)
	{
		//start the path
		ptPath=ptEnd;
		//take the value from the map
		lowvalue=MapPath[ptEnd.x][ptEnd.y];
		while(lowvalue>0)
		{
			found=false;
			do
			{
				do
				{
					//pick a random neighbor
					nx=rand()%3-1;
					ny=rand()%3-1;
				}while((nx==0 && ny==0) || (ptPath.x+nx)<0 || (ptPath.x+nx)>=MAPWIDTH || (ptPath.y+ny)<0 || (ptPath.y+ny)>=MAPHEIGHT);
				//check to see if the value is lower
				if(MapPath[ptPath.x+nx][ptPath.y+ny]<lowvalue)
				{
					//found!
					found=true;
					//set tile to path tile
					Map[ptPath.x][ptPath.y]=TILEPATH;
					m_vPath.push_back(ptPath);
					//move the path
					ptPath.x+=nx;
					ptPath.y+=ny;
					lowvalue=MapPath[ptPath.x][ptPath.y];
					
				}
			}
			while(!found);
		}
		//replace the end tile
		Map[ptEnd.x][ptEnd.y]=TILEEND;
	}

	//for (int i = 0; i < MAPWIDTH ; ++i)
	//{
	//	// Stock the row with -1 for open tile.
	//	for (int j = 0; j < MAPHEIGHT; ++j)
	//		if (Map[i][j] == 4)
	//		{
	//			POINT path;
	//			path.x = i;
	//			path.y = j;
	//			m_vPath.push_back(path);
	//		}
	//}
	//m_vPath.push_back(ptEnd);
	return m_vPath;
}

