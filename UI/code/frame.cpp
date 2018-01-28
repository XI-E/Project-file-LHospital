#include "ui/ui.hpp"

int frame::convert(int param)
{
	if(param & ui::top)
	{
		if(param & ui::left)
		{
			return 0;
		}
		else if(param & ui::right)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if(param & ui::bottom)
	{
		if(param & ui::left)
		{
			return 3;
		}
		else if(param & ui::right)
		{
			return 4;
		}
		else
		{
			return 5;
		}
	}
	else if(param & ui::left)
	{
		return 6;
	}
	else if(param & ui::right)
	{
		return 7;
	}

	return -1;
}

void frame::setside_visibility(int side, int visib)
{
	if( visib != 0 && visib != 1)
		return;		//No effect for invalid visibility

	if(side & ui::all)
	{
		for(int i = 0; i < 8; i++)
			sides_visibility[i] = visib;
		return;
	}

	int a = frame::convert(side);
	if(a == -1) return; //-1 indicates invalid side

	sides_visibility[a] = visib;
}

int frame::getside_visibility(int side)
{
	int a = convert(side);

	if(a == -1) return -1; //Wrong side selected

	return sides_visibility[a];
}


frame::frame(coord topleft, int w, int h)
{
	for(int i = 0; i < 8; i++)
	{
		border_chars[i] = '*';
		sides_visibility[i] = 1;
	}
	tcolor = ui::tcolor;
	bcolor = ui::bcolor;
	frame_visibility = 0;
	height = h;
	width = w;
	state = 0;
	corner_top_left = topleft;
}

void frame::display()
{
	print(1);
}

void frame::hide()
{
	print(0);
}

void frame::print(int param)
{
	textcolor(frame::tcolor);
	textbackground(frame::bcolor);

	char visible_chars[8];
	frame_visibility = param;

	int x = corner_top_left.x,
		y = corner_top_left.y;

	int arr[] = {
				ui::top,
				ui::bottom,
				ui::left,
				ui::right,
				ui::top | ui::left,
				ui::top | ui::right,
				ui::bottom | ui::left,
				ui::bottom | ui::right
			};

	char &top = visible_chars[0],
		 &bottom = visible_chars[1],
		 &left = visible_chars[2],
		 &right = visible_chars[3],
		 &top_left = visible_chars[4],
		 &top_right = visible_chars[5],
		 &bottom_left = visible_chars[6],
		 &bottom_right = visible_chars[7];

	for(int i = 0; i < 8; i++)
	{
		if(param == 1 && getside_visibility(arr[i]))
		{
			visible_chars[i] = getborder_char(arr[i]);
		}
		else
		{
			visible_chars[i] = ' ';
		}
	}

	gotoxy(x, y);

	cprintf("%c", top_left);

	for(i = 1; i < width - 1; i++)
	{
		cprintf("%c", top);
	}
	cprintf("%c", top_right);

	for(i = 1; i < height - 1; i++)
	{
		gotoxy(x, y + i); cprintf("%c", left);
		gotoxy(x + width - 1, y + i); cprintf("%c", right);
	}

	gotoxy(x, y + height - 1);
	cprintf("%c", bottom_left);
	for(i = 1; i < width - 1; i++)
	{
		cprintf("%c", bottom);
	}
	cprintf("%c", bottom_right);

	gotoxy(corner_top_left.x, corner_top_left.y);

	textcolor(ui::tcolor);
}

void frame::setvisibility_mode(int param)
{
	frame::setside_visibility(frame::all, 1);
	if(param & nosides)
	{
		frame::setside_visibility(ui::left, 0);
		frame::setside_visibility(ui::right, 0);
	}
	frame::display();
}

//Operator << is used to set border char
frame & frame::operator<<(int side)
{
	int a = frame::convert(side);

	if(a == -1)	return *this; //-1 indicates error

	state = a;

	return *this;
}

frame & frame::operator<<(char border_char)
{
	border_chars[frame::state] = border_char;
	return *this;
}

int frame::getheight()
{
	return height;
}

int frame::getwidth()
{
	return width;
}

coord frame::getcorner_top_left()
{
	return frame::corner_top_left;
}

int frame::getframe_visibility()
{
	return frame_visibility;
}

int frame::gettcolor()
{
	return tcolor;
}

int frame::getbcolor()
{
	return bcolor;
}

char frame::getborder_char(int side)
{
	int a = convert(side);

	if(a == -1)	return '\0'; //Error

	return frame::border_chars[a];
}

void frame::setheight(int h)
{
	if(h > ui::scr_height) return;

	hide();
	frame::height = h;
	display();
}

void frame::setwidth(int w)
{
	if(w > ui::scr_width) return;

	hide();
	frame::width = w;
	display();
}

void frame::settcolor(int c)
{
	tcolor = c;
	display();
}

void frame::setbcolor(int b)
{
	bcolor = b;
	display();
}

void frame::setcorner_top_left(coord c)
{
	hide();
	frame::corner_top_left = c;
	display();
}
