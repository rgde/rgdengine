#pragma once

#include <rgde/input/base.h>

namespace input
{
    //input device
    class device_dx9
    {
    public:
		device_dx9(types::device name, int index, input_impl &input_system);
        virtual ~device_dx9();

		//get device name
        types::device get_name () const {return m_name;}
		//get device index
        int		get_device_index () const {return m_index;}
		
        input_impl& get_impl()       {return m_input;}

		// get control
        Control* get_control       (types::control	contol_name);
        Control* get_control       (const std::wstring &contol_name);

		// is such control present in device
        bool is_control_present (types::control	contol_name) const;
        bool is_control_present (const std::wstring &contol_name) const;

    protected:
        friend class input_impl;
		//add button
        void add_button (types::control contol_name); 
		//add axis
        void add_axis   (types::control contol_name); 
		//deattach command from all controls
        void detach_command (command_ptr command); 

    private:
        int         m_index;
        types::device m_name;
        input_impl &m_input;

		typedef std::map<types::control, Control*> controls_map;
		
		//owned input controls
        controls_map m_controls; 
    };
}