#include <string>
#include <vector>

using namespace std;

vector<string> splitString(const string &str, char splitter);
int isNumber(string str);
int stringToInt(string str);
string readInput(char *argv[]);

vector<size_t> findAll(const string &str, const string &sub);