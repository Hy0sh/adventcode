import { Main } from '../main.class';
import { Point3D } from '../geometry/Point3D';

export class Day08 extends Main {

    private positions: Point3D[] = [];
    private pairs: {distance: number, pointA: Point3D, pointB: Point3D}[] = [];

    protected parseInput(): void {
        this.positions = this.lines.map(line => {
            const [x, y, z] = line.split(',').map(Number);
            return new Point3D(x, y, z);
        });
    
        for(let i = 0; i < this.positions.length; i++) {
            for(let j = i + 1; j < this.positions.length; j++) {
                const distance = this.positions[i].getDistanceSquared(this.positions[j]);
                this.pairs.push({distance, pointA: this.positions[i], pointB: this.positions[j]});
            }
        }
    
        this.pairs.sort((a, b) => a.distance - b.distance);
    }

    private mergeCircuits(
        circuits: Point3D[][], 
        pair: {pointA: Point3D, pointB: Point3D}
    ): Point3D[] | null {
        const circuitA = circuits.find(circuit => circuit.includes(pair.pointA));
        const circuitB = circuits.find(circuit => circuit.includes(pair.pointB));
        
        if(circuitA && circuitB) {
            if(circuitA !== circuitB) {
                circuitA.push(...circuitB);
                circuits.splice(circuits.indexOf(circuitB), 1);
                return circuitA;
            }
        } else if(circuitA) {
            circuitA.push(pair.pointB);
            return circuitA;
        } else if(circuitB) {
            circuitB.push(pair.pointA);
            return circuitB;
        } else {
            const newCircuit = [pair.pointA, pair.pointB];
            circuits.push(newCircuit);
        }
        return null;
    }
    
    protected solve1(): number {
        const circuits: Point3D[][] = [];
        const limit = this.positions.length === 20 ? 10 : 1000;
        
        for(let i = 0; i < limit && i < this.pairs.length; i++) {
            this.mergeCircuits(circuits, this.pairs[i]);
        }
        
        return circuits.sort((a, b) => b.length - a.length).slice(0, 3).reduce((acc, circuit) => acc * circuit.length, 1);
    }

    protected solve2(): number {
        const circuits: Point3D[][] = [];
        
        for(let i = 0; i < this.pairs.length; i++) {
            const pair = this.pairs[i];
            const mergedCircuit = this.mergeCircuits(circuits, pair);
            
            if(mergedCircuit && mergedCircuit.length === this.positions.length) {
                return pair.pointA.x * pair.pointB.x;
            }
        }

        return 0;
    }

}

console.log(new Day08().solve());

