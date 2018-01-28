#include "ui/ui.hpp"
#include "iface.hpp"

int init_lib_ui::counter = 0;

init_lib_ui::init_lib_ui()
{
	if(counter++ == 0)
	{
		ui::init();
	}
}

int manipulator::index = 0;

manipulator::manipulator()
{
	own_index = index;
	index++;
}

int manipulator::operator==(manipulator m)
{
	return own_index == m.own_index;
}

int ui::scr_height = 0,
	ui::scr_width = 0,
	ui::tcolor = LIGHTGRAY,
	ui::bcolor = BLACK;
manipulator ui::endl,
			ui::centeralign,
			ui::rightalign;

void ui::init()
{
	set_new_handler(ui::my_new_handler);

	ui::clrscr();

	textcolor(ui::tcolor);
	textbackground(ui::bcolor);

	struct text_info info;
	gettextinfo(&info);

	//height and width of screen
	scr_width = (int) info.screenwidth;
	scr_height = (int) info.screenheight;
}

void ui::clrscr()
{
	::clrscr();
}

void ui::my_new_handler()
{
	interface::log_this("Error in allocating memory. Exiting...");
	exit(1);
}

coord::coord(int X, int Y)
{
	x = X;
	y = Y;
}

coord & coord::operator+=(coord b)
{
	x += b.x;
	y += b.y;

	return *this;
}

coord & coord::operator-=(coord b)
{
	x -= b.x;
	y -= b.y;

	return *this;
}

coord coord::operator+(coord b)
{
	coord temp = *this;
	return temp += b ;
}

coord coord::operator-(coord b)
{
	coord temp = *this;
	return temp -= b;
}