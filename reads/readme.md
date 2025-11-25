Klar! Ich erstelle dir eine **übersichtliche Markdown-Datei**, die alle gängigen Möglichkeiten in Markdown kategorisiert, mit Beispielen.

---

`
# Markdown Referenz

Eine Übersicht über alle wichtigen Möglichkeiten, die Markdown unterstützt.

---

## 1. Überschriften (Headings)


# H1
## H2
### H3
#### H4
##### H5
###### H6

---

## 2. Textformatierung

* **Fett**: `**fett**` oder `__fett__`
* *Kursiv*: `*kursiv*` oder `_kursiv_`
* ~~Durchgestrichen~~: `~~durchgestrichen~~`
* `Monospace / Inline-Code`: `` `Code` ``
* **Blockquote**:

> Das ist ein Zitat
> Mit mehreren Zeilen

---

## 3. Listen

### Ungeordnete Liste

- Punkt 1
- Punkt 2
  - Unterpunkt
* Alternativ-Symbol
+ Auch möglich

### Geordnete Liste

1. Erstes
2. Zweites
3. Drittes

### Aufgabenliste (Checkbox)

- [x] Erledigt
- [ ] Offen
- [ ] In Arbeit

---

## 4. Links

[Linktext](https://example.com)
[Link mit Titel](https://example.com "Titel")
<https://example.com>

---

## 5. Bilder


![Alt-Text](bild.png)
![Alt-Text](bild.png "Titel")

---

## 6. Code

### Inline-Code

`ein Code-Snippet`

### Codeblock

```javascript
function hello() {
    console.log("Hallo Welt");
}
````


- Sprache kann angegeben werden (`javascript`, `python`, `bash`, etc.)

---

## 7. Tabellen


| Spalte 1 | Spalte 2 | Spalte 3 |
|----------|----------|----------|
| Wert 1   | Wert 2   | Wert 3   |
| Wert A   | Wert B   | Wert C   |

* Alignment:

| Links  | Zentriert | Rechts  |
|:-------|:---------:|--------:|
| a      | b         | c       |

---

## 8. Horizontale Linien

---
***
___

---

## 9. HTML in Markdown

Markdown erlaubt **HTML-Tags** direkt, z.B.:

<div style="color:red;">Roter Text</div>

---

## 10. Fußnoten

Das ist ein Satz mit einer Fußnote.[^1]

[^1]: Das ist der Text der Fußnote.

---

## 11. Definition Lists (nicht überall unterstützt)


Begriff
: Definition dazu

Anderer Begriff
: Noch eine Definition


---

## 12. Emoji (wenn unterstützt)

:smile: :rocket: :heart:

---

## 13. Kommentare (Markdown ignoriert sie)

<!-- Das ist ein Kommentar -->

---

## 14. Escaping (Sonderzeichen)

\*Nicht kursiv\*
\# Kein Heading
