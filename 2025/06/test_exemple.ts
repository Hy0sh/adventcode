// Test avec l'exemple de l'énoncé
const lines = [
    "123 328  51 64 ",
    " 45 64  387 23 ",
    "  6 98  215 314",
    "*   +   *   +  "
];

const rawLines = lines.slice(0, -1);

// Inverser chaque nombre
const reversedLines: string[] = [];
for(const line of rawLines) {
    let reversedLine = '';
    let currentNum = '';
    
    for(let i = 0; i < line.length; i++) {
        const char = line[i];
        if(char >= '0' && char <= '9') {
            currentNum += char;
        } else {
            if(currentNum.length > 0) {
                reversedLine += currentNum.split('').reverse().join('');
                currentNum = '';
            }
            reversedLine += char;
        }
    }
    if(currentNum.length > 0) {
        reversedLine += currentNum.split('').reverse().join('');
    }
    
    reversedLines.push(reversedLine);
}

console.log("Lignes inversées :");
reversedLines.forEach((l, i) => console.log(`  ${i}: "${l}"`));

// Lire colonne par colonne de droite à gauche
const maxLen = Math.max(...reversedLines.map(l => l.length));
const operatorLine = lines[lines.length - 1];

console.log(`\nLongueur max: ${maxLen}`);
console.log(`Opérateurs: "${operatorLine}"`);

const problems: Array<{numbers: number[], operator: string}> = [];
let currentProblemNums: number[] = [];
let currentOperator = '';

for(let charIdx = maxLen - 1; charIdx >= 0; charIdx--) {
    let columnDigits = '';
    let hasDigit = false;
    
    for(const line of reversedLines) {
        const char = line[charIdx] || ' ';
        if(char >= '0' && char <= '9') {
            columnDigits += char;
            hasDigit = true;
        }
    }
    
    const opChar = operatorLine[charIdx] || ' ';
    
    console.log(`Colonne ${charIdx}: digits="${columnDigits}", op="${opChar}"`);
    
    if(hasDigit) {
        currentProblemNums.push(Number(columnDigits));
        if(opChar !== ' ' && opChar !== undefined) {
            currentOperator = opChar;
        }
    } else if(opChar === ' ') {
        if(currentProblemNums.length > 0 && currentOperator !== '') {
            console.log(`  -> Problème: ${currentProblemNums.join(' ' + currentOperator + ' ')} = ?`);
            problems.push({
                numbers: currentProblemNums.slice(),
                operator: currentOperator
            });
            currentProblemNums = [];
            currentOperator = '';
        }
    }
}

if(currentProblemNums.length > 0 && currentOperator !== '') {
    console.log(`  -> Problème final: ${currentProblemNums.join(' ' + currentOperator + ' ')} = ?`);
    problems.push({
        numbers: currentProblemNums.slice(),
        operator: currentOperator
    });
}

console.log(`\nProblèmes trouvés: ${problems.length}`);
let grandTotal = 0;
for(const problem of problems) {
    let result = problem.operator === '*' ? 1 : 0;
    for(const num of problem.numbers) {
        result = problem.operator === '*' ? result * num : result + num;
    }
    console.log(`  ${problem.numbers.join(' ' + problem.operator + ' ')} = ${result}`);
    grandTotal += result;
}

console.log(`\nGrand total: ${grandTotal}`);
console.log(`Attendu: 3263827`);

