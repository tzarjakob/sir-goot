# Sir Goot: an ncurses game

`Work in progress`

## Overview

Exploring ncurses programming in c...

## Purpose of the game

The aim of the game is to complete the path of the various maps, solving some puzzles to obtain the keys necessary to continue the game.

### Parser

An important feature is the possibility to **insert maps of one's own inventions**. A parser processes the text in the map that you want to be rendered on the screen. If you want to load a personal set of maps, you must create a subfolder in [data](data/) we the name you prefer and follow these rules:

- comments are indicated with **(\*** for the start of the comment and **\*)** for the end of it. Everything inside these signs is ignored. Only multi line comments are supported.
- a **config.gigi** file is necessary: it must contains the name of the first map
- a map file (such as **map1.gigi**) contains the elements in that specific map and all the logic. The only mandatory elements are the starting point, the ending point, the size of the map and the name of the new map (if it is the last map use the **___** notation to conclude the game). In the [data](data/) folder you can have some examples for the simple syntax, looking to the examples is very recommended.

The supported elements in the map are:

1. **wall**: it cannot be crossed.
2. **doorv** and **doorh**: respectively a vertical and horizontal door. It can be opened only if you use a key and then it remains open for the rest of the game. 
3. **trap**: if you go into a trap, you lose the game
4. **gen_chest**: a chest in which you can find something useful. **It is not yet supported as game logic**
5. **portal**: an alternative point to go in another map

Technically the parser supports other types which can be loaded into the map, but there is yet no game logic behind them.

### Controls
The character moves with the **wasd** keys, with **e** the inventory is opened, with **q** the exit screen opens.

## Work in progress
We need to add puzzles and general interaction screens, as well as improve the general rendering.

## Build
The **sir-goot** game depends only from ncurses. The module can be easily installed from the package manager of the majors distributions. 

Here for the debian-based distros:

```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Then run the following commands to build and run the game.

```
./configure 
./make
./run
```


To clean the build and the configuration type:

```
./clean
```
