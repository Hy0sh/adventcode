#include <iostream>
#include <map>
#include "../Util.h"
#include <stdint.h>
#include <string>
#include <ranges>
using namespace std;

struct Instruction
{
	int64_t result;
	vector<int64_t> numbers;
};

vector<Instruction> parseInstruction(const string &str)
{
	vector<Instruction> instructions;
	for (auto &s : splitString(str, '\n'))
	{
		Instruction instr;
		vector<string> parts = splitString(s, ':');
		instr.result = strtoll(parts[0].c_str(), nullptr, 10);
		vector<string> parts2 = splitString(trim(parts[1]), ' ');
		for (auto &p : parts2)
		{
			instr.numbers.push_back(strtoll(p.c_str(), nullptr, 10));
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

int64_t resolve(vector<Instruction> instructions, vector<char> product)
{
	int64_t total = 0;
	map<int, vector<vector<char>>> cachProduct;
	for(auto &instr : instructions) {
		if(cachProduct.find(instr.numbers.size()) == cachProduct.end()) {
			cachProduct[instr.numbers.size()] = cartesianProduct(product, instr.numbers.size() - 1);
		}
		vector<vector<char>> products = cachProduct[instr.numbers.size()];
		int64_t subTotal = instr.numbers[0];
		for(auto &product : products) {
			subTotal = instr.numbers[0];
			for(int i = 0; i < product.size(); i++) {
				switch (product[i])
				{
					case '+':
						subTotal += instr.numbers[i + 1];
						break;
					case '|':
						subTotal = strtoll((to_string(subTotal) + to_string(instr.numbers[i + 1])).c_str(), nullptr, 10);
						break;
					case '*':
						subTotal *= instr.numbers[i + 1];
						break;
				
					default:
						cerr << "Error: invalid product" << '\n';
						exit(EXIT_FAILURE);
						break;
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

int64_t solution_1(vector<Instruction> instructions)
{	
	return resolve(instructions, {'*', '+'});	
}

int64_t solution_2(vector<Instruction> instructions)
{
	return resolve(instructions, {'*', '+', '|'});
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);
	vector<Instruction> instructions = parseInstruction(input);
	cout << "Part 1: " << solution_1(instructions) << '\n';
	cout << "Part 2: " << solution_2(instructions) << '\n';
	return 0;
}