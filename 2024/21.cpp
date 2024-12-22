#include <iostream>
#include "../Util.h"
#include <map>
#include <regex>

using namespace std;

struct Game
{
	int nbLayers;
	vector<string> codes;
	map<string, vector<vector<char>>> grids;
};

Game createGame(const string &input, int nbLayers)
{
	Game game;
	game.codes = splitString(input, '\n');
	game.grids["numpad"] = {
		{'7', '8', '9'},
		{'4', '5', '6'},
		{'1', '2', '3'},
		{' ', '0', 'A'}};
	game.grids["arrowPad"] = {
		{' ', '^', 'A'},
		{'<', 'v', '>'},
	};
	game.nbLayers = nbLayers;

	return game;
}

int getDigitPart(string code)
{
	regex re("\\d+");
	smatch match;
	regex_search(code, match, re);
	int i = stoi(match.str());
	return i;
}

vector<int> getPos(char c, Game *game, string grid)
{
	vector<vector<char>> rows = game->grids[grid];
	for (int i = 0; i < rows.size(); i++)
	{
		for (int j = 0; j < rows[i].size(); j++)
		{
			if (rows[i][j] == c)
			{
				return {i, j};
			}
		}
	}

	return {-1, -1};
}

tuple<char, char> getDirections(vector<int> start, vector<int> end)
{
	char dirHorizontal = '\0';
	char dirVertical = '\0';

	if (end[0] < start[0])
		dirVertical = '^';
	else if (end[0] > start[0])
		dirVertical = 'v';
	if (end[1] < start[1])
		dirHorizontal = '<';
	else if (end[1] > start[1])
		dirHorizontal = '>';

	return {dirHorizontal, dirVertical};
}

