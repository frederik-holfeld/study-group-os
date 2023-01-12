#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BALL_SPEED_MIN 1
#define BALL_SPEED_MAX 2

short x, y; // Global variable, so we only need to set them once.
// Players need to be global so we don't need to pass them to increment scores.
struct paddle player, ai;

// This struct defines a paddle of a player or AI.
struct paddle {
  short x; // Position is equal to a column.
  float y; // Needs to be float so movement is more smoothed out.
  char length;
  float speed;
  char score;
};
// This struct describes a ball in the game.
struct ball {
  float x, y, speedx, speedy;
};
// A little helper function that makes displaying centered text easier.
void print_centered(char *string) {
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
float random_speed(float min, float max) {
  float speed = min + (float) rand() / (float) RAND_MAX * (max - min);
  if (rand() % 2 == 0) {
    return speed;
  } else return -speed;
}
void ball_update(struct ball *ball) {
  ball->x = ball->x + ball->speedx;
  ball->y = ball->y + ball->speedy;
  if (ball->x < 3 && (ball->y > (player.y - player.length / 2) && ball->y < (player.y + player.length / 2))) {
    ball->speedx *= -1;
    ball->speedy = random_speed(BALL_SPEED_MIN, BALL_SPEED_MAX);
  } else if (ball->x > x - 3 && (ball->y > (ai.y - ai.length / 2) && ball->y < (ai.y + ai.length / 2))) {
    ball->speedx *= -1;
    ball->speedy = random_speed(BALL_SPEED_MIN, BALL_SPEED_MAX);
  } else if (ball->x > x) {
    player.score++;
    if (player.score >= 20) {
      print_centered("The player wins!");
    } else {
      print_centered("The player has scored a point!");
    }
    ball->x = x / 2;
    ball->y = y / 2;
    ball->speedx = random_speed(BALL_SPEED_MIN, BALL_SPEED_MAX);
    ball->speedy = random_speed(BALL_SPEED_MIN, BALL_SPEED_MAX);
    halfdelay(100);
    getch();
    halfdelay(1);
  } else if (ball->x < 0) {
    ai.score++;
    if (ai.score >= 20) {
      print_centered("The AI wins!");
    } else {
      print_centered("The AI has scored a point!");
    }
    ball->x = x / 2;
    ball->y = y / 2;
    ball->speedx = random_speed(BALL_SPEED_MIN, BALL_SPEED_MAX);
    ball->speedy = random_speed(BALL_SPEED_MIN, BALL_SPEED_MAX);
    halfdelay(100);
    getch();
    halfdelay(1);
  }
  if (ball->y + 1 > y) {
    ball->y -= (ball->y + 1 - y) * 2;
    ball->speedy *= -1;
  } else if (ball->y - 1 < 0) {
    ball->speedy *= -1;
  }
}
void draw_hud() {
  mvprintw(0, 0, "Player score: %d", player.score);
  mvprintw(0, x - 12, "AI score: %d", ai.score);
}
void ai_update(struct paddle *paddle, struct ball *ball) {
  paddle->speed += (ball->y - paddle->y) * 0.2;
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
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(1)); // Activate the new color pair 1.

  keypad(stdscr, TRUE);

  getmaxyx(stdscr, y, x); // Set x and y to the screen size along x and y axes.

  print_centered("Let's play some Pong!"); // Print a centered welcome message.
  refresh(); // Always call refresh() to update the screen.

  srand(time(NULL)); // Seed random for random ball movement.

  // Write player paddle data into global variable.
  player = (struct paddle) {
    .x = 1,
    .y = y / 2,
    .length = y / 5,
    .speed = 0,
    .score = 0
  };

  // Same for the ai.
  ai = (struct paddle) {
    .x = x - 2,
    .y = y / 2,
    .length = y / 5,
    .speed = 0,
    .score = 0
  };

  // Create one ball structure for a basic game.
  struct ball ball = {
    .x = x / 2,
    .y = y / 2,
    .speedx = random_speed(BALL_SPEED_MIN, BALL_SPEED_MAX),
    .speedy = random_speed(BALL_SPEED_MIN, BALL_SPEED_MAX)
  };

  getch(); // Wait for player input to start game.

  short input; // Short that is evaluated in the game loop for player input.
  halfdelay(1);

  // Enter the game loop.
  while (1) {
    clear();
    attron(COLOR_PAIR(2));
    paddle_draw(&player);
    attron(COLOR_PAIR(3));
    paddle_draw(&ai);
    attron(COLOR_PAIR(1));
    paddle_update(&player);
    ai_update(&ai, &ball);
    paddle_update(&ai);
    ball_draw(&ball);
    ball_update(&ball);
    draw_hud();
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
