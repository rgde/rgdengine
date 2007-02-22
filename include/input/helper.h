//helper.h
#pragma once
#include "base.h"
#include <boost/function.hpp>

namespace input
{
    class CButton;
    class CTrigger;
    class CKeyUp;
    class CKeyDown;
    class CRelativeAxis;
    class CAbsoluteAxis;
    class CKeyStream;

    /////////////
    // CHelper //
    /////////////

    //параметр для CHelper::Handler
    struct CHelperEvent
    {
        enum EType
        {
            Button,
            Axis
        } m_eType;

        bool m_bPress;
        int  m_nDelta;
        int  m_nTime;
    };

    //обьект-посредник для получения информации о событиях ввода
    class CHelper
    {
    public:
        typedef boost::function<void(const CHelperEvent&)> Handler;

        CHelper ();
        CHelper (const std::wstring &sCommandName);
        virtual ~CHelper();

        void attach (const std::wstring &sCommandName);
        void detach ();

        void operator += (Handler handler);

    protected:
        friend class CCommand;
        virtual void notify (const CControl &rControl);

    private:
        PCommand           m_pCommand;
        std::list<Handler> m_handlers;
    };

    /////////////
    // CButton //
    /////////////

    //обьект-посредник "кнопка"
    class CButton: public CHelper
    {
    public:
        typedef boost::function<void(bool)> ButtonHandler;
		CButton (): m_nPress(0) {}
        CButton (const std::wstring &sCommandName): CHelper(sCommandName), m_nPress(0) {}

        void operator += (ButtonHandler handler);
        operator bool () const {return m_nPress>0;}

    protected:
        friend class CCommand;
        virtual void notify (const CControl &rControl);

    private:
        int m_nPress;
        std::list<ButtonHandler> m_buttonHandlers;
    };

    //////////////
    // CTrigger //
    //////////////

    //обьект-посредник "триггер"
    class CTrigger: public CHelper
    {
    public:
        typedef boost::function<void(bool)> TriggerHandler;
		CTrigger (): m_bOn(false) {}
        CTrigger (const std::wstring &sCommandName): CHelper(sCommandName), m_bOn(false) {}

        void operator += (TriggerHandler handler);
        operator bool () const {return m_bOn;}

        void setState (bool bOn);

    protected:
        friend class CCommand;
        virtual void notify (const CControl &rControl);

    private:
        bool m_bOn;
        std::list<TriggerHandler> m_triggerHandlers;
    };

    ////////////
    // CKeyUp //
    ////////////

    //обьект-посредник "ОТжатие клавиши"
    class CKeyUp: public CHelper
    {
    public:
        typedef boost::function<void()> KeyUpHandler;
        CKeyUp () {}
        CKeyUp (const std::wstring &sCommandName): CHelper(sCommandName) {}

        void operator += (KeyUpHandler handler);

    protected:
        friend class CCommand;
        virtual void notify (const CControl &rControl);

    private:
        std::list<KeyUpHandler> m_keyupHandlers;
    };

    //////////////
    // CKeyDown //
    //////////////

    //обьект-посредник "НАжатие клавиши"
    class CKeyDown: public CHelper
    {
    public:
        typedef boost::function<void()> KeyDownHandler;
        CKeyDown () {}
        CKeyDown (const std::wstring &sCommandName): CHelper(sCommandName) {}

        void operator += (KeyDownHandler handler);

    protected:
        friend class CCommand;
        virtual void notify (const CControl &rControl);

    private:
        std::list<KeyDownHandler> m_keydownHandlers;
    };

    ///////////////////
    // CRelativeAxis //
    ///////////////////

    //обьект-посредник "относительная ось"
    class CRelativeAxis: public CHelper
    {
    public:
        typedef boost::function<void(int)> RelativeAxisHandler;
        CRelativeAxis () {}
        CRelativeAxis (const std::wstring &sCommandName): CHelper(sCommandName) {}

        void operator += (RelativeAxisHandler handler);

    protected:
        friend class CCommand;
        virtual void notify (const CControl &rControl);

    private:
        std::list<RelativeAxisHandler> m_raxisHandlers;
    };

    ///////////////////
    // CAbsoluteAxis //
    ///////////////////

    //обьект-посредник "абсолютная ось"
    class CAbsoluteAxis: public CHelper
    {
    public:
        typedef boost::function<void(int)> AbsoluteAxisHandler;
		CAbsoluteAxis ():
			m_nMin (0),
			m_nMax (100),
			m_nPos (0)
		{
		}
        CAbsoluteAxis (const std::wstring &sCommandName):
			CHelper(sCommandName),
			m_nMin (0),
			m_nMax (100),
			m_nPos (0)
		{
		}

        void operator += (AbsoluteAxisHandler handler);
        operator int () const {return m_nPos;}

		int  getMin ();
		int  getMax ();
        int  getPos ();
		void setMin (int nMin);
		void setMax (int nMax);
		void setPos (int nPos);

    protected:
        friend class CCommand;
        virtual void notify (const CControl &rControl);

    private:
		int m_nMin;
		int m_nMax;
		int m_nPos;
        std::list<AbsoluteAxisHandler> m_aaxisHandlers;
    };

    ////////////////
    // CKeyStream //
    ////////////////

    //параметр для CKeyStream::KeyStreamHandler
    struct CKeyStreamEvent
    {
        enum EType
        {
            KeyUp,
            KeyDown,
            KeyDownAuto,
            Char
        } m_eType;

        bool m_bPress;
        int  m_nKeyCode;
        int  m_nChar;
    };

    //обьект-посредник "поток символов"
    class CKeyStream: public CHelper
    {
    public:
        typedef boost::function<void(const CKeyStreamEvent&)> KeyStreamHandler;
		CKeyStream () {}

        void operator += (KeyStreamHandler handler);

    protected:
        friend class CCommand;
        virtual void notify (const CControl &rControl);

    private:
        std::list<KeyStreamHandler> m_ksHandlers;
    };

}