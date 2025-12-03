import { Main } from '../main.class';

export class Day01 extends Main {

    protected parseInput(): void {
        // Pas de parsing spécial nécessaire, on utilise directement this.lines
    }

    protected solve1(): number {
        let position = 50;
        let solution1 = 0;
        
        for (const line of this.lines) {
            const amount = parseInt(line.substring(1));

            if (line[0] === 'R') {
                position = (position + amount) % 100;
            } else {
                position = (position - amount % 100 + 100) % 100;
            }
            
            solution1 += Number(position === 0);
        }

        return solution1;
    }

    protected solve2(): number {
        let position = 50;
        let solution2 = 0;
        
        for (const line of this.lines) {
            const amount = parseInt(line.substring(1));

            if (line[0] === 'R') {
                solution2 += Math.floor((position + amount) / 100);
                position = (position + amount) % 100;
            } else {
                solution2 += Math.floor((amount - position) / 100) + Number(position !== 0);
                position = (position - amount % 100 + 100) % 100;
            }
        }

        return solution2;
    }

}

console.log(new Day01().solve());