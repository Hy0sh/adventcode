/**
 * Classe générique pour effectuer des parcours DFS (Depth-First Search)
 * sur des graphes avec différentes stratégies de comptage et mémoïsation.
 */

export interface DFSOptions<TNode, TState> {
    /** 
     * Fonction qui retourne les voisins d'un nœud 
     */
    getNeighbors: (node: TNode) => TNode[];
    
    /**
     * Fonction pour vérifier si on a atteint un nœud terminal
     */
    isTerminal: (node: TNode, state: TState) => boolean;
    
    /**
     * Fonction pour déterminer si on doit compter ce chemin terminal
     * @returns true si on doit compter ce chemin
     */
    shouldCount?: (node: TNode, state: TState) => boolean;
    
    /**
     * Fonction pour mettre à jour l'état lors de la visite d'un nœud
     */
    updateState?: (node: TNode, state: TState) => TState;
    
    /**
     * Fonction pour vérifier si on doit visiter un nœud
     * (utile pour éviter les cycles ou appliquer des contraintes)
     */
    shouldVisit?: (node: TNode, state: TState, visited: Set<TNode>) => boolean;
    
    /**
     * Fonction pour créer une clé de mémoïsation
     * Si définie, active la mémoïsation
     */
    getMemoKey?: (node: TNode, state: TState) => string;
    
    /**
     * Fonction pour comparer deux nœuds (pour le Set)
     * Par défaut utilise l'égalité de référence
     */
    nodeToString?: (node: TNode) => string;
}

/**
 * Classe DFS générique avec support pour :
 * - Comptage de chemins
 * - Mémoïsation
 * - États personnalisés
 * - Gestion des cycles
 */
export class DFS<TNode = string, TState = Record<string, any>> {
    private options: DFSOptions<TNode, TState>;
    private memo: Map<string, number> | null = null;
    
    constructor(options: DFSOptions<TNode, TState>) {
        this.options = options;
        
        // Activer la mémoïsation si getMemoKey est définie
        if (options.getMemoKey) {
            this.memo = new Map();
        }
    }
    
    /**
     * Compte le nombre de chemins de start à un nœud terminal
     * qui satisfait la condition shouldCount
     */
    countPaths(start: TNode, initialState: TState): number {
        if (this.memo) {
            return this.countPathsWithMemo(start, initialState);
        } else {
            return this.countPathsWithBacktracking(start, initialState, new Set());
        }
    }
    
    /**
     * Version avec mémoïsation (pour les DAG)
     */
    private countPathsWithMemo(node: TNode, state: TState): number {
        // Vérifier si on a atteint un nœud terminal
        if (this.options.isTerminal(node, state)) {
            const shouldCount = this.options.shouldCount?.(node, state) ?? true;
            return shouldCount ? 1 : 0;
        }
        
        // Vérifier le cache
        const key = this.options.getMemoKey!(node, state);
        if (this.memo!.has(key)) {
            return this.memo!.get(key)!;
        }
        
        let count = 0;
        const neighbors = this.options.getNeighbors(node);
        
        for (const neighbor of neighbors) {
            // Mettre à jour l'état si nécessaire
            const newState = this.options.updateState
                ? this.options.updateState(neighbor, state)
                : state;
            
            count += this.countPathsWithMemo(neighbor, newState);
        }
        
        this.memo!.set(key, count);
        return count;
    }
    
