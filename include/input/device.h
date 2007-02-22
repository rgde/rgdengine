//device.h
#pragma once
#include "base.h"
#include <map>

namespace input
{

    //устройство ввода
    class CDevice
    {
    public:
        CDevice(EDevice eName, int nIndx, CInputImpl &rInput):
            m_nIndx  (nIndx),
            m_eName  (eName),
            m_rInput (rInput)
        {
        }
        virtual ~CDevice();

        EDevice     getName () const {return m_eName;}  //получить 'имя' устройства
        int         getIndx () const {return m_nIndx;}  //получить порядковый номер устройства
        CInputImpl& getInput()       {return m_rInput;} //получить устройство ввода

        CControl* getControl       (EControl            eControlName);       //получить контрол
        CControl* getControl       (const std::wstring &sControlName);
        bool      isControlPresent (EControl            eControlName) const; //есть ли такой контрол
        bool      isControlPresent (const std::wstring &sControlName) const;

    protected:
        friend class CInputImpl;
        void addButton (EControl eControlName); //добавить кнопку
        void addAxis   (EControl eControlName); //добавить ось
        void detachCommand (PCommand pCommand); //отвязать команду ото всех контролов

        //->
        //init
        //getbuffereddata
        //process
        //-<

    private:
        int         m_nIndx;
        EDevice     m_eName;
        CInputImpl &m_rInput;

        std::map<EControl, CControl*> m_controls; //контролы, которые есть у устройства
    };
}