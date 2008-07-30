//////////////////////////////////////////////////////////////////////////
// File CEmitter.h
//
// Author: Rodney Kite (RK)
//
// Purpose: Contains class definition for CParticle Class, which holds
//			all of the info for a particle
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CEventSystem.h"
#include "CParticle.h"
#include "../irrxml/irrXML.h"
//using std::list;
using namespace irr;
using namespace io;

#define RAND_FLOAT(min,max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))

class CEmitter
{
private:

	CSGD_Direct3D*			m_pD3D;			// direct 3D
	CEventSystem*			m_pES;			// Event System
	CSGD_TextureManager*	m_pTM;			// Texture Manager

	int						m_nEmitterXLocation; // X location to draw the emitter
	int						m_nEmitterYLocation; // X location to draw the emitter

	float 					m_fPercentAlpha;	
	float 					m_fPercentRed;	
	float 					m_fPercentGreen;	
	float 					m_fPercentBlue;	

	float					m_fPercentXScale;
	float					m_fPercentYScale;

	float					m_fUpdateFloat;	
	float					m_fUpdateQuarter;
	int						m_nImageID;

	//	For calculating elapsed time
	DWORD				m_dwPreviousTime;	//	Time stamp of the last frame
	double				m_dElapsedTime;		//	Time elapsed (in seconds) since the previous frame.
	DWORD				m_dwCurrentTime;	// current time
	DWORD				m_dwFrameTimer;		// frame timer


	D3DXCOLOR			m_cStartColor;
	D3DXCOLOR			m_cEndColor;
	D3DXCOLOR			m_cCurrentColor;


	CParticle			particle;		// single particle

public:

	vector<CParticle>	ParticleList;	// list of particles

	//////////////////////////////////////////////////////////////////////////
	//  Function: CEmitter
	//	Last Modified: July 26, 2008
	//  Purpose : Default constructor
	//////////////////////////////////////////////////////////////////////////
	CEmitter(void);

	//////////////////////////////////////////////////////////////////////////
	//  Function: CEmitter
	//	Last Modified: July 26, 2008
	//  Purpose : Copy constructor.
	//////////////////////////////////////////////////////////////////////////
	CEmitter(const CEmitter& ref);

	//////////////////////////////////////////////////////////////////////////
	//  Function: operator=
	//	Last Modified: July 26, 2008
	//  Purpose : Assignment Operator.
	//////////////////////////////////////////////////////////////////////////
	CEmitter& operator=(const CEmitter& ref);

	//////////////////////////////////////////////////////////////////////////
	//  Function: ~CEmitter
	//	Last Modified: July 26, 2008
	//  Purpose : Destructor.
	//////////////////////////////////////////////////////////////////////////
	~CEmitter(void);

	//////////////////////////////////////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: July 25, 2008
	//  Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////
	int GetEmitterXLocation(void)	{return m_nEmitterXLocation;}
	int GetEmitterYLocation(void)	{return m_nEmitterYLocation;}
	int GetImageID(void)			{return m_nImageID;}

	//////////////////////////////////////////////////////////////////////////
	// Function: Modifiers
	//	Last Modified: July 25, 2008
	//  Purpose: Modifies the specified type.
	//////////////////////////////////////////////////////////////////////////
	void SetEmitterXLocation(int emitterXLocation)	{ m_nEmitterXLocation = emitterXLocation;}
	void SetEmitterYLocation(int emitterYLocation)	{ m_nEmitterYLocation = emitterYLocation;}
	int GetImageID(int ImageID)						{m_nImageID = ImageID;}

	//////////////////////////////////////////////////////////////////////////
	// Function: “Exit”
	//	Last Modified: July 25, 2008
	// Purpose: Exit the instance of the class and clean up memory
	//////////////////////////////////////////////////////////////////////////
	void Exit(void);

	//////////////////////////////////////////////////////////////////////////
	// Function: LoadXMLParicle
	//	Last Modified: July 27, 2008
	//  Purpose: Loads the particle emitter information from a XML file
	//////////////////////////////////////////////////////////////////////////
	void LoadXMLEmitter(char* emitterFileName, float locX, float locY, vector<CParticle> &vParticleList);


	//////////////////////////////////////////////////////////////////////////
	// Function: LoadXMLParicle
	//	Last Modified: July 27, 2008
	//  Purpose: Loads the particle emitter information from a XML file
	//////////////////////////////////////////////////////////////////////////
	void LoadBinaryEmitter(char* emitterFileName, float locX, float locY, vector<CParticle> &vParticleList);

	//////////////////////////////////////////////////////////////////////////
	// Function: LoadBinParicle
	//	Last Modified: July 27, 2008
	//  Purpose: Loads the particle  emitter information from a Binary file
	//////////////////////////////////////////////////////////////////////////
	void LoadBinEmitter(char* emitterFileName, float locX, float locY, vector<CParticle> &vParticleList);

};
