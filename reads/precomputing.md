# precomputing / Vorverarbeitung

Die Vorverarbeitung gibt PHPA* seine geschindigkeit und Optimalität. Es ermöglicht auch die Unterteilung in Unterprobleme. Diese Vorverarbeitung ist üblich sehr langsam und Rechenaufwändig. Im weiteren soll eine Methode zur Effizenten Berechnung und Massiven parralelisierung vorgestellt werden.

## Ablauf der Vorverarbeitung

Eingabe: Graph
Ziel: Berechne nächste Hierachie-Ebene

1. Berechne Clustering auf Graphen
   1. Alte Cluster müssen zusammenhängend bleiben!
   2. Permutation der Daten in Cluster Bereiche (Optional für Cache Effizenz)
2. Cluster Verarbeitung (Kann für jeden Cluster Parralel Starten)
   1. Für jeden Knoten - Entscheide ob Gate
      1. Jeden Nachbarn anschauen: wenn Nachbar nicht in Cluster -> Knoten ist Gate
      2. Wenn nicht Gate -> Return
      3. Sonst: Merke Knoten in Gate Liste
   2. Ermittle alle Relevanten Kanten Pro Knoten (Massiv Parrallel für jeden Knoten)
      1. Kopiere alle Kanten zu externen Knoten
      2. Berechne durch Breiten suche zu allen anderne Gate Knoten den Kürzesten Weg
         1. Breiten suche ausführen
         2. Bei jedem Knoten Prüfe auf Gate Knoten
         3. Wenn Gate -> Trage bisherige Distanz als Kante zu dem Knoten ein
      3. Schreibe alle Knoten und ihre kannten in Passendem Fort in nächste Hierachie Ebene
3. Up und Downlink berechnen
4. Knoten ID's Updaten und Überschreiben

## Aufwendigkeits analyse

Pro Knoten wird auf einem kleinen Raum (Größe des Clusters) eine Breiten suche ausgeführt. Dies liegt in O(n + m) wobei $n = |V|$ und $m = |E|$ pro Cluster! diese sind aber Beschränkt und könen somit als Konstant klein angesehen werden. Somit werden n (Anzahl globaler Knoten in ebene) viele Breiten suchen Durchgeführt. Da aber jeder Knoten in seinem Cluster die breiten suche unabhängig durchführen kann. Und jeder Cluster Selbst unabhängig betrachtet werden kann. Ist hier Massive Parralelisierung möglich. Je mehr Kerne -> desto besser. Theoretisch kann hier als obergrenze für die parrelelisierung n und für die Benutzte Kernanzahl n + 1 angegeben werden. jeder Knoten benutz einen kern, wobei mindestens ein Knoten den Verwaltungs Overhead für die Lastverteilung übernimmt und daten an die Worker Übergibt. Somit ist die Gesammt Laufzeit abgeschtätzt durch $O(n \cdot (|C_V|  + |C_E|))$ und die Wallclock time $O((n \cdot (|C_V| + |C_E|)) / n) = O(|C_V| + |C_E|)$ Damit ist der Hauptrechenaufwand druch eine Einzeilne Breiten Suche beschränkt und sollte "Schnell Genung" sein.

## Vorteil der Massiven parallelisieurng

Druch die Paralelle Vorverarbeitung ist es möglich auf Echtzeitdaten zu Reagieren. Durch das Dynanmische einbinden neuer Daten und Später mit weiterer Entwicklung Lokaler Neuberechnung sollte es möglich sein in Echtzeit (Minuten verzögerung) auf änderungne im Graphen zu reagieren und den Hierachie graphen neu zu berechnen ohne Lange wartezeiten.
