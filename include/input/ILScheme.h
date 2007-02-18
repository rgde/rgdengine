#pragma once

#include "input/ILBase.h"

namespace input
{
  /////////////////////////////////////////////////////////////
  // схема "какой контрол соответсвует определенной команде" //
  /////////////////////////////////////////////////////////////
  class CScheme
  {
  public:
    typedef std::multimap<ECommand, std::pair<EDevice,EControl> > BindsMap;
    typedef BindsMap::const_iterator                              iterator;

    //получить ссылку на синглтон схемы ввода
    static CScheme& Get ();		

    //удалить синглтон схемы ввода
    static void Destroy ();

    //инициализировать схему ввода
    void init (ISchemeImpl*);

    //управление св€зками команда-контрол
    static std::pair<iterator,iterator> GetCommandBinding (ECommand);  //получить св€зки команда-контрол
    static void AddCommandBinding       (ECommand, EDevice, EControl); //добавить св€зку
    static void RemoveCommandBinding    (ECommand);                    //удалить св€зку
    static bool IsCommandBindingPresent (ECommand);                    //существует ли св€зка дл€ данной команды

    //сохранение-загрузка
    static void Load (std::istream &instr);  //загрузка св€зок команда-контрол
    static void Save (std::ostream &outstr); //сохранение св€зок команда-контрол

  private:
    //только дл€ конструировани€ синглтона
    CScheme ();
   ~CScheme ();

    static CScheme             *ms_pInstance; //указатель на едиственный экземпл€р Scheme
    std::auto_ptr<ISchemeImpl>  m_pImpl;      //конкретна€ реализаци€ системы ввода
    BindsMap                    m_mapBinds;   //св€зки команда-контрол

    //запрет конструктора копировани€ и оператора присваивани€
    CScheme (const CScheme&);
    CScheme& operator= (const CScheme&);
  };
}