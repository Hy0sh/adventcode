#include <iostream>
#include "../Util.h"
#include <algorithm>
#include <map>

using namespace std;

vector<vector<int>> parseInput(const string& input) {
    vector<int> left;
    vector<int> right;
    for(auto& s : splitString(input, '\n')) {
        vector<string> parts = splitString(s, ' ');
        for(int i = 0; i < parts.size(); i++) {
            string p = parts[i];
            if( p.size() == 0) continue;
            int num = stringToInt(p);
            if(i % 2 == 0) {
                right.push_back(num);
            } else {
                left.push_back(num);
            }

        }
    }
    return {left, right};
}

int solution_1(const string& input) {
    vector<vector<int>> parsed = parseInput(input);
    vector<int> left = parsed[0];
    vector<int> right = parsed[1];
    
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    int dist = 0;
    for(int i = 0; i < left.size(); i++) {
        dist += abs(left[i] - right[i]);
    }

	return dist;
}

int solution_2(const string& input) {
    vector<vector<int>> parsed = parseInput(input);
    vector<int> left = parsed[0];
    vector<int> right = parsed[1];

    map<int, int> cache;

    int similar = 0;

    for(int i = 0; i < left.size(); i++) {
        if(cache.find(left[i]) == cache.end()) {
            cache[left[i]] = 0;
            for(int j = 0; j < right.size(); j++) {
                if(right[j] == left[i]) {
                    cache[left[i]]++;
                }
            }
            cache[left[i]] = cache[left[i]] * left[i];
        }
        
        similar += cache[left[i]];
        
    }

	return similar;
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}