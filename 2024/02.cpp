#include <iostream>
#include "../Util.h"

using namespace std;

bool isSafe(int a, int b)
{
	return abs(a - b) >= 1 && abs(a - b) <= 3;
}

int solution_1(const string &input)
{

	int nbsafe = 0;
	for (auto &s : splitString(input, '\n'))
	{
		vector<string> parts = splitString(s, ' ');
		bool safe = true;
		string decOrInc;
		for (int i = 1; i < parts.size(); i++)
		{
			int a = stoi(parts[i - 1]);
			int b = stoi(parts[i]);
			int diff = a - b;
			if(i == 1) {
				decOrInc = diff > 0 ? "dec" : "inc";
			}

			string currentIncDec = diff > 0 ? "dec" : "inc";

			safe = safe && isSafe(a,b) && (decOrInc == currentIncDec);
		}

		nbsafe += safe;
	}

	return nbsafe;
}

int solution_2(const string &input)
{
	int nbsafe = 0;
	for (auto &s : splitString(input, '\n'))
	{
		vector<string> parts = splitString(s, ' ');
		bool safe = true;
		bool errorJocker = false;
		bool forceResetDecOrInc = false;
		string decOrInc;
		int b = stoi(parts[0]);
		for (int i = 1; i < parts.size(); i++)
		{
			int a = stoi(parts[i]);
			int diff = a - b;
			if(i == 1 || forceResetDecOrInc) {
				decOrInc = diff > 0 ? "dec" : "inc";
			}

			string currentIncDec = diff > 0 ? "dec" : "inc";

			safe = safe && isSafe(a,b) && (decOrInc == currentIncDec);
			if (!safe && !errorJocker)
			{
				errorJocker = true;
				safe = true;
				forceResetDecOrInc = i == 1;
				continue;
			}

			b = a;
		}

		nbsafe += safe;
	}

	return nbsafe;
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}