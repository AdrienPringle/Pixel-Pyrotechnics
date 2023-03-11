#include "stdafx.h"

#include "Level.h"

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <streambuf>
#include <vector>

namespace Level {
    void ReadLevelsFromFile(LEVEL* &l, int &len) {
        levels_size = 0;

        int buf_offset = 0;
        std::ifstream infile(".\\assets\\level.txt");
        LEVEL new_level;
        bool is_start = true;
        int x, y, z;
        float scale;
        std::string line;


        while (std::getline(infile, line)) {
            std::istringstream iss(line);

            if (line.length() <= 0) {
                continue;
            }
            else if (line[0] == '#') {
                continue;
            }
            else if (line[0] == '=') {
                // level_vec.push_back(new_level);
                level_buf[levels_size] = new_level;
                levels_size++;
                is_start = true;
                continue;
            }
            else if (is_start) {
                if (!(iss >> x >> y >> z >> scale)) { throw "line 1 needs 3 numbers"; } // error
                new_level = LEVEL{ x, y, z, 0, scale, &level_data_buf[buf_offset] };
                is_start = false;
                continue;
            }
            else {
                std::istringstream iss(line);
                int a;
                while (iss >> a)
                {
                    level_data_buf[buf_offset] = a;
                    buf_offset++;
                }
                continue;
            }
        }

        l = level_buf;
        len = levels_size;
    }

};