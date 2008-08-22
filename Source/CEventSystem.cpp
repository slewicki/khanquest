//////////////////////////////////////////////////////////////////////////
// File: CEventSystem.cpp
//
// Author: Jensen Rivera
//
// Purpose: Function definitions for Event System
//////////////////////////////////////////////////////////////////////////

#include "CEventSystem.h"
#include "CProfile.h"

//	GetInstance.
CEventSystem* CEventSystem::GetInstance(void)
{
	PROFILE("CEventSystem::GetInstance()");
	//	Make sure that the instance does not exist before trying to make it.
	static CEventSystem instance;
	//	Return the memory address pointing to the instance.
	return &instance;
}

//	Register Client.
void CEventSystem::RegisterClient(EVENTID eventID, IListener *pClient)
{
	PROFILE("CEventSystem::RegisterClient(EVENTID, IListener*)");
	//	Error check.
	if (!pClient)	return;

	//	Register (push) our client into the database.
	m_Database.insert(make_pair(eventID, pClient));
}

//	Unregister client
void CEventSystem::UnregisterClient(IListener *pClient)
{
	PROFILE("CEventSystem::UnregisterClient(IListner*)");
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

void CEventSystem::DispatchEvent(CEvent *pEvent)
{
	PROFILE("CEventSystem::DispatchEvent(CEvent*)");
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

void CEventSystem::SendEvent(EVENTID eventID, void *pData)
{
	PROFILE("CEventSystem::SendEvent(EVENTID, void*)");
	//	Push the event into my list.
	CEvent newEvent;
	newEvent.SetEventParams(eventID, pData);
	m_CurrentEvents.push_back(newEvent);
}

void CEventSystem::ProcessEvents(void)
{
	PROFILE("CEventSystem::ProcessEvents()");
	//	Go through the linked list and process all the events it contains.
	while (m_CurrentEvents.size())
	{
		DispatchEvent(&m_CurrentEvents.front());
		m_CurrentEvents.pop_front();
	}
}
void CEventSystem::ClearEvents(void) 
{ 
	PROFILE("CEventSystem::ClearEvents()");
	m_CurrentEvents.clear(); 
	m_Database.clear(); 


}