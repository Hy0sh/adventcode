import { Main } from '../main.class';
import { Point } from '../geometry/Point';
import { Rectangle } from '../geometry/Rectangle';
import { isPointInsidePolygon } from '../geometry/ray-casting-alghorithm';


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
        // compression des coordonnées
        // https://medium.com/algorithms-digest/coordinate-compression-2fff95326fb
        // 1. on récupère les x et y uniques et on les trie
        const allX = [...new Set(this.points.map(p => p.x))].sort((a, b) => a - b);
        const allY = [...new Set(this.points.map(p => p.y))].sort((a, b) => a - b);
        
        const W = allX.length;
        const H = allY.length;
        
        // 2. Créer les mappings coord → index
        const xToIdx = new Map<number, number>();
        const yToIdx = new Map<number, number>();
        allX.forEach((x, idx) => xToIdx.set(x, idx));
        allY.forEach((y, idx) => yToIdx.set(y, idx));
        
        // 3. on crée le masque des points valides (rouge/vert)
        const mask: boolean[][] = Array(H).fill(0).map(() => Array(W).fill(false));
        
        // on marque les points rouges
        for (const p of this.points) {
            const ix = xToIdx.get(p.x)!;
            const iy = yToIdx.get(p.y)!;
            mask[iy][ix] = true;
        }
        
        // on marque les segments verts (lignes horizontales ou verticales)
        for (let i = 0; i < this.points.length; i++) {
            const p1 = this.points[i];
            const p2 = this.points[(i + 1) % this.points.length];
            
            const idxX1 = xToIdx.get(p1.x)!;
            const idxY1 = yToIdx.get(p1.y)!;
            const idxX2 = xToIdx.get(p2.x)!;
            const idxY2 = yToIdx.get(p2.y)!;
            
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
        this.fillPolygon(mask, W, H, allX, allY);
        
        // 4. Créer le prefix sum pour tests O(1)
        // https://www.geeksforgeeks.org/dsa/prefix-sum-2d-array/
        console.log("Création des prefix sums...");
        const prefixSum: number[][] = Array(H + 1).fill(0).map(() => Array(W + 1).fill(0));
        
        for (let iy = 0; iy < H; iy++) {
            for (let ix = 0; ix < W; ix++) {
                prefixSum[iy + 1][ix + 1] = 
                    (mask[iy][ix] ? 1 : 0) +
                    prefixSum[iy][ix + 1] +
                    prefixSum[iy + 1][ix] -
                    prefixSum[iy][ix];
            }
        }
        
        // 5. Tester tous les rectangles en O(1)
        console.log("Test des rectangles...");
        let bestArea = 0;
        
        for (const squareArea of this.squareAreas) {
            const area = squareArea.rectangle.squareArea();
            if (area <= bestArea) break;
            
            const x1 = Math.min(squareArea.pointA.x, squareArea.pointB.x);
            const x2 = Math.max(squareArea.pointA.x, squareArea.pointB.x);
            const y1 = Math.min(squareArea.pointA.y, squareArea.pointB.y);
            const y2 = Math.max(squareArea.pointA.y, squareArea.pointB.y);
            
            const ix1 = xToIdx.get(x1)!;
            const ix2 = xToIdx.get(x2)!;
            const iy1 = yToIdx.get(y1)!;
            const iy2 = yToIdx.get(y2)!;
            
            // Nombre de cellules dans le rectangle compressé
            const cellsInRect = (ix2 - ix1 + 1) * (iy2 - iy1 + 1);
            
            // Nombre de points valides dans le rectangle (via prefix sum)
            const validCount = 
                prefixSum[iy2 + 1][ix2 + 1] -
                prefixSum[iy1][ix2 + 1] -
                prefixSum[iy2 + 1][ix1] +
                prefixSum[iy1][ix1];
            
            // Si TOUS les points du rectangle sont valides
            if (validCount === cellsInRect) {
                console.log(`✓ Valid: area=${area}`);
                bestArea = area;
            }
        }
        
        return bestArea;
    }
    
    private fillPolygon(mask: boolean[][], W: number, H: number, allX: number[], allY: number[]): void {
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

