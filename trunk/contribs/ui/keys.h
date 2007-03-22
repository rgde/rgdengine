//keys.h
#pragma once

namespace ui
{

    namespace keys
    {

        enum MouseButtons 
        {
            Left   = 1,
            Middle = 2,
            Right  = 4
        };

        enum Keys
        {
            K_ESCAPE = 0x01,
            K_1,
            K_2,
            K_3,
            K_4,
            K_5,
            K_6,
            K_7,
            K_8,
            K_9,
            K_0,
            K_MINUS,                /* - on main keyboard */
            K_EQUALS,
            K_BACK,                 /* backspace */
            K_TAB,
            K_Q,
            K_W,
            K_E,
            K_R,
            K_T,
            K_Y,
            K_U,
            K_I,
            K_O,
            K_P,
            K_LBRACKET,
            K_RBRACKET,
            K_RETURN,               /* Enter on main keyboard */
            K_LCONTROL,
            K_A,
            K_S,
            K_D,
            K_F,
            K_G,
            K_H,
            K_J,
            K_K,
            K_L,
            K_SEMICOLON,
            K_APOSTROPHE,
            K_GRAVE,                /* accent grave */
            K_LSHIFT,
            K_BACKSLASH,
            K_Z,
            K_X,
            K_C,
            K_V,
            K_B,
            K_N,
            K_M,
            K_COMMA,
            K_PERIOD,               /* . on main keyboard */
            K_SLASH,
            K_RSHIFT,
            K_MULTIPLY,             /* * on numeric keypad */
            K_LMENU,
            K_SPACE,
            K_CAPITAL,
            K_F1,
            K_F2,
            K_F3,
            K_F4,
            K_F5,
            K_F6,
            K_F7,
            K_F8,
            K_F9,
            K_F10,
            K_NUMLOCK,
            K_SCROLL,               /* Scroll Lock */
            K_NUMPAD7,
            K_NUMPAD8,
            K_NUMPAD9,
            K_SUBTRACT,             /* - on numeric keypad */
            K_NUMPAD4,
            K_NUMPAD5,
            K_NUMPAD6,
            K_ADD,                  /* + on numeric keypad */
            K_NUMPAD1,
            K_NUMPAD2,
            K_NUMPAD3,
            K_NUMPAD0,
            K_DECIMAL,              /* . on numeric keypad */
            K_OEM_102 = 0x56,       /* <> or \| on RT 102-key keyboard (Non-U.S.) */
            K_F11,
            K_F12,
            K_F13 = 0x64,           /*                     (NEC PC98) */
            K_F14,                  /*                     (NEC PC98) */
            K_F15,                  /*                     (NEC PC98) */
            K_KANA = 0x70,          /* (Japanese keyboard)            */
            K_ABNT_C1 = 0x73,       /* /? on Brazilian keyboard */
            K_CONVERT = 0x79,       /* (Japanese keyboard)            */
            K_NOCONVERT = 0x7B,     /* (Japanese keyboard)            */
            K_YEN,                  /* (Japanese keyboard)            */
            K_ABNT_C2,              /* Numpad . on Brazilian keyboard */
            K_NUMPADEQUALS = 0x8D,  /* = on numeric keypad (NEC PC98) */
            K_PREVTRACK = 0x90,     /* Previous Track (K_CIRCUMFLEX on Japanese keyboard) */
            K_AT,                   /*                     (NEC PC98) */
            K_COLON,                /*                     (NEC PC98) */
            K_UNDERLINE,            /*                     (NEC PC98) */
            K_KANJI,                /* (Japanese keyboard)            */
            K_STOP,                 /*                     (NEC PC98) */
            K_AX,                   /*                     (Japan AX) */
            K_UNLABELED,            /*                        (J3100) */
            K_NEXTTRACK,            /* Next Track */
            K_NUMPADENTER = 0x9C,   /* Enter on numeric keypad */
            K_RCONTROL,
            K_MUTE = 0xA0,          /* Mute */
            K_CALCULATOR,           /* Calculator */
            K_PLAYPAUSE,            /* Play / Pause */
            K_MEDIASTOP = 0xA4,     /* Media Stop */
            K_VOLUMEDOWN = 0xAE,    /* Volume - */
            K_VOLUMEUP = 0xB0,      /* Volume + */
            K_WEBHOME = 0xB2,       /* Web home */
            K_NUMPADCOMMA,          /* , on numeric keypad (NEC PC98) */
            K_DIVIDE = 0xB5,        /* / on numeric keypad */
            K_SYSRQ = 0xB7,
            K_RMENU,                /* right Alt */
            K_PAUSE = 0xC5,         /* Pause */
            K_HOME  = 0xC7,         /* Home on arrow keypad */
            K_UP,                   /* UpArrow on arrow keypad */
            K_PRIOR,                /* PgUp on arrow keypad */
            K_LEFT = 0xCB,          /* LeftArrow on arrow keypad */
            K_RIGHT= 0xCD,          /* RightArrow on arrow keypad */
            K_END = 0xCF,           /* End on arrow keypad */
            K_DOWN,                 /* DownArrow on arrow keypad */
            K_NEXT,                 /* PgDn on arrow keypad */
            K_INSERT,               /* Insert on arrow keypad */
            K_DELETE,               /* Delete on arrow keypad */
            K_LWIN = 0xDB,          /* Left Windows key */
            K_RWIN,                 /* Right Windows key */
            K_APPS,                 /* AppMenu key */
            K_POWER,                /* System Power */
            K_SLEEP,                /* System Sleep */
            K_WAKE = 0xE3,          /* System Wake */
            K_WEBSEARCH = 0xE5,     /* Web Search */
            K_WEBFAVORITES,         /* Web Favorites */
            K_WEBREFRESH,           /* Web Refresh */
            K_WEBSTOP,              /* Web Stop */
            K_WEBFORWARD,           /* Web Forward */
            K_WEBBACK,              /* Web Back */
            K_MYCOMPUTER,           /* My Computer */
            K_MAIL,                 /* Mail */
            K_MEDIASELECT           /* Media Select */
        };

    } //namespace keys

} //namespace ui