import osmnx as ox
import networkx as nx

# Lugar del cual se obtendra el grafo
lugar = "Arequipa, Peru"

print(f"Descargando el mapa de calles de {lugar}...")
# Grafo de calles para vehiculos
G = ox.graph_from_place(lugar, network_type='drive')

# Reetiquetar nodos a enteros simples
mapeo_nodos = {osmid: i for i, osmid in enumerate(G.nodes())}
G_simple = nx.relabel_nodes(G, mapeo_nodos)

print("Mapa descargado. Procesando y guardando archivos...")

# 1. Guardar grafo: nodo1 nodo2 peso (longitud en metros)
with open("arequipa_grafo.txt", "w") as f_grafo:
    f_grafo.write(f"{G_simple.number_of_nodes()} {G_simple.number_of_edges()}\n")
    for u, v, data in G_simple.edges(data=True):
        peso = int(data.get('length', 1))
        f_grafo.write(f"{u} {v} {peso}\n")

# 2. Guardar coordenadas de cada nodo
with open("arequipa_posiciones.txt", "w") as f_pos:
    for osmid, i in mapeo_nodos.items():
        nodo_data = G.nodes[osmid]
        lon, lat = nodo_data['x'], nodo_data['y']
        f_pos.write(f"{i} {lon} {lat}\n")

print("Archivos 'arequipa_grafo.txt' y 'arequipa_posiciones.txt' generados.")
print(f"Total de nodos: {G_simple.number_of_nodes()}")

print("\nDescargando puntos de interes...")

# Etiquetas de puntos de interes a buscar
tags = {
    "amenity": ["hospital", "university"]
}

# Obtención de puntos de interes con esas etiquetas
pois = ox.features_from_place(lugar, tags)

# 3. Guardar puntos de interes: lon lat nombre
with open("arequipa_pois.txt", "w") as f_pois:
    for _, poi in pois.iterrows():
        if 'name' in poi and isinstance(poi['name'], str):
            lon = poi.geometry.representative_point().x
            lat = poi.geometry.representative_point().y
            nombre = poi['name']
            f_pois.write(f"{lon} {lat} {nombre}\n")

print(f"Archivo 'arequipa_pois.txt' generado con {len(pois)} POIs.")
print("--- Proceso de obtencion de mapa completo ---")
