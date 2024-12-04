#include <iostream>
#include "../Util.h"
#include <regex>

using namespace std;

int solution_1(const string &s)
{
	int total = 0;
	regex re("mul\\(([0-9]+),([0-9]+)\\)");
	smatch match;
	for (auto i = sregex_iterator(s.begin(), s.end(), re); i != sregex_iterator(); i++)
	{
		smatch m = *i;
		int a = stringToInt(m[1].str());
		int b = stringToInt(m[2].str());
		total += a * b;
	}
	return total;
}

int solution_2(const string &s)
{
	int total = 0;
	int start = 0;
	int end = s.find("don't()", start);

	while (end != string::npos)
	{
		string substr = s.substr(start, end - start);
		total += solution_1(substr);
		start = s.find("do()", end);
		end = s.find("don't()", start);
	}

	return total;
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}