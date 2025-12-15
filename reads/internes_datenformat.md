# Internes Datenformat

Für eine saubere Verarbeitung der Daten ist ein Einheitliches datenformat des Algorithmus in sämtlichen Phasen notwendig. Sollte dieses Format verletzt werden kann die Verarbeitung erst wieder aufgenommen werden wenn das Format wieder instandt gesetzt wurde. Dieses Format sollte gegebene Ziele umsetzten und möglichst alle Daten bereitstellen welche in sämtlichen Verarbeitungs schritten benötigt werden.

## Ziele

- [ ] Einfachheit
- [ ] Kompakt
- [ ] Einfach zu lesen
- [ ] Streambar
- [ ] Einfach zu debuggen
- [ ] Möglichst einfach auf Fehler zu testen

## Format

Knoten Liste: `<Node-Type>`

| BytePos | ByteSize | DataType | Name        | Usage                                                |
| :-----: | :------: | -------- | ----------- | ---------------------------------------------------- |
|  0 - 1  |    2     | short    | Node Degree | Speichert den Knoten Grad                            |
|  2 - 3  |    2     | short    | Node Level  | Gibt das level an in dem Dieser Knotne Enthalten ist |
|  4 - 7  |    4     | int      | ClusterID   | Gibt die zugehörigkeit zu einem Gewissen Cluster an  |
| 8 - 11  |    4     | int      | x           | x koordinate des Knoten                              |
| 12 - 15 |    4     | int      | y           | y koordiante des Knoten                              |

Für höher diemensionale Räume nutze statt x und y einfach `<N-Dim-Point-Type>`:

| BytePos | ByteSize | DataType | Name  | Usage                                                     |
| :-----: | :------: | -------- | ----- | --------------------------------------------------------- |
|  i * 4  |    4     | int      | $X_i$ | i-te Koordinaten Komponente eines N-dimensionales Punktes |

Adjazenz Liste: `<Edge-Type>`
| BytePos | ByteSize | DataType | Name   | Usage                                                     |
| :-----: | :------: | -------- | ------ | --------------------------------------------------------- |
|  0 - 7  |    8     | long     | NodeID | Index in der Knoten Liste an dem der Nachbar Knoten steht |

