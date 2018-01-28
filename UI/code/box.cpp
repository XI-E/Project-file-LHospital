#include "ui/ui.hpp"
#include "iface.hpp"

line::line()
{
    strcpy(left, "");
    strcpy(middle, "");
    strcpy(right, "");
    width = ui::scr_width - 2;
    tcolor = ui::tcolor;
    bcolor = ui::bcolor;
    corner_top_left = coord(0,0);
}

void line::display()
{
    print(1);
}

void line::hide()
{
    print(0);
}

void line::clear()
{
    hide();
    strcpy(left, "");
    strcpy(middle, "");
    strcpy(right, "");
    display();
}

void line::print(int mode)
{
    coord curr_pos = coord(wherex(), wherey()),
    &ctl = corner_top_left;
    gotoxy(ctl.x, ctl.y);
    textcolor(tcolor);
    textbackground(bcolor);

    if(mode == 1)
    {
        cprintf("%s", left);
    }
    else
    {
        for(int i = 0; i < strlen(left); i++)
        {
            cprintf(" ");
        }
    }

    gotoxy(ctl.x + (width - strlen(middle)) / 2,
                wherey());
    if(mode == 1)
    {
        cprintf("%s", middle);
    }
    else
    {
        for(int i = 0; i < strlen(middle); i++)
        {
            cprintf(" ");
        }
    }

    gotoxy(ctl.x + width - strlen(right), wherey());
    if(mode == 1)
    {
        cprintf("%s", right);
    }
    else
    {
        for(int i = 0; i < strlen(right); i++)
        {
            cprintf(" ");
        }
    }

    gotoxy(curr_pos.x, curr_pos.y);
}

int default_back_func()
{
    return 0;
}

int box::wrap(char str[], int length, int return_one_line)
{
	int num_lines = 1;
    char out_str[300] = "";
    
    int pos_old_newline = -1,
        pos_curr_newline = -1;

    int len_str = strlen(str);

    //Iterating upto len_str because the '\0' at the end of the string
    //would be interpreted as a newline
    for(int i = 0; i <= len_str; i++)
    {
        if(str[i] == '\n' || i == len_str)
        {
            pos_old_newline = pos_curr_newline;
            pos_curr_newline = i;

            if(pos_curr_newline != len_str) num_lines++;
            
            int chars_read = 0,
                read,
                written = 0;

            char word[30];

            str[pos_curr_newline] = '\0';

            char *line = str + pos_old_newline + 1;
            while(sscanf(line + chars_read, "%s%n", word, &read) > 0)
            {
                int word_len = strlen(word);
                if(written + word_len  > length)
                {
                    num_lines++;
                    sprintf(out_str + strlen(out_str), "\n%s ", word);
                    written = word_len + 1;
                }
				else if(written + word_len < length)
                {
                    sprintf(out_str + strlen(out_str), "%s ", word);
                    written += word_len + 1;
                }
                else //Not to add the space at the end if the line just completes
                {
                    sprintf(out_str + strlen(out_str), "%s", word);
                    written += word_len;
                }
                
                chars_read += read;
            }

            if(pos_curr_newline != len_str)
                sprintf(out_str + strlen(out_str), "\n"); 
                str[pos_curr_newline] = '\n';
        }
    }
    
    //An extra space is at the end of the string which has to be removed
    //out_str[strlen(out_str) - 1] = '\0';
    sprintf(str, "%s", out_str);
    
    if(!return_one_line)   return num_lines;

    len_str = strlen(str);

    for(i = 0; i <= len_str; i++)   
    {
        if(i == len_str)
        {
            break;
        }
        else if(str[i] == '\n')
        {
            str[i] = '\0';
            break;
        }
    }

    return num_lines;
}

void box::set_tbox(int data_type, void *ptr)
{
    text_box *new_tbox;

    if(data_type == info_tbox::PASSWORD)
    {
        new_tbox = 
            (text_box *) layout.settext_box(pos_pointer, 1);
    }
    else
    {
        new_tbox = 
            (text_box *) layout.settext_box(pos_pointer);
    }
    
    if(default_toggle)
    {
        default_toggle = 0;
        new_tbox -> setstr(default_text);
    }

    pos_pointer.y++;
    pos_pointer.x = layout.getcorner_top_left().x;

    list_interactive[index_interactive]
        = (interactive *) new_tbox;
    info_tbox &t = list_tbox[index_tbox];
    index_interactive++;
    index_tbox++;

    t.tbox = new_tbox;
    t.type = data_type;
    t.data_store = ptr;
    t.validator = validation::getvalidator(data_type, temp_validator);

    temp_validator = NULL;
}

