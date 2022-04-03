### Ideas... 

1. screen rendering with **colors**, animation and similar stuff
2. add more objects to the scene, to get more interaction
3. prepare enigmas to continue the game
4. handle the errors in parsing with some message on screen
5. (eventually) add the option to choose which map to play
6. handle error if not find log file
7. set stricter constraints for map dimension 
8. replace all the 0 with EMPTY_SPACE_T
9. end_p must have also as attribute the map to continue, in this way in one map it is possible to have more exit points
10. implement going back to one point: restore the state left

`add life system`: you have x lives, if you lose them you lose the game

`add monster system`: monsters are always RED
1. zombies: if they see the hero they proceed towards him  
2. ghosts: they can go even through the walls

`add help sistem`: if you press h you can navigate around the screen with a cursor and get info about what you want

`add custom characters`: horizontal door need both _ and high _ at the same time

`not urgent`: replace where possible int -> uint8_t or similar  

