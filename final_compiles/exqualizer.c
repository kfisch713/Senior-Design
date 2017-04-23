// esplora-graph.c
//
// This is the outline for your program
// Please implement the functions given by the prototypes below and
// complete the main function to make the program complete.
// You must implement the functions which are prototyped below exactly
//  as they are requested.

#include <stdio.h>
#include <math.h>
#define PI 3.141592653589

//NO GLOBAL VARIABLES ALLOWED


//PRE: Arguments must point to double variables or int variables as appropriate
//This function scans a line of explore data, and returns
//  True when left button is pressed
//  False Otherwise
//POST: it modifies its arguments to return values read from the input line.
int read_acc(double* a_x, double* a_y, double* a_z, int* time, int* Button_UP, int* Button_DOWN, int* Button_LEFT, int* Button_RIGHT){
	scanf(" %d, %lf, %lf, %lf, %d, %d, %d, %d", time, a_x, a_y, a_z, Button_DOWN, Button_UP, Button_LEFT, Button_RIGHT); 

	return 0;
}

// PRE: -1.0 <= x_mag <= 1.0
// This function computes the roll of the esplora in radians
// POST: -PI/2 <= return value <= PI/2
double roll(double x_mag){
	if (x_mag > 1) {x_mag = 1;}
	else if (x_mag < -1) {x_mag = -1;}
	else x_mag = asin(x_mag);
	return x_mag;
}

// PRE: -1.0 <= y_mag <= 1.0
// This function computes the pitch of the esplora in radians
// if y_mag outside of -1 to 1, treat it as if it were 1 or -1
// POST: -PI/2 <= return value <= PI/2
double pitch(double y_mag){
	if (y_mag > 1) {y_mag = 1;}
	else if (y_mag <-1) {y_mag = -1;}
	else y_mag = asin(y_mag);
	return y_mag;
}


// PRE: -PI/2 <= rad <= PI/2
// This function scales the roll value to fit on the screen
// POST: -(1/2 screen width - 1) <= return value <= (1/2 screen width -1)
int scaleRadsForScreen(double rad){
	rad = (78.0/PI)*rad;
	return rad;
}

// PRE: num >= 0
// This function prints the character use to the screen num times
// POST: nothing is returned, but use has been printed num times
void print_chars(int num, char use){
	int i;
	for(i=0;i<num;++i){
		printf("%c", use);
	}
	
}
	

//PRE: -39 <= 0 <=39
// Uses print_chars to graph a number from -39 to 39 on the screen.
// You may assume that the screen is 80 characters wide.
void graph_line(int number){
	if (number>0){
		print_chars(40-number, ' ');
		print_chars(number, 'l');
		printf("\n");
	}
	if (number<0){
		print_chars(40,' ');
		print_chars((-1)*number, 'r');
		printf("\n");
	}
	if (number==0){
		print_chars(40,' ');
		printf("0\n");
	}
}

int main()
{
	double x, y, z;			// magnitude values of x, y, and z accelerations
	int b_Up, b_Down, b_Left, b_Right;	// variables to hold the button statuses
	double roll_rad, pitch_rad;		// value of the roll measured in radians
	int scaled_value; 	// value of the roll adjusted to fit screen display
	
	//insert any beginning needed code here
	int t; //t is for time
	int b_Joystick, Slider;
	int toggle = 0;
	do
		{
		// Get line of input
		read_acc(&x, &y, &z, &t, &b_Up, &b_Down, &b_Left, &b_Right);

		// calculate roll and pitch
		roll_rad = roll(x);
		pitch_rad = pitch(y);


		// switch between roll and pitch(up vs. down button)
		if (b_Up){
			toggle = 0;
		}
		if (b_Down){
			toggle = 1;
		}
		
		if (toggle){
			scaled_value = scaleRadsForScreen(pitch_rad);
		}
		else scaled_value = scaleRadsForScreen(roll_rad);
		

		// Output your graph line
		graph_line(scaled_value);
		
		if (b_Left){
			return 0;
		}


		fflush(stdout);
		} while (1 ); // Modify to stop when left button is pressed
	return 0;
}

