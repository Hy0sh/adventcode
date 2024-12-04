#include <iostream>
#include <vector>
#include "Util.h"

using namespace std;

const string DIRECTION_RIGHT = "right";
const string DIRECTION_LEFT = "left";
const string DIRECTION_UP = "up";
const string DIRECTION_DOWN = "down";
const string DIRECTION_UP_RIGHT = "up_right";
const string DIRECTION_UP_LEFT = "up_left";
const string DIRECTION_DOWN_RIGHT = "down_right";
const string DIRECTION_DOWN_LEFT = "down_left";

class Grid {
    public: 
        Grid() {
            rows = vector<vector<char>>();
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

        static vector<string> getAviablesDirections() {
            return {DIRECTION_RIGHT, DIRECTION_LEFT, DIRECTION_UP, DIRECTION_DOWN, DIRECTION_UP_RIGHT, DIRECTION_UP_LEFT, DIRECTION_DOWN_RIGHT, DIRECTION_DOWN_LEFT};
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

        char get(int x, int y) {
            if(x < 0 || x >= rows.size() || y < 0 || y >= rows[0].size()) {
                return '\0';
            }
            return rows[x][y];
        }

        string getStr(int startX, int startY, string direction, int length) {
            if(!isDirectionValid(direction)) {
                return "";
            }

            string str = "";

            if(direction == DIRECTION_RIGHT) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(startX, startY + i));
                }
            } else if(direction == DIRECTION_LEFT) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(startX, startY - i));
                }
            } else if(direction == DIRECTION_UP) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(startX - i, startY));
                }
            } else if(direction == DIRECTION_DOWN) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(startX + i, startY));
                }
            } else if(direction == DIRECTION_UP_RIGHT) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(startX - i, startY + i));
                }
            } else if(direction == DIRECTION_UP_LEFT) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(startX - i, startY - i));
                }
            } else if(direction == DIRECTION_DOWN_RIGHT) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(startX + i, startY + i));
                }
            } else if(direction == DIRECTION_DOWN_LEFT) {
                for(int i = 0; i < length; i++) {
                    str.push_back(get(startX + i, startY - i));
                }
            }

            return str;
        }

        bool isDirectionValid(string direction) {
            return  direction == DIRECTION_RIGHT || 
                    direction == DIRECTION_LEFT || 
                    direction == DIRECTION_UP || 
                    direction == DIRECTION_DOWN || 
                    direction == DIRECTION_UP_RIGHT || 
                    direction == DIRECTION_UP_LEFT || 
                    direction == DIRECTION_DOWN_RIGHT || 
                    direction == DIRECTION_DOWN_LEFT;
        }

    private:
        vector<vector<char>> rows;
};