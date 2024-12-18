#include <iostream>
#include <vector>
#include "Util.h"
#include <climits>

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
            debug = false;
        }

        Grid(int length, int height) {
            rows = vector<vector<char>>(length, vector<char>(height, '.'));
            x = 0;
            y = 0;
            debug = false;
        }

        static Grid* createFromVectors(vector<vector<char>> rows) {
            Grid *grid = new Grid();
            grid->rows = rows;
            return grid;
        }

        static Grid* createFromInput(string input) {
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

        bool nextInDirection(Direction direction, int steps = 1)
        {
            switch(direction) {
                case Direction::RIGHT:
                    if(y + steps < rows[x].size()) {
                        y++;
                    } else {
                        return false;
                    }
                    break;
                case Direction::LEFT:
                    if(y - steps >= 0) {
                        y--;
                    } else {
                        return false;
                    }
                    break;
                case Direction::UP:
                    if(x - steps >= 0) {
                        x--;
                    } else {
                        return false;
                    }
                    break;
                case Direction::DOWN:
                    if(x + steps < rows.size()) {
                        x++;
                    } else {
                        return false;
                    }
                    break;
                case Direction::UP_RIGHT:
                    if (x - steps >= 0 && y + steps < rows[x].size()) {
                        x--;
                        y++;
                    } else {
                        return false;
                    }
                    break;
                case Direction::UP_LEFT:
                    if (x - steps >= 0 && y - steps >= 0) {
                        x--;
                        y--;
                    } else {
                        return false;
                    }
                    break;
                case Direction::DOWN_RIGHT:
                    if (x + steps < rows.size() && y + steps < rows[x].size()) {
                        x++;
                        y++;
                    } else {
                        return false;
                    }
                    break;
                case Direction::DOWN_LEFT:
                    if (x + steps < rows.size() && y - steps >= 0) {
                        x++;
                        y--;
                    } else {
                        return false;
                    }
                    break;
            }

            return true;
        }

        Direction getReveseDirection(Direction direction) {
            switch(direction) {
                case Direction::RIGHT:
                    return Direction::LEFT;
                case Direction::LEFT:
                    return Direction::RIGHT;
                case Direction::UP:
                    return Direction::DOWN;
                case Direction::DOWN:
                    return Direction::UP;
                case Direction::UP_RIGHT:
                    return Direction::DOWN_LEFT;
                case Direction::UP_LEFT:
                    return Direction::DOWN_RIGHT;
                case Direction::DOWN_RIGHT:
                    return Direction::UP_LEFT;
                case Direction::DOWN_LEFT:
                    return Direction::UP_RIGHT;
            }

            return direction;
        }

        void backwardDirection(Direction previousdirection) {
            Direction direction;
            switch (previousdirection)
            {
                case Direction::RIGHT:
                    direction = Direction::LEFT;
                break;
                case Direction::LEFT:
                    direction = Direction::RIGHT;
                break;
                case Direction::UP:
                    direction = Direction::DOWN;
                break;
                case Direction::DOWN:
                    direction = Direction::UP;
                break;
                case Direction::UP_RIGHT:
                    direction = Direction::DOWN_LEFT;
                break;
                case Direction::UP_LEFT:
                    direction = Direction::DOWN_RIGHT;
                break;
                case Direction::DOWN_RIGHT:
                    direction = Direction::UP_LEFT;
                break;
                case Direction::DOWN_LEFT:
                    direction = Direction::UP_RIGHT;
                break;
            }
            nextInDirection(direction);
        }

        vector<int> getPos() {
            return {x, y};
        }

        int countChar(char c) {
            int count = 0;
            for(int i = 0; i < rows.size(); i++) {
                for(int j = 0; j < rows[i].size(); j++) {
                    if(rows[i][j] == c) {
                        count++;
                    }
                }
            }
            return count;
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

        int getX() {
            return x;
        }

        int getY() {
            return y;
        }

        int getCurrentIntValue() {
            if(current() == '\0') {
                return INT_MIN;
            }
            return current() - '0';
        }

        void setDebug(bool debug) {
            this->debug = debug;
        }

        void setDebugChar(char debugChar) {
            this->debugChar = debugChar;
        }

        char getDebugChar() {
            return debugChar;
        }

        void setCharAt(int x, int y, char c) {
            if(x < 0 || x >= rows.size() || y < 0 || y >= rows[0].size()) {
                return;
            }
            rows[x][y] = c;
        }

        vector<vector<int>> getCharPositions(char c) {
            vector<vector<int>> positions;
            for(int i = 0; i < rows.size(); i++) {
                for(int j = 0; j < rows[i].size(); j++) {
                    if(rows[i][j] == c) {
                        positions.push_back({i, j});
                    }
                }
            }
            return positions;
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

        vector<int> find(char c) {
            for(int i = 0; i < rows.size(); i++) {
                for(int j = 0; j < rows[i].size(); j++) {
                    if(rows[i][j] == c) {
                        return {i, j};
                    }
                }
            }

            return {-1, -1};
        }

        vector<int> findNext(char c, Direction direction) {
            while(current() != c && current() != '\0' && nextInDirection(direction)) {
                if(debug && current() != c && current() != '\0') {
                    rows[x][y] = 'x';
                }
                if(current() == c || current() == '\0') {
                    return {x, y};
                }

            }
            
            return {INT_MIN};
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

        void print() {
            cout << "Grid:\n";
            for(int i = 0; i < rows.size(); i++) {
                for(int j = 0; j < rows[i].size(); j++) {
                    cout << rows[i][j];
                }
                cout << '\n';
            }
        }

    private:
        vector<vector<char>> rows;
        int x;
        int y;
        bool debug;
        char debugChar = 'x';
};