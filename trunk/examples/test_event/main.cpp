#include <rgde/engine.h>

struct click_event
{
    click_event (int _x, int _y)
		 : x(_x), y(_y) 
	{
	}

    int x,y;
};

struct Button : public event::sender
{
    void Click (int x, int y)
    {
        send( click_event(x,y) );
    }
};

struct Menu : public event::listener
{
    Menu()
    {
		//получать события только от exit_button
        subscribe<click_event> (&Menu::on_exit, &exit_button);

		//получать события только от exit_button
		subscribe<click_event> (&Menu::on_help, &exit_button);

		//получать события только от btnРудз
        subscribe<click_event> (&Menu::on_help, &help_button);

		//получать события от всех
        subscribe<click_event> (&Menu::on_beep);
    }

    void on_exit (const click_event &e) 
	{
		std::cout << "Menu::on_exit()\n";
	}

    void on_help (const click_event &e) 
	{
		std::cout << "Menu::on_help()\n";
	}

    void on_beep (const click_event &e) 
	{
		std::cout << "Menu::on_beep()\n";
	}

	Button exit_button;
	Button help_button;
};

int main ()
{
    Menu menu;

    menu.exit_button.Click(10,20);
    menu.help_button.Click(30,40);

    return 0;
}