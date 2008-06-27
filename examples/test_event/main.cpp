#include <rgde/engine.h>

struct ClickEvent
{
    ClickEvent (int _x, int _y)
		 : x(_x), y(_y) 
	{
	}

    int x,y;
};

struct Button : public event::sender
{
    void Click (int x, int y)
    {
        send_event( ClickEvent(x,y) );
    }
};

struct Menu : public event::listener
{
    Menu()
    {
		//получать события только от btnExit
        subscribe<ClickEvent,Menu> (&Menu::onExit, &btnExit);

		//получать события только от btnExit
		subscribe<ClickEvent,Menu> (&Menu::onHelp, &btnExit);

		//получать события только от btnРудз
        subscribe<ClickEvent,Menu> (&Menu::onHelp, &btnHelp);

		//получать события от всех
        subscribe<ClickEvent,Menu> (&Menu::onBeep);
    }

    void onExit (const ClickEvent &e) 
	{
		std::cout << "Menu::onExit()\n";
	}

    void onHelp (const ClickEvent &e) 
	{
		std::cout << "Menu::onHelp()\n";
	}

    void onBeep (const ClickEvent &e) 
	{
		std::cout << "Menu::onBeep()\n";
	}

	Button btnExit;
	Button btnHelp;
};

int main ()
{
    Menu menu;

    menu.btnExit.Click(10,20);
    menu.btnHelp.Click(30,40);

    return 0;
}