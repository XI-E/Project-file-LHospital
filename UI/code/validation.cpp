#include "ui/ui.hpp"

int validation::vint(const char *str)
{
    if(!validation::vlong(str)) return 0;

    char *end;
    long l = strtol(str, &end, 10);
    if(l > INT_MAX || l < INT_MIN)
    {
        return 0;
    }

    return 1;
}

int validation::vlong(const char *str)
{
    char *end;
	long val = strtol(str, &end, 10);

	if (errno == ERANGE || (errno != 0 && val == 0))
	{
        //If the converted value would fall
        //out of the range of the result type.
        return 0;   
    }
	if (end == str) 
	{
       //No digits were found.
       return 0;
	}
	
    //Check if the string was fully processed.
    return *end == '\0';
}

int validation::vunsigned_long(const char *str)
{
    char *end;
    unsigned long val = strtoul(str, &end, 10);

    if (errno == ERANGE || (errno != 0 && val == 0))
    {
        return 0;
    }
    if (end == str || *end != '\0')
    {
        return 0;
    }

    int len = strlen(str);
    for(int i = 0; i < len && isspace(str[i]); i++);

    if(str[i] == '-')  return 0;

    return 1;
}

int validation::vstring(const char *str)
{
    return 1;
}

int validation::vchar(const char *str)
{
    if(strlen(str) == 1 && isalnum(str[0]))
    {
        return 1;
    }
    return 0;
}

int validation::vdouble(const char *str)
{
    char *end;
	double val = strtod(str, &end);

	if (errno == ERANGE)
	{
		//If the converted value would fall
		//out of the range of the result type.
		return 0;
	}
	if (end == str)
	{
	   //No digits were found.
	   return 0;
	}

    return *end == '\0';
}

int validation::vfloat(const char *str)
{
    return validation::vdouble(str);
}

validator_f validation::getvalidator
                (int type, validator_f v)
{
    if(v != NULL) return v;

    switch(type)
    {
        case info_tbox::INT:
            return validation::vint;
        case info_tbox::LONG:
            return validation::vlong;
        case info_tbox::UNSIGNED_LONG:
            return validation::vunsigned_long;
        case info_tbox::STRING:
        case info_tbox::PASSWORD:
            return validation::vstring;
        case info_tbox::CHAR:
            return validation::vchar;
        case info_tbox::DOUBLE:
            return validation::vdouble;
        case info_tbox::FLOAT:
            return validation::vfloat;
    }
    
    //TODO: log undefined behaviour
    return NULL;
}