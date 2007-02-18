//ILSchemeImpl.h

//IL - Input Library (by Dub)
//класс-предок конкретизации для получения связок "команда-контрол"

#ifndef _ILSchemeImpl_
#define _ILSchemeImpl_

#include "input/ILBase.h"
#include <iostream>

namespace input
{

  ///////////////////////////////////////////////////////////////////////
  // класс-предок конкретизации для получения связок "команда-контрол" //
  ///////////////////////////////////////////////////////////////////////
  class ISchemeImpl
  {
  public:
    virtual ~ISchemeImpl() {}

    virtual void load (std::istream &instr)  = 0; //загрузка связок команда-контрол
    virtual void save (std::ostream &outstr) = 0; //сохранение связок команда-контрол
  };

} //namespace input

#endif //_ILSchemeImpl_