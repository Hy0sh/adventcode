#include <iostream>
#include "../Util.h"
#include <map>
#include <set>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

map<string, set<string>> parseInput(const string &input)
{
	vector<tuple<string, string>> result;
	vector<string> lines = splitString(input, '\n');
	for (const string &line : lines)
	{
		vector<string> parts = splitString(line, '-');
		result.push_back({parts[0], parts[1]});
	}

	map<string, set<string>> graph;
	for (const auto &[from, to] : result)
	{
		graph[from].insert(to);
		graph[to].insert(from);
	}
	return graph;
}

int solution_1(const string &input)
{
	map<string, set<string>>  graph = parseInput(input);
	map<tuple<string, string, string>,bool> threeConnections;
	for (const auto &[from, tos] : graph)
	{
		for (int i = 0; i < tos.size(); i++)
		{
			for (int j = i + 1; j < tos.size(); j++)
			{
				string neighbor1 = *next(tos.begin(), i);
				string neighbor2 = *next(tos.begin(), j);
				if(graph[neighbor1].find(neighbor2) != graph[neighbor1].end())
				{
					if(from.find("t") == 0 || neighbor1.find("t") == 0 || neighbor2.find("t") == 0){
						vector<string> sorted = {from, neighbor1, neighbor2};
						sort(sorted.begin(), sorted.end());
						threeConnections[{sorted[0], sorted[1], sorted[2]}] = true;

					}
				}
			}
		}
	}

	return threeConnections.size();
}

vector<vector<string>> bronKerbosch(set<string> R, set<string> P, set<string> X, map<string, set<string>> &graph)
{
	
	if (P.empty() && X.empty())
	{
		return {vector<string>(R.begin(), R.end())};
	}

	vector<vector<string>> cliques;
	for (int i = 0; i < P.size(); i++)
	{
		string v = *P.begin();
		set<string> new_R = R;
		new_R.insert(v);
		set<string> new_P;
		set<string> new_X;

		for (const string &neighbor : graph[v])
		{
			if (P.find(neighbor) != P.end())
			{
				new_P.insert(neighbor);
			}
			if (X.find(neighbor) != X.end())
			{
				new_X.insert(neighbor);
			}
		}

		vector<vector<string>> new_cliques = bronKerbosch(new_R, new_P, new_X, graph);
		cliques.insert(cliques.end(), new_cliques.begin(), new_cliques.end());

		P.erase(v);
		X.insert(v);
	}

	return cliques;

}

string solution_2(const string &input)
{
	map<string, set<string>>  graph = parseInput(input);
	set<string> allNodes;
	for(auto &[from, _] : graph){
		allNodes.insert(from);
	}

	vector<string> largestClique;
	for(vector<string> clique : bronKerbosch({}, allNodes, {}, graph)){
		if(clique.size() > largestClique.size()){
			largestClique = clique;
		}
	}

	sort(largestClique.begin(), largestClique.end());

	return accumulate(largestClique.begin(), largestClique.end(), string(), [](string a, string b)
					  { return a + (a.length() > 0 ? "," : "") + b; });
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}