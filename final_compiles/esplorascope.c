#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define TIME_STEP 1
#define WIDTH    160
#define HEIGHT    48
#define MIN_A     -5.0
#define MAX_A      5.0

#define F(a) (a < MIN_A) ? 0 : (a > MAX_A ? HEIGHT-1 : (a - MIN_A)/(MAX_A - MIN_A) * (HEIGHT - 1))

void draw_display(char display[HEIGHT][WIDTH]);

int main(int argc, char* argv[]){
  int t, t_prev, buffer_size, points_read;
  int i, j;
  double a_x, a_y, a_z;
  double sum_x, sum_y, sum_z;
  double *x_buff, *y_buff, *z_buff;
  char display[HEIGHT][WIDTH];

  if (argc < 2){
	buffer_size = 1;
  }
  else{
	buffer_size = atoi(argv[1]);
  }
  x_buff = malloc(sizeof(double) * buffer_size);
  y_buff = malloc(sizeof(double) * buffer_size);
  z_buff = malloc(sizeof(double) * buffer_size);
  
  if (!(x_buff && y_buff && z_buff)){
	printf("Malloc Failed. Check buffer size and try again.\n");
	return 1;
  }
  
  a_x = 0.0;
  while(a_x == 0.0){
	  scanf(" %d, %lf, %lf, %lf", &t, &a_x, &a_y, &a_z);
	  printf(" %d, %lf, %lf, %lf, %d\n", t, a_x, a_y, a_z);
  }
  //printf("past init loop\n");
  
  initscr();
  curs_set(0);
  noecho();
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK); 
  init_pair(3, COLOR_CYAN, COLOR_BLACK);
  t_prev = 0;

  for (i = 0; i < HEIGHT; ++i){
    for (j = 0; j < WIDTH; ++j){
      display[i][j] = ' ';
    }
  }

  points_read = 0;
  
  while(1){
    scanf(" %d, %lf, %lf, %lf", &t, &a_x, &a_y, &a_z);
	mvprintw(0,0, "%lf", a_x);
	refresh();
    if (t - t_prev < TIME_STEP){
      continue;
    }
	++points_read;
	
	sum_x = sum_y = sum_z = 0;
	for (i = 0; i < buffer_size - 1; ++i){
		sum_x += x_buff[i] = x_buff[i + 1];
		sum_y += y_buff[i] = y_buff[i + 1];
		sum_z += z_buff[i] = z_buff[i + 1];
	}
	sum_x += x_buff[i] = a_x;
	sum_y += y_buff[i] = a_y;
	sum_z += z_buff[i] = a_z;
	
	a_x = sum_x / buffer_size;
	a_y = sum_y / buffer_size;
	a_z = sum_z / buffer_size;
	
    t_prev = t;

    for (i = 0; i < HEIGHT; ++i){
      for (j = 0; j < WIDTH - 1; ++j){
        display[i][j] = display[i][j + 1];
      }
      display[i][j] = ' ';
    }

	if (points_read > buffer_size){
      display[(int)(F(a_x))][WIDTH - 1] = 'X';
      display[(int)(F(a_y))][WIDTH - 1] = 'Y';
      display[(int)(F(a_z))][WIDTH - 1] = 'Z';
	}
    draw_display(display);
  }

  endwin();

  return 0;
}

void draw_display(char display[HEIGHT][WIDTH]){
  int i, j;

  for (i = 0; i < HEIGHT; ++i){
    for (j = 0; j < WIDTH; ++j){
      if (display[i][j] > ' '){
        attron(COLOR_PAIR(display[i][j] - 'X' + 1));
      }
      mvaddch(HEIGHT - 1 - i, j, display[i][j]);
      if (display[i][j] > ' '){
        attroff(COLOR_PAIR(display[i][j] - 'X' + 1));
      }
    }
  }

  refresh();
}
