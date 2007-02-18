#pragma once
//простая реализация схемы команда-контрол

#include "input/ILBase.h"
#include "ILSchemeImpl.h"

namespace input
{

  class CSchemeImplSimple: public ISchemeImpl
  {
  public:
    void load (std::istream &instr);  //загрузка связок команда-контрол
    void save (std::ostream &outstr); //сохранение связок команда-контрол
  };

}