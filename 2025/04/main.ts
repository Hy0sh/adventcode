import { Direction, Grid, Position } from '../grid.class';
import { Main } from '../main.class';

export class Day04 extends Main {

    private grid: Grid = new Grid([]);

    protected parseInput(): void {
        this.grid = new Grid(this.lines);
    }

    protected solve1(): number {
        let nbAviableRollsOfPaper = 0;
        let canMove = true;
        do {
            const adjacentPositions = this.grid.getAdjacentPositions([
                Direction.RIGHT, 
                Direction.LEFT, 
                Direction.DOWN, 
                Direction.UP, 
                Direction.DOWN_RIGHT, 
                Direction.DOWN_LEFT, 
                Direction.UP_RIGHT, 
                Direction.UP_LEFT
            ]);
            if(adjacentPositions.length > 0 && adjacentPositions.filter(position => this.grid.get(position) === '@').length < 4 && this.grid.getCurrentChar() === '@') {
                nbAviableRollsOfPaper++;

            }
            canMove = this.grid.move(Direction.RIGHT);
            if(!canMove) {
                canMove = this.grid.moveToNextLine();
            }
        } while (canMove);

        return nbAviableRollsOfPaper;
    }

    protected solve2(): number {
        let nbAviableRollsOfPaper = 0;
        let canMove = true;
        let positions: Position[] = [];
        do {
            positions = [];
            this.grid.setPosition(new Position(0, 0));
            do {
                const adjacentPositions = this.grid.getAdjacentPositions([
                    Direction.RIGHT, 
                    Direction.LEFT, 
                    Direction.DOWN, 
                    Direction.UP, 
                    Direction.DOWN_RIGHT, 
                    Direction.DOWN_LEFT, 
                    Direction.UP_RIGHT, 
                    Direction.UP_LEFT
                ]);
                if(adjacentPositions.length > 0 && adjacentPositions.filter(position => this.grid.get(position) === '@').length < 4 && this.grid.getCurrentChar() === '@') {
                    nbAviableRollsOfPaper++;
                    positions.push(this.grid.getPosition());
    
                }
                canMove = this.grid.move(Direction.RIGHT);
                if(!canMove) {
                    canMove = this.grid.moveToNextLine();
                }
            } while (canMove);
    
            for(const position of positions) {
                this.grid.setPosition(position);
                this.grid.setCurrentChar('.');
            }
        }while(positions.length > 0);
        

        return nbAviableRollsOfPaper;
    }

}

console.log(new Day04().solve());

