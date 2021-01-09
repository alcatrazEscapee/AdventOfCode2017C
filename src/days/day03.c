#include "aoc.h"

#define INPUT 277678


int main(void)
{
    Vec2i* p = new(Vec2i, 0, 0);
    Vec2i* dp = new(Vec2i, 1, 0);

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

    v2i_set(p, 0, 0);
    v2i_set(dp, 1, 0);
    max_steps = INPUT, walk_count = 0, step_max = 1, step_count = 0;
    uint32_t part2 = 0;

    Vec2i* q = new(Vec2i, 0, 0);
    ArrayHashMap* points = new(ArrayHashMap, 32, class(Vec2i), class(Int32));
    ahm_put(points, copy(Vec2i, p), new(Int32, 1));

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
                v2i_set(q, p->x + dx, p->y + dy);
                int32_t* value = ahm_get(points, q);
                if (value != NULL)
                {
                    next_value += *value;
                }
            }
        }
        ahm_put(points, copy(Vec2i, p), new(Int32, next_value));
        part2 = next_value;
    }

    // Cleanup
    del(ArrayHashMap, points);
    del(Vec2i, q);
    del(Vec2i, p);
    del(Vec2i, dp);

    ANSWER_UINT(3, 1, 475, part1);
    ANSWER_INT(3, 2, 279138, part2);

    return 0;
}