#include "Grid.cpp"
#include <queue>
#include <map>


struct PrintNode {
	int x;
	int y;
	int distance;
	Direction direction;

	PrintNode(int x, int y, Direction direction, int distance)
	{
		this->x = x;
		this->y = y;
		this->direction = direction;
		this->distance = distance;
	}

	bool operator==(const PrintNode &a) const
	{
		return x == a.x && y == a.y;
	}

	bool operator<(const PrintNode &a) const
	{
		return x < a.x || (x == a.x && y < a.y);
	}
};

struct Node {
	int x;
	int y;
	int distance;
	Direction direction;

	vector<PrintNode> path;

	bool operator>(const Node &a) const
	{
		return distance > a.distance;
	}

	bool operator==(const Node &a) const
	{
		return x == a.x && y == a.y;
	}

	string toString()
	{
		return to_string(x) + "," + to_string(y) + "," + to_string(direction);
	}

    Node(vector<int> pos)
    {
        x = pos[0];
        y = pos[1];
        distance = 0;
    }
};

bool isWall(char c, vector<char> walls){
    for(char wall : walls){
        if(c == wall){
            return true;
        }
    }
    return false;
}

struct DjikstraInput {
    Grid *grid;
    vector<char> walls;
    bool debug;
	bool combineSameWays;
    int(*distanceFunction) (Node &current, Node &next, Direction dir);
	Node start;
	Node end;
	vector<Direction> directions;
};

struct DjikstraOutput {
	int bestScore;
	vector<vector<PrintNode>> paths;
}; 

DjikstraOutput solveDjikstra(const DjikstraInput &solveInput){
	Grid *grid = solveInput.grid;

	vector<Direction> directions = solveInput.directions;

	map<string, int> distances;

	do{
		for(Direction dir : directions){
			Node node(grid->getPos());
			node.direction = dir;
			distances[node.toString()] = INT_MAX;
		}
	}while(grid->next());


	priority_queue<Node,vector<Node>, greater<>> pq;

	pq.push(solveInput.start);

	int bestScore = INT_MAX;
	vector<vector<PrintNode>> paths;

	while(!pq.empty()){
		Node current = pq.top();
		pq.pop();

		if(distances[current.toString()] < current.distance){
			continue;
		}

		if(current.x == solveInput.end.x && current.y == solveInput.end.y  && current.distance <= bestScore){
			bestScore = min(bestScore, current.distance);
			PrintNode pr(current.x, current.y, current.direction, current.distance);
			current.path.push_back(pr);
			paths.push_back(current.path);
			if(!solveInput.combineSameWays){
				break;
			}
		}

		distances[current.toString()] = current.distance;

		for(Direction dir : directions){
			if(dir == grid->getReveseDirection(current.direction)){
				continue;
			}
			grid->setPos(current.x, current.y);
			if(grid->nextInDirection(dir) && !isWall(grid->current(), solveInput.walls)){
				Node next(grid->getPos());
                next.distance = solveInput.distanceFunction(current, next, dir);
				next.direction = dir;
				next.path = current.path;
				next.path.push_back({current.x, current.y, dir, current.distance});

				pq.push(next);
			}

		}
	}

    if(solveInput.debug){
        for(auto node : paths[bestScore]){
            char c = ' ';
            switch (node.direction)
            {
                case Direction::UP:
                    c = '^';
                    break;
                case Direction::DOWN:
                    c = 'v';
                    break;
                case Direction::LEFT:
                    c = '<';
                    break;
                case Direction::RIGHT:
                    c = '>';
                    break;
            }
            grid->setCharAt(node.x, node.y, c);
        }
        grid->print();
    }

	return {bestScore, paths};
}