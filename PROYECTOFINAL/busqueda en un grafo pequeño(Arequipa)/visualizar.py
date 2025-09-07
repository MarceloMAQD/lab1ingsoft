import matplotlib.pyplot as plt
import networkx as nx
import argparse
import os
from adjustText import adjust_text

# Variables globales para almacenar el origen y destino seleccionados
selected_origin = None
selected_destination = None
selection_phase = 0 # 0: seleccionando origen, 1: seleccionando destino
cid = None # Para almacenar el ID del evento de clic

#Manejador de eventos de clic para selección interactiva en el mapa.
def on_click(event, G, pos, ax):
    global selected_origin, selected_destination, selection_phase, cid

    # Ignorar clics si el usuario está usando herramientas de zoom/pan
    if ax.get_figure().canvas.toolbar.mode != '':
        return

    # Solo procesar clics dentro del área del gráfico
    if event.inaxes != ax: return

    # Obtener la posición (x, y) del clic en coordenadas del gráfico
    clicked_pos = (event.xdata, event.ydata)
    
    # Buscar el nodo más cercano al click
    min_dist = float('inf')
    closest_node = None

    for node_id, node_pos in pos.items():
        dist = (node_pos[0] - clicked_pos[0])**2 + (node_pos[1] - clicked_pos[1])**2
        if dist < min_dist:
            min_dist = dist
            closest_node = node_id

    #Manejar seleccion de origen y destino
    if closest_node is not None:
        if selection_phase == 0:
            # Primer clic: seleccionar ORIGEN
            selected_origin = closest_node
            print(f"Origen seleccionado: Nodo {selected_origin}")
            selection_phase = 1

            # Redibujar el mapa para mostrar el origen en verde
            ax.clear()
            pois = load_pois(ax.figure.archivo_pois)
            draw_map_elements(G, pos, ax, None, None, None, selected_origin, selected_destination, pois=pois)
            plt.draw() # Refrescar la ventana

        elif selection_phase == 1:
            # Segundo clic: seleccionar DESTINO
            selected_destination = closest_node
            print(f"Destino seleccionado: Nodo {selected_destination}")
            plt.disconnect(cid) 
            plt.close()
    
def draw_map_elements(G, pos, ax, camino_aristas, obstaculos_aristas, camino_nodos, current_origin=None, current_destination=None, pois=None):
    # Dibuja las calles del mapa
    nx.draw_networkx_edges(G, pos, ax=ax, alpha=0.4, edge_color='gray', width=0.5)
    
    # Dibuja todos los nodos del grafo en naranja
    nx.draw_networkx_nodes(G, pos, ax=ax, node_size=3, node_color='orange', alpha=0.7)
    
    # Dibuja los obstáculos si existen
    if obstaculos_aristas:
        nx.draw_networkx_edges(G, pos, ax=ax, edgelist=obstaculos_aristas, width=1.5, edge_color='red', style='dashed', alpha=0.8)
    
    # Dibuja la ruta calculada
    if camino_aristas:
        nx.draw_networkx_edges(G, pos, ax=ax, edgelist=camino_aristas, width=2.5, edge_color='blue', alpha=0.8)
    if camino_nodos:
        nx.draw_networkx_nodes(G, pos, ax=ax, nodelist=camino_nodos, node_size=15, node_color='skyblue', alpha=0.8)
    
    # Dibuja el origen y el destino
    if current_origin is not None:
        nx.draw_networkx_nodes(G, pos, ax=ax, nodelist=[current_origin], node_size=150, node_color='green', edgecolors='black', linewidths=1.5)
    if current_destination is not None:
        nx.draw_networkx_nodes(G, pos, ax=ax, nodelist=[current_destination], node_size=150, node_color='purple', edgecolors='black', linewidths=1.5)

    # Dibuja los nombres de los POIs
    if pois:
        textos = []
        poi_coords = {name: (lon, lat) for lon, lat, name in pois}
        for lon, lat, name in pois:
            texto = ax.text(lon, lat, name, fontsize=8, color='darkred', ha='center')
            textos.append(texto)
        adjust_text(textos, ax=ax,
                    arrowprops=dict(arrowstyle='-', color='gray', lw=0.5,
                                    mutation_scale=15,
                                    shrinkA=0, shrinkB=0,
                                    connectionstyle='arc3,rad=0.0'),
                    only_move={'points':'xy', 'text':'xy', 'objects':'xy'},
                    expand_points=(2,2), expand_text=(1.2,1.2))

        # Dibuja los puntos morados en la ubicación original de cada POI
        for lon, lat, _ in pois:
            ax.plot(lon, lat, marker='o', markersize=3, color='purple')

    ax.set_title("Ruta Optima en Arequipa")
    ax.set_aspect('equal')
    plt.tight_layout()

#Carga los Puntos de Interés (POIs) desde un archivo de texto.
def load_pois(archivo_pois):
    """Funcion auxiliar para cargar los POIs desde un archivo."""
    pois = []
    print(f"Intentando cargar POIs desde el archivo: {archivo_pois}")

    if archivo_pois and os.path.exists(archivo_pois):
        try:
            # Abrimos el archivo en modo lectura usando 'latin-1' para evitar errores de codificación
            with open(archivo_pois, 'r', encoding='latin-1') as f:
                for linea in f:
                    # Dividir la línea en partes: lon, lat, nombre(s)
                    partes = linea.strip().split()
                    if len(partes) >= 3:
                        lon = float(partes[0]) # Longitud (x)
                        lat = float(partes[1]) # Latitud (y)
                        nombre = ' '.join(partes[2:]) # Unir el resto como nombre completo
                        pois.append((lon, lat, nombre))
            print(f"EXITO: Se cargaron {len(pois)} POIs.")
        except Exception as e:
            print(f"ERROR al cargar POIs desde '{archivo_pois}': {e}")
    else:
        print("ADVERTENCIA: El archivo de POIs no existe o no fue proporcionado.")
    return pois

