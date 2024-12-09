#include <iostream>
#include "../Util.h"
#include <map>

using namespace std;

struct Block
{
	int posStart;
	int posEnd;
	int weight;
};

struct Instructions
{
	map<int, vector<Block>> blocks;
	int maxPos;
	int maxId;
};

int64_t checkSum(vector<int> blocks) {
	int64_t sum = 0;
	for(int i=0; i < blocks.size(); i++) {
		if(blocks[i] != -1) {
			sum += blocks[i] * i;
		}
	}

	return sum;
}

int64_t checkSum(Instructions instructions) {
	int64_t sum = 0;
	for(auto &[id, blocks] : instructions.blocks) {
		if(id == -1) {
			continue;
		}
		for(auto &block : blocks) {
			for(int i=block.posStart; i < block.posEnd; i++) {
				sum += id * i;
			}
		}
	}

	return sum;
}

Instructions getInstructions(string input)
{
	Instructions instructions;
	instructions.blocks = map<int, vector<Block>>();
	instructions.maxPos = 0;
	instructions.maxId = 0;

	int currentIndex = 0;

	for (int i = 0; i < input.size(); i++)
	{
		int size = input[i] - '0';
		int currentBlock = -1;
		if (i % 2 == 0)
		{
			currentBlock = instructions.maxId++;
		}
		if (instructions.blocks.find(currentBlock) == instructions.blocks.end())
		{
			instructions.blocks[currentBlock] = {};
		}

		instructions.blocks[currentBlock].push_back({currentIndex, currentIndex + size, size});
		currentIndex += size;
		instructions.maxPos = max(instructions.maxPos, currentIndex);
	}


		instructions.maxId--; // The last id is not used
	return instructions;
}

vector<int> instructionsToVector(Instructions instructions)
{

	vector<int> vectorBlocksPos = vector<int>(instructions.maxPos);
	for (auto &[id, blocksPos] : instructions.blocks)
	{
		for (auto &block : blocksPos)
		{
			for (int i = block.posStart; i < block.posEnd; i++)
			{
				vectorBlocksPos[i] = id;
			}
		}
	}
	return vectorBlocksPos;
}

int64_t solution_1(string &input)
{
	Instructions instructions = getInstructions(input);
	vector<int> blocks = instructionsToVector(instructions);

	for(int i=0; i < blocks.size(); i++) {
		if(blocks[i] == -1) {
			for(int j=blocks.size() - 1; j > i; j--) {
				if(blocks[j] != -1) {
					blocks[i] = blocks[j];
					blocks[j] = -1;
					break;
				}
			}
		}
	}

	return checkSum(blocks);
}

int64_t solution_2(const string &input)
{
	Instructions instructions = getInstructions(input);
	vector<Block> freeSpaces = instructions.blocks[-1];
	for(int i=instructions.maxId; i >= 0; i--) {
		for(Block &block : instructions.blocks[i]) {
			for(Block &freeSpace : instructions.blocks[-1]) {
				if(freeSpace.weight >= block.weight && freeSpace.posStart <= block.posStart) {
					int tmpPosFree = freeSpace.posStart;
					freeSpace.posStart += block.weight;
					freeSpace.weight -= block.weight;
					instructions.blocks[-1].push_back({block.posStart , block.posEnd, block.weight});
					block.posStart = tmpPosFree;
					block.posEnd = tmpPosFree + block.weight;
					break;
				}
			}
		}
	}

	return checkSum(instructions);
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}
