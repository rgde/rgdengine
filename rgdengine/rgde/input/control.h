#pragma once

#include <rgde/input/base.h>

namespace input
{
    //элемент (кнопка, ось и т.п.) устройства ввода
    class Control
    {
    public:        
        //виды контролов
        enum EType
        {
            Button,
            Axis
        };

        Control(types::control name, EType type, device_dx9 &device);

        types::control get_name () const {return m_name;}
		EType    getType () const {return m_type;}

        //добавить наблюдателя
        void bind (CommandPtr command);
        void bind (const std::wstring &command_name);

        //удалить наблюдателя
        void unbind (CommandPtr command);
        void unbind (const std::wstring &command_name);

        //добавлен ли такой наблюдатель
        bool isbind (CommandPtr command);
        bool isbind (const std::wstring &command_name);

        //уведомить наблюдателей о своем изменении
        void notifyAllObservers (); 

		bool getPress() const {return m_press;}

        int  m_time;
        bool m_press;
        int  m_delta;

    private:
        device_dx9          &m_device; //устройство, которому принадлежит контрол
        EType            m_type;   //тип контрола
        types::control  m_name;   //'имя' контрола

		typedef std::list<CommandPtr> CommandsList;
		typedef CommandsList::iterator CommandsIter;

		//команды, к которой привязан контрол
        CommandsList m_commands;
    };
}