    /**
     * Version avec backtracking (pour les graphes avec cycles)
     */
    private countPathsWithBacktracking(
        node: TNode, 
        state: TState, 
        visited: Set<string>
    ): number {
        // Vérifier si on a atteint un nœud terminal
        if (this.options.isTerminal(node, state)) {
            const shouldCount = this.options.shouldCount?.(node, state) ?? true;
            return shouldCount ? 1 : 0;
        }
        
        let count = 0;
        const neighbors = this.options.getNeighbors(node);
        
        for (const neighbor of neighbors) {
            // Convertir le nœud en string pour le Set
            const neighborStr = this.options.nodeToString
                ? this.options.nodeToString(neighbor)
                : String(neighbor);
            
            // Vérifier si on doit visiter ce nœud
            if (visited.has(neighborStr)) {
                continue;
            }
            
            if (this.options.shouldVisit && !this.options.shouldVisit(neighbor, state, visited as any)) {
                continue;
            }
            
            // Mettre à jour l'état si nécessaire
            const newState = this.options.updateState
                ? this.options.updateState(neighbor, state)
                : state;
            
            // Marquer comme visité
            visited.add(neighborStr);
            
            // Récursion
            count += this.countPathsWithBacktracking(neighbor, newState, visited);
            
            // Backtrack
            visited.delete(neighborStr);
        }
        
        return count;
    }
    
    /**
     * Trouve tous les chemins de start à un nœud terminal
     * Attention : peut être très lent si beaucoup de chemins !
     */
    findAllPaths(start: TNode, initialState: TState, maxPaths = 10000): TNode[][] {
        const paths: TNode[][] = [];
        const visited = new Set<string>();
        
        const nodeStr = this.options.nodeToString
            ? this.options.nodeToString(start)
            : String(start);
        visited.add(nodeStr);
        
        this.findAllPathsHelper(start, initialState, visited, [start], paths, maxPaths);
        return paths;
    }
    
    private findAllPathsHelper(
        node: TNode,
        state: TState,
        visited: Set<string>,
        path: TNode[],
        paths: TNode[][],
        maxPaths: number
    ): void {
        if (paths.length >= maxPaths) {
            return; // Limiter le nombre de chemins
        }
        
        if (this.options.isTerminal(node, state)) {
            const shouldCount = this.options.shouldCount?.(node, state) ?? true;
            if (shouldCount) {
                paths.push([...path]);
            }
            return;
        }
        
        const neighbors = this.options.getNeighbors(node);
        
        for (const neighbor of neighbors) {
            const neighborStr = this.options.nodeToString
                ? this.options.nodeToString(neighbor)
                : String(neighbor);
            
            if (visited.has(neighborStr)) {
                continue;
            }
            
            if (this.options.shouldVisit && !this.options.shouldVisit(neighbor, state, visited as any)) {
                continue;
            }
            
            const newState = this.options.updateState
                ? this.options.updateState(neighbor, state)
                : state;
            
            visited.add(neighborStr);
            path.push(neighbor);
            
            this.findAllPathsHelper(neighbor, newState, visited, path, paths, maxPaths);
            
            path.pop();
            visited.delete(neighborStr);
        }
    }
    
    /**
     * Effectue une action sur chaque nœud visité (DFS classique)
     */
    traverse(start: TNode, initialState: TState, onVisit: (node: TNode, state: TState) => void): void {
        const visited = new Set<string>();
        const nodeStr = this.options.nodeToString
            ? this.options.nodeToString(start)
            : String(start);
        visited.add(nodeStr);
        
        this.traverseHelper(start, initialState, visited, onVisit);
    }
    
    private traverseHelper(
        node: TNode,
        state: TState,
        visited: Set<string>,
        onVisit: (node: TNode, state: TState) => void
    ): void {
        onVisit(node, state);
        
        if (this.options.isTerminal(node, state)) {
            return;
        }
        
        const neighbors = this.options.getNeighbors(node);
        
        for (const neighbor of neighbors) {
            const neighborStr = this.options.nodeToString
                ? this.options.nodeToString(neighbor)
                : String(neighbor);
            
            if (visited.has(neighborStr)) {
                continue;
            }
            
            if (this.options.shouldVisit && !this.options.shouldVisit(neighbor, state, visited as any)) {
                continue;
            }
            
            const newState = this.options.updateState
                ? this.options.updateState(neighbor, state)
                : state;
            
            visited.add(neighborStr);
            this.traverseHelper(neighbor, newState, visited, onVisit);
        }
    }
    
    /**
     * Réinitialise le cache de mémoïsation
     */
    clearMemo(): void {
        if (this.memo) {
            this.memo.clear();
        }
    }
}

