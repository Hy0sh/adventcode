import { Main } from '../main.class';

export class Day03 extends Main {

    private batteries: number[][] = [];

    protected parseInput(): void {
        this.batteries = this.lines.map(line => line.split('').map(Number));
    }

    protected solve1(): number {
        let solution1 = 0;
        for (const bank of this.batteries) {
            solution1 += this.getMaxJolt(bank, 2);
        }
        return solution1;
    }

    protected solve2(): number {
        let solution2 = 0;
        for (const bank of this.batteries) {
            solution2 += this.getMaxJolt(bank, 12);
        }
        return solution2;
    }

    private getMaxJolt(bank: number[], digitLength: number) {
        const result: number[] = [];
        let startIndex = 0;
        
        for (let pos = 0; pos < digitLength; pos++) {
            const remainingDigits = digitLength - pos - 1;
            // on réduit la recherche pour laisser assez de batteries pour les digits restants
            const searchLimit = bank.length - remainingDigits;
            let maxDigit = -1;
            // on initialise le max digit avec le premier digit
            let maxIndex = startIndex;
            
            for (let i = startIndex; i < searchLimit; i++) {
                if (bank[i] > maxDigit) {
                    maxDigit = bank[i];
                    maxIndex = i;
                }
            }
            
            result.push(maxDigit);
            // on avance l'index de la recherche
            startIndex = maxIndex + 1;
        }
        
        return Number(result.join(''));
    }

}



console.log(new Day03().solve());