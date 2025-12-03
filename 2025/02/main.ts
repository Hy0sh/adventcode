import { Main } from '../main.class';

export class Day02 extends Main {

    private coupleIds: number[][] = [];

    protected parseInput(): void {
        this.coupleIds = this.lines.map(line => line.split(',').map(ids => ids.split('-').map(Number))).flat();
    }

    protected solve1(): number {
        let solution1 = 0;
        
        for (const coupleId of this.coupleIds) {
            const [id1, id2] = coupleId;
            for(let i = id1; i <= id2; i++) {
                const iString = i.toString();
                if(iString.length % 2 === 0 && iString.substring(0, iString.length / 2) === iString.substring(iString.length / 2)) {
                    solution1 += i;
                }
            }
        }

        return solution1;
    }

    protected solve2(): number {
        let solution2 = 0;
        
        for (const coupleId of this.coupleIds) {
            const [id1, id2] = coupleId;
            for(let i = id1; i <= id2; i++) {
                if(this.isInvalidId(i)) {
                    solution2 += i;
                }
            }
        }

        return solution2;
    }

    private isInvalidId(id: number): boolean {
        const idString = id.toString();
        
        for(let i = 0; i < idString.length; i++) {
            if(i > idString.length / 2) {
                return false;
            }
            const subIdString = idString.substring(0, i);
            if(idString.length % subIdString.length !== 0) {
                continue;
            }
            const nbPossiblePattern = idString.length / subIdString.length;
            if(nbPossiblePattern > 1 && idString.includes(subIdString.repeat(nbPossiblePattern))) {
                return true;
            }
        }
        return false;
    }

}

console.log(new Day02().solve());