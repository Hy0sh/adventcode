import fs from 'fs';

const input = fs.readFileSync('input.txt', 'utf8');

const lines = input.split('\n').filter(line => line.length > 0);

const solution = (lines: string[]) => {
    let position = 50;
    let solution1 = 0;
    let solution2 = 0;
    
    for (const line of lines) {
        const amount = parseInt(line.substring(1));

        if (line[0] === 'R') {
            solution2 += Math.floor((position + amount) / 100);
            position = (position + amount) % 100;
        } else {
            solution2 += Math.floor((amount - position) / 100) + Number(position !== 0);
            position = (position - amount % 100 + 100) % 100;
        }
        
        solution1 += Number(position === 0);
    }

    return {solution1, solution2};
}

console.log(solution(lines));