manipulator box::setheader,
            box::setfooter,
            box::setpassword;

box::box(coord c, int w, int h) : f(c, w, h)
{
    width = w;
    height = h;
    padding = 1;

    corner_top_left = c;

    f << (ui::top | ui::left) << (char) 201
      << (ui::bottom | ui::left) << (char) 200
      << (ui::top | ui::right) << (char) 187
      << (ui::bottom | ui::right) << (char) 188
      << ui::top << (char) 205
      << ui::bottom << (char) 205
      << ui::left << (char) 186
      << ui::right << (char) 186;

    layout.setwidth(w - 2 - 2 * padding);
    layout.setheight(h - 2 - 2 * padding);
    //                   ^bcoz of frame
    layout.setcorner_top_left(c + 
        coord(1 + padding, 1 + padding));

    pos_pointer = layout.getcorner_top_left();

    for(int i = 0; i < 30; i++)
    {
        list_interactive[i] = NULL;
    }
    exit_btn = NULL;
    index_interactive = index_tbox = 0;
    center_toggle = 0;
    default_toggle = 0;
    right_toggle = 0;
    header_toggle = 0;
    footer_toggle = 0;
    password_toggle = 0;
    strcpy(default_text, "");
    temp_validator = NULL;

    header.width = footer.width = w - 2;
    header.corner_top_left = c + coord(1,0);
    footer.corner_top_left = c + coord(0, h-1);

    back_func = default_back_func;

    f.display();
}

coord box::getcorner_top_left()
{
    return corner_top_left;
}

int box::getheight()
{
    return height;
}

int box::getwidth()
{
    return width;
}

int box::getpadding()
{
    return padding;
}

void box::setcorner_top_left(coord c)
{
    corner_top_left = c;
    f.setcorner_top_left(c);
    c += coord(1 + padding, 1 + padding);
    layout.setcorner_top_left(c);

    pos_pointer = c;
}

void box::setheight(int h)
{
    height = h;
    f.setheight(h);
    layout.setheight(h - 2 - 2 * padding);
}

void box::setpadding(int p)
{
    hide();
    padding = p;
    setheight(height);
    display();
}

void box::settcolor(int c)
{
    layout.settcolor(c);
}

void box::setbcolor(int c)
{
    layout.setbcolor(c);
}

void box::settcolor_selected(int c)
{
    layout.settcolor_selected(c);
}

void box::setbcolor_selected(int c)
{
    layout.setbcolor_selected(c);
}

void box::settcolor_input(int c)
{
    layout.settcolor_input(c);
}

void box::setbcolor_input(int c)
{
    layout.setbcolor_input(c);
}

void box::setback_func( int(*f)(void) )
{
    back_func = f;
}