#Visualiza el grafo y las rutas sobre un mapa.
def visualizar_mapa(archivo_grafo, archivo_pos, archivo_camino=None, archivo_obstaculos=None, archivo_pois=None, interactive_selection=False):
    global selected_origin, selected_destination, selection_phase, cid
    pos = {}

    #Cargar posiciones de nodos
    try:
        with open(archivo_pos, 'r') as f:
            for linea in f:
                partes = linea.strip().split()
                nodo_id, lon, lat = int(partes[0]), float(partes[1]), float(partes[2])
                pos[nodo_id] = (lon, lat)
    except FileNotFoundError:
        print(f"Error: Archivo de posiciones '{archivo_pos}' no encontrado.")
        return None, None
    
    #Cargar aristas del grafo
    G = nx.Graph()
    try:
        with open(archivo_grafo, 'r') as f:
            next(f)
            for linea in f:
                partes = linea.strip().split()
                u, v, _ = int(partes[0]), int(partes[1]), int(partes[2])
                G.add_edge(u, v)
    except FileNotFoundError:
        print(f"Error: Archivo de grafo '{archivo_grafo}' no encontrado.")
        return None, None
    
    #Cargar aristas de obstaculos
    obstaculos_aristas = []
    if archivo_obstaculos and os.path.exists(archivo_obstaculos):
        with open(archivo_obstaculos, 'r') as f:
            for linea in f:
                partes = linea.strip().split()
                u, v = int(partes[0]), int(partes[1])
                obstaculos_aristas.append((u, v))
    
    # Cargar los POIs
    pois = load_pois(archivo_pois)

    #Configurar la figura y ejes de matplotlib
    fig, ax = plt.subplots(figsize=(15, 15))
    fig.archivo_pois = archivo_pois

    # Ajustar límites del mapa para encuadrar todos los nodos con un pequeño margen
    if pos:
        min_lon, max_lon = min(p[0] for p in pos.values()), max(p[0] for p in pos.values())
        min_lat, max_lat = min(p[1] for p in pos.values()), max(p[1] for p in pos.values())
        margin_lon, margin_lat = (max_lon - min_lon) * 0.02, (max_lat - min_lat) * 0.02
        ax.set_xlim(min_lon - margin_lon, max_lon + margin_lon)
        ax.set_ylim(min_lat - margin_lat, max_lat + margin_lat)
        ax.set_aspect('equal')

    #Modo interactivo: seleccionar origen y destino
    if interactive_selection:
        print("Haz clic en el mapa para seleccionar el nodo de ORIGEN.")
        selection_phase = 0
        cid = fig.canvas.mpl_connect('button_press_event', lambda event: on_click(event, G, pos, ax))
        draw_map_elements(G, pos, ax, None, obstaculos_aristas, None, pois=pois)
        plt.show()
        return selected_origin, selected_destination
    #Modo no interactivo: mostrar ruta calculada
    else:
        camino_nodos = []
        if archivo_camino and os.path.exists(archivo_camino):
            # Leer nodos del camino y crear aristas consecutivas
            with open(archivo_camino, 'r') as f:
                camino_nodos = [int(linea.strip()) for linea in f]
            camino_aristas = list(zip(camino_nodos, camino_nodos[1:]))
            origen = camino_nodos[0] if camino_nodos else None
            destino = camino_nodos[-1] if camino_nodos else None
        else:
            camino_aristas, origen, destino = None, None, None

        draw_map_elements(G, pos, ax, camino_aristas, obstaculos_aristas, camino_nodos, origen, destino, pois)
        plt.show()
        return None, None

#PARSEADOR DE ARGUMENTOS: Permite ejecutar el script con opciones desde la línea de comandos ---    
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Visualizador de grafos de rutas.")
    parser.add_argument('--obstaculos', type=str, help="Archivo con las aristas de obstáculos.")
    parser.add_argument('--pois', type=str, help="Archivo con los puntos de interes (POIs).")
    parser.add_argument('--interactive', action='store_true', help="Habilita la selección interactiva de origen y destino.")
    # Se procesan los argumentos ingresados por el usuario
    args = parser.parse_args()

    # Si se activó el modo interactivo (selección manual de origen y destino con clics)
    if args.interactive:
        print("Iniciando modo de selección interactiva...")
        origen_seleccionado, destino_seleccionado = visualizar_mapa(
            archivo_grafo="arequipa_grafo.txt",
            archivo_pos="arequipa_posiciones.txt",
            archivo_obstaculos=args.obstaculos,
            archivo_pois=args.pois,
            interactive_selection=True
        )
        # Si se seleccionaron ambos nodos correctamente, se muestran
        if origen_seleccionado is not None and destino_seleccionado is not None:
            print(f"SELECTED_NODES: {origen_seleccionado} {destino_seleccionado}")
        else:
            print("Selección cancelada o incompleta.")
    else:
        # Si no se activa el modo interactivo, simplemente se visualiza el mapa con una ruta ya calculada
        visualizar_mapa(
            archivo_grafo="arequipa_grafo.txt",
            archivo_pos="arequipa_posiciones.txt",
            archivo_camino="camino_calculado.txt",
            archivo_obstaculos=args.obstaculos,
            archivo_pois=args.pois,
            interactive_selection=False
        )
