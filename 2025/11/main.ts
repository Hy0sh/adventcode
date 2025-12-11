import { Main } from '../main.class';
import { DFS } from '../graph/dfs.class';

export class Day11 extends Main {

    private devices: Map<string, string[]> = new Map();

    protected parseInput(): void {
        this.devices.clear();
        for (const line of this.lines) {
            const [device, connections] = line.split(': ');
            this.devices.set(device, connections.split(' '));
        }
    }

    protected solve1(): number {
        const dfs = new DFS<string, {}>({
            getNeighbors: (node) => this.devices.get(node) || [],
            isTerminal: (node) => node === 'out',
        });

        return dfs.countPaths('you', {});
    }


    protected solve2(): number {
        interface PathState {
            visitedDac: boolean;
            visitedFft: boolean;
        }

        const dfs = new DFS<string, PathState>({
            getNeighbors: (node) => this.devices.get(node) || [],
            isTerminal: (node) => node === 'out',
            shouldCount: (_, state) => state.visitedDac && state.visitedFft,
            updateState: (node, state) => ({
                visitedDac: state.visitedDac || node === 'dac',
                visitedFft: state.visitedFft || node === 'fft',
            }),
            getMemoKey: (node, state) => 
                `${node}|${state.visitedDac}|${state.visitedFft}`,
        });

        return dfs.countPaths('svr', { visitedDac: false, visitedFft: false });
    }

}

console.log(new Day11().solve());

