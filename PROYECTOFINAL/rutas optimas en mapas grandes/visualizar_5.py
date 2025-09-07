import matplotlib.pyplot as plt
import networkx as nx
import os
import json

# Funcion para cargar posiciones
def cargar_posiciones(G):
    if os.path.exists("posiciones.txt"):
        with open("posiciones.txt") as f:
            raw_pos = json.load(f)
            return {int(k): tuple(v) for k, v in raw_pos.items()}
    else:
        pos = nx.spring_layout(G, seed=42)
        with open("posiciones.txt", "w") as f:
            json.dump({str(k): list(v) for k, v in pos.items()}, f)
        return pos

# Leer grafo.txt
G = nx.Graph()
with open("grafo.txt") as f:
    n, m = map(int, f.readline().strip().split())
    for line in f:
        u, v, peso = map(int, line.strip().split())
        G.add_edge(u, v, weight=peso)

pos = cargar_posiciones(G)
labels = nx.get_edge_attributes(G, 'weight')

# Listado de algoritmos y sus archivos
algoritmos = [
    ("Dijkstra", "camino_dijkstra.txt"),
    ("A*", "camino_astar.txt"),
    ("DFS", "camino_dfs.txt"),
    ("BFS", "camino_bfs.txt"),
    ("Best First Search", "camino_best.txt")
]

# Leer obstáculos (si existen)
aristas_obstaculo = []
if os.path.exists("obstaculos.txt"):
    with open("obstaculos.txt") as f:
        for line in f:
            u, v, peso = map(int, line.strip().split())
            aristas_obstaculo.append((u, v))
    print(f"Obstaculos: {len(aristas_obstaculo)} aristas bloqueadas.")

# Crear subplots
fig, axs = plt.subplots(2, 3, figsize=(18, 10))
axs = axs.flatten()

for idx, (nombre_algoritmo, archivo_camino) in enumerate(algoritmos):
    ax = axs[idx]

    # Dibujar nodos y aristas normales
    nx.draw_networkx_nodes(G, pos, node_size=30, node_color='lightgray', ax=ax)
    nx.draw_networkx_edges(G, pos, edge_color='gray', width=0.5, ax=ax)
    nx.draw_networkx_edge_labels(G, pos, edge_labels=labels, font_size=8, ax=ax)
    nx.draw_networkx_labels(G, pos, labels={n: str(n) for n in G.nodes()},
                             font_size=8, font_color='black', font_weight='bold', ax=ax)

    # Dibujar obstaculos
    for (u, v) in aristas_obstaculo:
        if u in pos and v in pos:
            xs = [pos[u][0], pos[v][0]]
            ys = [pos[u][1], pos[v][1]]
            ax.plot(xs, ys, color='blue', linewidth=1.5)

    # Dibujar camino si existe
    if os.path.exists(archivo_camino):
        with open(archivo_camino) as f:
            camino = [int(line.strip()) for line in f if line.strip()]
        if len(camino) >= 2:
            camino_aristas = [(camino[i], camino[i + 1]) for i in range(len(camino) - 1)]
            nx.draw_networkx_edges(G, pos, edgelist=camino_aristas,
                                   edge_color='red', width=2.5, ax=ax)
            print(f"{nombre_algoritmo}: {camino[0]} -> {camino[-1]}")
        else:
            print(f"{nombre_algoritmo}: sin camino encontrado.")

    ax.set_title(nombre_algoritmo, fontsize=12)
    ax.axis('off')
# Dibujar el ultimo subplot vacio
axs[-1].axis('off')

plt.tight_layout()
plt.show()
