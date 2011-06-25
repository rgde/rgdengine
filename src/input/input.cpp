#include "stdafx.h"


#include <rgde/input/input.h>
#include <rgde/input/device.h>

#include <rgde/core/windows.h>
#include <rgde/core/file_system.h>


#include "inputimpl.h"

namespace rgde
{
namespace input
{
    system::system (core::windows::window_ptr window, bool exclusive, bool foreground)
    {		
		m_impl.reset(new input_impl(window, exclusive, foreground));
    }

    system::~system ()
    {
    }

    bool system::set_mode(bool exclusive, bool foreground)
    {
        return m_impl->set_mode(exclusive,foreground);
    }

    void system::load_from_string (const std::string &xml_str)
    {
        m_impl->load(xml_str);
    }

    void system::load (core::vfs::istream_ptr src)
    {
        std::string xml_data;
		size_t size = src->get_size();
		xml_data.resize(size);
		src->read(&xml_data[0], size);
        m_impl->load(xml_data);
    }

    void system::update ()
    {
        m_impl->update();
    }

    void system::save (std::string &xml_str)
    {
        m_impl->save(xml_str);
    }

    device* system::get_device (types::EDevice device_type, int indx)
    {
        return m_impl->get_device(device_type, indx);
    }

    device* system::get_device (const std::wstring &device_name, int indx)
    {
        return m_impl->get_device(device_name, indx);
    }

	control* system::get_control(types::EDevice type, int index, types::EControl control)
	{
		if (device* dev = get_device(type, index))
			return dev->get_control(control);

		return NULL;
	}

    //есть ли такое устройство
    bool system::is_device_present (types::EDevice device_type, int indx)
    {
        return m_impl->is_device_present(device_type,indx);
    }

    //есть ли такое устройство
    bool system::is_device_present (const std::wstring &device_name, int indx)
    {
        return m_impl->is_device_present(device_name, indx);
    }

    //добавить команду
    void system::add_command (const std::wstring &command_name)
    {
        m_impl->add_command(command_name);
    }

    //получить команду
    command_ptr system::get_command (const std::wstring &command_name)
    {
        return m_impl->get_command(command_name);
    }

    //есть ли такая команда
    bool system::is_command_present (const std::wstring &command_name)
    {
        return m_impl->is_command_present(command_name);
    }

    //отвязать команду ото всех контролов
    void system::detach_command (command_ptr command)
    {
        m_impl->detach_command(command);
    }
}
}