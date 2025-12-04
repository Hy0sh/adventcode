export enum Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    UP_RIGHT,
    UP_LEFT,
    DOWN_RIGHT,
    DOWN_LEFT,
}

export class Position {
    constructor(public x: number, public y: number) {
    }

    getAdjacentInLimits(direction: Direction, limitX: number, limitY: number) : Position | null {
        const nextPosition = this.getAdjacent(direction);
        if(nextPosition.x < 0 || nextPosition.x >= limitX || nextPosition.y < 0 || nextPosition.y >= limitY) {
            return null;
        }
        return nextPosition;
    }

    getAdjacent(direction: Direction) : Position {
        switch (direction) {
            case Direction.RIGHT:
                return new Position(this.x + 1, this.y);
            case Direction.LEFT:
                return new Position(this.x - 1, this.y);
            case Direction.UP:
                return new Position(this.x, this.y - 1);
            case Direction.DOWN:
                return new Position(this.x, this.y + 1);
            case Direction.UP_RIGHT:
                return new Position(this.x - 1, this.y + 1);
            case Direction.UP_LEFT:
                return new Position(this.x - 1, this.y - 1);
            case Direction.DOWN_RIGHT:
                return new Position(this.x + 1, this.y + 1);
            case Direction.DOWN_LEFT:
                return new Position(this.x + 1, this.y - 1);
        }
    }
}

export class Grid {
    private grid: string[][];
    private currentPosition: Position;
    private limitX: number;
    private limitY: number;

    constructor(lines: string[]) {
        this.grid = lines.map(line => line.split(''));
        this.currentPosition = new Position(0, 0);
        this.limitX = this.grid.length;
        this.limitY = this.grid[0]?.length ?? 0;
    }

    get(position: Position) : string | null {
        if(position.x < 0 || position.x >= this.limitX || position.y < 0 || position.y >= this.limitY) {
            return null;
        }
        return this.grid[position.x][position.y];
    }

    setPosition(position: Position) {
        this.currentPosition = position;
    }

    getPosition() : Position {
        return this.currentPosition;
    }

    setCurrentChar(char: string) {
        this.grid[this.currentPosition.x][this.currentPosition.y] = char;
    }

    getCurrentChar() : string {
        return this.grid[this.currentPosition.x][this.currentPosition.y];
    }

    getAdjacentPositions(directions: Direction[]) : Position[] {
        return directions.map(direction => this.currentPosition.getAdjacentInLimits(direction, this.limitX, this.limitY)).filter(position => position !== null);
    }

    move(direction: Direction, steps: number = 1) : boolean {
        for(let i = 0; i < steps; i++) {
            const nextPosition = this.currentPosition.getAdjacentInLimits(direction, this.limitX, this.limitY);
            if(nextPosition === null) {
                return false;
            }
            this.currentPosition = nextPosition;
        }
        return true;
    }

    moveToNextLine() {
        if(!this.move(Direction.DOWN)) {
            return false;
        }
        this.currentPosition = new Position(0, this.currentPosition.y);
        return true;
    }

    print() {
        this.grid.forEach(line => {
            console.log(line.join(''));
        });
    }
}