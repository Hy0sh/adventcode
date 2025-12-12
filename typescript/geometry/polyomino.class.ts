import { Grid, Position } from './grid.class';

/**
 * Represents a polyomino shape as a set of cell offsets from origin.
 * Provides methods for transformations (rotation, flip) and normalization.
 */
export class Polyomino {
    readonly cells: Position[];
    readonly width: number;
    readonly height: number;
    readonly area: number;
    private _key: string | null = null;

    private constructor(cells: Position[]) {
        this.cells = cells;
        this.area = cells.length;
        
        if (cells.length === 0) {
            this.width = 0;
            this.height = 0;
        } else {
            this.width = Math.max(...cells.map(p => p.x)) + 1;
            this.height = Math.max(...cells.map(p => p.y)) + 1;
        }
    }

    /**
     * Create a Polyomino from a Grid where '#' marks filled cells.
     */
    static fromGrid(grid: Grid): Polyomino {
        const cells: Position[] = [];
        for (let y = 0; y < grid.getHeight(); y++) {
            for (let x = 0; x < grid.getWidth(); x++) {
                if (grid.get(new Position(x, y)) === '#') {
                    cells.push(new Position(x, y));
                }
            }
        }
        return new Polyomino(Polyomino.normalize(cells));
    }

    /**
     * Create a Polyomino from raw cell positions.
     */
    static fromCells(cells: Position[]): Polyomino {
        return new Polyomino(Polyomino.normalize(cells));
    }

    /**
     * Normalize cells so minimum x and y are 0.
     */
    private static normalize(cells: Position[]): Position[] {
        if (cells.length === 0) return [];
        const minX = Math.min(...cells.map(p => p.x));
        const minY = Math.min(...cells.map(p => p.y));
        return cells.map(p => new Position(p.x - minX, p.y - minY));
    }

    /**
     * Get a unique key for this polyomino shape (for deduplication).
     */
    get key(): string {
        if (this._key === null) {
            this._key = this.cells
                .map(p => `${p.x},${p.y}`)
                .sort()
                .join('|');
        }
        return this._key;
    }

    /**
     * Rotate 90 degrees clockwise.
     */
    rotate90(): Polyomino {
        const rotated = this.cells.map(p => new Position(-p.y, p.x));
        return new Polyomino(Polyomino.normalize(rotated));
    }

    /**
     * Flip horizontally.
     */
    flipHorizontal(): Polyomino {
        const flipped = this.cells.map(p => new Position(-p.x, p.y));
        return new Polyomino(Polyomino.normalize(flipped));
    }

    /**
     * Generate all unique transformations (up to 8: 4 rotations × 2 flips).
     */
    allTransformations(): Polyomino[] {
        const variants: Polyomino[] = [];
        const seen = new Set<string>();

        let current: Polyomino = this;
        for (let rot = 0; rot < 4; rot++) {
            // Check rotation
            if (!seen.has(current.key)) {
                seen.add(current.key);
                variants.push(current);
            }

            // Check horizontal flip
            const flipped = current.flipHorizontal();
            if (!seen.has(flipped.key)) {
                seen.add(flipped.key);
                variants.push(flipped);
            }

            current = current.rotate90();
        }

        return variants;
    }

    /**
     * Check if this polyomino can be placed at position on a grid.
     */
    canPlaceAt(grid: Uint8Array, baseX: number, baseY: number, gridWidth: number, gridHeight: number): boolean {
        for (const cell of this.cells) {
            const x = baseX + cell.x;
            const y = baseY + cell.y;
            if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
                return false;
            }
            if (grid[y * gridWidth + x] !== 0) {
                return false;
            }
        }
        return true;
    }

    /**
     * Place this polyomino on the grid (mark cells as occupied).
     */
    placeAt(grid: Uint8Array, baseX: number, baseY: number, gridWidth: number): void {
        for (const cell of this.cells) {
            grid[(baseY + cell.y) * gridWidth + (baseX + cell.x)] = 1;
        }
    }

    /**
     * Remove this polyomino from the grid (mark cells as empty).
     */
    removeFrom(grid: Uint8Array, baseX: number, baseY: number, gridWidth: number): void {
        for (const cell of this.cells) {
            grid[(baseY + cell.y) * gridWidth + (baseX + cell.x)] = 0;
        }
    }
}

