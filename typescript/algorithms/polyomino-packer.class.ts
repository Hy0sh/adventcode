import { Polyomino } from '../geometry/polyomino.class';

/**
 * Configuration for a packing problem.
 */
export interface PackingProblem {
    width: number;
    height: number;
    /** Count of each shape type to place */
    shapeCounts: number[];
}

/**
 * A shape instance to place (with all its variants pre-computed).
 */
interface ShapeInstance {
    variants: Polyomino[];
    key: string;
}

/**
 * Generic polyomino packing solver using backtracking with symmetry breaking.
 * 
 * Determines if a set of polyomino shapes can be placed on a rectangular
 * grid without overlapping. Shapes can be rotated and flipped.
 * 
 * Uses symmetry breaking to avoid exploring equivalent placements of
 * identical shapes.
 * 
 * @example
 * ```ts
 * const shapes = [Polyomino.fromGrid(grid1), Polyomino.fromGrid(grid2)];
 * const packer = new PolyominoPacker(shapes);
 * 
 * const canFit = packer.canPack({
 *     width: 10,
 *     height: 10,
 *     shapeCounts: [2, 3]  // 2 of shape 0, 3 of shape 1
 * });
 * ```
 */
export class PolyominoPacker {
    private readonly shapeVariants: Polyomino[][];
    private readonly shapeAreas: number[];
    private readonly shapeKeys: string[];

    /**
     * Create a packer with the given base shapes.
     * All rotations and flips are pre-computed.
     */
    constructor(shapes: Polyomino[]) {
        this.shapeVariants = shapes.map(p => p.allTransformations());
        this.shapeAreas = shapes.map(p => p.area);
        this.shapeKeys = shapes.map(p => p.key);
    }

    /**
     * Check if the shapes can be packed into the region without overlap.
     * 
     * @param problem - The packing problem specification
     * @param requireExactCover - If true, all grid cells must be covered
     * @returns true if packing is possible
     */
    canPack(problem: PackingProblem, requireExactCover: boolean = false): boolean {
        const gridSize = problem.width * problem.height;

        // Calculate total area of shapes
        let totalArea = 0;
        for (let i = 0; i < problem.shapeCounts.length; i++) {
            totalArea += problem.shapeCounts[i] * this.shapeAreas[i];
        }

        // Pre-check: area constraints
        if (totalArea > gridSize) {
            return false;
        }
        if (requireExactCover && totalArea !== gridSize) {
            return false;
        }

        // Build list of shape instances to place
        const instances: ShapeInstance[] = [];
        for (let i = 0; i < problem.shapeCounts.length; i++) {
            for (let j = 0; j < problem.shapeCounts[i]; j++) {
                instances.push({
                    variants: this.shapeVariants[i],
                    key: this.shapeKeys[i]
                });
            }
        }

        if (instances.length === 0) {
            return true;
        }

        // Sort by key to group identical shapes (for symmetry breaking)
        instances.sort((a, b) => a.key.localeCompare(b.key));

        const grid = new Uint8Array(gridSize);
        return this.backtrack(grid, instances, 0, problem.width, problem.height, 0, '');
    }

    private backtrack(
        grid: Uint8Array,
        instances: ShapeInstance[],
        index: number,
        width: number,
        height: number,
        lastPlacement: number,
        prevKey: string
    ): boolean {
        if (index === instances.length) {
            return true;
        }

        const { variants, key } = instances[index];
        // Symmetry breaking: if same shape as previous, only try positions >= lastPlacement
        const minPlacement = (key === prevKey) ? lastPlacement : 0;

        for (let y = 0; y < height; y++) {
            for (let x = 0; x < width; x++) {
                const placement = y * width + x;
                if (placement < minPlacement) continue;

                for (const variant of variants) {
                    if (variant.canPlaceAt(grid, x, y, width, height)) {
                        variant.placeAt(grid, x, y, width);

                        if (this.backtrack(grid, instances, index + 1, width, height, placement, key)) {
                            return true;
                        }

                        variant.removeFrom(grid, x, y, width);
                    }
                }
            }
        }

        return false;
    }
}

