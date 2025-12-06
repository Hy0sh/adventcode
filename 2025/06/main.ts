import { Main } from '../main.class';

export class Day06 extends Main {
    numbers: string[][] = [];
    operators: string[] = [];
    
    protected parseInput(): void {
        const charGrid = this.lines.map(line => line.split(''));
        const numLines = charGrid.length;
        const lineLength = charGrid[0].length;
        
        const separatorColumns = new Set<number>();
        for (let col = 0; col < lineLength; col++) {
            if (charGrid.every(line => line[col] === ' ')) {
                separatorColumns.add(col);
            }
        }
        
        for (let lineIdx = 0; lineIdx < numLines; lineIdx++) {
            const chars = charGrid[lineIdx];
            const isLastLine = lineIdx === numLines - 1;
            const targetArray = isLastLine ? this.operators : (this.numbers[lineIdx] = []);
            
            let currentString = '';
            for (let charIdx = 0; charIdx < chars.length; charIdx++) {
                const char = chars[charIdx];
                
                if (char !== ' ') {
                    currentString += char;
                } else if (separatorColumns.has(charIdx)) {
                    if (currentString) {
                        targetArray.push(isLastLine ? currentString.trim() : currentString);
                        currentString = '';
                    }
                } else {
                    currentString += ' ';
                }
            }
            
            if (currentString) {
                targetArray.push(isLastLine ? currentString.trim() : currentString);
            }
        }
    }

    protected solve1(): number {
        let grandTotal = 0;
        
        for (let colIdx = 0; colIdx < this.operators.length; colIdx++) {
            const operator = this.operators[colIdx];
            let result = operator === '*' ? 1 : 0;
            
            for (const numbers of this.numbers) {
                const num = Number(numbers[colIdx].trim());
                result = operator === '*' ? result * num : result + num;
            }
            
            grandTotal += result;
        }
        
        return grandTotal;
    }

    protected solve2(): number {
        let grandTotal = 0;
        
        for (let colIdx = this.operators.length - 1; colIdx >= 0; colIdx--) {
            const operator = this.operators[colIdx];
            const stringLength = this.numbers[0][colIdx].length;
            
            let result = operator === '*' ? 1 : 0;
            
            for (let charIdx = stringLength - 1; charIdx >= 0; charIdx--) {
                let verticalNumber = '';
                for (const rowNumbers of this.numbers) {
                    verticalNumber += rowNumbers[colIdx][charIdx];
                }
                const num = Number(verticalNumber);
                
                result = operator === '*' ? result * num : result + num;
            }
            
            grandTotal += result;
        }
        
        return grandTotal;
    }
}

console.log(new Day06().solve());

