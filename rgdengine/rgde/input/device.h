#pragma once

#include <rgde/input/base.h>

namespace input
{
    //input device
    class device
    {
    public:
		device(devices type, int index, input_impl &input_system);
        virtual ~device();

		//get device type
        devices get_type () const {return m_type;}
		//get device index
        int		get_device_index () const {return m_index;}
		
        input_impl& get_impl()       {return m_input;}

		// get control
        Control* get_control       (controls	contol_name);
        Control* get_control       (const std::wstring &contol_name);

		// is such control present in device
        bool is_control_present (controls	contol_name) const;
        bool is_control_present (const std::wstring &contol_name) const;

    protected:
        friend class input_impl;
		//add button
        void add_button (controls contol_name); 
		//add axis
        void add_axis   (controls contol_name); 
		//deattach command from all controls
        void detach_command (command_ptr command); 

    private:
        int         m_index;
        devices m_type;
        input_impl &m_input;

		typedef std::map<controls, Control*> controls_map;
		
		//owned input controls
        controls_map m_controls; 
    };
}