#include <iostream>
#include <vector>
#include "Util.h"

using namespace std;

enum Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    UP_RIGHT,
    UP_LEFT,
    DOWN_RIGHT,
    DOWN_LEFT,
};

class Grid {
    public: 
        Grid() {
            rows = vector<vector<char>>();
            x= 0;
            y = 0;
        }

        static Grid *createFromInput(string input) {
            Grid *grid = new Grid();
            vector<string> lines = splitString(input, '\n');
            for (int i = 0; i < lines.size(); i++) {
                string line = lines[i];
                vector<char> row = vector<char>();
                for (int j = 0; j < line.size(); j++) {
                    row.push_back(line[j]);
                }
                grid->rows.push_back(row);
            }
            return grid;
        }

        static vector<Direction> getDirections() {
            return {
                Direction::RIGHT,
                Direction::LEFT,
                Direction::UP,
                Direction::DOWN,
                Direction::UP_RIGHT,
                Direction::UP_LEFT,
                Direction::DOWN_RIGHT,
                Direction::DOWN_LEFT
            };
        }

        vector<vector<char>> getRows() {
            return rows;
        }

        int getRowsCount() {
            return rows.size();
        }

        vector<char> getRow(int x) {
            if(x < 0 || x >= rows.size()) {
                return vector<char>();
            }
            return rows[x];
        }

        bool next()
        {
            if(y + 1 < rows[x].size()) {
                y++;
            } else if(x + 1 < rows.size()) {
                x++;
                y = 0;
            } else {
                return false;
            }

            return true;
        }

        vector<int> getPos() {
            return {x, y};
        }

        void setPos(int x, int y) {
            this->x = x;
            this->y = y;
        }

        char current() {
            return get(x, y);
        }

        char get(int x, int y) {
            if(x < 0 || x >= rows.size() || y < 0 || y >= rows[0].size()) {
                return '\0';
            }
            return rows[x][y];
        }

        char getByDirection(Direction direction) {

            switch(direction) {
                case Direction::RIGHT:
                    return get(x, y + 1);
                case Direction::LEFT:
                    return get(x, y - 1);
                case Direction::UP:
                    return get(x - 1, y);
                case Direction::DOWN:
                    return get(x + 1, y);
                case Direction::UP_RIGHT:
                    return get(x - 1, y + 1);
                case Direction::UP_LEFT:
                    return get(x - 1, y - 1);
                case Direction::DOWN_RIGHT:
                    return get(x + 1, y + 1);
                case Direction::DOWN_LEFT:
                    return get(x + 1, y - 1);
            }

            return '\0';
        }

        string getStr(Direction direction, int length) {
            string str = "";

            if(direction == Direction::RIGHT) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(x, y + i));
                }
            }
            if (direction == Direction::LEFT) {
                for (int i = 0; i < length; i++) {
                    str.push_back(get(x, y - i));
                }
            }
            if (direction == Direction::UP) {
                for (int i = 0; i < length; i++) {
                    str.push_back(get(x - i, y));
                }
            }
            if (direction == Direction::DOWN) {
                for (int i = 0; i < length; i++) {
                    str.push_back(get(x + i, y));
                }
            }
            if (direction == Direction::UP_RIGHT) {
                for (int i = 0; i < length; i++) {
                    str.push_back(get(x - i, y + i));
                }
            }
            if (direction == Direction::UP_LEFT) {
                for (int i = 0; i < length; i++) {
                    str.push_back(get(x - i, y - i));
                }
            }
            if (direction == Direction::DOWN_RIGHT) {
                for (int i = 0; i < length; i++) {
                    str.push_back(get(x + i, y + i));
                }
            }
            if (direction == Direction::DOWN_LEFT) {
                for (int i = 0; i < length; i++) {
                    str.push_back(get(x + i, y - i));
                }
            }

            return str;
        }

    private:
        vector<vector<char>> rows;
        int x;
        int y;
};