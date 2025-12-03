import fs from 'fs';

const testMode = process.argv[2] === 'test';

export abstract class Main {
    protected lines: string[];

    constructor() {
        const input = fs.readFileSync(testMode ? 'exemple.txt' : 'input.txt', 'utf8');
        this.lines = input.split('\n').filter(line => line.length > 0);
    }


    protected abstract parseInput(): void;
    protected abstract solve1(): number;
    protected abstract solve2(): number;

    public solve() : {solution1: number, solution2: number} {
        this.parseInput();
        return {solution1: this.solve1(), solution2: this.solve2()};
    }
}