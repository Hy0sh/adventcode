#include <map>
#include <string>

using namespace std;

struct Operation
{
	string op;
	string x;
	string y;

	bool operator<(const Operation &other) const
	{
		if (op != other.op)
			return op < other.op;
		if (x != other.x)
			return x < other.x;
		return y < other.y;
	}

	bool operator==(const Operation &other) const
	{
		return op == other.op && x == other.x && y == other.y;
	}
};


bool verifyIntermediateXOR(string wire, int number, map<string, Operation> operations);
bool verifyRecarry(string wire, int number, map<string, Operation> operations);
bool verifyDirectCarry(string wire, int number, map<string, Operation> operations);
bool verifyCarryBit(string wire, int number, map<string, Operation> operations);
bool verifyZ(string wire, int number, map<string, Operation> operations);
bool verify(int number, map<string, Operation> operations);