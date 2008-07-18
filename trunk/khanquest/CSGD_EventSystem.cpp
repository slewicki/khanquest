//////////////////////////////////////////////////////////////////////////////////////////////////
// File: CSGD_EventSystem.cpp
// Author: Jensen Rivera
// Purpose: Function definitions for Event System
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "CSGD_EventSystem.h"

//	GetInstance.
CSGD_EventSystem* CSGD_EventSystem::GetInstance(void)
{
	//	Make sure that the instance does not exist before trying to make it.
	static CSGD_EventSystem instance;
	//	Return the memory address pointing to the instance.
	return &instance;
}

//	Register Client.
void CSGD_EventSystem::RegisterClient(EVENTID eventID, IListener *pClient)
{
	//	Error check.
	if (!pClient)	return;

	//	Register (push) our client into the database.
	m_Database.insert(make_pair(eventID, pClient));
}

//	Unregister client
void CSGD_EventSystem::UnregisterClient(IListener *pClient)
{
	multimap<string, IListener*>::iterator mmIter = m_Database.begin();

	while (mmIter != m_Database.end())
	{
		if ((*mmIter).second == pClient)
		{
			mmIter = m_Database.erase(mmIter);
			continue;
		}
		++mmIter;
	}
}

void CSGD_EventSystem::DispatchEvent(CEvent *pEvent)
{
	//	Make an iterator that will iterate all of our clients that
	//	should be receiving an event.
	pair<multimap<EVENTID, IListener *>::iterator,
		 multimap<EVENTID, IListener *>::iterator> range;

	//	Finds all the clients in a database that can receive this event information.
	range = m_Database.equal_range(pEvent->GetEventID());

	//	Call their EventHandler functions.
	for (multimap<EVENTID, IListener *>::iterator mmIter = range.first;
			mmIter != range.second; mmIter++)
	{
		//	Pass the event to this client.
 		(*mmIter).second->EventHandler(pEvent);
	}
}

void CSGD_EventSystem::SendEvent(EVENTID eventID, void *pData)
{
	//	Push the event into my list.
	CEvent newEvent;
	newEvent.SetEventParams(eventID, pData);
	m_CurrentEvents.push_back(newEvent);
}

void CSGD_EventSystem::ProcessEvents(void)
{
	//	Go through the linked list and process all the events it contains.
	while (m_CurrentEvents.size())
	{
		DispatchEvent(&m_CurrentEvents.front());
		m_CurrentEvents.pop_front();
	}
}
void CSGD_EventSystem::ClearEvents(void) 
{ 
	m_CurrentEvents.clear(); 
	m_Database.clear(); 


}