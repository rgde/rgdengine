#include "precompiled.h"

#include "input/ILSchemeImplSimple.h"

#include "input/ILScheme.h"
#include "input/ILCommandEnum.h"

namespace input
{

  //загрузка связок команда-контрол
  void CSchemeImplSimple::load (std::istream &instr)
  {
    using namespace std;

    //формат:
    // command name: (device name, control name) ...
    // ';' - коментарий до конца строки

    const int bufsize = 1024;
    char      buf[bufsize]; //буфер, в который читатеся строка
    char     *str,          //вспомогательные указатели для разбора строки
             *rstr;

    string                       command;
    string                       device;
    string                       control;
    vector<pair<string,string> > pairs;

    while (instr)
    {
      command.clear();
      device.clear();
      control.clear();
      pairs.clear();

      //прочитаем строку из буфера
      instr.getline(buf,bufsize);

      //получим указатель на первый непробельный символ
      str = buf;
      str += strspn(&(str[0])," \t");

      //проигнорируем пустые строки и строки с коментариями
      if (str[0]==0 || str[0]==';')
        continue;

      //вырежем имя команды
      rstr = strchr(str,':');
      if (!rstr)
        continue;
      rstr[0]=0;
      ++rstr;
      command = str;
      str = rstr;

      //вырежем имена контролов
      while (1)
      {
        str += strspn(&(str[0])," \t");
        if (str[0]==0 || str[0]==';')
          break;

        //название устройства
        str = strchr(str,'(');
        if (!str)
          break;
        ++str;
        str += strspn(&(str[0])," \t");
        if (str[0]==0 || str[0]==';')
          break;
        rstr = strchr(str,',');
        if (!rstr)
          break;
        rstr[0]=0;
        ++rstr;
        device = str;
        str = rstr;

        //название контрола
        str += strspn(&(str[0])," \t");
        if (str[0]==0 || str[0]==';')
          break;
        rstr = strchr(str,')');
        if (!rstr)
          break;
        rstr[0]=0;
        ++rstr;
        control = str;
        str = rstr;

        //добавляем пару <имя устройтсва, имя контрола>
        pairs.push_back(make_pair<string,string>(device,control));
      }

      //свяжем команду c контролами
      if (pairs.size() > 0)
      {
        ECommand cmd;
        EDevice  dev;
        EControl ctrl;

        cmd = String2ECommand(command);
        if (cmd == InvalidCommand)
          break; //... в файле со схемой прочитано имя команды, которому не сопоставлено ни одно значение из enum

        vector<pair<string,string> >::const_iterator i   = pairs.begin(),
                                                     end = pairs.end();
        while (i != end)
        {
          dev  = String2EDevice  (i->first);
          ctrl = String2EControl (i->second);
          if (dev == InvalidDevice)
            continue; //... в файле со схемой прочитано имя устройства, которому не сопоставлен enum
          if (ctrl == InvalidControl)
            continue; //... в файле со схемой прочитано имя устройства, которому не сопоставлен enum
          
          CScheme::AddCommandBinding(cmd,dev,ctrl);
          ++i;
        }
      } //if (pairs.size() > 0)
    } //while (instr)
  }

  //сохранение связок команда-контрол
  void CSchemeImplSimple::save (std::ostream &outstr)
  {
    using namespace std;

    for (int c=0; c<CommandEnumSize; ++c)
    {
      outstr << ECommand2String((ECommand)c) << ": ";

      CScheme::iterator i   = CScheme::GetCommandBinding((ECommand)c).first,
                        end = CScheme::GetCommandBinding((ECommand)c).second;
      while (i != end)
      {
        outstr << "(";
        outstr << EDevice2String (i->second.first)  << ", ";
        outstr << EControl2String(i->second.second) << ") ";
        ++i;
      }

      outstr << endl;
    }
  }

} //namespace input
