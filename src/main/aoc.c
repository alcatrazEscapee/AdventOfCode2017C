
#include "aoc.h"

String* read_file(char* file_name, uint32_t estimated_size)
{
    FILE* file = fopen(file_name, "r");
    PANIC_IF_NULL(file, "Unable to open file '%s'", file_name);

    String* s = str_create_with_length(estimated_size);
    char c = '\0';
    while ((c = fgetc(file)) != EOF)
    {
        if (c != '\r')
        {
            str_append_char(s, c);
        }
    }

    fclose(file);

    return s;
}