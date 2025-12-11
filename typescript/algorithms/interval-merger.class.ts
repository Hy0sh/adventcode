/**
 * Classe pour fusionner des intervalles qui se chevauchent ou sont adjacents
 * 
 * Algorithme classique de fusion d'intervalles :
 * 1. Trier les intervalles par début
 * 2. Fusionner les intervalles qui se chevauchent ou sont adjacents
 * 
 * Complexité: O(n log n) pour le tri + O(n) pour la fusion = O(n log n)
 */

export interface Interval {
    start: number;
    end: number;
}

export class IntervalMerger {
    /**
     * Fusionne des intervalles qui se chevauchent ou sont adjacents
     * 
     * @param intervals Liste d'intervalles à fusionner
     * @param allowAdjacent Si true, fusionne aussi les intervalles adjacents (par défaut: true)
     * @returns Liste d'intervalles fusionnés, triés par début
     * 
     * @example
     * merge([[1, 3], [2, 6], [8, 10], [15, 18]])
     * // => [[1, 6], [8, 10], [15, 18]]
     * 
     * @example
     * merge([[1, 4], [5, 6]]) // allowAdjacent = true par défaut
     * // => [[1, 6]]
     * 
     * @example
     * merge([[1, 4], [5, 6]], false) // allowAdjacent = false
     * // => [[1, 4], [5, 6]]
     */
    static merge(intervals: [number, number][], allowAdjacent: boolean = true): [number, number][] {
        if (intervals.length === 0) {
            return [];
        }
        
        // Trier par début
        const sorted = [...intervals].sort((a, b) => a[0] - b[0]);
        
        const merged: [number, number][] = [];
        let current = sorted[0];
        
        for (let i = 1; i < sorted.length; i++) {
            const next = sorted[i];
            const gap = allowAdjacent ? 1 : 0;
            
            // Si l'intervalle suivant chevauche ou est adjacent
            if (next[0] <= current[1] + gap) {
                // Fusionner en étendant la fin
                current = [current[0], Math.max(current[1], next[1])];
            } else {
                // Pas de chevauchement, ajouter l'intervalle courant
                merged.push(current);
                current = next;
            }
        }
        
        // Ajouter le dernier intervalle
        merged.push(current);
        
        return merged;
    }
    
    /**
     * Fusionne des intervalles avec des objets personnalisés
     */
    static mergeIntervals(intervals: Interval[], allowAdjacent: boolean = true): Interval[] {
        if (intervals.length === 0) {
            return [];
        }
        
        // Trier par début
        const sorted = [...intervals].sort((a, b) => a.start - b.start);
        
        const merged: Interval[] = [];
        let current = { ...sorted[0] };
        
        for (let i = 1; i < sorted.length; i++) {
            const next = sorted[i];
            const gap = allowAdjacent ? 1 : 0;
            
            // Si l'intervalle suivant chevauche ou est adjacent
            if (next.start <= current.end + gap) {
                // Fusionner en étendant la fin
                current.end = Math.max(current.end, next.end);
            } else {
                // Pas de chevauchement, ajouter l'intervalle courant
                merged.push(current);
                current = { ...next };
            }
        }
        
        // Ajouter le dernier intervalle
        merged.push(current);
        
        return merged;
    }
    
    /**
     * Calcule le nombre total d'unités couvertes par les intervalles
     * (après fusion des chevauchements)
     */
    static getTotalCoverage(intervals: [number, number][]): number {
        const merged = this.merge(intervals, false);
        return merged.reduce((total, [start, end]) => total + (end - start + 1), 0);
    }
    
    /**
     * Vérifie si un point est couvert par au moins un intervalle
     */
    static isPointCovered(intervals: [number, number][], point: number): boolean {
        return intervals.some(([start, end]) => point >= start && point <= end);
    }
    
    /**
     * Trouve les lacunes entre les intervalles fusionnés
     * @returns Liste d'intervalles représentant les lacunes
     */
    static findGaps(intervals: [number, number][]): [number, number][] {
        if (intervals.length === 0) {
            return [];
        }
        
        const merged = this.merge(intervals, false);
        const gaps: [number, number][] = [];
        
        for (let i = 0; i < merged.length - 1; i++) {
            const gapStart = merged[i][1] + 1;
            const gapEnd = merged[i + 1][0] - 1;
            
            if (gapStart <= gapEnd) {
                gaps.push([gapStart, gapEnd]);
            }
        }
        
        return gaps;
    }
}
