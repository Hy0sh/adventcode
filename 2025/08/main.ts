import { Main } from '../../typescript/main.class';
import { Point3D } from '../../typescript/geometry/Point3D';
import { UnionFind } from '../../typescript/graph/union-find.class';

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
    
    protected solve1(): number {
        const uf = new UnionFind<Point3D>((point) => `${point.x},${point.y},${point.z}`);
        
        // Initialiser tous les points
        for (const point of this.positions) {
            uf.makeSet(point);
        }
        
        const limit = this.positions.length === 20 ? 10 : 1000;
        
        // Union des pairs les plus proches
        for(let i = 0; i < limit && i < this.pairs.length; i++) {
            uf.union(this.pairs[i].pointA, this.pairs[i].pointB);
        }
        
        // Obtenir les 3 plus grands circuits
        const sets = uf.getSets();
        const circuitSizes = Array.from(sets.values())
            .map(circuit => circuit.length)
            .sort((a, b) => b - a)
            .slice(0, 3);
        
        return circuitSizes.reduce((acc, size) => acc * size, 1);
    }

    protected solve2(): number {
        const uf = new UnionFind<Point3D>((point) => `${point.x},${point.y},${point.z}`);
        
        // Initialiser tous les points
        for (const point of this.positions) {
            uf.makeSet(point);
        }
        
        // Union progressive jusqu'à tout connecter
        for(let i = 0; i < this.pairs.length; i++) {
            const pair = this.pairs[i];
            uf.union(pair.pointA, pair.pointB);
            
            // Vérifier si tous les points sont connectés
            if(uf.getSetSize(pair.pointA) === this.positions.length) {
                return pair.pointA.x * pair.pointB.x;
            }
        }

        return 0;
    }

}

console.log(new Day08().solve());

