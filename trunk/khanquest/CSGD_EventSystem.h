//////////////////////////////////////////////////////////////////////////////////////////////////
// File: CSGD_EventSystem.h
// Author: Jensen Rivera
// Purpose: Sends out events to anyone who can "listen" for them.
//////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <map>
using std::multimap;
using std::pair;
using std::list;

#include "IListener.h"

class CSGD_EventSystem
{
	private:
		//	Our client list, The Database.  Classes that can listen for events.
		multimap<EVENTID, IListener *>		m_Database;

		//	Events waiting to be processed.
		list<CEvent>						m_CurrentEvents;

		//	Sends an event to all the clients that are registered to receive event information.
		void DispatchEvent(CEvent *pEvent);

		// Trilogy of Evil
			// Constructor
		CSGD_EventSystem() {}
			// Copy Constructor
		CSGD_EventSystem(const CSGD_EventSystem& ref);
			// Assignment Operator
		CSGD_EventSystem* operator=(const CSGD_EventSystem& ref);

		// Destructor
		~CSGD_EventSystem() {}
	public:
		///////////////////////////
		//	Function: GetInstance	
		//	Purpose: Creates and returns the one and only instance of this class.
		///////////////////////////
		static CSGD_EventSystem* GetInstance(void);

		//	Builds out the database of clients that can receive event information.
		void RegisterClient(EVENTID eventID, IListener *pClient);

		//	Removes the client from the database.
		void UnregisterClient(IListener* pClient);

		//	Sends an event to be processed out to the listeners later on.
		void SendEvent(EVENTID eventID, void *pData = 0);

		//	Processes all events in our event list.
		void ProcessEvents(void);

		// clear all events
		void ClearEvents(void); 

};