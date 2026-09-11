# Sabrina's Game
Constructive algorithm building valid solutions to Sabrina's Game.

## Compile (requires a C++20 compiler)
```shell
$> make
```

## Run
For getting one solution and display it with a pretty ASCII drawing: `$> bin/sabrinasgame_v1 WIDTH HEIGHT`

Example:
```shell
./bin/sabrinasgame_v1 4 6
```

For running it multiple times and getting a simple solution display: `$> bin/sabrinasgame_v1 WIDTH HEIGHT RUNS`

Example for 10 runs:
```shell
./bin/sabrinasgame_v1 4 6 10
```

## Algorithms
Each algorithm maintains the number of empty adjacent cells for each cell of the grid.

### v1
Fill cells from top-left to bottom-right, placing a random tile (horizontally or vertically) and forcing a tile if one cell has only one empty adjacent cell.

### v2
Same as v1, but taking into account a special case where a cell on the last row has exactly two empty adjacent cells.

### v3
Randomly select a cell with 1 (in priority) or 2 empty adjacent cells, then place a random tile (horizontally or vertically).

### v4
Same as v3, but selecting corner cells only, for the ones with 2 empty adjacent cells. Corner cells are the ones forming a triangular shape with their two empty adjacent cells.


## Known issues
The algorithms are not perfect yet. In some rare case, the greedy construction takes a path where no solutions exist. On large instance (with 300 cells and more), some algorithms do not terminate.
