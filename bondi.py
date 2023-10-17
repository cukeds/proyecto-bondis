import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

recorrido_bondis = pd.read_csv("recorrido-colectivo.csv")
recorrido_metros = pd.read_csv("recorrido-de-metrobus.csv")

recorrido_bondis["desde"] = recorrido_bondis["desde"].str.replace(r'\(.*\)', '', regex=True).str.strip()
recorrido_bondis["hasta"] = recorrido_bondis["hasta"].str.replace(r'\(.*\)', '', regex=True).str.strip()

df_estaciones = pd.concat([recorrido_bondis["desde"], recorrido_bondis["hasta"]], ignore_index=True)
df_estaciones = df_estaciones.drop_duplicates().reset_index().drop("index", axis=1)
df_estaciones.columns = ["ids"]
df_estaciones_mapping = df_estaciones["ids"].reset_index().set_index("ids")['index']

recorrido_bondis["desde_id"] = recorrido_bondis["desde"].map(df_estaciones_mapping)
recorrido_bondis["hasta_id"] = recorrido_bondis["hasta"].map(df_estaciones_mapping)
grafo_estaciones = nx.DiGraph()
for id in df_estaciones_mapping:
    grafo_estaciones.add_node(id)


for index, recorrido_bondi in recorrido_bondis.iterrows():
    grafo_estaciones.add_edge(recorrido_bondi["desde_id"], recorrido_bondi["hasta_id"])


nx.set_node_attributes(grafo_estaciones, df_estaciones["ids"].to_dict(), 'label')

nx.draw(grafo_estaciones)
plt.show()

nx.write_gexf(grafo_estaciones, "grafo_estaciones.gexf")

