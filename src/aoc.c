
#include "aoc.h"

#define CHUNK_SIZE 16

bool read_line(FILE* file, char** line)
{
    uint32_t size = CHUNK_SIZE;
    uint32_t length = 0;
    char chunk[CHUNK_SIZE] = { '\0' };

    if (*line == NULL)
    {
        *line = (char*) malloc(sizeof(char) * CHUNK_SIZE);
    }

    while(fgets(chunk, CHUNK_SIZE, file) != NULL)
    {
        // Update the length of the line
        uint32_t chunk_length = strlen(chunk);

        // Copy the chunk into the line
        memcpy(*line + length, chunk, chunk_length);
        length += chunk_length;
        
        // Check if we've ran into a newline
        if ((*line)[length - 1] == '\n')
        {
            // Null terminate, removing the newline character
            (*line)[length - 1] = '\0';
            if (length - 1 == 0)
            {
                free(*line);
            }
            return length - 1 > 0;
        }

        // If nessecary, double the line length
        if (length >= size - CHUNK_SIZE)
        {
            *line = realloc(*line, size * 2);
            PANIC_IF_NULL(*line, "Memory Overflow at aoc#read_line(FILE*, char*)");
            size *= 2;
        }
    }
    free(*line);
    return false;
}