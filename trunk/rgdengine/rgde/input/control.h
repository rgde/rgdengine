#pragma once

#include <rgde/input/base.h>

namespace input
{
    //элемент (кнопка, ось и т.п.) устройства ввода
    class Control
    {
    public:        
        //виды контролов
        enum type
        {
            Button,
            Axis
        };

        Control(controls name, type type, device &device);

        controls get_name () const {return m_name;}
		type    get_type () const {return m_type;}

        //добавить наблюдателя
        void bind (command_ptr command);
        void bind (const std::wstring &command_name);

        //удалить наблюдателя
        void unbind (command_ptr command);
        void unbind (const std::wstring &command_name);

        //добавлен ли такой наблюдатель
        bool is_bind (command_ptr command);
        bool is_bind (const std::wstring &command_name);

        //уведомить наблюдателей о своем изменении
        void notify_all (); 

		bool getPress() const {return m_press;}

        int  m_time;
        bool m_press;
        int  m_delta;

    private:
        device          &m_device; //устройство, которому принадлежит контрол
        type            m_type;   //тип контрола
        controls  m_name;   //'имя' контрола

		typedef std::list<command_ptr> CommandsList;
		typedef CommandsList::iterator commands_iter;

		//команды, к которой привязан контрол
        CommandsList m_commands;
    };
}