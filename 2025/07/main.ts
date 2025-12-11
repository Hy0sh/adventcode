import { Direction, Grid, Position } from '../../typescript/geometry/grid.class';
import { Main } from '../../typescript/main.class';

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
        
        // Map: position string -> nombre de timelines à cette position
        // Utilise toString() car les objets Position sont comparés par référence, pas par valeur
        let currentTimelines: Map<string, { position: Position, count: number }> = new Map();
        
        // Commencer juste après le 'S', comme dans solve1()
        const startPos = new Position(this.grid.getPosition().x, this.grid.getPosition().y + 1);
        currentTimelines.set(startPos.toString(), { position: startPos, count: 1 });
        
        let totalFinishedTimelines = 0;
        
        // Tant qu'il reste des timelines actives
        while (currentTimelines.size > 0) {
            const nextTimelines: Map<string, { position: Position, count: number }> = new Map();
            
            for (const { position, count } of currentTimelines.values()) {
                this.grid.setPosition(position);
                
                // Si on rencontre un splitter, diviser en deux
                if (this.grid.getCurrentChar() === '^') {
                    this.grid.getAdjacentPositions([Direction.LEFT,Direction.RIGHT]).forEach(adjPos => {
                        const key = adjPos.toString();
                        const existing = nextTimelines.get(key);
                        if (existing) {
                            existing.count += count;
                        } else {
                            nextTimelines.set(key, { position: adjPos, count });
                        }
                    });
                } else {
                    // Sinon, essayer de continuer à descendre
                    if (this.grid.move(Direction.DOWN)) {
                        const currentPos = this.grid.getPosition();
                        const key = currentPos.toString();
                        const existing = nextTimelines.get(key);
                        if (existing) {
                            existing.count += count;
                        } else {
                            nextTimelines.set(key, { position: currentPos, count });
                        }
                    } else {
                        // Si on ne peut pas bouger, la timeline se termine
                        totalFinishedTimelines += count;
                    }
                }
            }
            
            currentTimelines = nextTimelines;
        }
        
        return totalFinishedTimelines;
    }

}

console.log(new Day07().solve());

