#include "aoc.h"

#define INPUT 277678

// Declare a tuple type for two int32_t values
#define Tuple Point, int32_t, x, int32_t, y
#include "../lib/collections/tuple.template.c"

int main(void)
{
    Point p = new(Point, 0, 0);
    Point dp = new(Point, 1, 0);

    uint32_t max_steps = INPUT, walk_count = 0, step_max = 1, step_count = 0;
    for (uint32_t i = 2; i <= max_steps; i++)
    {
        p->x += dp->x;
        p->y += dp->y;

        step_count += 1;

        if (step_count == step_max)
        {
            step_count = 0;
            walk_count += 1;
            
            int32_t t = dp->y;
            dp->y = dp->x;
            dp->x = -t;
        }

        if (walk_count == 2)
        {
            walk_count = 0;
            step_max += 1;
        }
    }

    uint32_t part1 = abs(p->x) + abs(p->y);

    // Reset points and counters
    p->x = 0;
    p->y = 0;
    dp->x = 1;
    dp->y = 0;
    max_steps = INPUT, walk_count = 0, step_max = 1, step_count = 0;
    uint32_t part2 = 0;

    Point q = new(Point, 0, 0);
    Map points = new(Map, 32, class(Point), class(Int32));
    map_put(points, copy(Point, p), new(Int32, 1));

    for (uint32_t i = 2; part2 <= INPUT; i++)
    {
        p->x += dp->x;
        p->y += dp->y;

        step_count += 1;

        if (step_count == step_max)
        {
            step_count = 0;
            walk_count += 1;
            
            int32_t t = dp->y;
            dp->y = dp->x;
            dp->x = -t;
        }

        if (walk_count == 2)
        {
            walk_count = 0;
            step_max += 1;
        }

        int32_t next_value = 0;
        for (int32_t dx = -1; dx <= 1; dx++)
        {
            for(int32_t dy = -1; dy <= 1; dy++)
            {
                q->x = p->x + dx;
                q->y = p->y + dy;
                Int32 value = map_get(points, q);
                if (value != NULL)
                {
                    next_value += *value;
                }
            }
        }
        map_put(points, copy(Point, p), new(Int32, next_value));
        part2 = next_value;
    }

    // Cleanup
    del(Map, points);
    del(Point, q);
    del(Point, p);
    del(Point, dp);

    ANSWER(475, part1, 279138, part2);
}