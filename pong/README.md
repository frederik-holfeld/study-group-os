# Pong

This little project is supposed to give my fellow students a fun playground to learn C. It utilizes ncurses to display the game in a terminal window with color (currently color support is a hard requirement). The game is at a very early stage, but the basics are in a somewhat functional state.

## How to build

Because we use ncurses, we need the ncurses library installed to compile the game. For some Linux distros it may already be installed. On Ubuntu we can get it by installing the package libncurses-dev by running "sudo apt install libncurses-dev". When compiling, we also need to link the ncurses library with the "-l ncurses" parameter when using GCC, so the command to compile could look something like "gcc pong.c -l ncurses -o pong". Then start the game with "./pong".

## To-dos

- Ability to start a new match.
- Better AI.
- Customization:
  - Change player, AI and background colors.
  - Background design (assigned to Joanna).
- Multiplayer support:
  - Local play.
  - Network play.
  - Maybe more than two players? Could also use the upper and lower screen edges instead of just the left and right ones for four player games.
- Sound support:
  - Background music?
  - Sound effects.
- Power-ups:
  - Ball splits into two.
  - Speed multiplier for players or ball.
  - Score multiplier. Score more than one point per goal.
  - Single use item that freezes the screen for a moment.
  - Single use item to build a temporary wall in the middle of the screen to trap the other player with the ball.
  - Longer paddles (and smaller paddles as a disadvantage).
  - To make things more interesting, power-up that makes the ball fly in curves instead of straight lines.
- Optimize game performance and make framerate more stable.
- Ability to assign new button layouts.
  - Move using scroll wheel.
- More polish and bug fixing in general.
