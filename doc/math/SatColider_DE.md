## SAT (Separating Axis Theorem) Kollisionserkennung

### Einführung:

Stelle dir zwei flache, unregelmäßige Formen auf einem Tisch vor. Wenn du zwischen diesen beiden Formen in jede Richtung einen dünnen Streifen Papier schieben könntest, ohne dass er von den Formen blockiert wird, dann berühren sie sich nicht. Wenn du jedoch in keine Richtung einen Streifen schieben kannst, ohne dass er blockiert wird, dann berühren sich die beiden Formen.

Der SAT (Separating Axis Theorem) verwendet genau dieses Konzept, aber in einer mathematischen Form, um zu überprüfen, ob zwei Formen kollidieren.

### Theorie:

1. **Achsenwahl**: Für jede Kante einer Form wird eine "Achse" (eine gerade Linie) erzeugt, die senkrecht darauf steht. Das bedeutet, wenn wir zwei Vierecke haben, dann haben wir insgesamt 8 Kanten und somit 8 Achsen.

2. **Projektion**: Jede Form wird dann auf jede dieser Achsen projiziert. Man kann sich das wie einen Schatten vorstellen, den eine Form wirft, wenn eine Lichtquelle genau von der Seite darauf scheint.

3. **Überlappungsprüfung**: Nun schauen wir, ob die Schatten (Projektionen) der beiden Formen auf jeder Achse sich überlappen. Wenn sie sich auf einer Achse nicht überlappen, dann berühren sich die Formen nicht.

4. **Kollisionserkennung**: Wenn sich die Schatten auf allen Achsen überlappen, dann kollidieren die beiden Formen.

### Warum ist das nützlich?

Mit SAT können wir nicht nur herausfinden, ob zwei Formen kollidieren, sondern auch, wie stark und in welche Richtung sie sich überlappen. Das ist sehr nützlich für Computerspiele und Simulationen, um realistische Reaktionen auf Kollisionen zu erzeugen.

### Zusammenfassung:

Um mit SAT zu überprüfen, ob zwei Formen kollidieren:

1. Wähle Achsen basierend auf den Kanten der Formen.
2. "Zeichne" Schatten der Formen auf jeder Achse.
3. Prüfe, ob sich die Schatten auf allen Achsen überlappen.
4. Wenn ja, kollidieren die Formen!

