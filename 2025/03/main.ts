import fs from 'fs';

const input = fs.readFileSync('input.txt', 'utf8');

const lines = input.split('\n').filter(line => line.length > 0);

const solution = (lines: string[]) => {
    let solution1 = 0;
    let solution2 = 0;

    for (const line of lines) {
        const batteries = line.split('').map(Number);

        solution1 += getMaxJolt(batteries, 2);
        solution2 += getMaxJolt(batteries, 12);
    }

    return {solution1, solution2};
}

function getMaxJolt(batteries: number[], digitLength: number) {
    const result: number[] = [];
    let startIndex = 0;
    
    for (let pos = 0; pos < digitLength; pos++) {
        const remainingDigits = digitLength - pos - 1;
        // on réduit la recherche pour laisser assez de batteries pour les digits restants
        const searchLimit = batteries.length - remainingDigits;
        let maxDigit = -1;
        // on initialise le max digit avec le premier digit
        let maxIndex = startIndex;
        
        for (let i = startIndex; i < searchLimit; i++) {
            if (batteries[i] > maxDigit) {
                maxDigit = batteries[i];
                maxIndex = i;
            }
        }
        
        result.push(maxDigit);
        // on avance l'index de la recherche
        startIndex = maxIndex + 1;
    }
    
    return Number(result.join(''));
}

console.log(solution(lines));