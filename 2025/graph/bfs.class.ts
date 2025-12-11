/**
 * Classe générique pour effectuer des parcours BFS (Breadth-First Search)
 * Utile pour trouver le chemin le plus court, explorer niveau par niveau, etc.
 */

export interface BFSOptions<TNode, TState> {
    /** 
     * Fonction qui retourne les voisins d'un nœud 
     */
    getNeighbors: (node: TNode, state: TState) => TNode[];
    
    /**
     * Fonction pour vérifier si on a atteint un nœud cible
     */
    isTarget?: (node: TNode, state: TState) => boolean;
    
    /**
     * Fonction pour mettre à jour l'état lors de la visite d'un nœud
     */
    updateState?: (node: TNode, state: TState) => TState;
    
    /**
     * Fonction pour vérifier si on doit visiter un nœud
     */
    shouldVisit?: (node: TNode, state: TState, visited: Set<string>) => boolean;
    
    /**
     * Fonction pour convertir un nœud en string (pour le Set)
     */
    nodeToString?: (node: TNode) => string;
    
    /**
     * Fonction pour créer une clé d'état (pour éviter de revisiter le même état)
     */
    getStateKey?: (node: TNode, state: TState) => string;
}

interface QueueItem<TNode, TState> {
    node: TNode;
    state: TState;
    path: TNode[];
    distance: number;
}

/**
 * Classe BFS générique
 */
export class BFS<TNode = string, TState = Record<string, any>> {
    private options: BFSOptions<TNode, TState>;
    
    constructor(options: BFSOptions<TNode, TState>) {
        this.options = options;
    }
    
    /**
     * Trouve le chemin le plus court vers un nœud cible
     */
    findShortestPath(start: TNode, initialState: TState): TNode[] | null {
        const queue: QueueItem<TNode, TState>[] = [{
            node: start,
            state: initialState,
            path: [start],
            distance: 0,
        }];
        
        const visited = new Set<string>();
        const startKey = this.getNodeKey(start, initialState);
        visited.add(startKey);
        
        while (queue.length > 0) {
            const item = queue.shift()!;
            const { node, state, path } = item;
            
            // Vérifier si on a atteint la cible
            if (this.options.isTarget && this.options.isTarget(node, state)) {
                return path;
            }
            
            // Explorer les voisins
            const neighbors = this.options.getNeighbors(node, state);
            
            for (const neighbor of neighbors) {
                const newState = this.options.updateState
                    ? this.options.updateState(neighbor, state)
                    : state;
                
                const neighborKey = this.getNodeKey(neighbor, newState);
                
                if (visited.has(neighborKey)) {
                    continue;
                }
                
                if (this.options.shouldVisit && !this.options.shouldVisit(neighbor, newState, visited)) {
                    continue;
                }
                
                visited.add(neighborKey);
                queue.push({
                    node: neighbor,
                    state: newState,
                    path: [...path, neighbor],
                    distance: item.distance + 1,
                });
            }
        }
        
        return null; // Pas de chemin trouvé
    }
    
    /**
     * Trouve la distance minimale vers un nœud cible
     */
    findShortestDistance(start: TNode, initialState: TState): number {
        const path = this.findShortestPath(start, initialState);
        return path ? path.length - 1 : -1;
    }
    
    /**
     * Trouve tous les nœuds atteignables depuis start
     */
    findReachable(start: TNode, initialState: TState): Set<TNode> {
        const queue: QueueItem<TNode, TState>[] = [{
            node: start,
            state: initialState,
            path: [start],
            distance: 0,
        }];
        
        const visited = new Set<string>();
        const reachable = new Set<TNode>();
        
        const startKey = this.getNodeKey(start, initialState);
        visited.add(startKey);
        reachable.add(start);
        
        while (queue.length > 0) {
            const item = queue.shift()!;
            const { node, state } = item;
            
            const neighbors = this.options.getNeighbors(node, state);
            
            for (const neighbor of neighbors) {
                const newState = this.options.updateState
                    ? this.options.updateState(neighbor, state)
                    : state;
                
                const neighborKey = this.getNodeKey(neighbor, newState);
                
                if (visited.has(neighborKey)) {
                    continue;
                }
                
                if (this.options.shouldVisit && !this.options.shouldVisit(neighbor, newState, visited)) {
                    continue;
                }
                
                visited.add(neighborKey);
                reachable.add(neighbor);
                queue.push({
                    node: neighbor,
                    state: newState,
                    path: [...item.path, neighbor],
                    distance: item.distance + 1,
                });
            }
        }
        
        return reachable;
    }
    
