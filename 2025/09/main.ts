import { Main } from '../../typescript/main.class';
import { Point } from '../../typescript/geometry/Point';
import { Rectangle } from '../../typescript/geometry/Rectangle';
import { isPointInsidePolygon } from '../../typescript/geometry/ray-casting-alghorithm';
import { PrefixSum2D } from '../../typescript/algorithms/prefix-sum-2d.class';
import { CoordinateCompression2D } from '../../typescript/algorithms/coordinate-compression.class';


export class Day09 extends Main {

    private points: Point[] = [];
    private squareAreas: {rectangle: Rectangle, pointA: Point, pointB: Point}[] = [];

    protected parseInput(): void {
        this.points = this.lines.map(line => {
            const [x, y] = line.split(',').map(Number);
            return new Point(x, y);
        });

    }
    
    protected solve1(): number {
        for(let i = 0; i < this.points.length; i++) {
            for(let j = i + 1; j < this.points.length; j++) {
                this.squareAreas.push({rectangle: new Rectangle(this.points[i], this.points[j]), pointA: this.points[i], pointB: this.points[j]});
            }
        }
    
        this.squareAreas.sort((a, b) => b.rectangle.squareArea() - a.rectangle.squareArea());
        return this.squareAreas[0].rectangle.squareArea();
    }

    protected solve2(): number {
        // Compression des coordonnées
        // https://medium.com/algorithms-digest/coordinate-compression-2fff95326fb
        const compression = new CoordinateCompression2D(
            this.points.map(p => [p.x, p.y] as [number, number])
        );
        
        const [W, H] = compression.getCompressedSize();
        
        // Créer le masque des points valides (rouge/vert)
        const mask: boolean[][] = Array(H).fill(0).map(() => Array(W).fill(false));
        
        // Marquer les points rouges
        for (const p of this.points) {
            const compressed = compression.compress([p.x, p.y])!;
            mask[compressed[1]][compressed[0]] = true;
        }
        
        // Marquer les segments verts (lignes horizontales ou verticales)
        for (let i = 0; i < this.points.length; i++) {
            const p1 = this.points[i];
            const p2 = this.points[(i + 1) % this.points.length];
            
            const [idxX1, idxY1] = compression.compress([p1.x, p1.y])!;
            const [idxX2, idxY2] = compression.compress([p2.x, p2.y])!;
            
            if (p1.y === p2.y) {
                // Segment horizontal
                const minIx = Math.min(idxX1, idxX2);
                const maxIx = Math.max(idxX1, idxX2);
                for (let ix = minIx; ix <= maxIx; ix++) {
                    mask[idxY1][ix] = true;
                }
            } else if (p1.x === p2.x) {
                // Segment vertical
                const minIy = Math.min(idxY1, idxY2);
                const maxIy = Math.max(idxY1, idxY2);
                for (let iy = minIy; iy <= maxIy; iy++) {
                    mask[iy][idxX1] = true;
                }
            }
        }
        
        // Remplir l'intérieur du polygone (flood fill ou scanline)
        console.log("Remplissage du polygone...");
        this.fillPolygon(mask, compression);
        
        // Créer le prefix sum pour tests O(1)
        // https://www.geeksforgeeks.org/dsa/prefix-sum-2d-array/
        console.log("Création des prefix sums...");
        const prefixSum2D = new PrefixSum2D(mask);
        
        // Tester tous les rectangles en O(1)
        console.log("Test des rectangles...");
        let bestArea = 0;
        
        for (const squareArea of this.squareAreas) {
            const area = squareArea.rectangle.squareArea();
            if (area <= bestArea) break;
            
            const x1 = Math.min(squareArea.pointA.x, squareArea.pointB.x);
            const x2 = Math.max(squareArea.pointA.x, squareArea.pointB.x);
            const y1 = Math.min(squareArea.pointA.y, squareArea.pointB.y);
            const y2 = Math.max(squareArea.pointA.y, squareArea.pointB.y);
            
            const [ix1, iy1] = compression.compress([x1, y1])!;
            const [ix2, iy2] = compression.compress([x2, y2])!;
            
            // Vérifier si TOUS les points du rectangle sont valides
            if (prefixSum2D.isRectangleFull(iy1, ix1, iy2, ix2)) {
                console.log(`✓ Valid: area=${area}`);
                bestArea = area;
            }
        }
        
        return bestArea;
    }
    
    private fillPolygon(mask: boolean[][], compression: CoordinateCompression2D): void {
        const allX = compression.xCompression.getAllCoordinates();
        const allY = compression.yCompression.getAllCoordinates();
        const [W, H] = compression.getCompressedSize();
        
        // Pour chaque ligne Y, utiliser ray-casting
        for (let iy = 0; iy < H; iy++) {
            const y = allY[iy];
            const point = new Point(0, y);
            
            for (let ix = 0; ix < W; ix++) {
                if (mask[iy][ix]) continue; // Déjà marqué (rouge ou sur segment)
                
                const x = allX[ix];
                point.x = x;
                
                // Tester si à l'intérieur du polygone
                if (isPointInsidePolygon(this.points, point)) {
                    mask[iy][ix] = true;
                }
            }
        }
    }

}

console.log(new Day09().solve());