int64_t findShortestPath(vector<int> start, vector<int> end, Game *game, int layer, map<string, int64_t> *cache)
{
    // directions horizontale et verticale
    tuple<char, char> directions = getDirections(start, end);
    char dirHorizontal = get<0>(directions);
    char dirVertical = get<1>(directions);
    vector<int> getNextVertical = getPos(dirVertical, game, "arrowPad");
    vector<int> getNextHorizontal = getPos(dirHorizontal, game, "arrowPad");
    vector<int> getAPos = getPos('A', game, "arrowPad");
    int nextLayer = layer - 1;
    int distanceVertical = abs(end[0] - start[0]) - 1; // Distance verticale
    int distanceHorizontal = abs(end[1] - start[1]) - 1; // Distance horizontale

    string key = to_string(start[0]) + to_string(start[1]) + to_string(end[0]) + to_string(end[1]) + to_string(layer);
    
    if (cache->find(key) != cache->end())
    {
        return cache->at(key);
    }

    int64_t result = 0;

    if (layer == 0)
    {
        return 1;
    }
    // Si aucune direction n'est nécessaire
	// on calcule le résultat pour revenir à 'A'.
    else if (dirHorizontal == '\0' && dirVertical == '\0')
    {
        result = findShortestPath(getAPos, getAPos, game, nextLayer, cache);
    }
    // Si seul un mouvement horizontal est nécessaire
    else if (dirHorizontal != '\0' && dirVertical == '\0')
    {
        result = (findShortestPath(getAPos, getNextHorizontal, game, nextLayer, cache) +  // Aller de 'A' à la direction horizontale
                  distanceHorizontal * findShortestPath(getNextHorizontal, getNextHorizontal, game, nextLayer, cache) +  // Déplacer horizontalement
                  findShortestPath(getNextHorizontal, getAPos, game, nextLayer, cache));  // Revenir à 'A'
    }
    // Si seul un mouvement vertical est nécessaire
    else if (dirVertical != '\0' && dirHorizontal == '\0')
    {
        result = (findShortestPath(getAPos, getNextVertical, game, nextLayer, cache) +  // Aller de 'A' à la direction verticale
                  distanceVertical * findShortestPath(getNextVertical, getNextVertical, game, nextLayer, cache) +  // Déplacer verticalement
                  findShortestPath(getNextVertical, getAPos, game, nextLayer, cache));  // Revenir à 'A'
    }
	// deux déplacements sont nécessaires mais :
	// - soit on est sur le pavé de direction avec le dépar sur la première colonne
	// - soit on est sur le pavé numérique avec l'arrivée sur la première colonne et la fin sur la dernière ligne
    else if ((start[1] == 0 && layer < game->nbLayers) || (start[1] == 0 && end[0] == 3 && layer == game->nbLayers))
    {
        result = (findShortestPath(getAPos, getNextHorizontal, game, nextLayer, cache) +  // Aller de 'A' à la direction horizontale
                  distanceHorizontal * findShortestPath(getNextHorizontal, getNextHorizontal, game, nextLayer, cache) +  // Déplacer horizontalement
                  findShortestPath(getNextHorizontal, getNextVertical, game, nextLayer, cache) +  // Passer à la direction verticale
                  distanceVertical * findShortestPath(getNextVertical, getNextVertical, game, nextLayer, cache) +  // Déplacer verticalement
                  findShortestPath(getNextVertical, getAPos, game, nextLayer, cache));  // Revenir à 'A'
    }
	// deux déplacements sont nécessaires mais :
	// - soit l'arrivée est sur la première colonne et le layer est inférieur au nombre maximal de layers
	// - soit l'arrivée est sur la première colonne et le départ est sur la dernière ligne et le layer est égal au nombre maximal de layers
    else if ((end[1] == 0 && layer < game->nbLayers) || (end[1] == 0 && start[0] == 3 && layer == game->nbLayers))
    {
        result = (findShortestPath(getAPos, getNextVertical, game, nextLayer, cache) +  // Aller de 'A' à la direction verticale
                  distanceVertical * findShortestPath(getNextVertical, getNextVertical, game, nextLayer, cache) +  // Déplacer verticalement
                  findShortestPath(getNextVertical, getNextHorizontal, game, nextLayer, cache) +  // Passer à la direction horizontale
                  distanceHorizontal * findShortestPath(getNextHorizontal, getNextHorizontal, game, nextLayer, cache) +  // Déplacer horizontalement
                  findShortestPath(getNextHorizontal, getAPos, game, nextLayer, cache));  // Revenir à 'A'
    }
    else
    {
        // On prend le minimum des deux chemins possibles : 
        // soit d'abord horizontal puis vertical, soit d'abord vertical puis horizontal.
        result = min(
            (findShortestPath(getAPos, getNextHorizontal, game, nextLayer, cache) + // Aller de 'A' à la direction horizontale
             distanceHorizontal * findShortestPath(getNextHorizontal, getNextHorizontal, game, nextLayer, cache) +  // Déplacer horizontalement
             findShortestPath(getNextHorizontal, getNextVertical, game, nextLayer, cache) + // Passer à la direction verticale
             distanceVertical * findShortestPath(getNextVertical, getNextVertical, game, nextLayer, cache) + // Déplacer verticalement
             findShortestPath(getNextVertical, getAPos, game, nextLayer, cache)), // Revenir à 'A'
            (findShortestPath(getAPos, getNextVertical, game, nextLayer, cache) + // Aller de 'A' à la direction verticale
             distanceVertical * findShortestPath(getNextVertical, getNextVertical, game, nextLayer, cache) + // Déplacer verticalement
             findShortestPath(getNextVertical, getNextHorizontal, game, nextLayer, cache) + // Passer à la direction horizontale
             distanceHorizontal * findShortestPath(getNextHorizontal, getNextHorizontal, game, nextLayer, cache) + // Déplacer horizontalement
             findShortestPath(getNextHorizontal, getAPos, game, nextLayer, cache)) // Revenir à 'A'
        );
    }

    cache->insert({key, result});
    
    return result;
}


vector<tuple<char, char>> getPairs(string code)
{
	vector<tuple<char, char>> pairs;
	for (int i = 0; i < code.size() - 1; i++)
	{
		pairs.push_back({code[i], code[i + 1]});
	}
	return pairs;
}

int64_t resolve(const string &input, int nbLayers)
{
	Game game = createGame(input, nbLayers);

	int64_t result = 0;

	map<string, int64_t> cache;

	for (string code : game.codes)
	{
		int digitPart = getDigitPart(code);
		code = 'A' + code; // the initial start position is A
		vector<tuple<char, char>> pairs = getPairs(code);
		int64_t total_presses = 0;
		for (tuple<char, char> pair : pairs)
		{
			vector<int> start_pos = getPos(get<0>(pair), &game, "numpad");
			vector<int> end_pos = getPos(get<1>(pair), &game, "numpad");

			total_presses += findShortestPath(start_pos, end_pos, &game, game.nbLayers, &cache);
		}

		result += total_presses * digitPart;
	}

	return result;
}

int64_t solution_1(const string &input)
{
	return resolve(input, 3);
}

int64_t solution_2(const string &input)
{
	return resolve(input, 26);
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}