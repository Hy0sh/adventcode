/**
 * Index des algorithmes génériques disponibles
 * 
 * Ce fichier facilite l'import de tous les algorithmes en un seul endroit.
 */

// Algorithmes de graphes
export { BFS, BFSOptions } from '../graph/bfs.class';
export { DFS, DFSOptions } from '../graph/dfs.class';
export { UnionFind } from '../graph/union-find.class';

// Algorithmes sur les intervalles
export { IntervalMerger, Interval } from './interval-merger.class';

// Algorithmes de compression et optimisation
export { CoordinateCompression, CoordinateCompression2D } from './coordinate-compression.class';
export { PrefixSum2D } from './prefix-sum-2d.class';

// Algorithmes de pavage
export { PolyominoPacker, PackingProblem } from './polyomino-packer.class';

// Géométrie
export { Polyomino } from '../geometry/polyomino.class';
