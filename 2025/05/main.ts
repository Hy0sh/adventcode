import { Main } from '../main.class';

export class Day05 extends Main {

    private ranges: number[][] = []
    private numbers: number[] = [];
    protected parseInput(): void {
        this.ranges = this.lines.filter(line => line.includes('-')).map(line => line.split('-').map(Number));
        this.numbers = this.lines.filter(line => !line.includes('-')).map(line => Number(line));
    }

    protected solve1(): number {
        return this.numbers.filter(number => this.ranges.some(range => number >= range[0] && number <= range[1])).length;
    }

    protected solve2(): number {
        const sorted = this.ranges.sort((a, b) => a[0] - b[0]);
        
        const merged: number[][] = [];
        for (const range of sorted) {
            const last = merged.at(-1); // get the last range in the merged array
            if (last && range[0] <= last[1] + 1) {
                last[1] = Math.max(last[1], range[1]);
            } else {
                merged.push(range);
            }
        }
        
        return merged.reduce((total, range) => total + (range[1] - range[0] + 1), 0);
    }

}

console.log(new Day05().solve());

