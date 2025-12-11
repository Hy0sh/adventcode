/**
 * Prefix Sum 2D (Somme de Préfixes 2D)
 * 
 * Structure de données pour calculer efficacement la somme d'éléments
 * dans un rectangle quelconque d'une matrice.
 * 
 * Après une préparation en O(W×H), permet de répondre à des requêtes
 * de somme de rectangle en O(1).
 * 
 * Utile pour :
 * - Vérifier si un rectangle contient uniquement des valeurs valides
 * - Calculer des sommes de sous-matrices rapidement
 * 
 * @see https://www.geeksforgeeks.org/dsa/prefix-sum-2d-array/
 */
export class PrefixSum2D {
    private prefixSum: number[][];
    private height: number;
    private width: number;
    
    /**
     * Crée une structure de prefix sum 2D
     * @param matrix Matrice de valeurs (height × width)
     */
    constructor(matrix: number[][] | boolean[][]) {
        this.height = matrix.length;
        this.width = matrix.length > 0 ? matrix[0].length : 0;
        
        // Initialiser avec des dimensions +1 pour simplifier les calculs
        this.prefixSum = Array(this.height + 1)
            .fill(0)
            .map(() => Array(this.width + 1).fill(0));
        
        // Construire le tableau de prefix sum
        for (let row = 0; row < this.height; row++) {
            for (let col = 0; col < this.width; col++) {
                const value = typeof matrix[row][col] === 'boolean' 
                    ? (matrix[row][col] ? 1 : 0)
                    : matrix[row][col] as number;
                
                this.prefixSum[row + 1][col + 1] = 
                    value +
                    this.prefixSum[row][col + 1] +
                    this.prefixSum[row + 1][col] -
                    this.prefixSum[row][col];
            }
        }
    }
    
    /**
     * Calcule la somme dans le rectangle [row1, col1] à [row2, col2] (inclus)
     * 
     * @param row1 Ligne de départ (0-indexed)
     * @param col1 Colonne de départ (0-indexed)
     * @param row2 Ligne de fin (0-indexed, incluse)
     * @param col2 Colonne de fin (0-indexed, incluse)
     * @returns Somme des valeurs dans le rectangle
     */
    getRectangleSum(row1: number, col1: number, row2: number, col2: number): number {
        // Convertir en coordonnées 1-indexed pour le tableau de prefix sum
        return (
            this.prefixSum[row2 + 1][col2 + 1] -
            this.prefixSum[row1][col2 + 1] -
            this.prefixSum[row2 + 1][col1] +
            this.prefixSum[row1][col1]
        );
    }
    
    /**
     * Vérifie si toutes les cellules du rectangle sont non-nulles (ou true pour boolean)
     * 
     * @param row1 Ligne de départ (0-indexed)
     * @param col1 Colonne de départ (0-indexed)
     * @param row2 Ligne de fin (0-indexed, incluse)
     * @param col2 Colonne de fin (0-indexed, incluse)
     * @returns true si toutes les cellules sont non-nulles
     */
    isRectangleFull(row1: number, col1: number, row2: number, col2: number): boolean {
        const sum = this.getRectangleSum(row1, col1, row2, col2);
        const expectedCount = (row2 - row1 + 1) * (col2 - col1 + 1);
        return sum === expectedCount;
    }
    
    /**
     * Vérifie si le rectangle contient au moins une cellule non-nulle
     */
    hasAnyNonZero(row1: number, col1: number, row2: number, col2: number): boolean {
        return this.getRectangleSum(row1, col1, row2, col2) > 0;
    }
    
    /**
     * Retourne le nombre de cellules non-nulles dans le rectangle
     */
    countNonZero(row1: number, col1: number, row2: number, col2: number): number {
        return this.getRectangleSum(row1, col1, row2, col2);
    }
    
    /**
     * Retourne les dimensions de la matrice [hauteur, largeur]
     */
    getDimensions(): [number, number] {
        return [this.height, this.width];
    }
}
