#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include<vector>
#include <ctime>
#include <string> 
using namespace std;

/*
*  Concepts Used:
*  1) To render points on screen
*  2) To render lines on the screen
*     a) Which line drawing algorithm is used to draw line in GLUT?
*     
*  SD - System Defined
*/

bool GAME_ON = true; //To ensure game is not over
int GAME_TIME = 0;   // Use to keep track of user current score
double backR = 0, backG = 0, backB = 0;
const double HEIGHT = 500;  // Height of screen
const double WIDTH = 500;   // Width of screen
const double PI = 22.0 / 7;
const int PIPES_WIDTH = 50; // Width of the obstacle
int frameCount = 0;         // To maintain a particular FPS (In our case it is  60 FPS)
double GAME_SPEED = 1;      // To increase the level of hardness I am increasing the game speed
double BALL_SIZE = 20;      // The size of the ball
int FPS = 120;               // Frame Per Second

void drawText(float x, float y, string str) {
	// To print the string on the screen
	glRasterPos2f(x, y);  // SD

	for (auto character: str) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, character); //SD
	}
}

class Ball {
public:
	double y, x, velocity = 0.0;
	const double gravity = 0.01; // Acceleration due to gravity
	const double liftForce = -0.35; // Force towards upward

	Ball(double x) { // Constructor for the Ball Class
		this->y = HEIGHT / 2;
		this->x = x;
	}

	void show() {
		glColor3f(1.0, 0.0, 0.0); // To define the color of the ball
		glPointSize(BALL_SIZE);   // To define the size of the ball
		glBegin(GL_POINTS);       // SD
		glVertex2d(x, y);         // SD
		glEnd();                 // SD
	}

	void update() {
		velocity = velocity + gravity;
		if (y - velocity < 0){
			velocity = 0; // To stop the ball of if it goes out of the boundry
		}
		if (y - velocity > HEIGHT - 10){
			velocity = 0; // To stop the ball of if it goes out of the boundry
		}
		y = y - velocity;
	}

	void upLift() {
		velocity = velocity + liftForce;
		if (y - velocity > HEIGHT - 10){
			velocity = 0; // To stop the ball of if it goes out of the boundry
		}
	}
};

static Ball* ball;
class Pipe { // For obstacles
private:
	double RandomDoubleGenerator(double a, double b) { // To choose the random height of the pipe
		double random = ((double)rand()) / (double)RAND_MAX;
		double diff = b - a;
		double r = random * diff;
		return a + r;
	}
public:
	double top, bottom, x = WIDTH, y, red, green, blue;
	
	Pipe() {
		this->top = RandomDoubleGenerator((HEIGHT / 2) + 50, HEIGHT);
		this->bottom = RandomDoubleGenerator(0, (HEIGHT / 2) - 50);
		this->red = RandomDoubleGenerator(0, 1);
		this->green = RandomDoubleGenerator(0, 1);
		this->blue = RandomDoubleGenerator(0, 1);
	}

	void showBottom() { // To render the bottom pipe
		glColor3f(red, green, blue);
		glLineWidth(PIPES_WIDTH);
		glBegin(GL_LINES);
		glVertex2d(x, 0);
		glVertex2d(x, bottom);
		glEnd();
	}

	bool hits() { // To check if the ball hit the pipe or not
		if (ball->x + BALL_SIZE >= this->x and ball->x - BALL_SIZE <= this->x + PIPES_WIDTH) {
			if (ball->y < bottom or ball->y > top) {
				return true;
			}
		}
		return false;
	}

	void showTop() { // To render the top pipe
		glColor3f(red, green, blue);
		glLineWidth(PIPES_WIDTH);
		glBegin(GL_LINES);
		glVertex2d(x, HEIGHT);
		glVertex2d(x, top);
		glEnd();
	}

	void update() { // To move the pipe backwards
		x = x - GAME_SPEED;
		if (x <= 0) {
			x == 0;
		}
	}
};
vector<Pipe*> pipes;

void display() {
	glClear(GL_COLOR_BUFFER_BIT); // SD
	for (int i = 0; i < pipes.size(); i++) {
		pipes[i]->showBottom();
		pipes[i]->showTop();
		pipes[i]->update();

		if (pipes[i]->hits()) {
			GAME_ON = false;
		}

		if (pipes[i]->x < 0) {
			pipes.erase(pipes.begin());
		}
	}
	if (GAME_TIME != 0 and GAME_TIME % 5 == 0) {
		GAME_SPEED += 0.01;
	}
	ball->show();
	ball->update();
	frameCount++;
	if (frameCount == INT_MAX) {
		frameCount = 0;
	}
	if (frameCount % FPS == 0) {
		pipes.push_back(new Pipe()); // Randomly generating a pipe after every 1 second
		GAME_TIME++;
	}
	drawText(WIDTH - 125, HEIGHT - 20, "SCORE: " + to_string(GAME_TIME));
	glutSwapBuffers(); // SD
}

void myinit() { // Initialisation of the initial game screen
	glClearColor(backR, backG, backB, 0); // SD
	glPointSize(5.0);                     // SD
	glMatrixMode(GL_PROJECTION); // SD
	glLoadIdentity(); // SD
	gluOrtho2D(0.0, 499.0, 0.0, 499.0); // SD
}

void timer(int) { // To control the FPS of the game
	if (GAME_ON) {
		glutPostRedisplay(); // To call the display function
		glutTimerFunc(1000 / FPS, timer, 0); // To cal the display function at every 1 / 60th second
	}	
}

void keyboard(unsigned char key, int x, int y) { // Call Back Function to handle spacebar key interupt
	if (key == ' ') {
		ball->upLift();
	}
}

void main(int argc, char** argv) {
	ball = new Ball(20);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // GLUT_DOUBLE to render Animation 
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Ball Game"); // Title of the screen
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	myinit();
	glutMainLoop();
}
