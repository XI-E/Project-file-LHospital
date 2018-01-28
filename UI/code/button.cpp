#include "ui/ui.hpp"

button::button()
{
    tcolor_selected = BLACK;
    bcolor_selected = LIGHTGRAY;
}

void button::settcolor_selected(int c)
{
    tcolor_selected = c;
}

void button::setbcolor_selected(int c)
{
    bcolor_selected = c;
}

int button::gettcolor_selected()
{
    return tcolor_selected;
}

int button::getbcolor_selected()
{
    return bcolor_selected;
}

int button::input(int offset)
{
    coord c = getpos();
    setoffset(offset);
    c.y += offset;
    gotoxy(c.x, c.y);

    print(1);

    int state_to_return;
    while(1)
    {
        if(kbhit())
        {
            char ch = interactive::getkey();
            switch((int) ch)
            {
                case interactive::ENTER :
                    state_to_return = interactive::CLICKED;
                    goto next;
                case interactive::DOWN :
                case interactive::TAB  :
                    state_to_return = interactive::GOTONEXT;
                    goto next;
                case interactive::UP :
                case interactive::SHIFT_TAB :
                    state_to_return = interactive::GOTOPREV;
                    goto next;
                case interactive::SHIFT_BACKSPACE :
                    state_to_return = interactive::BACK;
                    goto next;
            }
        }
    }

    next:
    {
        if (
            state_to_return == interactive::GOTONEXT ||
            state_to_return == interactive::GOTOPREV
           )
        {
            print(0);
        }

        return state_to_return;
    }
}

void button::print(int isselected)
{
    if(isselected)
    {
        textcolor(tcolor_selected);
        textbackground(bcolor_selected);
    }
    else
    {
        textcolor(gettcolor());
        textbackground(getbcolor());
    }

    coord init_pos(wherex(), wherey());
    coord c = getpos();
    gotoxy(c.x, c.y + getoffset());
    cprintf(getstr());
    gotoxy(init_pos.x, init_pos.y);
}
