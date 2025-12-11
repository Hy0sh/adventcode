import fs from 'fs';

const testMode = process.argv[2] === 'test';

export abstract class Main {
    protected lines: string[];
    private hasDifferentExmpleInput: boolean = false;
    constructor() {
        try {
            fs.readFileSync('exemple-01.txt', 'utf8');
            fs.readFileSync('exemple-02.txt', 'utf8');
            this.hasDifferentExmpleInput = testMode;
        } catch (error) {
            this.hasDifferentExmpleInput = false;
        }

        if(!this.hasDifferentExmpleInput) {
            const input = fs.readFileSync(testMode ? 'exemple.txt' : 'input.txt', 'utf8');
            this.lines = input.split('\n').filter(line => line.length > 0);
        }else {
            this.lines = [];
        }
    }


    protected abstract parseInput(): void;
    protected abstract solve1(): number;
    protected abstract solve2(): number;

    public solve() : {solution1: number, solution2: number} {
        const startParseInput = performance.now();
        if(this.hasDifferentExmpleInput) {
            const input1 = fs.readFileSync('exemple-01.txt', 'utf8');
            this.lines = input1.split('\n').filter(line => line.length > 0);
        }
        this.parseInput();
        console.log(`Time taken to parse input: ${performance.now() - startParseInput} milliseconds`);
        const startSolve1 = performance.now();
        const solution1 = this.solve1();
        console.log(`Time taken to solve part 1: ${performance.now() - startSolve1} milliseconds`);
        if(this.hasDifferentExmpleInput) {
            const input1 = fs.readFileSync('exemple-02.txt', 'utf8');
            this.lines = input1.split('\n').filter(line => line.length > 0);
            this.parseInput();
        }
        const startSolve2 = performance.now();
        const solution2 = this.solve2();
        console.log(`Time taken to solve part 2: ${performance.now() - startSolve2} milliseconds`);
        return {solution1, solution2};
    }
}