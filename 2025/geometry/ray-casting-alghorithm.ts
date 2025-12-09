import { Point } from "./Point";

/**
 * Détermine si un point est à l'intérieur d'un polygone en utilisant l'algorithme ray-casting
 * @param polygonVertices - Les sommets du polygone dans l'ordre
 * @param point - Le point à tester
 * @return true si le point est à l'intérieur du polygone
 */
export function isPointInsidePolygon(polygonVertices: Point[], point: Point): boolean {
    // https://rosettacode.org/wiki/Ray-casting_algorithm
    // Compte combien de fois un rayon horizontal depuis le point traverse le polygone
    let intersectionCount = 0;
    
    for (let i = 0; i < polygonVertices.length; i++) {
        const currentVertex = polygonVertices[i];
        const nextVertex = polygonVertices[(i + 1) % polygonVertices.length];
        
        if (isPointWestOfEdge(currentVertex, nextVertex, point)) {
            intersectionCount++;
        }
    }
    
    // Si le nombre d'intersections est impair, le point est à l'intérieur
    return intersectionCount % 2 !== 0;
}

/**
 * Détermine si un point est à l'ouest (à gauche) d'une arête du polygone
 * Utilisé pour déterminer si un rayon horizontal traverse l'arête
 * @param edgeStart - Point de départ de l'arête
 * @param edgeEnd - Point de fin de l'arête
 * @param point - Le point à tester
 * @return true si le point est à l'ouest de l'arête
 */
function isPointWestOfEdge(edgeStart: Point, edgeEnd: Point, point: Point): boolean {
    if (edgeStart.y <= edgeEnd.y) {
        // Le point est en dehors de la plage verticale de l'arête
        if (point.y <= edgeStart.y || point.y > edgeEnd.y || 
            (point.x >= edgeStart.x && point.x >= edgeEnd.x)) {
            return false;
        } 
        // Le point est clairement à l'ouest de l'arête
        else if (point.x < edgeStart.x && point.x < edgeEnd.x) {
            return true;
        } 
        // Calcul de la pente pour déterminer la position
        else {
            return (point.y - edgeStart.y) / (point.x - edgeStart.x) > (edgeEnd.y - edgeStart.y) / (edgeEnd.x - edgeStart.x);
        }
    } else {
        // Inverse les points pour avoir edgeStart.y <= edgeEnd.y
        return isPointWestOfEdge(edgeEnd, edgeStart, point);
    }
}