# Sabrina's Game
Constructive algorithm building valid solutions to Sabrina's Game.

## Compile (requires a C++20 compiler)
```shell
$> make
```

## Run
For getting one solution and display it with a pretty ASCII drawing: `$> bin/sabrinasgame WIDTH HEIGHT`

Example:
```shell
./bin/sabrinasgame 4 6
```

For running it multiple times and getting a simple solution display: `$> bin/sabrinasgame WIDTH HEIGHT RUNS`

Example for 10 runs:
```shell
./bin/sabrinasgame 4 6 10
```

## Known issues
The algorithm is not perfect yet. In some rare case, this greedy construction takes a path where no solutions exist. The smallest example we found is in a 13x10 grid, and happen roughly once every 2500 solutions.
