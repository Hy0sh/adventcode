// Test final avec la bonne compréhension

const lines = [
    "123 328  51 64 ",
    " 45 64  387 23 ",
    "  6 98  215 314",
    "*   +   *   +  "
];

const rawLines = lines.slice(0, -1);
const operatorLine = lines[lines.length - 1];

// Étape 1: Identifier les positions des problèmes (séparés par des espaces)
// On parse normalement pour identifier les colonnes de nombres
const numbersPerLine: number[][] = [];
for(const line of rawLines) {
    const nums = line.split(' ').map(part => part.trim()).filter(p => p.length > 0).map(Number);
    numbersPerLine.push(nums);
}

console.log("Nombres par ligne:");
numbersPerLine.forEach((nums, i) => console.log(`  ${i}: ${nums.join(', ')}`));

const operators = operatorLine.split(' ').map(part => part.trim()).filter(p => p.length > 0);
console.log("Opérateurs:", operators);

// Étape 2: Inverser chaque nombre
const reversedNumbers: number[][] = numbersPerLine.map(row => 
    row.map(num => Number(num.toString().split('').reverse().join('')))
);

console.log("\nNombres inversés:");
reversedNumbers.forEach((nums, i) => console.log(`  ${i}: ${nums.join(', ')}`));

// Étape 3: Lire les problèmes de DROITE à GAUCHE
// Pour chaque position de problème (en partant de la droite):
const numProblems = operators.length;
const problems: Array<{numbers: number[], operator: string}> = [];

for(let probIdx = numProblems - 1; probIdx >= 0; probIdx--) {
    console.log(`\n=== Problème ${probIdx} (opérateur: ${operators[probIdx]}) ===`);
    
    // Récupérer les nombres de ce problème (inversés)
    const problemNumbers: number[] = [];
    for(const row of reversedNumbers) {
        if(probIdx < row.length) {
            problemNumbers.push(row[probIdx]);
        }
    }
    
    console.log(`Nombres inversés pour ce problème: ${problemNumbers.join(', ')}`);
    
    // Étape 4: Lire ces nombres colonne par colonne (de droite à gauche)
    // pour reconstruire de nouveaux nombres
    const numberStrings = problemNumbers.map(n => n.toString());
    const maxLen = Math.max(...numberStrings.map(s => s.length));
    
    const reconstructedNumbers: number[] = [];
    
    for(let digitPos = 0; digitPos < maxLen; digitPos++) {
        let digitStr = '';
        for(const numStr of numberStrings) {
            const digit = numStr[digitPos] || '';
            if(digit !== '') {
                digitStr += digit;
            }
        }
        if(digitStr.length > 0) {
            reconstructedNumbers.push(Number(digitStr));
        }
    }
    
    console.log(`Nombres reconstruits (lus de droite à gauche par chiffres): ${reconstructedNumbers.join(', ')}`);
    
    problems.push({
        numbers: reconstructedNumbers,
        operator: operators[probIdx]
    });
}

// Calculer le total
console.log("\n=== CALCULS ===");
let grandTotal = 0;
for(const problem of problems) {
    let result = problem.operator === '*' ? 1 : 0;
    for(const num of problem.numbers) {
        result = problem.operator === '*' ? result * num : result + num;
    }
    console.log(`${problem.numbers.join(' ' + problem.operator + ' ')} = ${result}`);
    grandTotal += result;
}

console.log(`\nGrand Total: ${grandTotal}`);
console.log(`Attendu: 3263827`);

