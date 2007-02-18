///**--------------------------------------------------------------------------------------
//
//									RGDE Engine
//
//----------------------------------------------------------------------------------------
//\author Dub (Originally created)
//
//\author Andrew "Zlobnik" Chouprina, 2005
//mail:		dxprg@mail.ru
//
//Created:	15 june 2005
//---------------------------------------------------------------------------------------*/
//#include "precomp.h"
//#include "Event.h"
//
//
//
//namespace test_PacMan
//{
//
//	//IEventManager
//	IEventManager::EventManagers IEventManager::ms_listInstances;
//
//	void IEventManager::DeleteAllMangers()
//	{
//		for (iterator i = ms_listInstances.begin(); i != ms_listInstances.end(); ++i)
//		{
//			IEventManager* p = *i;
//			delete p;
//		}
//		ms_listInstances.clear();
//	}
//
//	//CListener
//	CListener::~CListener() 
//	{
//		unsubscribe();
//	}
//
//	void CListener::unsubscribe ()
//	{
//		EventManagers::iterator i = m_listManagers.begin(),
//			end = m_listManagers.end();
//		while (i != end)
//		{
//			(*i)->unsubscribe(this);
//			++i;
//		}
//		m_listManagers.clear();
//	}
//
//}