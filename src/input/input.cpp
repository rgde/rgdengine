//input.cpp
#include "precompiled.h"
#include "input/input.h"
#include "io/io.h"

namespace input
{

    //указатель на единственный экземпл€р
    //по стандарту и так ноль, но все равно спокойней на душе :)
    CInput* CInput::ms_pInstance = 0;

    //только дл€ конструировани€ синглтона
    CInput::CInput ()
    {
        assert(!ms_pInstance);
        ms_pInstance = this;
    }

    CInput::~CInput ()
    {
        assert(ms_pInstance);
        ms_pInstance = 0;
    }

    //получить указатель на синглтон системы ввода
    CInput& CInput::Get ()
    {
        if (!ms_pInstance)
            ms_pInstance = new CInput;
        return *ms_pInstance;
    }

    //удалить синглтон системы ввода
    void CInput::Destroy ()
    {
        if (ms_pInstance)
            delete ms_pInstance;
        ms_pInstance = 0;
    }

    //инициализировать систему ввода
    void CInput::init (CInputImpl *pImpl)
    {
        m_pImpl.reset(pImpl);
    }

    //изменить режим работы устройств ввода
    bool CInput::SetMode (bool exclusive, bool foreground)
    {
        return Get().m_pImpl->SetMode(exclusive,foreground);
    }

    //загрузить раскладку
    void CInput::LoadFromString (const std::string &sXml)
    {
        Get().m_pImpl->Load(sXml);
    }

    void CInput::LoadFromFile (const std::string &sFileName)
    {
        std::vector<char> data;

        io::CFileSystem &fs    = io::TheFileSystem::Get();
        io::PReadStream stream = fs.findFile(sFileName);
        io::StreamToVector<char>(data, stream);

        Get().m_pImpl->Load(std::string(data.begin(), data.end()));
    }

    //считать из буфера все событи€ от устройств ввода
    void CInput::Update ()
    {
        Get().m_pImpl->Update();
    }

    //сохранить раскладку
    void CInput::Save (std::string &sXml)
    {
        Get().m_pImpl->Save(sXml);
    }

    //получить устройство
    CDevice* CInput::getDevice (EDevice eDeviceName, int indx)
    {
        return Get().m_pImpl->getDevice(eDeviceName, indx);
    }

    //получить устройство
    CDevice* CInput::getDevice (const std::wstring &sDeviceName, int indx)
    {
        return Get().m_pImpl->getDevice(sDeviceName, indx);
    }

    //есть ли такое устройство
    bool CInput::isDevicePresent (EDevice eDeviceName, int indx)
    {
        return Get().m_pImpl->isDevicePresent(eDeviceName,indx);
    }

    //есть ли такое устройство
    bool CInput::isDevicePresent (const std::wstring &sDeviceName, int indx)
    {
        return Get().m_pImpl->isDevicePresent(sDeviceName, indx);
    }

    //добавить команду
    void CInput::addCommand (const std::wstring &sCommandName)
    {
        Get().m_pImpl->addCommand(sCommandName);
    }

    //получить команду
    PCommand CInput::getCommand (const std::wstring &sCommandName)
    {
        return Get().m_pImpl->getCommand(sCommandName);
    }

    //есть ли така€ команда
    bool CInput::isCommandPresent (const std::wstring &sCommandName)
    {
        return Get().m_pImpl->isCommandPresent(sCommandName);
    }

    //отв€зать команду ото всех контролов
    void CInput::detachCommand (PCommand pCommand)
    {
        Get().m_pImpl->detachCommand(pCommand);
    }

} //namespace input
