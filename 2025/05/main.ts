import { Main } from '../../typescript/main.class';
import { IntervalMerger } from '../../typescript/algorithms/interval-merger.class';

export class Day05 extends Main {

    private ranges: number[][] = []
    private numbers: number[] = [];
    protected parseInput(): void {
        this.ranges = this.lines.filter(line => line.includes('-')).map(line => line.split('-').map(Number));
        this.numbers = this.lines.filter(line => !line.includes('-')).map(line => Number(line));
    }

    protected solve1(): number {
        return this.numbers.filter(number => 
            IntervalMerger.isPointCovered(this.ranges.map(r => [r[0], r[1]] as [number, number]), number)
        ).length;
    }

    protected solve2(): number {
        const merged = IntervalMerger.merge(
            this.ranges.map(r => [r[0], r[1]] as [number, number]),
            true // allowAdjacent
        );
        
        return merged.reduce((total, range) => total + (range[1] - range[0] + 1), 0);
    }

}

console.log(new Day05().solve());

