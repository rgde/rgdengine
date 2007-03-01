//main.cpp
#include <iostream>
#include <engine.h>

struct CClickEvent
{
    CClickEvent (int _x, int _y): x(_x), y(_y) {}
    int x,y;
};

struct CButton: public event::CSender
{
    void Click (int x, int y)
    {
        sendEvent<CClickEvent>( CClickEvent(x,y) );
    }
};

struct CMenu: public event::CListener
{
    CButton btnExit;
    CButton btnHelp;

    CMenu ()
    {
        subscribe<CClickEvent,CMenu> (&CMenu::onExit, &btnExit); //получать события только от btnExit
        subscribe<CClickEvent,CMenu> (&CMenu::onHelp, &btnHelp); //получать события только от btnРудз
        subscribe<CClickEvent,CMenu> (&CMenu::onBeep);           //получать события от всех
    }

    void onExit (const CClickEvent &e) {std::cout << "CMenu::onExit()\n";}
    void onHelp (const CClickEvent &e) {std::cout << "CMenu::onHelp()\n";}
    void onBeep (const CClickEvent &e) {std::cout << "CMenu::onBeep()\n";}
};

int main ()
{
    CMenu menu;

    menu.btnExit.Click(10,20);
    menu.btnHelp.Click(30,40);

    return 0;
}