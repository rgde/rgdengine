#include "precompiled.h"

#include "input/ILControl.h"
#include "input/ILCommand.h"

namespace input
{

  CControl::~CControl ()
  {
	  // создать копию списка
	  std::list<CCommand*> listObservers(m_listObservers);

	  // очистить старый список
	  m_listObservers.clear();

	  // отписать всех обзерверов
	  for (std::list<CCommand*>::iterator it = listObservers.begin();
		  it != listObservers.end(); ++it)
	  {
		  CCommand* pObserver = *it;
		  pObserver->detachFromControl(this);
	  }
  }

  //добавить наблюдателя
  bool CControl::attachObserver (CCommand *pCommand)
  {
    //проверка на совместимость команды и контрола
    if
    (
      (
        m_eType == CControl::EType::Axis &&
        (
          pCommand->getType() == CCommand::EType::Trigger ||
          pCommand->getType() == CCommand::EType::Button
        )
      )
      ||
      (
        m_eType == CControl::EType::Button &&
        (
          pCommand->getType() == CCommand::EType::AbsoluteAxis ||
          pCommand->getType() == CCommand::EType::RelativeAxis
        )
      )
    )
    {
      //... сообщить о несовпадении типов контрола и команды
      return false;
    }

    m_listObservers.push_back(pCommand);
    return true;
  }

  //удалить наблюдателя
  void CControl::detachObserver (CCommand *pCommand)
  {
    std::list<CCommand*>::iterator i = std::find
    (
      m_listObservers.begin(),
      m_listObservers.end(),
      pCommand
    );

    if (i == m_listObservers.end())
    {
      //... сообщить об отсутствии команды в списке наблюдателей
      return;
    }

    m_listObservers.erase(i);
  }

  //уведомить наблюдателей о своем изменении
  void CControl::notifyAllObservers ()
  {
//->
    //во время уведомления об измнении статуса контрола может произойти
    //изменение списка обсерверов -> список итераторов может измениться,
    //а значит итераторы могут стать невалидными. переписываем с учетом
    //этого.
    std::list<CCommand*> listTempObservers(m_listObservers);
    std::list<CCommand*>::iterator i   = listTempObservers.begin(),
                                   end = listTempObservers.end();
    for(; i != end; ++i)
      if
      (
        std::find
        (
          m_listObservers.begin(),
          m_listObservers.end(),
          *i
        ) !=
        m_listObservers.end()
      )
        (*i)->notify(this);
/*
    std::list<CCommand*>::iterator i   = m_listObservers.begin(),
                                   end = m_listObservers.end();
    for(; i!=end; ++i)
      (*i)->notify(this);
*/
//-<
  }

} //namespace input
