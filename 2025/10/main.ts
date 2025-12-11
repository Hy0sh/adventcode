import { Main } from '../main.class';
import { BFS } from '../graph/bfs.class';
// @ts-ignore
import lpSolver from 'javascript-lp-solver';

class Machine {
    private lightState: boolean[] = [];
    private targetLightState: boolean[] = [];
    private buttons: number[][] = [];
    private jolts: number[] = [];

    constructor(input: string) {
        const regexState = /\[([.#]+)\]/;
        const matchState = input.match(regexState);
        if (matchState) {
            this.targetLightState = matchState[1].split('').map((char) => char === '#');
            this.lightState = matchState[1].split('').map((char) => false);
        }
        const regexButtons = /\(([^)]+)\)/g;
        const matchButtons = [...input.matchAll(regexButtons)];
        if (matchButtons.length > 0) {
            this.buttons = matchButtons.map(match => match[1].split(',').map(Number));
        }
        const regexJolts = /\s*\{(.*)\}/;
        const matchJolts = input.match(regexJolts);
        if (matchJolts) {
            this.jolts = matchJolts[1].split(',').map(jolt => Number(jolt));
        }
    }

    isTargetState(currentState: boolean[]) :boolean {
        return currentState.every((lightState, index) => lightState === this.targetLightState[index]);
    }

    // Convertir l'état en clé unique
    stateToKey(state: boolean[]): string {
        return state.map(b => b ? '1' : '0').join('');
    }

    applyButton(state: boolean[], button: number[]) :boolean[] {
        return state.map((lightState, index) => button.includes(index) ? !lightState : lightState);
    }

    // BFS pour trouver le nombre minimal d'étapes pour atteindre l'état cible
    turnOnSequence(): number {
        const bfs = new BFS<boolean[], {}>({
            getNeighbors: (state) => {
                return this.buttons.map(button => this.applyButton(state, button));
            },
            isTarget: (state) => this.isTargetState(state),
            nodeToString: (state) => this.stateToKey(state),
        });
        
        return bfs.findShortestDistance(this.lightState.slice(), {});
    }

    adjustJolts() {
        // Programmation Linéaire en Nombres Entiers (ILP)
        const model: any = {
            optimize: "presses",
            opType: "min",
            constraints: {},
            variables: {},
            ints: {}
        };
        
        // Créer une variable pour chaque bouton
        for (let i = 0; i < this.buttons.length; i++) {
            const varName = `btn${i}`;
            model.variables[varName] = { presses: 1 };
            model.ints[varName] = 1;
        }
        
        // Créer une contrainte pour chaque position (jolt)
        for (let pos = 0; pos < this.jolts.length; pos++) {
            const constraintName = `pos${pos}`;
            model.constraints[constraintName] = { equal: this.jolts[pos] };
            
            // Pour chaque bouton qui touche cette position
            for (let i = 0; i < this.buttons.length; i++) {
                if (this.buttons[i].includes(pos)) {
                    const varName = `btn${i}`;
                    if (!model.variables[varName][constraintName]) {
                        model.variables[varName][constraintName] = 0;
                    }
                    model.variables[varName][constraintName] += 1;
                }
            }
        }
        
        return lpSolver.Solve(model).result;
    }
}

export class Day10 extends Main {

    private machines: Machine[] = [];
    protected parseInput(): void {
        this.machines = this.lines.map(line => new Machine(line));
    }

    protected solve1(): number {
        return this.machines.map(machine => machine.turnOnSequence()).reduce((a, b) => a + b, 0);
    }

    protected solve2(): number {
        return this.machines.map(machine => machine.adjustJolts()).reduce((a, b) => a + b, 0);
    }

}

console.log(new Day10().solve());

