/**
 * Compression de Coordonnées
 * 
 * Technique pour réduire un grand espace de coordonnées à un espace plus petit
 * tout en préservant l'ordre relatif des coordonnées.
 * 
 * Utile pour :
 * - Réduire l'utilisation de mémoire pour des grilles éparses
 * - Accélérer les algorithmes qui parcourent toutes les coordonnées
 * 
 * Exemple :
 * Coordonnées originales : [1, 5, 100, 200, 1000]
 * Coordonnées compressées : [0, 1, 2, 3, 4]
 * 
 * Complexité : O(n log n) pour le tri initial
 * 
 * @see https://medium.com/algorithms-digest/coordinate-compression-2fff95326fb
 */
export class CoordinateCompression {
    private coordToIndex: Map<number, number>;
    private indexToCoord: number[];
    
    /**
     * Crée une compression de coordonnées à partir d'une liste de coordonnées
     * @param coordinates Liste de coordonnées à compresser
     */
    constructor(coordinates: number[]) {
        // Supprimer les doublons et trier
        const uniqueSet = new Set(coordinates);
        const unique = Array.from(uniqueSet).sort((a, b) => a - b);
        
        this.indexToCoord = unique;
        this.coordToIndex = new Map();
        
        unique.forEach((coord, index) => {
            this.coordToIndex.set(coord, index);
        });
    }
    
    /**
     * Convertit une coordonnée originale en index compressé
     */
    compress(coord: number): number | undefined {
        return this.coordToIndex.get(coord);
    }
    
    /**
     * Convertit un index compressé en coordonnée originale
     */
    decompress(index: number): number | undefined {
        return this.indexToCoord[index];
    }
    
    /**
     * Retourne le nombre de coordonnées uniques
     */
    getSize(): number {
        return this.indexToCoord.length;
    }
    
    /**
     * Retourne toutes les coordonnées originales triées
     */
    getAllCoordinates(): number[] {
        return [...this.indexToCoord];
    }
    
    /**
     * Trouve l'index du plus grand élément <= coord
     * @returns -1 si aucun élément n'est <= coord
     */
    findLowerBoundIndex(coord: number): number {
        let left = 0;
        let right = this.indexToCoord.length - 1;
        let result = -1;
        
        while (left <= right) {
            const mid = Math.floor((left + right) / 2);
            
            if (this.indexToCoord[mid] <= coord) {
                result = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
    }
    
    /**
     * Trouve l'index du plus petit élément >= coord
     * @returns -1 si aucun élément n'est >= coord
     */
    findUpperBoundIndex(coord: number): number {
        let left = 0;
        let right = this.indexToCoord.length - 1;
        let result = -1;
        
        while (left <= right) {
            const mid = Math.floor((left + right) / 2);
            
            if (this.indexToCoord[mid] >= coord) {
                result = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        return result;
    }
}

/**
 * Classe helper pour compresser des points 2D
 */
export class CoordinateCompression2D {
    public readonly xCompression: CoordinateCompression;
    public readonly yCompression: CoordinateCompression;
    
    /**
     * @param points Liste de points [x, y] à compresser
     */
    constructor(points: [number, number][]) {
        const allX = points.map(p => p[0]);
        const allY = points.map(p => p[1]);
        
        this.xCompression = new CoordinateCompression(allX);
        this.yCompression = new CoordinateCompression(allY);
    }
    
    /**
     * Compresse un point [x, y] en [indexX, indexY]
     */
    compress(point: [number, number]): [number, number] | undefined {
        const ix = this.xCompression.compress(point[0]);
        const iy = this.yCompression.compress(point[1]);
        
        if (ix === undefined || iy === undefined) {
            return undefined;
        }
        
        return [ix, iy];
    }
    
    /**
     * Décompresse un point [indexX, indexY] en [x, y]
     */
    decompress(indices: [number, number]): [number, number] | undefined {
        const x = this.xCompression.decompress(indices[0]);
        const y = this.yCompression.decompress(indices[1]);
        
        if (x === undefined || y === undefined) {
            return undefined;
        }
        
        return [x, y];
    }
    
    /**
     * Retourne les dimensions de la grille compressée [largeur, hauteur]
     */
    getCompressedSize(): [number, number] {
        return [this.xCompression.getSize(), this.yCompression.getSize()];
    }
}
