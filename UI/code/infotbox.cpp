#include "ui/ui.hpp"
#include "iface.hpp"

info_tbox::info_tbox()
{
    tbox = NULL;
    data_store = NULL;
    type = OTHER;
    validator = NULL;
}

int info_tbox::setdata()
{
    if(validator(tbox->getstr()) == 0)
    {
        return 0;
    }

    char *fstr;
    switch(type)
    {
        case INT:
        {
            fstr = "%d";
            break;
        }
        case LONG:
        {
            fstr = "%ld";
            break;
        }
        case UNSIGNED_LONG:
        {
            fstr = "%lu";
            break;
        }
        case STRING:
        case PASSWORD:
        {
            char *s = (char *) data_store;
            strcpy(s, tbox->getstr());
            return 1;
        }
        case CHAR:
        {
            fstr = "%c";
            break;
        }
        case DOUBLE:
        {
            fstr = "%g";
            break;
        }
        case FLOAT:
        {
            fstr = "%f";
            break;
        }
        default:
            return 0;
    }

    sscanf(tbox->getstr(), fstr, data_store);

    return 1;
}
