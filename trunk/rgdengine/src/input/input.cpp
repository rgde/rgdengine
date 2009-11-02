#include "precompiled.h"

#include <rgde/io/io.h>
#include <rgde/input/input.h>
#include <rgde/input/device.h>


#include "inputimpl.h"

namespace input
{
    Input* Input::ms_instance = 0;

    //только для конструирования синглтона
    Input::Input ()
    {
        assert(!ms_instance);
        ms_instance = this;
    }

    Input::~Input ()
    {
        assert(ms_instance);
        ms_instance = 0;
    }

    Input& Input::get ()
    {
        if (!ms_instance)
            ms_instance = new Input;
        return *ms_instance;
    }

    //удалить синглтон системы ввода
    void Input::destroy ()
    {
        if (ms_instance)
            delete ms_instance;
        ms_instance = 0;
    }

    //инициализировать систему ввода
    void Input::init (input_impl *pImpl)
    {
        m_pImpl.reset(pImpl);
    }

    bool Input::set_mode (bool exclusive, bool foreground)
    {
        return get().m_pImpl->set_mode(exclusive,foreground);
    }

    void Input::LoadFromString (const std::string &sXml)
    {
        get().m_pImpl->Load(sXml);
    }

    void Input::LoadFromFile (const std::string &sFileName)
    {
        std::vector<char> data;

        io::CFileSystem &fs    = io::TheFileSystem::get();
        io::readstream_ptr stream = fs.find(sFileName);
        io::stream_to_vector<char>(data, stream);

        get().m_pImpl->Load(std::string(data.begin(), data.end()));
    }

    void Input::update ()
    {
        get().m_pImpl->update();
    }

    void Input::Save (std::string &sXml)
    {
        get().m_pImpl->Save(sXml);
    }

    device_dx9* Input::get_device (types::device eDeviceName, int indx)
    {
        return get().m_pImpl->get_device(eDeviceName, indx);
    }

    device_dx9* Input::get_device (const std::wstring &sDeviceName, int indx)
    {
        return get().m_pImpl->get_device(sDeviceName, indx);
    }

	Control* Input::GetControl(types::device device, int dev_index, types::control control)
	{
		if (device_dx9* dev = get_device(device, dev_index))
			return dev->get_control(control);

		return NULL;
	}

    //есть ли такое устройство
    bool Input::is_present (types::device eDeviceName, int indx)
    {
        return get().m_pImpl->is_present(eDeviceName,indx);
    }

    //есть ли такое устройство
    bool Input::is_present (const std::wstring &sDeviceName, int indx)
    {
        return get().m_pImpl->is_present(sDeviceName, indx);
    }

    //добавить команду
    void Input::add_command (const std::wstring &command_name)
    {
        get().m_pImpl->add_command(command_name);
    }

    //получить команду
    command_ptr Input::getCommand (const std::wstring &command_name)
    {
        return get().m_pImpl->getCommand(command_name);
    }

    //есть ли такая команда
    bool Input::isCommandPresent (const std::wstring &command_name)
    {
        return get().m_pImpl->isCommandPresent(command_name);
    }

    //отвязать команду ото всех контролов
    void Input::detach_command (command_ptr pCommand)
    {
        get().m_pImpl->detach_command(pCommand);
    }
}