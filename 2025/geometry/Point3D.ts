export class Point3D {
    constructor(public x: number, public y: number, public z: number) {}

    getDistanceSquared(other: Point3D): number {
        return Math.pow(this.x - other.x, 2) + Math.pow(this.y - other.y, 2) + Math.pow(this.z - other.z, 2);
    }

    toString(): string {
        return `(${this.x},${this.y},${this.z})`;
    }
}

