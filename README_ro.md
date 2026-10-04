# Knowledge Graph Pathfinding

Acest proiect reprezintă o implementare eficientă în **C** a unui *Knowledge Graph* (Graf de Cunoștințe) direcționat și ponderat. Proiectul permite încărcarea de entități și relații din fișiere CSV și rezolvarea rapidă a unor interogări de bază, precum și calcularea drumurilor optime între entități folosind algoritmii BFS și Dijkstra.

## Structuri de Date Utilizate

* **Graf cu Listă de Adiacență:** Optim în cazul unui Knowledge Graph, deoarece acestea sunt de regulă grafuri rare. Astfel, se economisește memorie (complexitate spațială `O(n + m)`) și se permit traversări rapide prin vecinii unui nod.
* **BST (Arbore de Căutare Binară):** Indexare implementată pentru a evita căutările liniare repetitive la fiecare interogare. Reduce timpul de localizare a unui nod de la `O(n)` la `O(h)`, unde *h* este înălțimea arborelui.
* **Coadă FIFO:** Utilizată pentru implementarea parcurgerii în lățime pentru BFS (interogarea `PATH`) și pentru reținerea secvențială a interogărilor citite din fișier.
* **Min-Heap (Coadă de priorități):** Implementat peste un vector dinamic pentru a susține algoritmul lui Dijkstra. Permite extragerea eficientă a nodului cu cel mai mic cost curent în timp `O(log n)`, îmbunătățind semnificativ performanța față de o căutare liniară `O(n)`.

## Complexitatea Operațiilor

### Interogări de Bază
* **EXISTS (`O(h)`):** Verifică existența unei entități. Depinde strict de performanța BST-ului, independent de dimensiunea totală a grafului.
* **EDGE (`O(h + d)`):** Verifică existența unei conexiuni directe. Presupune două căutări în BST (`O(h)`) și iterarea prin lista de adiacență a nodului sursă (unde *d* este gradul nodului).
* **NEIGHBORS (`O(h + d)`):** Implică o căutare în BST pentru nodul sursă (`O(h)`) și o parcurgere liniară a listei sale de vecini direct conectați (`O(d)`).

### Algoritmi de Drum (Pathfinding)
* **PATH / BFS (`O(h + n + m)`):** Găsirea nodurilor de start și stop prin BST durează `O(h)`. Algoritmul BFS vizitează fiecare nod și parcurge fiecare muchie o singură dată. Memoria suplimentară utilizată (coadă, vectori de vizitare) este `O(n)`.
* **DIJKSTRA (`O(h + (n + m) log n)`):** Căutarea inițială costă `O(h)`. Algoritmul utilizează Min-Heap-ul pentru extragerea nodurilor (`O(n log n)`) și pentru relaxarea muchiilor (`O(m log n)`). Utilizarea Min-Heap-ului previne degradarea performanței la `O(n^2)`.

## Rulare și Compilare

Programul așteaptă trei argumente: fișierul cu entități, fișierul cu relații și fișierul cu interogări.

```bash
# Exemplu de rulare
make public-test


# Pentru a rula testele publice doar pentru un pas:

./build/public_pas1 tests/public/pas1/entitati.csv tests/public/pas1/relatii.csv tests/public/pas1/interogari.txt
./build/public_pas2 tests/public/pas2/entitati.csv tests/public/pas2/relatii.csv tests/public/pas2/interogari.txt
./build/public_pas3 tests/public/pas3/entitati.csv tests/public/pas3/relatii.csv tests/public/pas3/interogari.txt
./build/public_pas4 tests/public/pas4/entitati.csv tests/public/pas4/relatii.csv tests/public/pas4/interogari.txt

Fiecare test public are și un fișier expected.txt cu rezultatul așteptat.