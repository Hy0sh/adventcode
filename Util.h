#include <string>
#include <vector>

using namespace std;

vector<string> splitString(const string &str, char splitter);
int isNumber(string str);
int stringToInt(string str);
string readInput(char *argv[]);
string trim(const string& str);
vector<size_t> findAll(const string &str, const string &sub);
int sumVector(const vector<int>& vec, int start, int end) ;