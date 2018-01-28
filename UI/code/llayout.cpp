#include "ui/ui.hpp"

list_layout_node::list_layout_node()
{
    next = NULL;
    tcolor = ui::tcolor;
    bcolor = ui::bcolor;
    strcpy(str, "");
    print_type = DEFAULT;
}

list_layout_node::~list_layout_node()
{
    delete next;
    next = NULL;
}

//Setters
void list_layout_node::setnext(list_layout_node *n)
{
    next = n;
}

void list_layout_node::setpos(coord p)
{
    pos = p;
}

void list_layout_node::settcolor(int t)
{
    tcolor = t;
}

void list_layout_node::setbcolor(int b)
{
    bcolor = b;
}

void list_layout_node::setstr(const char * s)
{
    strcpy(str, s);
}

void list_layout_node::setprint_type(int p)
{
    print_type = p;
}

//Getters
list_layout_node * list_layout_node::getnext()
{
    return next;
}

coord list_layout_node::getpos()
{
    return pos;
}

int list_layout_node::gettcolor()
{
    return tcolor;
}

int list_layout_node::getbcolor()
{
    return bcolor;
}

const char * list_layout_node::getstr()
{
    return str;
}

int list_layout_node::getprint_type()
{
    return print_type;
}

void list_layout::print(int print_mode)
{
    coord init_pos(wherex(), wherey());
    for(list_layout_node *curr = head; curr; curr = curr->getnext())
    {
        coord c = curr->getpos();
        int new_y = c.y - lines_scrolled;

        coord ctl = getcorner_top_left();
        if(new_y < ctl.y || new_y > ctl.y + height - 1) continue;

        gotoxy(c.x, new_y);
        textcolor(curr->gettcolor());
        textbackground(curr->getbcolor());
        if(print_mode == DISPLAY)
        {
            if(curr->getprint_type() ==
                 list_layout_node::PASSWORD)
            {
                int len = strlen(curr->getstr());
                for(int i = 0; i < len; i++)
                {
                    cprintf("*");
                }
            }
            else if(current->getprint_type() == 
                        list_layout_node::DEFAULT)
            {
                cprintf("%s", curr->getstr());
            }
        }
        else if(print_mode == HIDE)
        {
            int len = strlen(curr->getstr());
            for(int i = 0; i < len; i++)
            {
                cprintf(" ");
            }
        }
    }
    gotoxy(init_pos.x, init_pos.y);
}

list_layout::list_layout()
{
    head = NULL,
    current = NULL;

    tcolor = ui::tcolor;
    bcolor = ui::bcolor;
    tcolor_selected = ui::bcolor;
    bcolor_selected = ui::tcolor;
    tcolor_input = tcolor;
    bcolor_input = bcolor;

    height = ui::scr_height - 1;
    width = ui::scr_width;
    lines_scrolled = 0;
}

list_layout& list_layout::operator<<(coord c)
{
    pos = c;
    return *this;
}

list_layout& list_layout::operator<<(const char *str)
{
    if(!head) //empty list
    {
        head = new list_layout_node;
        current = head;
    }
    else
    {
        list_layout_node *new_node = new list_layout_node;
        current->setnext(new_node);
        current = current->getnext();
    }

    current->setpos(pos);
    current->setstr(str);
    current->settcolor(tcolor);
    current->setbcolor(bcolor);

    print();

    return *this;
}

interactive * list_layout::settext_box(coord c, int is_pwd)
{
    interactive *new_node = new text_box;
    new_node->setpos(c);
    new_node->settcolor(tcolor_input);
    new_node->setbcolor(bcolor_input);

    if(is_pwd) 
    {
        ((text_box *) new_node)->setis_password(1);
        new_node->setprint_type(list_layout_node::PASSWORD);
    }

    current->setnext(new_node);
    current = current->getnext();

    return new_node;
}

interactive * list_layout::setbutton(coord c, const char *s)
{
    button *new_node = new button;
    new_node->setpos(c);
    new_node->settcolor(tcolor);
    new_node->setbcolor(bcolor);
    new_node->settcolor_selected(tcolor_selected);
    new_node->setbcolor_selected(bcolor_selected);
    new_node->setstr(s);

    interactive *n = (interactive *) new_node;
    current->setnext(n);
    current = current->getnext();

    return n;
}

void list_layout::settcolor(int c)
{
    tcolor = c;
    tcolor_input = c;
}

void list_layout::setbcolor(int c)
{
    bcolor = c;
    bcolor_input = c;
}

void list_layout::settcolor_selected(int c)
{
    tcolor_selected = c;
}

void list_layout::setbcolor_selected(int c)
{
    bcolor_selected = c;
}

void list_layout::settcolor_input(int c)
{
    tcolor_input = c;
}

void list_layout::setbcolor_input(int c)
{
    bcolor_input = c;
}

void list_layout::setcorner_top_left(coord c)
{
    hide();

    coord offset = c - corner_top_left;
    //offset isn't a coordinate but it's just a pair of values

    for(list_layout_node *curr = head; curr; curr = curr->getnext())
    {
        coord a = curr->getpos();
        a += offset;
        curr->setpos(a);
    }

    corner_top_left += offset;
    pos += offset;

    display();
}

void list_layout::setheight(int h)
{
    hide();
    height = h;
    display();
}

void list_layout::setwidth(int w)
{
    width = w;
}

void list_layout::setlines_scrolled(int l)
{
    hide();
    lines_scrolled = l;
    display();
}

void list_layout::setpos(coord c)
{
    pos = c;
}

int list_layout::getheight()
{
    return height;
}

int list_layout::getwidth()
{
    return width;
}

int list_layout::getlines_scrolled()
{
    return lines_scrolled;
}

coord list_layout::getpos()
{
    return pos;
}

coord list_layout::getcorner_top_left()
{
    return corner_top_left;
}

void list_layout::display()
{
    print(DISPLAY);
}

void list_layout::hide()
{
    print(HIDE);
}

void list_layout::clear()
{
    list_layout_node *curr = head;
    head = current = NULL;

    while(curr)
    {
        list_layout_node *temp = curr->getnext();
        delete curr;
        curr = temp;
    }

    lines_scrolled = 0;
    pos = corner_top_left;
}
