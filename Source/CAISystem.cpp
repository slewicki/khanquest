//////////////////////////////////////////////////////////////////////////
// File CAISystem.h
//
// Author: Rodney Kite (RK)
// Last Modified: August 20, 2008
// Purpose: Contains class definition for CAISystem Class, which handles
//			all of the unit AI
//////////////////////////////////////////////////////////////////////////
#include "CAISystem.h"
#include "SGD_Math.h"

CAISystem::CAISystem(void)
{
	PROFILE("CAISystem::CAISystem()");
	m_pTE = CTileEngine::GetInstance();
	STOP("CAISystem::CAISystem()");
}

CAISystem::~CAISystem(void)
{
}

void CAISystem::Exit(void)
{
	PROFILE("CAISystem::Exit()");
	for (int i = 0; i < m_nMapWidth; ++i)
	{
		delete Map[i];
	}
	delete [] Map;

	for (int i = 0; i < m_nMapWidth; ++i)
	{
		delete MapMark[i];
	}
	delete [] MapMark;

	for (int i = 0; i < m_nMapWidth; ++i)
	{
		delete MapPath[i];
	}
	delete [] MapPath;
	STOP("CAISystem::Exit()");
}
void CAISystem::UpdateState(void)
{

}

list<POINT> CAISystem::FindPath(CTile* current, CTile* destination)//find the path
{
	PROFILE("CAISystem::FindPath(CTile*, CTile*)");
	m_nMapWidth  =	m_pTE->GetMapWidth();
	m_nMapHeight =	m_pTE->GetMapHeight();

	m_vPath.clear();

	if (destination->bIsCollision)// || destination->bIsOccupied)
	{
		STOP("CAISystem::FindPath(CTile*, CTile*)");
		return m_vPath;
	}
	POINT ptStart	= m_pTE->IsoMouse(current->ptLocalAnchor.x, current->ptLocalAnchor.y, 0);
	POINT ptEnd		= m_pTE->IsoMouse(destination->ptLocalAnchor.x, destination->ptLocalAnchor.y, 0);
	POINT ptPath;
	//////////////////////////////////////////////////////////////////////////
	// array to hold the map tile data
	Map = new int*[m_nMapWidth];
	for (int i = 0; i < m_nMapWidth; ++i)
	{
		Map[i] = new int[m_nMapHeight];
	}
	// array to hold the map collision data
	MapMark = new bool*[m_nMapWidth];
	for (int i = 0; i < m_nMapWidth; ++i)
	{
		MapMark[i] = new bool[m_nMapHeight];
	}
	// array to hold the path
	MapPath = new int*[m_nMapWidth];
	for (int i = 0; i < m_nMapWidth; ++i)
	{
		MapPath[i] = new int[m_nMapHeight];
	}

	//set initial start and end points
	for (int i = 0; i < m_nMapWidth; ++i)
	{

		// Stock the row with -1 for open tile.
		for (int j = 0; j < m_nMapHeight ; ++j)
			if (!m_pTE->GetTile(0,i,j)->bIsCollision && !m_pTE->GetTile(0,i,j)->bIsOccupied)
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
	for(x=0;x<m_nMapWidth;x++)
	{
		for(y=0;y<m_nMapHeight;y++)
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
	for(x=0;x<m_nMapWidth;x++)
	{
		for(y=0;y<m_nMapHeight;y++)
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
		for(x=0;x<m_nMapWidth;x++)
		{
			for(y=0;y<m_nMapHeight;y++)
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
							if(nx>=0 && ny>=0 && nx<m_nMapWidth && ny<m_nMapHeight && !(nx==x && ny==y))
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
		for(x=0;x<m_nMapWidth;x++)
		{
			for(y=0;y<m_nMapHeight;y++)
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
							if(nx>=0 && ny>=0 && nx<m_nMapWidth && ny<m_nMapHeight)
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
			
				//do
				//{
				//	//pick a random neighbor
				//	nx=rand()%3-1;
				//	ny=rand()%3-1;
				//}while((nx==0 && ny==0) || (ptPath.x+nx)<0 || (ptPath.x+nx)>=m_nMapWidth || (ptPath.y+ny)<0 || (ptPath.y+ny)>=m_nMapHeight);
				////check to see if the value is lower
				// go thru all neighbors until we find our path 
				
				for (nx = -1; nx <= 1 && !found; nx++)
				{
					for (ny = -1; ny <= 1 && !found; ny++)
					{
						if((nx==0 && ny==0) || (ptPath.x+nx)<0 || (ptPath.x+nx)>=m_nMapWidth || (ptPath.y+ny)<0 || (ptPath.y+ny)>=m_nMapHeight)
							continue;
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
				}
				// if we didnt find a path from our neighbors, we done messed up somewhere
				if(!found)
					return m_vPath;
				
			
		}
		//replace the end tile
		Map[ptEnd.x][ptEnd.y]=TILEEND;
	}
	Exit();
	STOP("CAISystem::FindPath(CTile*, CTile*)");
	return m_vPath;
}

