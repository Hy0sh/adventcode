import { Direction, Grid, Position } from '../geometry/grid.class';
import { Main } from '../main.class';

export class Day07 extends Main {

    private grid: Grid = new Grid([]);
    protected parseInput(): void {
        this.grid = new Grid(this.lines);
    }
    
    protected solve1(): number {
        // find the start position
        this.grid.scrollGrid((position: Position) => this.grid.getCurrentChar() === 'S');
        const beams: Map<string, Position> = new Map();
        const initialPosition = this.grid.getPosition();
        initialPosition.y++;
        beams.set(initialPosition.toString(), initialPosition);
        
        let splitCount = 0;
        let stop = false;
        do {
            let atLeastOneBeamMoved = false;
            for(const beamPosition of beams.values()) {
                this.grid.setPosition(beamPosition);
                if(this.grid.move(Direction.DOWN)) {
                    atLeastOneBeamMoved = true;
                    if(this.grid.getCurrentChar() === '^') {
                        splitCount++; // Count each split
                        const potentialBeams = [
                            new Position(this.grid.getPosition().x + 1, this.grid.getPosition().y),
                            new Position(this.grid.getPosition().x - 1, this.grid.getPosition().y),
                        ];
                        for(const potentialBeam of potentialBeams) {
                            if(this.grid.get(potentialBeam) && !beams.has(potentialBeam.toString())) {
                                beams.set(potentialBeam.toString(), potentialBeam);
                            }
                        }
                    }else{
                        beams.set(this.grid.getPosition().toString(), this.grid.getPosition());
                    }
                    beams.delete(beamPosition.toString());
                }
            }
            stop = !atLeastOneBeamMoved;
        } while (!stop);

        return splitCount;
    }

    protected solve2(): number {
        this.grid.setPosition(new Position(0, 0));
        this.grid.scrollGrid((position: Position) => this.grid.getCurrentChar() === 'S');
        
        // Map: position -> nombre de timelines à cette position
        let currentTimelines: Map<Position, number> = new Map();
        currentTimelines.set(this.grid.getPosition(), 1);
        
        let totalFinishedTimelines = 0;
        
        // Tant qu'il reste des timelines actives
        while (currentTimelines.size > 0) {
            const nextTimelines: Map<Position, number> = new Map();
            
            for (const [position, count] of currentTimelines.entries()) {
                this.grid.setPosition(position);
                this.grid.move(Direction.DOWN);
                
                // Si on sort de la grille, ces timelines se terminent
                if (!this.grid.get(this.grid.getPosition())) {
                    totalFinishedTimelines += count;
                    continue;
                }
                
                // Si on rencontre un splitter, diviser en deux
                if (this.grid.getCurrentChar() === '^') {
                    this.grid.getAdjacentPositions([Direction.LEFT,Direction.RIGHT]).forEach(position => {
                        nextTimelines.set(position, (nextTimelines.get(position) || 0) + count);
                    });
                } else {
                    // Sinon, continuer à descendre
                    nextTimelines.set(this.grid.getPosition(), (nextTimelines.get(this.grid.getPosition()) || 0) + count);
                }
            }
            
            currentTimelines = nextTimelines;
        }
        
        return totalFinishedTimelines;
    }

}

console.log(new Day07().solve());

