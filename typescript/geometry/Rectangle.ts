import { Point } from './Point';

export class Rectangle {
    constructor(private pointA: Point, private pointB: Point) {}
    

    getMinX(): number {
        return Math.min(this.pointA.x, this.pointB.x);
    }

    getMaxX(): number {
        return Math.max(this.pointA.x, this.pointB.x);
    }

    getMinY(): number {
        return Math.min(this.pointA.y, this.pointB.y);
    }

    getMaxY(): number {
        return Math.max(this.pointA.y, this.pointB.y);
    }


    squareArea(): number {
        return this.distanceX() * this.distanceY();
    }
    
    distanceX(): number {
        return Math.abs(this.pointA.x - this.pointB.x) + 1;
    }
    
    distanceY(): number {
        return Math.abs(this.pointA.y - this.pointB.y) + 1;
    }
}

