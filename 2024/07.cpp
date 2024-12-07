#include <iostream>
#include <map>
#include "../Util.h"
#include "../bigint.h"
using namespace std;

struct Instruction
{
	bigint result;
	vector<bigint> numbers;
};

vector<Instruction> parseInstruction(const string &str)
{
	vector<Instruction> instructions;
	for (auto &s : splitString(str, '\n'))
	{
		Instruction instr;
		vector<string> parts = splitString(s, ':');
		instr.result = bigint(parts[0]);
		parts[1] = trim(parts[1]);
		vector<string> parts2 = splitString(parts[1], ' ');
		for (auto &p : parts2)
		{
			instr.numbers.push_back(bigint(p));
		}
		instructions.push_back(instr);
	}
	return instructions;
}

vector<vector<char>> cartesianProduct(const vector<char> &vec, int length)
{
	vector<vector<char>> result;
	for(int i = 0; i < length; i++) {
		vector<vector<char>> newResult;
		for(auto &v : vec) {
			if(result.size() == 0) {
				newResult.push_back({v});
			} else {
				for(auto &r : result) {
					vector<char> newR = r;
					newR.push_back(v);
					newResult.push_back(newR);
				}
			}
		}
		result = newResult;
	}

	return result;
}

bigint resolve(const string &input, vector<char> product)
{
	vector<Instruction> instructions = parseInstruction(input);

	bigint total = 0;
	map<int, vector<vector<char>>> cachProduct;
	for(auto &instr : instructions) {
		if(cachProduct.find(instr.numbers.size()) == cachProduct.end()) {
			cachProduct[instr.numbers.size()] = cartesianProduct(product, instr.numbers.size() - 1);
		}
		vector<vector<char>> products = cachProduct[instr.numbers.size()];
		bigint subTotal = instr.numbers[0];
		for(auto &product : products) {
			subTotal = instr.numbers[0];
			for(int i = 0; i < product.size(); i++) {
				if(product[i] == '+') {
					subTotal += instr.numbers[i + 1];
				} else if (product[i] == '|'){
					subTotal |= instr.numbers[i + 1];
				}
				else {
					subTotal *= instr.numbers[i + 1];
				}
			}
			if(subTotal == instr.result) {
				total += instr.result;
				break;
			}
		}
		
	}
	return total;
}

bigint solution_1(const string &input)
{	
	return resolve(input, {'*', '+'});	
}

bigint solution_2(const string &input)
{
	return resolve(input, {'*', '+', '|'});
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);
	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';
	return 0;
}