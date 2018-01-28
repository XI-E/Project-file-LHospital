#include "ui/ui.hpp"

string_node::string_node()
{
    next = NULL;
    prev = NULL;
    data = '\0';
}

interactive::interactive()
{
    prev = NULL;
    next = NULL;
}

interactive::~interactive()
{
    delete next;
    next = NULL;
    prev = NULL;
}

int interactive::input(int)
{
    return -1;
}

void interactive::setoffset(int o)
{
    offset = o;
}

int interactive::getoffset()
{
    return offset;
}

int interactive::getkey()
{
    char ch = getch();
    switch(ch)
    {
        case 9:     return TAB;
        case 13:    return ENTER;
        case 8:     
        {
            unsigned char far *key_state_byte
                = (unsigned char far*) 0x00400017;
            int key_state = (int) *key_state_byte;

            if(key_state & 2) return SHIFT_BACKSPACE;
            else              return BACKSPACE;
        }
        case 0:     break;
        default:    return ch;
    }

    ch = getch();

    unsigned char far *key_state_byte
    = (unsigned char far*) 0x00400017;
    int key_state = (int) *key_state_byte;

    switch(ch)
    {
        case 72:    return UP;
        case 80:    return DOWN;
        case 75:    return LEFT;
        case 77:    return RIGHT;
        case 15:    if (key_state & 2) return SHIFT_TAB;
        //           ^^ Checks if shift was pressed
        case 83:    return DELETE;
        case 71:    return HOME;
        case 79:    return END;
    }

    return -1;
}
