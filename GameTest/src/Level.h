#ifndef _LEVEL_H
#define _LEVEL_H

namespace Level
{
    typedef struct level
    {
        int x;
        int y;
        int z;
        int *data;
    } LEVEL;

    static int LEVEL_1_DATA[5 * 5 * 3] = {
        0, 0, 0,    0, 0, 0,    1, 0, 0,    1, 0, 0,    1, 4, 0,
        0, 0, 0,    1, 1, 0,    1, 1, 0,    1, 0, 0,    3, 0, 0,
        1, 0, 0,    2, 0, 0,    1, 2, 1,    1, 0, 0,    1, 0, 0,
        2, 0, 0,    2, 0, 0,    1, 0, 2,    1, 0, 0,    0, 0, 3,
        1, 0, 0,    1, 0, 0,    1, 0, 0,    0, 0, 0,    0, 0, 0
    };
    static LEVEL LEVEL1 = LEVEL{5, 3, 5, LEVEL_1_DATA};
};
#endif