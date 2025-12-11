/**
 * Union-Find (Disjoint Set Union)
 * Structure de données pour maintenir des ensembles disjoints
 * et effectuer des opérations d'union et de recherche efficacement.
 * 
 * Complexité :
 * - find: O(α(n)) ≈ O(1) amortie (α est la fonction inverse d'Ackermann)
 * - union: O(α(n)) ≈ O(1) amortie
 * - isConnected: O(α(n)) ≈ O(1) amortie
 */
export class UnionFind<T> {
    private parent: Map<string, string> = new Map();
    private rank: Map<string, number> = new Map();
    private setSize: Map<string, number> = new Map();
    private toString: (item: T) => string;
    
    /**
     * @param toString Fonction pour convertir un élément en string unique
     */
    constructor(toString: (item: T) => string = (item) => String(item)) {
        this.toString = toString;
    }
    
    /**
     * Crée un nouvel ensemble pour l'élément
     */
    makeSet(item: T): void {
        const key = this.toString(item);
        if (!this.parent.has(key)) {
            this.parent.set(key, key);
            this.rank.set(key, 0);
            this.setSize.set(key, 1);
        }
    }
    
    /**
     * Trouve le représentant de l'ensemble contenant l'élément
     * Utilise la compression de chemin pour optimiser les recherches futures
     */
    find(item: T): string {
        const key = this.toString(item);
        
        if (!this.parent.has(key)) {
            this.makeSet(item);
        }
        
        // Compression de chemin (itérative pour éviter la récursion)
        let root = key;
        while (this.parent.get(root) !== root) {
            root = this.parent.get(root)!;
        }
        
        // Compression : mettre à jour tous les parents vers la racine
        let current = key;
        while (current !== root) {
            const next = this.parent.get(current)!;
            this.parent.set(current, root);
            current = next;
        }
        
        return root;
    }
    
    /**
     * Fusionne les ensembles contenant les deux éléments
     * Utilise l'union par rang pour maintenir l'arbre équilibré
     * @returns true si une fusion a eu lieu, false si déjà dans le même ensemble
     */
    union(itemA: T, itemB: T): boolean {
        const rootA = this.find(itemA);
        const rootB = this.find(itemB);
        
        if (rootA === rootB) {
            return false; // Déjà dans le même ensemble
        }
        
        // Union par rang
        const rankA = this.rank.get(rootA)!;
        const rankB = this.rank.get(rootB)!;
        
        if (rankA < rankB) {
            this.parent.set(rootA, rootB);
            this.setSize.set(rootB, this.setSize.get(rootB)! + this.setSize.get(rootA)!);
        } else if (rankA > rankB) {
            this.parent.set(rootB, rootA);
            this.setSize.set(rootA, this.setSize.get(rootA)! + this.setSize.get(rootB)!);
        } else {
            this.parent.set(rootB, rootA);
            this.rank.set(rootA, rankA + 1);
            this.setSize.set(rootA, this.setSize.get(rootA)! + this.setSize.get(rootB)!);
        }
        
        return true;
    }
    
    /**
     * Vérifie si deux éléments sont dans le même ensemble
     */
    isConnected(itemA: T, itemB: T): boolean {
        return this.find(itemA) === this.find(itemB);
    }
    
    /**
     * Retourne la taille de l'ensemble contenant l'élément
     */
    getSetSize(item: T): number {
        const root = this.find(item);
        return this.setSize.get(root) || 0;
    }
    
    /**
     * Retourne tous les ensembles distincts
     * @returns Map où la clé est le représentant et la valeur est la liste des éléments
     */
    getSets(): Map<string, string[]> {
        const sets = new Map<string, string[]>();
        
        const keys = Array.from(this.parent.keys());
        for (const key of keys) {
            // Trouver directement la racine sans parser
            let root = key;
            while (this.parent.get(root) !== root) {
                root = this.parent.get(root)!;
            }
            
            if (!sets.has(root)) {
                sets.set(root, []);
            }
            sets.get(root)!.push(key);
        }
        
        return sets;
    }
    
    /**
     * Retourne le nombre d'ensembles distincts
     */
    getNumberOfSets(): number {
        const roots = new Set<string>();
        const keys = Array.from(this.parent.keys());
        for (const key of keys) {
            // Trouver directement la racine sans parser
            let root = key;
            while (this.parent.get(root) !== root) {
                root = this.parent.get(root)!;
            }
            roots.add(root);
        }
        return roots.size;
    }
}
