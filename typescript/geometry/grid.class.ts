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
                return new Position(this.x + 1, this.y - 1);
            case Direction.UP_LEFT:
                return new Position(this.x - 1, this.y - 1);
            case Direction.DOWN_RIGHT:
                return new Position(this.x + 1, this.y + 1);
            case Direction.DOWN_LEFT:
                return new Position(this.x - 1, this.y + 1);
        }
    }

    toString() : string {
        return "(" + this.x + "," + this.y + ")";
    }
}

export class Grid {
    private grid: string[][];
    private currentPosition: Position;
    private limitX: number;
    private limitY: number;
    private nbRotations: number = 0;

    constructor(lines: string[]) {
        this.grid = lines.map(line => line.split(''));
        this.currentPosition = new Position(0, 0);
        this.limitY = this.grid.length;
        this.limitX = this.grid[0]?.length ?? 0;
        this.nbRotations = 0;
    }

    get(position: Position) : string | null {
        if(position.x < 0 || position.x >= this.limitX || position.y < 0 || position.y >= this.limitY) {
            return null;
        }
        return this.grid[position.y][position.x];
    }

    setPosition(position: Position) {
        this.currentPosition = position;
    }

    setCharAt(position: Position, char: string | null) {
        if(char === null) {
            return;
        }
        this.grid[position.y][position.x] = char;
    }

    getPosition() : Position {
        return this.currentPosition;
    }

    setCurrentChar(char: string) {
        this.grid[this.currentPosition.y][this.currentPosition.x] = char;
    }

    getCurrentChar() : string {
        return this.grid[this.currentPosition.y][this.currentPosition.x];
    }

    getAdjacentPositions(directions: Direction[]) : Position[] {
        return directions.map(direction => this.currentPosition.getAdjacentInLimits(direction, this.limitX, this.limitY)).filter(position => position !== null);
    }

    getWidth() : number {
        return this.grid[0]?.length ?? 0;
    }

    getHeight() : number {
        return this.grid.length;
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

    moveToPreviousLine() {
        if(!this.move(Direction.UP)) {
            return false;
        }
        this.currentPosition = new Position(this.limitX - 1, this.currentPosition.y);
        return true;
    }

    scrollGrid(callback: (position: Position) => boolean) {
        while (!callback(this.currentPosition) && (this.move(Direction.RIGHT) || this.moveToNextLine()));
    }


    merge(position: Position, grid: Grid, charMerged: string) {
        for(let i = 0; i < grid.getHeight(); i++) {
            for(let j = 0; j < grid.getWidth(); j++) {
                const newPosition = new Position(position.x + j, position.y + i);
                if(this.get(newPosition) === charMerged) {
                    continue;
                }
                this.setCharAt(newPosition, grid.get(new Position(j, i)));
            }
        }
    }

    rotate() {
        // Rotation 90° horaire: (x, y) -> (height-1-y, x)
        const height = this.grid.length;
        const width = this.grid[0]?.length ?? 0;
        const newGrid: string[][] = [];
        
        for(let j = 0; j < width; j++) {
            const newLine: string[] = [];
            for(let i = height - 1; i >= 0; i--) {
                newLine.push(this.grid[i][j]);
            }
            newGrid.push(newLine);
        }
        
        this.grid = newGrid;
        this.limitX = newGrid[0]?.length ?? 0;
        this.limitY = newGrid.length;
    }

    flipHorizontal() {
        // Miroir horizontal (inversion gauche-droite)
        this.grid = this.grid.map(line => line.slice().reverse());
    }

    print() {
        this.grid.forEach(line => {
            console.log(line.join(''));
        });
    }

    clone() : Grid {
        return new Grid(this.grid.map(line => line.join('')));
    }

    static fromWidthAndHeight(width: number, height: number) : Grid {
        let rows: string[] = [];
        for(let i = 0; i < height; i++) {
            rows.push('.'.repeat(width));
        }
        return new Grid(rows);
    }
}