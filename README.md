This is the repository of my Sudoku project. It is a graphic interface developed using C++ and QML for the Sudoku game.

### Logic of the game

- The player chooses a difficulty (grid partially pre-filled according to the level).

- They can click on empty cells to change their value: each click increments the value between 1 → 9 → empty.

- The "Check" button automatically analyzes the grid:

- detects duplicates in rows, columns, and 3x3 blocks,

- displays error messages in case of conflicts.

- The player can save their progress and reload it later.

- A "Restart" button allows them to start a new grid at the same level.

### Getting started

   ```bash
   git clone https://gitlab.ec-lyon.fr/akrid/sudoku/
   
   Select "Open Project" in Qt Creator and choose the CMakeLists.txt file.

   Make sure an executable is configured in the project.
        
   Compile the project by pressing Ctrl + B or clicking "Build".
        
   Then launch the program by clicking the "Run" button.




