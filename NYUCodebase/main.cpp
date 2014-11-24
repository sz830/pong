//two options iwth loading textures, load them all in before the loop. could take a while, or load each texture again and again in the loop, starts faster, slower gametime. bad idea. thus, we load all at beginning!
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include <cstdlib>
#include <iostream>

SDL_Window* displayWindow;
Entity ball;
Entity paddleleft;
Entity paddleright;
Entity rightwinE;
Entity leftwinE;
bool rightwin = false;
bool leftwin = false;

typedef struct {
	float x;
	float y;
	float r;
	float g;
	float b;
} Vertex2D;
GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID); // how many names to generate, and address of arrays where names go
	glBindTexture(GL_TEXTURE_2D, textureID); // says, gltexture2d is bound to textureid now
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);//sets texture data of target
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//target, property, property definition, sets the gltextureminfilter property of gltexture2d to gllinear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}
GLuint LoadTexture24(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID); // how many names to generate, and address of arrays where names go
	glBindTexture(GL_TEXTURE_2D, textureID); // says, gltexture2d is bound to textureid now
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);//sets texture data of target
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//target, property, property definition, sets the gltextureminfilter property of gltexture2d to gllinear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}
void DrawSprite(GLint texture, float x, float y, float rotation, float scale) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture); // so gltexture_2d knows what textureid to draw
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);//enables blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void drawSquare(float x, float y, float len, float scale, float rotate)
{
	glMatrixMode(GL_MODELVIEW);
	GLfloat square[] = { -len / 2., len / 2., -len / 2., -len / 2., len / 2., -len / 2., len / 2., len / 2. }; // make square
	glVertexPointer(2, GL_FLOAT, 0, square);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat squareColors[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, .5, .5, .5 };
	glColorPointer(3, GL_FLOAT, 0, squareColors);
	glEnableClientState(GL_COLOR_ARRAY);

	glLoadIdentity(); // seems we only really mess with the modelview matrix, the projection matrix is always ortho, that's it.
	glTranslatef(x, y, 0.0);
	glRotatef(rotate, 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float scale, float rotate)
{
	glMatrixMode(GL_MODELVIEW);
	Vertex2D triangle[3] = { {x1,y1,1,0,0}, {x2,y2,0,1,0}, {x3,y3,0,0,1} };
	glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, triangle);//read two elements from every 5. of triangle
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &triangle[0].r);
	glEnableClientState(GL_COLOR_ARRAY);

	glLoadIdentity(); // seems we only really mess with the modelview matrix, the projection matrix is always ortho, that's it.
	glRotatef(rotate, 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void initBall(GLuint id)
{
	ball.x = -.7;
	ball.y = 0;
	ball.height = .1;
	ball.width = .1;
	ball.textureID = id;
	ball.rotation = 0;
	ball.velocity_x = .4f;
	ball.velocity_y = .4f;
}
bool isRightPaddleCollision()
{
	if (ball.x > paddleright.x - .05&&ball.x<paddleright.x + .05&&    ball.y>paddleright.y - .2&&ball.y < paddleright.y + .2)
		return true;
	if (ball.x+.05 > paddleright.x - .05&&ball.x+.05<paddleright.x + .05&&    ball.y>paddleright.y - .2&&ball.y < paddleright.y + .2)
		return true;
	if (ball.x - .05 > paddleright.x - .05&&ball.x - .05<paddleright.x + .05&&    ball.y>paddleright.y - .2&&ball.y < paddleright.y + .2)
		return true;
	return false;
}
bool isLeftPaddleCollision()
{
	if (ball.x > paddleleft.x - .05&&ball.x<paddleleft.x + .05&&    ball.y>paddleleft.y - .2&&ball.y < paddleleft.y + .2)
		return true;
	if (ball.x + .05 > paddleleft.x - .05&&ball.x + .05<paddleleft.x + .05&&    ball.y>paddleleft.y - .2&&ball.y < paddleleft.y + .2)
		return true;
	if (ball.x - .05 > paddleleft.x - .05&&ball.x - .05<paddleleft.x + .05&&    ball.y>paddleleft.y - .2&&ball.y < paddleleft.y + .2)
		return true;
	return false;
}
bool isRoofCollision(){
	if (ball.y >= 1.-.05)
		return true;
	return false;
}
bool isFloorCollision(){
	if (ball.y <= -1.+.05)
		return true;
	return false;
}
void updateBall(float elapsed){
	if (isRightPaddleCollision())
	{
		ball.x = 1.33 - .15; // offset, move ball x value to right before right paddle
		ball.velocity_x *= -1.0f;
	}
	if (isLeftPaddleCollision())
	{
		ball.x = -1.33 + .15; // offset, move ball x value to right before left paddle
		ball.velocity_x *= -1.0f;
	}
	if (isRoofCollision())
	{
		ball.y = 1.0f - .05f;
		ball.velocity_y *= -1.0f;
	}
	if (isFloorCollision())
	{
		ball.y = -1.0f + .05f;
		ball.velocity_y *= -1.0f;
	}
	ball.x += ball.velocity_x*elapsed;
	ball.y += ball.velocity_y*elapsed;
}
void initPaddleLeft(GLuint id)
{
	paddleleft.x = -1.33 + .05;
	paddleleft.y = 0;
	paddleleft.height = .4;
	paddleleft.width = .1;
	paddleleft.textureID = id;
	paddleleft.rotation = 0;
	paddleleft.velocity_y = .9;
}
void initPaddleRight(GLuint id)
{
	paddleright.x = 1.33 - .05;
	paddleright.y = 0;
	paddleright.height = .4;
	paddleright.width = .1;
	paddleright.textureID = id;
	paddleright.rotation = 180;
	paddleright.velocity_y = .9; //actually is speed
}
void initRightwin(GLuint id)
{
	rightwinE.x = 0;
	rightwinE.y = 0;
	rightwinE.width = 2.66;
	rightwinE.height = 2.0f;
	rightwinE.textureID = id;
	rightwinE.rotation = 0.0;
}
void initLeftwin(GLuint id)
{
	leftwinE.x = 0;
	leftwinE.y = 0;
	leftwinE.width = 2.66;
	leftwinE.height = 2.0f;
	leftwinE.textureID = id;
	leftwinE.rotation = 0.0;
}
void isWin()
{
	if (ball.x > 1.33 - .05)
		leftwin = true;
	if (ball.x < -1.33 + .05)
		rightwin = true;
}
void endGame()
{
	ball.x = 0; ball.y = 0;
	ball.velocity_x = 0; ball.velocity_y = 0;
}
int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Simon Project 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL); // SDL Window, title, position,position,width,height, type of graphics?
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glClearColor(0.5, 0.5, 0.0, .9);//Green-color

	//Setup
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	//LoadTextures
	GLuint paddleID = LoadTexture("pngs/paddle.png");
	GLuint ballID = LoadTexture("pngs/ball.png");
	GLuint leftwinID = LoadTexture24("pngs/leftwin.png");
	GLuint rightwinID = LoadTexture24("pngs/rightwin.png");
	//Init Entities
	initBall(ballID);
	initPaddleLeft(paddleID);
	initPaddleRight(paddleID);
	initRightwin(rightwinID);
	initLeftwin(leftwinID);

	//Time
	float lastFrameTicks = 0.0;
	
	SDL_Event event;
	bool done = false;
	while (!done) {
		glClear(GL_COLOR_BUFFER_BIT);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		//Read Input and Update World
		while (SDL_PollEvent(&event)) { // a lot of events could cue up, no?, could also get all keyboard states and check for those. I think I like that better. - specially for pong
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_UP]) {
			paddleright.y += elapsed*paddleright.velocity_y;
			if (paddleright.y > (1 - .2)){ paddleright.y = 1 - .2; }
		}
		else if (keys[SDL_SCANCODE_DOWN]) {
			paddleright.y -= elapsed*paddleright.velocity_y;
			if (paddleright.y < (-1 + .2)){ paddleright.y = -1 + .2; }
		}
		if (keys[SDL_SCANCODE_W]) {
			paddleleft.y += elapsed*paddleleft.velocity_y;
			if (paddleleft.y >(1 - .2)){ paddleleft.y = 1 - .2; }
		}
		else if (keys[SDL_SCANCODE_S]) {
			paddleleft.y -= elapsed*paddleleft.velocity_y;
			if (paddleleft.y < (-1 + .2)){ paddleleft.y = -1 + .2; }
		}

		updateBall(elapsed);
		isWin();

		//Render World
		ball.Draw();
		paddleleft.Draw();
		paddleright.Draw();
		if (rightwin)
		{
			endGame();
			rightwinE.Draw();
		}
		if (leftwin)
		{
			endGame();
			leftwinE.Draw();
		}

		SDL_GL_SwapWindow(displayWindow);//Swaps old modelview with new one?
	}

	SDL_Quit();
	return 0;
}