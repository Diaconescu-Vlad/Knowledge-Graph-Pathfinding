---
### Varianta în Engleză (Recomandată ca `README.md` principal)

```markdown
# Knowledge Graph Pathfinding

This project is a highly efficient **C** implementation of a directed, weighted *Knowledge Graph*. It parses entities and their relationships from CSV files and processes various queries, ranging from simple edge verification to optimal route calculations using Breadth-First Search (BFS) and Dijkstra's algorithm.

## Data Structures Used

* **Adjacency List Graph:** Chosen as the core structure since Knowledge Graphs are typically sparse. This optimizes memory usage (Space Complexity: `O(n + m)`) and allows for fast traversals among adjacent nodes.
* **Binary Search Tree (BST):** Implemented to index node names, preventing repetitive linear scans during queries. It reduces node lookup time from `O(n)` to `O(h)`, where *h* is the height of the tree.
* **FIFO Queue:** Utilized to manage the sequence of incoming queries and to facilitate the level-by-level exploration required by the BFS algorithm (`PATH` queries).
* **Min-Heap (Priority Queue):** Backed by a dynamic array, this structure drives Dijkstra's algorithm. It ensures that the extraction of the lowest-cost node occurs in `O(log n)` time, vastly outperforming a naive `O(n)` linear search.

## Operation Complexities

### Basic Queries
* **EXISTS (`O(h)`):** Checks if an entity exists. Performance depends entirely on the BST, independent of the overall graph size.
* **EDGE (`O(h + d)`):** Checks for a direct connection. Requires two BST lookups (`O(h)`) and iterating through the source node's adjacency list, where *d* is the out-degree of the node.
* **NEIGHBORS (`O(h + d)`):** Locates the source node via the BST (`O(h)`) and linearly traverses its direct neighbors (`O(d)`).

### Pathfinding Algorithms
* **PATH / BFS (`O(h + n + m)`):** After identifying the start and target nodes via the BST (`O(h)`), the BFS algorithm visits each node and edge at most once. Auxiliary memory (queue, visited arrays) is `O(n)`.
* **DIJKSTRA (`O(h + (n + m) log n)`):** Initial node lookup takes `O(h)`. The algorithm uses the Min-Heap for both node extraction (`O(n log n)`) and edge relaxation (`O(m log n)`). Implementing the Min-Heap effectively avoids the `O(n^2)` bottleneck typical of unoptimized Dijkstra implementations.

## Build and Run

The executable requires three input files: entities, relationships, and queries.

```bash
# Execution example
./build/knowledge_graph data/entities.csv data/relations.csv data/queries.txt