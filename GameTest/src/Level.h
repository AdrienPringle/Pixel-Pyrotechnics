#ifndef _LEVEL_H
#define _LEVEL_H

namespace Level
{
    enum BlockType {
        air = 0,
        block = 1,
        brick = 2,
        door = 3,
        barrel = 4,
        man = 5,
    };

    static char *textures[] = {
        "",
        "..\\assets\\blocks.bmp",
        "..\\assets\\bricks.bmp",
        "..\\assets\\door.bmp",
        "..\\assets\\barrel.bmp",
        "..\\assets\\man.bmp",
    };

    typedef struct level
    {
        int x;
        int y;
        int z;
        int *data;
    } LEVEL;

    static int LEVEL_1_DATA[6 * 5 * 3] = {
        0, 0, 0,    0, 0, 0,    1, 0, 0,    1, 1, 1,    1, 4, 0,
        0, 0, 0,    1, 1, 0,    1, 1, 0,    1, 0, 0,    3, 0, 0,
        0, 0, 0,    2, 5, 0,    1, 2, 1,    1, 0, 0,    1, 0, 0,
        2, 0, 0,    2, 0, 0,    1, 0, 2,    1, 0, 0,    0, 0, 3,
        1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
        1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0
    };
    static LEVEL LEVEL1 = LEVEL{5, 3, 6, LEVEL_1_DATA};
};
#endif