box & box::operator<< (char *inp_str)
{
    char string[100];
    char *str = string;
    strcpy(string, inp_str);

    coord c = layout.getcorner_top_left();

    if(header_toggle || footer_toggle)
    {
        line *lp;
        if(header_toggle)
        {
            header_toggle = 0;
            lp = &header;
        }        
        if(footer_toggle)
        {
            footer_toggle = 0;
            lp = &footer;
        }
        line &l = *lp;

        int len = strlen(string);
        if(center_toggle)
        {
            center_toggle = 0;
            if(len <= l.width)
            {
                if((l.width - len) / 2 > strlen(l.left))
                {
                    strcpy(l.middle, string);
                }
            }
        }
        else if(right_toggle)
        {
            right_toggle = 0;
            if(len <= l.width)
            {
                if(len < (l.width - strlen(l.middle)) / 2)
                {
                    strcpy(l.right, string);
                }
            }
        }
        else
        {
            if(len < (l.width - strlen(l.middle)) / 2)
            {
                strcpy(l.left, string);
            }
        }

        //Printing the newly set line
        l.hide();
        l.display();

        return *this;
    }

    if(center_toggle)
    {
        int len = strlen(string);
        center_toggle = 0;
        if(len <= layout.getwidth())
        {
            int x_center_pos = 
                c.x + (layout.getwidth() - len) / 2;
            
            if(pos_pointer.x > x_center_pos)
            {
                pos_pointer.y++;
            }
            pos_pointer.x = x_center_pos;
            layout << pos_pointer << str;
            pos_pointer.x += len;
            return *this;
        }
    }
    else if(right_toggle)
    {
        int len = strlen(string);
        right_toggle = 0;
        if(len <= layout.getwidth())
        {
            int x_right_pos =
                c.x + (layout.getwidth() - len);
            
            if(pos_pointer.x > x_right_pos)
            {
                pos_pointer.y++;
            }
            pos_pointer.x = x_right_pos;
            layout << pos_pointer << str;
            pos_pointer.y++;
            pos_pointer.x = c.x;
            return *this;
        }
    }
    
    int num_lines;

    if(pos_pointer.x != c.x)
    {
        int remaining_space = layout.getwidth() - 
        (pos_pointer.x - layout.getcorner_top_left().x);
        char s[100];
        strcpy(s, str);
        num_lines = wrap(s, remaining_space, 1);

        layout << pos_pointer << s;

        if(num_lines > 1)   
        {
            pos_pointer.x = c.x;
            pos_pointer.y++;
        }
        else
        {
            pos_pointer.x += strlen(s);
        }

        if (num_lines == 1 || 
            str[strlen(str) - 1] == '\n')   return *this;

        str += strlen(s); //There's an extra space at the end of s
    }

    num_lines = wrap(str, layout.getwidth());

    int len_str = strlen(str),
        pos_curr_newline = -1,
        chars_to_forward = 0;

    for(int i = 0; i < len_str; i++)
    {
        if(str[i] == '\n')
        {
            pos_curr_newline = i;

            str[pos_curr_newline] = '\0';
            layout << pos_pointer << str + chars_to_forward;
            pos_pointer.y++;

            chars_to_forward += 
                strlen(str + chars_to_forward) + 1;
        }
    }

    if(i == len_str - 1)    return *this;

    layout << pos_pointer << str + chars_to_forward;
    pos_pointer.x += strlen(str + chars_to_forward);

    return *this;
}

box & box::operator<<(char ch)
{
    char str[] = {ch, '\0'};
    return (*this) << str;
}

box & box::operator<<(int i)
{
    return (*this) << (long) i;
}

box & box::operator<<(long l)
{
    char str[100];
    sprintf(str,"%ld", l);
    return (*this) << str;
}

box & box::operator<<(unsigned long ul)
{
    char str[100];
    sprintf(str, "%lu", ul);
    return (*this) << str;
}

box & box::operator<<(double d)
{
    char str[100];
    sprintf(str, "%g", d);
    return (*this) << str;
}

box & box::operator<<(float f)
{
    char str[100];
    sprintf(str, "%f", f);
    return (*this) << str;
}

box & box::operator<<(manipulator m)
{
    if(m == ui::endl)
    {
        pos_pointer.y++;
        pos_pointer.x = layout.getcorner_top_left().x;
    }
    else if(m == ui::centeralign)
    {
        center_toggle = 1;
    }
    else if(m == ui::rightalign)
    {
        right_toggle = 1;
    }
    else if(m == box::setheader)
    {
        header_toggle = 1;
    }
    else if(m == box::setfooter)
    {
        footer_toggle = 1;
    }
    return *this;
}

box & box::operator>>(char *&s)
{
    if(password_toggle)
    {
        password_toggle = 0;
        set_tbox(info_tbox::PASSWORD, (void *) s);
    }
    else
    {
        set_tbox(info_tbox::STRING, (void *) s);
    }
    return *this;
}

box & box::operator>>(char &ch)
{
    set_tbox(info_tbox::CHAR, (void *) &ch);
    return *this;
}

box & box::operator>>(int &i)
{
    set_tbox(info_tbox::INT, (void *) &i);
    return *this;
}

box & box::operator>>(long &l)
{
    set_tbox(info_tbox::LONG, (void *) &l);
    return *this;
}

box & box::operator>>(unsigned long &ul)
{
    set_tbox(info_tbox::UNSIGNED_LONG, (void *) &ul);
    return *this;
}

