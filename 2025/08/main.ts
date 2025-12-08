import { Main } from '../main.class';

class Position3D {
    constructor(public x: number, public y: number, public z: number) {}

    getDistanceSquared(other: Position3D): number {
        return Math.pow(this.x - other.x, 2) + Math.pow(this.y - other.y, 2) + Math.pow(this.z - other.z, 2);
    }

    toString(): string {
        return `(${this.x},${this.y},${this.z})`;
    }
}

export class Day08 extends Main {

    private positions: Position3D[] = [];
    private pairs: {distance: number, pointA: Position3D, pointB: Position3D}[] = [];

    protected parseInput(): void {
        this.positions = this.lines.map(line => {
            const [x, y, z] = line.split(',').map(Number);
            return new Position3D(x, y, z);
        });
    
        for(let i = 0; i < this.positions.length; i++) {
            for(let j = i + 1; j < this.positions.length; j++) {
                const distance = this.positions[i].getDistanceSquared(this.positions[j]);
                this.pairs.push({distance, pointA: this.positions[i], pointB: this.positions[j]});
            }
        }
    
        this.pairs.sort((a, b) => a.distance - b.distance);
    }

    private findCircuitContaining(circuits: Position3D[][], point: Position3D, pointToCircuit: Map<Position3D, Position3D[]>): Position3D[] | undefined {
        return pointToCircuit.get(point);
    }

    private addToCircuitMap(pointToCircuit: Map<Position3D, Position3D[]>, circuit: Position3D[]): void {
        for (const point of circuit) {
            pointToCircuit.set(point, circuit);
        }
    }

    private mergeCircuits(
        circuits: Position3D[][], 
        pointToCircuit: Map<Position3D, Position3D[]>,
        pair: {pointA: Position3D, pointB: Position3D}
    ): Position3D[] | null {
        const circuitA = this.findCircuitContaining(circuits, pair.pointA, pointToCircuit);
        const circuitB = this.findCircuitContaining(circuits, pair.pointB, pointToCircuit);
        
        if(circuitA && circuitB) {
            if(circuitA !== circuitB) {
                circuitA.push(...circuitB);
                // Mettre à jour tous les points de circuitB pour qu'ils pointent vers circuitA
                for (const point of circuitB) {
                    pointToCircuit.set(point, circuitA);
                }
                circuits.splice(circuits.indexOf(circuitB), 1);
                return circuitA;
            }
        } else if(circuitA) {
            circuitA.push(pair.pointB);
            pointToCircuit.set(pair.pointB, circuitA);
            return circuitA;
        } else if(circuitB) {
            circuitB.push(pair.pointA);
            pointToCircuit.set(pair.pointA, circuitB);
            return circuitB;
        } else {
            const newCircuit = [pair.pointA, pair.pointB];
            circuits.push(newCircuit);
            this.addToCircuitMap(pointToCircuit, newCircuit);
        }
        return null;
    }
    
    protected solve1(): number {
        const circuits: Position3D[][] = [];
        const pointToCircuit = new Map<Position3D, Position3D[]>();
        const limit = this.positions.length === 20 ? 10 : 1000;
        
        for(let i = 0; i < limit && i < this.pairs.length; i++) {
            this.mergeCircuits(circuits, pointToCircuit, this.pairs[i]);
        }
        
        return circuits.sort((a, b) => b.length - a.length).slice(0, 3).reduce((acc, circuit) => acc * circuit.length, 1);
    }

    protected solve2(): number {
        const circuits: Position3D[][] = [];
        const pointToCircuit = new Map<Position3D, Position3D[]>();
        
        for(let i = 0; i < this.pairs.length; i++) {
            const pair = this.pairs[i];
            const mergedCircuit = this.mergeCircuits(circuits, pointToCircuit, pair);
            
            if(mergedCircuit && mergedCircuit.length === this.positions.length) {
                return pair.pointA.x * pair.pointB.x;
            }
        }

        return 0;
    }

}

console.log(new Day08().solve());

