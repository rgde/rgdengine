#pragma once

#include <rgde/input/base.h>

namespace input
{
    //устройство ввода
    class Device
    {
    public:
		Device(types::EDevice name, int index, input_impl &input_system);
        virtual ~Device();

		//получить 'имя' устройства
        types::EDevice getName () const {return m_eName;}
		//получить порядковый номер устройства
        int		getIndx () const {return m_nIndx;}
		//получить устройство ввода
        input_impl& getInput()       {return m_rInput;}

		//получить контрол
        Control* get_control       (types::EControl	eControlName);
        Control* get_control       (const std::wstring &sControlName);

		//есть ли такой контрол
        bool isControlPresent (types::EControl	eControlName) const;
        bool isControlPresent (const std::wstring &sControlName) const;

    protected:
        friend class input_impl;
        void addButton (types::EControl controlName); //добавить кнопку
        void addAxis   (types::EControl controlName); //добавить ось
        void detachCommand (CommandPtr command); //отвязать команду ото всех контролов

    private:
        int         m_nIndx;
        types::EDevice m_eName;
        input_impl &m_rInput;

        std::map<types::EControl, Control*> m_controls; //контролы, которые есть у устройства
    };
}