    /**
     * Parcourt le graphe niveau par niveau
     */
    traverseLevelOrder(
        start: TNode,
        initialState: TState,
        onVisit: (node: TNode, state: TState, level: number) => void
    ): void {
        const queue: QueueItem<TNode, TState>[] = [{
            node: start,
            state: initialState,
            path: [start],
            distance: 0,
        }];
        
        const visited = new Set<string>();
        const startKey = this.getNodeKey(start, initialState);
        visited.add(startKey);
        
        while (queue.length > 0) {
            const item = queue.shift()!;
            const { node, state, distance } = item;
            
            onVisit(node, state, distance);
            
            const neighbors = this.options.getNeighbors(node, state);
            
            for (const neighbor of neighbors) {
                const newState = this.options.updateState
                    ? this.options.updateState(neighbor, state)
                    : state;
                
                const neighborKey = this.getNodeKey(neighbor, newState);
                
                if (visited.has(neighborKey)) {
                    continue;
                }
                
                if (this.options.shouldVisit && !this.options.shouldVisit(neighbor, newState, visited)) {
                    continue;
                }
                
                visited.add(neighborKey);
                queue.push({
                    node: neighbor,
                    state: newState,
                    path: [...item.path, neighbor],
                    distance: distance + 1,
                });
            }
        }
    }
    
    /**
     * Trouve tous les chemins de longueur minimale vers un nœud cible
     */
    findAllShortestPaths(start: TNode, initialState: TState): TNode[][] {
        const queue: QueueItem<TNode, TState>[] = [{
            node: start,
            state: initialState,
            path: [start],
            distance: 0,
        }];
        
        const visited = new Map<string, number>(); // key -> distance minimale
        const paths: TNode[][] = [];
        let minDistance = Infinity;
        
        const startKey = this.getNodeKey(start, initialState);
        visited.set(startKey, 0);
        
        while (queue.length > 0) {
            const item = queue.shift()!;
            const { node, state, path, distance } = item;
            
            // Si on a déjà trouvé des chemins plus courts, ignorer
            if (distance > minDistance) {
                continue;
            }
            
            // Vérifier si on a atteint la cible
            if (this.options.isTarget && this.options.isTarget(node, state)) {
                if (distance < minDistance) {
                    minDistance = distance;
                    paths.length = 0; // Réinitialiser
                }
                if (distance === minDistance) {
                    paths.push([...path]);
                }
                continue;
            }
            
            const neighbors = this.options.getNeighbors(node, state);
            
            for (const neighbor of neighbors) {
                const newState = this.options.updateState
                    ? this.options.updateState(neighbor, state)
                    : state;
                
                const neighborKey = this.getNodeKey(neighbor, newState);
                const newDistance = distance + 1;
                
                // Visiter si pas encore visité ou si même distance
                const visitedDistance = visited.get(neighborKey);
                if (visitedDistance !== undefined && visitedDistance < newDistance) {
                    continue;
                }
                
                if (this.options.shouldVisit && !this.options.shouldVisit(neighbor, newState, visited as any)) {
                    continue;
                }
                
                visited.set(neighborKey, newDistance);
                queue.push({
                    node: neighbor,
                    state: newState,
                    path: [...path, neighbor],
                    distance: newDistance,
                });
            }
        }
        
        return paths;
    }
    
    private getNodeKey(node: TNode, state: TState): string {
        if (this.options.getStateKey) {
            return this.options.getStateKey(node, state);
        }
        
        if (this.options.nodeToString) {
            return this.options.nodeToString(node);
        }
        
        return String(node);
    }
}

