//////////////////////////////////////////////////////////////////////////////////////////////////
// File: CEventSystem.h
//
// Author: Rodney Kite (RK)
//
// Purpose: Sends out events to anyone who can "listen" for them.
//////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <map>
using std::multimap;
using std::pair;
using std::list;

#include "IListener.h"

class CEventSystem
{
	private:
		multimap<EVENTID, IListener *>		m_Database;

		//	list of events waiting to be processed.
		list<CEvent>						m_CurrentEvents;

		//////////////////////////////////////////////////////////////////////////
		//	Function: Constructor	
		//	Last Modified: 7/17/08
		//	Purpose: Sends an event to all the clients that are registered to receive event information.
		//////////////////////////////////////////////////////////////////////////
		void DispatchEvent(CEvent *pEvent);

		// Trilogy of Evil
		//////////////////////////////////////////////////////////////////////////
		//	Function: Constructor	
		//	Last Modified: 7/17/08
		//	Purpose: Default Constructor.
		//////////////////////////////////////////////////////////////////////////
		CEventSystem() {}

		//////////////////////////////////////////////////////////////////////////
		//	Function: Copy Constructor	
		//	Last Modified: 7/17/08
		//	Purpose: Copies one to another.
		//////////////////////////////////////////////////////////////////////////
		CEventSystem(const CEventSystem& ref);

		//////////////////////////////////////////////////////////////////////////
		//	Function: Assignment Operator	
		//	Last Modified: 7/17/08
		//	Purpose: Assigns one to another.
		//////////////////////////////////////////////////////////////////////////
		CEventSystem* operator=(const CEventSystem& ref);

		// Destructor
		//////////////////////////////////////////////////////////////////////////
		//	Function: Destructor	
		//	Last Modified: 7/17/08
		//	Purpose: Destroys class and cleans memory.
		//////////////////////////////////////////////////////////////////////////
		~CEventSystem() {}

	public:
		//////////////////////////////////////////////////////////////////////////
		//	Function: GetInstance	
		//	Last Modified: 7/17/08
		//	Purpose: Creates and returns the one and only instance of this class.
		//////////////////////////////////////////////////////////////////////////
		static CEventSystem* GetInstance(void);

		//////////////////////////////////////////////////////////////////////////
		//	Function: RegisterClient	
		//	Last Modified: 7/17/08
		//	Purpose: Builds out the database of clients that can receive event information.
		//////////////////////////////////////////////////////////////////////////
		void RegisterClient(EVENTID eventID, IListener *pClient);

		//////////////////////////////////////////////////////////////////////////
		//	Function: UnregisterClient	
		//	Last Modified: 7/17/08
		//	Purpose: Removes the client from the database.
		//////////////////////////////////////////////////////////////////////////
		void UnregisterClient(IListener* pClient);

		//////////////////////////////////////////////////////////////////////////
		//	Function: SendEvent	
		//	Last Modified: 7/17/08
		//	Purpose: Sends an event to be processed out to the listeners later on.
		//////////////////////////////////////////////////////////////////////////
		void SendEvent(EVENTID eventID, void *pData = 0);

		//////////////////////////////////////////////////////////////////////////
		//	Function: ProcessEvents	
		//	Last Modified: 7/17/08
		//	Purpose: Processes all events in our event list.
		//////////////////////////////////////////////////////////////////////////
		void ProcessEvents(void);

		//////////////////////////////////////////////////////////////////////////
		//	Function: ProcessEvents	
		//	Last Modified: 7/17/08
		//	Purpose: Clears all events.
		//////////////////////////////////////////////////////////////////////////
		void ClearEvents(void); 

};