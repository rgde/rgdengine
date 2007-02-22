//control.h
#pragma once
#include "base.h"

namespace input
{

    //элемент (кнопка, ось и т.п.) устройства ввода
    class CControl
    {
    public:
        
        //виды контролов
        enum EType
        {
            Button,
            Axis
        };

        CControl(EControl eName, EType eType, CDevice &rDevice);

        EControl getName () const {return m_eName;} //получить 'имя' контрола
        EType    getType () const {return m_eType;} //получить тип контрола

        //добавить наблюдателя
        void bind (PCommand pCommand);
        void bind (const std::wstring &sCommandName);

        //удалить наблюдателя
        void unbind (PCommand pCommand);
        void unbind (const std::wstring &sCommandName);

        //добавлен ли такой наблюдатель
        bool isbind (PCommand pCommand);
        bool isbind (const std::wstring &sCommandName);

        //уведомить наблюдателей о своем изменении
        void notifyAllObservers (); 

        int  m_nTime;
        bool m_bPress;
        int  m_nDelta;

    private:
        CDevice            &m_rDevice;  //устройство, которому принадлежит контрол
        EType               m_eType;    //тип контрола
        EControl            m_eName;    //'имя' контрола
        std::list<PCommand> m_commands; //команда, к которой привязан контрол
    };

}