box & box::operator>>(double &d)
{
    set_tbox(info_tbox::DOUBLE, (void *) &d);
    return *this;
}

box & box::operator>>(float &f)
{
    set_tbox(info_tbox::FLOAT, (void *) &f);
    return *this;
}

box & box::operator>>(manipulator m)
{
    if(m == box::setpassword)
    {
        password_toggle = 1;
    }
    return *this;
}

box & box::operator>>(int (*f)(const char *))
{
    temp_validator = f;
    return *this;
}

void box::setexit_button(char *str)
{
    coord c = layout.getcorner_top_left();
    if(pos_pointer.x != c.x)
        pos_pointer.y++;
    
    pos_pointer.x = c.x + (layout.getwidth() - strlen(str)) / 2;

    button * new_btn = 
        (button *) layout.setbutton(pos_pointer, str);

    pos_pointer.y++;
    pos_pointer.x = c.x;

    exit_btn = new_btn;
    list_interactive[index_interactive]
        = (interactive *) new_btn;
    index_interactive++;
}

void box::setdefault(char *s)
{
    default_toggle = 1;
    strcpy(default_text, s);
}

void box::setdefault(char c)
{
    char s[] = {c, '\0'};
    setdefault(s);
}

void box::setdefault(int i)
{
    setdefault( (long) i);
}

void box::setdefault(long l)
{
    char s[100];
    sprintf(s, "%ld", l);
    setdefault(s);
}

void box::setdefault(unsigned long ul)
{
    char s[100];
    sprintf(s, "%lu", ul);
    setdefault(s);
}

void box::setdefault(double d)
{
    char s[100];
    sprintf(s, "%g", d);
    setdefault(s);
}

void box::setdefault(float f)
{
    char s[100];
    sprintf(s, "%f", f);
    setdefault(s);
}

void box::loop()
{
    int j = 0,
    lines_scrolled = layout.getlines_scrolled(),
    height = layout.getheight(),
    index_last_interactive = index_interactive - 1,
    &ili = index_last_interactive;
    int temp_tbox_color, temp_index = -1;

    inf_loop:
    while(1)
    {
        coord c = list_interactive[j]->getpos(),
              ctl = layout.getcorner_top_left();
        if(c.y - ctl.y - lines_scrolled + 1 > height)
		{
			lines_scrolled = c.y - ctl.y - height + 1;
		}
		else if(c.y - lines_scrolled < ctl.y)
		{
            lines_scrolled = 
                c.y - ctl.y;
		}

		layout.setlines_scrolled(lines_scrolled);
        int response = 
            list_interactive[j]->input(-lines_scrolled);

        if(response == interactive::GOTONEXT)
        {
            if(j < ili) j++; else j = 0;
        }
        else if(response == interactive::GOTOPREV)
        {
            if(j > 0) j--; else j = ili;
        }
        else if(response == interactive::CLICKED)
        {
            break;
        }
        else if(response == interactive::BACK && back_func())
        {
            return;
        }
    }

    interface::clear_error();
    if(temp_index != -1)
    {
        list_tbox[temp_index].tbox->settcolor(temp_tbox_color);
    }
    for(int i = 0; i < index_tbox; i++)
    {
        if(list_tbox[i].setdata() == 0)
        {
            interface::error("INVALID INPUT!");
            temp_tbox_color = list_tbox[i].tbox->gettcolor();
            list_tbox[i].tbox->settcolor(RED);
            temp_index = i;
            goto inf_loop;
        }
    }
}

void box::display()
{
    layout.display();
    f.display();
    header.display();
    footer.display();
}

void box::hide()
{
    layout.hide();
    f.hide();
    header.hide();
    footer.hide();
}

void box::clear()
{
    layout.hide();
    layout.clear();
    pos_pointer = layout.getcorner_top_left();
    index_interactive = index_tbox = 0;
    exit_btn = NULL;
    f.display();
}

void box::setheader_tcolor(int c)
{
    header.tcolor = c;
}

void box::setfooter_tcolor(int c)
{
    footer.tcolor = c;
}

void box::clear_header()
{
    header.clear();
    f.display();
    footer.display();
}

void box::clear_footer()
{
    footer.clear();
    f.display();
    header.display();
}