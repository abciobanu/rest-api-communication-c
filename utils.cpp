#include "utils.hpp"

#include <string.h>

char *get_cookie_from_response(char *response)
{
    char *cookie_start = strstr(response, "connect.sid");
    char *cookie_end = strchr(cookie_start, ';');
    *cookie_end = '\0';

    return cookie_start;
}
