# Coin Collector
Platformer game written in C++ using the SDL2 library (https://libsdl.org). Contains a CMake file that can be used to generate projects for different systems. This project has been tested on Windows 10 and Linux.

The game starts out in the main menu which has three buttons: Play Game, which starts the game; Instructions, which opens another menu that describes the controls and how the game is player; and Quit - which closes the game. The game itself includes a character which is controlled with the keyboard, is affected by gravity and has collision and bounds testing. There are three coins that need to be collected to win the game. To collect a coin, the player needs to collide with one. When all thre coins are collected, the game goes to a Game Won scene, which has the text "YOU WIN!" and a button to go back to the main menu.

To build the game:

0. Steps 1 and 2 are for building on Windows. For linux, skip to step 3.
1. If you are using Windows, open the CMakeLists.txt file in the project root directory and find the line where it says set(SDL2_DIR ...)
2. Change the \<PathToSDL2\> to point to the SDL2 path on your system (e.g. C:/Users/<yourusername>/.../SDL2-2.0.22)
3. Open a command prompt or a terminal and change your directory to the project's root directory (where the CMakeLists.txt file is)
4. Run cmake \<dir\>, substituting \<dir\> for the directory to place the build files (e.g. cmake . will place them in the same directory as the CMakeLists.txt file)
5. Now the build files should be generated. To build, type in the command prompt/terminal the following command: cmake --build \<dir\>, with \<dir\> being the project directory.
