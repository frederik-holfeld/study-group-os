#include <ncurses.h>
#include <string.h>
#include <unistd.h>

// This struct defines a paddle of a player or AI.
struct paddle {
  short x; // Position is equal to a column.
  float y; // Needs to be float so movement is more smoothed out.
  char length;
  float speed;
};
// This struct describes a ball in the game.
struct ball {
  float x, y, speedx, speedy;
};
// A little helper function that makes displaying centered text easier.
void print_centered(char *string) {
  short x, y;
  getmaxyx(stdscr, y, x);
  mvprintw(y / 2, (x - strlen(string)) / 2, "%s", string);
}
// Function that draws the player paddles.
void paddle_draw(struct paddle *paddle) {
  for (short i = 0; i < paddle->length; i++) {
    mvprintw(paddle->y - paddle->length / 2 + i, paddle->x - 1, "LLL");
  }
}

void paddle_update(struct paddle *paddle) {
  paddle->y = paddle->y + paddle->speed;
  paddle->speed /= 1.5;
}

void ball_draw(struct ball *ball) {
  for (short i = 0; i < 3; i++) {
    mvprintw(ball->y - 1 + i, ball->x - 1, "OOO");
  }
}

void ball_update(struct ball *ball) {
  ball->x = ball->x + ball->speedx;
  ball->y = ball->y + ball->speedy;
}

int main(int argc, char const *argv[]) {
  initscr(); // Initializes curses mode.
  noecho(); // We don't want to print characters that the player inputs.
  // Check if the player's terminal can display colors.
  // If it can, the game can start, otherwise we quit the game.
  if (has_colors() == FALSE) {
    // If it can't, then display error message, wait for input and end game.
    print_centered("We can't display colors, press any key to quit!");
    refresh();
    getch();
    endwin();

    return 1;
  }
  start_color(); // We want to have some color.
  // Set foreground color to red and background color to black.
  init_pair(1, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(1)); // Activate the new color pair 1.

  keypad(stdscr, TRUE);

  print_centered("Let's play some Pong!"); // Print a centered welcome message.
  refresh(); // Always call refresh() to update the screen.

  // We need some preparation to start the game.
  short x, y;
  getmaxyx(stdscr, y, x);

  // Create a player paddle.
  struct paddle player = {
    .x = 1,
    .y = y / 2,
    .length = y / 5,
    .speed = 0
  };

  // Create the ai paddle to play against.
  struct paddle ai = {
    .x = x - 2,
    .y = y / 2,
    .length = y / 5,
    .speed = 0
  };

  // Create one ball structure for a basic game.
  struct ball ball = {
    .x = x / 2,
    .y = y / 2,
    .speedx = 1,
    .speedy = 1
  };

  getch(); // Wait for player input to start game.

  short input; // Short that is evaluated in the game loop for player input.
  halfdelay(1);

  // Enter the game loop.
  while (1) {
    clear();
    paddle_draw(&player);
    paddle_draw(&ai);
    paddle_update(&player);
    paddle_update(&ai);
    ball_draw(&ball);
    ball_update(&ball);
    refresh();
    input = getch();
    // usleep(16);
    switch (input) {
      case KEY_UP:
        player.speed -= 1;
        if (player.speed < -2) player.speed = -2;
        break;
      case KEY_DOWN:
        player.speed += 1;
        if (player.speed > 2) player.speed = 2;
        break;
      case 27: // 27 is the escape key. Seems the solution isn't ideal though.
        return 0;
    }
  }
  endwin();

  return 0;
}
