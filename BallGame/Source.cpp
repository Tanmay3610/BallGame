#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include<vector>
#include <ctime>
#include <string> 
using namespace std;

bool GAME_ON = true;
int GAME_TIME = 0;
double backR = 0, backG = 0, backB = 0;
const double HEIGHT = 500;
const double WIDTH = 500;
const double PI = 22.0 / 7;
const int PIPES_WIDTH = 50;
int frameCount = 0;
double GAME_SPEED = 1;
double BALL_SIZE = 20;

void drawText(float x, float y, string str) {
	glRasterPos2f(x, y);

	for (auto character: str) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, character);
	}
}

class Ball {
public:
	double y, x, velocity = 0.0;
	const double gravity = 0.01;
	const double liftForce = -0.35;

	Ball(double x) {
		this->y = HEIGHT / 2;
		this->x = x;
	}

	void show() {
		glColor3f(1.0, 0.0, 0.0);
		glPointSize(BALL_SIZE);
		glBegin(GL_POINTS);
		glVertex2d(x, y);
		glEnd();
	}

	void update() {
		velocity = velocity + gravity;
		if (y - velocity < 0){
			velocity = 0;
		}
		if (y - velocity > HEIGHT - 10){
			velocity = 0;
		}
		y = y - velocity;
	}

	void upLift() {
		velocity = velocity + liftForce;
		if (y - velocity > HEIGHT - 10){
			velocity = 0;
		}
	}
};

static Ball* ball;
class Pipe {
private:
	double RandomDoubleGenerator(double a, double b) {
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

	void showBottom() {
		glColor3f(red, green, blue);
		glLineWidth(PIPES_WIDTH);
		glBegin(GL_LINES);
		glVertex2d(x, 0);
		glVertex2d(x, bottom);
		glEnd();
	}

	bool hits() {
		if (ball->x + BALL_SIZE >= this->x and ball->x - BALL_SIZE <= this->x + PIPES_WIDTH) {
			if (ball->y < bottom or ball->y > top) {
				return true;
			}
		}
		return false;
	}

	void showTop() {
		glColor3f(red, green, blue);
		glLineWidth(PIPES_WIDTH);
		glBegin(GL_LINES);
		glVertex2d(x, HEIGHT);
		glVertex2d(x, top);
		glEnd();
	}

	void update() {
		x = x - GAME_SPEED;
		if (x <= 0) {
			x == 0;
		}
	}
};
vector<Pipe*> pipes;

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
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
	if (frameCount % 60 == 0) {
		pipes.push_back(new Pipe());
		GAME_TIME++;
	}
	drawText(WIDTH - 125, HEIGHT - 20, "SCORE: " + to_string(GAME_TIME));
	glutSwapBuffers();
}

void myinit() {
	glClearColor(backR, backG, backB, 0);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 499.0, 0.0, 499.0);
}

void timer(int) {
	if (GAME_ON) {
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, timer, 0);
	}	
}

void keyboard(unsigned char key, int x, int y) {
	if (key == ' ') {
		ball->upLift();
	}
}

void main(int argc, char** argv) {
	ball = new Ball(20);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Points");
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	myinit();
	glutMainLoop();
}