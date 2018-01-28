#include "ui/ui.hpp"

text_box::text_box()
{
    is_password = 0;
}

/*
* Despite trying, this function has grown quite large
* Basically, it allows the user to enter text in the box
* and stores it.
* Returns GOTONEXT or GOTOPREV as per user's request to 
* go to the next or the previous text box respectively
*/
int text_box::input(int a)
{
    coord c = getpos();
    setoffset(a);
    c.y += a;
    gotoxy(c.x, c.y);

    const char *string = getstr();
    char str[100];
    strcpy(str, string);

    string_node *head = new string_node,
                *current = head;

    int len = strlen(str);
    string_node *temp_prev = NULL;
    for(int i = 0; i < len ; i++)
    {
        current->data = str[i];
        current->next = new string_node;
        current->prev = temp_prev;
        temp_prev = current;
        current = current->next;
    }

    //At the end is a box with \0
    current->data = '\0';
    current->prev = temp_prev;
    current = head;

    int state_to_return = -1;

    while(1)
    {
        if(kbhit())
        {
            char ch = interactive::getkey();

            switch((int)ch)
            {
                case TAB :
                case ENTER :
                    state_to_return = GOTONEXT;
                    goto convert_to_str;
                case BACKSPACE :
                    if(current)
                    {
                        if(!current->prev)  break; //No character to be deleted

                        string_node *node_to_delete = current->prev;

                        if(node_to_delete->prev) node_to_delete->prev->next = current;
                        else                     head = current; //If the node to be deleted is the head

                        current->prev = node_to_delete->prev;

                        delete node_to_delete;

                        gotoxy(wherex() - 1, wherey());

                        print_str(head);
                    }
                    break;
                case DELETE:
                    if(current)
                    {
                        if(current->data == '\0') break; //No character to be deleted

                        string_node *node_to_delete = current;

                        if(current->prev) current->prev->next = current->next;
                        else              head = current->next;

                        if(current->next) current->next->prev = current->prev;

                        current = current->next;
                        delete node_to_delete;

                        print_str(head);

                    }
                    break;
                case HOME:
                    gotoxy(c.x, c.y);
                    current = head;
                    break;
                case END:
                    while(current->next)
                    {
                        current = current->next;
                        gotoxy(wherex()+1, wherey());
                    }
                    break;
                case SHIFT_BACKSPACE:
                    state_to_return = BACK;
                    goto convert_to_str;
                case SHIFT_TAB:
                    state_to_return = GOTOPREV;
                    goto convert_to_str;
                case UP:
                    state_to_return = GOTOPREV;
                    goto convert_to_str;
                case DOWN:
                    state_to_return = GOTONEXT;
                    goto convert_to_str;
                case LEFT:
                    if(current->prev)
                    {
                        current = current->prev; 
                        gotoxy(wherex()-1, wherey());   
                    }
                    break;
                case RIGHT: //Right arrow key
                    if(current->next)
                    {
                        current = current->next;
                        gotoxy(wherex()+1, wherey());
                    }
                    break;
                default:
                    if(isprint(ch))
                    {
                        /*
                        * When a new node is to be added, it is added behind 
                        * the current node
                        */

                        string_node *new_node = new string_node;
                        new_node->data = ch;
                        new_node->next = current;
                        new_node->prev = current->prev;

                        if(current->prev) current->prev->next = new_node;
                        else              head = new_node;
                        current->prev = new_node;

                        gotoxy(wherex()+1, wherey());

                        print_str(head);
                    }
            }
        }
    }

    convert_to_str:
    {
        char a[100]; int insert_pointer = 0;
        for(current = head; current; current = current->next)
        {
            a[insert_pointer] = current->data;
            insert_pointer++;
        }
        
        setstr(a);

        //Deleting the list
        current = head;
        head = NULL;
        while(current)
        {
            string_node *temp = current->next;
            delete current;
            current = temp;
        }

        return state_to_return;
    }

}

/*
* Prints the string as represented by a doubly
* linked list whose head is pointed to by the 
* parameter.
*/
void text_box::print_str(string_node *head)
{
    coord init = coord(wherex(), wherey());
    coord c = getpos();
    gotoxy(c.x, c.y + getoffset());
    textcolor(gettcolor());
    textbackground(getbcolor());
    for(string_node *current = head; current; current = current->next)
    {
        if(is_password)
        {
            if(current->data != '\0')
            {
                cprintf("*");
            }
            else
            {
                cprintf(" ");
            }
        }
        else            cprintf("%c", current->data);
    }
    gotoxy(init.x, init.y);
}

void text_box::setis_password(int a)
{
    is_password = a;
}