import matplotlib.pyplot as plt
import numpy as np

# ─── Datos de construccion ───────────────────────────────────────────────────

N_values = [2**i for i in range(15, 25)]

with open("timeResults.txt") as f:
    lines = f.readlines()

def parse_times(line):
    start = line.index('[') + 1
    end = line.index(']')
    return list(map(int, line[start:end].split(',')))

rnd_nx  = parse_times(lines[1])
rnd_str = parse_times(lines[2])
eu_nx   = parse_times(lines[4])
eu_str  = parse_times(lines[5])

plt.figure()
plt.plot(N_values, rnd_nx,  marker='o', label='Random Nearest-X')
plt.plot(N_values, rnd_str, marker='s', label='Random STR')
plt.plot(N_values, eu_nx,   marker='^', label='Europa Nearest-X')
plt.plot(N_values, eu_str,  marker='D', label='Europa STR')
plt.xscale('log', base=2)
plt.xlabel('N')
plt.ylabel('Tiempo (ms)')
plt.title('Tiempo de construccion vs N')
plt.legend()
plt.tight_layout()
plt.savefig('graficos/construccion.png', dpi=150)
plt.close()

# ─── Datos de consultas ───────────────────────────────────────────────────────

s_values = [0.0025, 0.005, 0.01, 0.025, 0.05]

trees = ['Random Nearest-X', 'Europa Nearest-X', 'Random STR', 'Europa STR']

ios_avg  = {t: [] for t in trees}
ios_std  = {t: [] for t in trees}
pts_avg  = {t: [] for t in trees}
pts_std  = {t: [] for t in trees}

with open("queryResults.txt") as f:
    content = f.read()

blocks = [b.strip() for b in content.strip().split('\n\n') if b.strip()]

for block in blocks:
    lines = block.split('\n')
    tree_name = lines[0].replace('Arbol: ', '')
    for line in lines[1:]:
        parts = line.split()
        ios_avg[tree_name].append(float(parts[1].split('=')[1]))
        ios_std[tree_name].append(float(parts[2].split('=')[1]))
        pts_avg[tree_name].append(float(parts[3].split('=')[1]))
        pts_std[tree_name].append(float(parts[4].split('=')[1]))

# ─── Grafico I/Os vs s ────────────────────────────────────────────────────────

plt.figure()
for tree in trees:
    plt.plot(s_values, ios_avg[tree], marker='o', label=tree)
plt.xlabel('s')
plt.ylabel('I/Os promedio')
plt.title('I/Os promedio vs s')
plt.legend()
plt.tight_layout()
plt.savefig('graficos/ios_vs_s.png', dpi=150)
plt.close()

# ─── Grafico puntos encontrados vs s - dataset aleatorio ─────────────────────

plt.figure()
for tree in ['Random Nearest-X', 'Random STR']:
    plt.errorbar(s_values, pts_avg[tree], yerr=pts_std[tree], marker='o', capsize=4, label=tree)
plt.xlabel('s')
plt.ylabel('Puntos encontrados (promedio)')
plt.title('Puntos encontrados vs s - Dataset aleatorio')
plt.legend()
plt.tight_layout()
plt.savefig('graficos/puntos_random.png', dpi=150)
plt.close()

# ─── Grafico puntos encontrados vs s - dataset Europa ────────────────────────

plt.figure()
for tree in ['Europa Nearest-X', 'Europa STR']:
    plt.errorbar(s_values, pts_avg[tree], yerr=pts_std[tree], marker='o', capsize=4, label=tree)
plt.xlabel('s')
plt.ylabel('Puntos encontrados (promedio)')
plt.title('Puntos encontrados vs s - Dataset Europa')
plt.legend()
plt.tight_layout()
plt.savefig('graficos/puntos_europa.png', dpi=150)
plt.close()
