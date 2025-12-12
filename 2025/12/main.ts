import { Grid } from '../../typescript/geometry/grid.class';
import { Polyomino } from '../../typescript/geometry/polyomino.class';
import { PolyominoPacker } from '../../typescript/algorithms/polyomino-packer.class';
import { Main } from '../../typescript/main.class';

export class Day12 extends Main {
    private shapes: Grid[] = [];
    private regions: { width: number; height: number; shapeCounts: number[] }[] = [];

    protected parseInput(): void {
        const regexShapeHeader = /^(\d+):$/;
        const regexRegion = /^(\d+)x(\d+):\s*(.+)$/;
        let currentShape: string[] = [];

        for (const line of this.lines) {
            if (regexShapeHeader.test(line)) {
                if (currentShape.length > 0) {
                    this.shapes.push(new Grid(currentShape));
                }
                currentShape = [];
                continue;
            }

            const regionMatch = line.match(regexRegion);
            if (regionMatch) {
                if (currentShape.length > 0) {
                    this.shapes.push(new Grid(currentShape));
                    currentShape = [];
                }
                const width = parseInt(regionMatch[1], 10);
                const height = parseInt(regionMatch[2], 10);
                const shapeCounts = regionMatch[3].trim().split(/\s+/).map(Number);
                this.regions.push({ width, height, shapeCounts });
            } else if (line.length > 0) {
                currentShape.push(line);
            }
        }

        if (currentShape.length > 0) {
            this.shapes.push(new Grid(currentShape));
        }
    }

    protected solve1(): number {
        const polyominoes = this.shapes.map(s => Polyomino.fromGrid(s));
        const packer = new PolyominoPacker(polyominoes);

        let count = 0;
        for (const region of this.regions) {
            if (packer.canPack(region)) {
                count++;
            }
        }
        return count;
    }

    protected solve2(): number {
        return 0;
    }
}

console.log(new Day12().solve());
