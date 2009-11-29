#include "stdafx.h"


#include <rgde/input/input.h>
#include <rgde/input/device.h>


#include "inputimpl.h"

namespace input
{
    system* system::ms_instance = 0;

    //только для конструирования синглтона
    system::system ()
    {
        assert(!ms_instance);
        ms_instance = this;
    }

    system::~system ()
    {
        assert(ms_instance);
        ms_instance = 0;
    }

    system& system::get ()
    {
        if (!ms_instance)
            ms_instance = new system;
        return *ms_instance;
    }

    //удалить синглтон системы ввода
    void system::destroy ()
    {
        if (ms_instance)
            delete ms_instance;
        ms_instance = 0;
    }

    //инициализировать систему ввода
    void system::init (input_impl *pImpl)
    {
        m_impl.reset(pImpl);
    }

    bool system::set_mode (bool exclusive, bool foreground)
    {
        return get().m_impl->set_mode(exclusive,foreground);
    }

    void system::load_from_string (const std::string &xml_str)
    {
        get().m_impl->load(xml_str);
    }

    void system::load_from_file (const std::string &file_name)
    {
        std::vector<char> data;

        //io::CFileSystem &fs    = io::TheFileSystem::get();
        //io::readstream_ptr stream = fs.findFile(file_name);
        //io::stream_to_vector<char>(data, stream);

        //get().m_impl->load(std::string(data.begin(), data.end()));
    }

    void system::update ()
    {
        get().m_impl->update();
    }

    void system::save (std::string &xml_str)
    {
        get().m_impl->save(xml_str);
    }

    device* system::get_device (types::EDevice device_type, int indx)
    {
        return get().m_impl->get_device(device_type, indx);
    }

    device* system::get_device (const std::wstring &device_name, int indx)
    {
        return get().m_impl->get_device(device_name, indx);
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
        return get().m_impl->is_device_present(device_type,indx);
    }

    //есть ли такое устройство
    bool system::is_device_present (const std::wstring &device_name, int indx)
    {
        return get().m_impl->is_device_present(device_name, indx);
    }

    //добавить команду
    void system::add_command (const std::wstring &command_name)
    {
        get().m_impl->add_command(command_name);
    }

    //получить команду
    command_ptr system::get_command (const std::wstring &command_name)
    {
        return get().m_impl->get_command(command_name);
    }

    //есть ли такая команда
    bool system::is_command_present (const std::wstring &command_name)
    {
        return get().m_impl->is_command_present(command_name);
    }

    //отвязать команду ото всех контролов
    void system::detach_command (command_ptr command)
    {
        get().m_impl->detach_command(command);
    }
}