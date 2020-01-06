#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <math.h>


#include "AudioManager.h"
using namespace std;


#define ULTIMATE_TIMER 300 //means ultimate gt 5 second

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")


//AudioManager * sounds = new AudioManager;

#define WINDOW_TITLE "Assignment"
void drawBackground(string getFileName);
void drawBody();void drawLeftHand();void drawFinger();void drawWholeBody();void drawHead();void drawEars();
// rotate camera
float angle = 0.0f, angle2 = 90.0f, rotateCam = 0.0f, rotateCam2 = 0.0f;
float viewLeft = -5.0, viewRight = 5.0, bottom = -5.0, top = 5.0, nearOrtho = -5.0, farOrtho = 5.0;

//finger
float fingerAngleRotate = 0.0f;
float fingerXRotate = 0.0f;
float fingerYRotate = 0.0f;
float fingerZRotate = 0.0f;

float increment = 0.0f;
float atkIncrement = 0.0f;
float atkIncrement2 = 0.0f;
int ultimateTimer =450;

float z = 0.0, speed = 1.0;
string file[] = { "body.bmp", "gold_metal.bmp" };
int i = 0, backGround = 0;
string getFileName = file[i];
//texture is for background, texture 2 is for rectangle, so is BMP, BMP2.....
GLuint texture = 0, texture2 = 0, texture3 = 0, texture4 = 0;
BITMAP BMP, BMP2, BMP3, BMP4;
HBITMAP hBMP = NULL, hBMP2 = NULL, hBMP3 = NULL, hBMP4 = NULL;
boolean checkinput = 0, switchONOFF = 1;
// set view and coordinate
bool reset = false, changeCamera = false, changeWeapon = true, timerCountDown=false, playSong = false;
boolean orThoView = 1, perspectiveView = 0, isRotate = 0, weaponOneStatus = 0, weaponTwoStatus = 1, pokeballStatus = 0,backgroundStatus = 2;
//boolean orThoView = 0, perspectiveView = 1, isRotate = 0, weaponStatus = 1;
//robot status
//boolean isRun = 0, isRunUpper = 0, isRunLower = 0;
bool isRun = false, isRunUpper = false, isReachTop = false, isReachBottom = false, isAttack = false;
bool isHammerTop = false, isHammerBottom = false, isSwordTop = false;
bool isAtk1 = false, isAtk2 = false, isAtk3 = false, isAtk4 = false;
float runAngle = 0,attackAngle1 = 0.0, attackAngle2 =0.0, attackAngle3 = 0.0, attackAngle4 = 0.0;
 //global variables




LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0); //Go to Defination

		else if (wParam == 'Q') {
			if (i == 0)
			{
				i++;
				getFileName = file[i];
			}
			else if (i == 1)
			{
				i = 0;
				getFileName = file[i];
			}

		}

		else if (wParam == 'U') {
			z += speed;
		}


		else if (wParam == 'J') {
			z -= speed;
		}

		else if (wParam == VK_LEFT)
		{
			rotateCam -= 10;
		}
		else if (wParam == VK_RIGHT)
		{
			rotateCam += 10;
		}
		else if (wParam == VK_UP)
		{

			rotateCam2 += 20;


		}
		else if (wParam == VK_DOWN)
		{

			rotateCam2 -= 20;
		}
		else if (wParam == 0x31) {
			timerCountDown = true;

			if (pokeballStatus == 0 && backgroundStatus == 0) {
				pokeballStatus = 1;
				backgroundStatus = 1;
				//sounds->playPowerUpEffect();
			}

			
		}

		else if (wParam == 0x33) {
			changeWeapon = true;
			weaponOneStatus = !weaponOneStatus;
			weaponTwoStatus = !weaponTwoStatus;

			reset = true;
		}

		else if (wParam =='W') {
			if (isRun == false) {
				isRun = true;
				isRunUpper = false;
				increment = 4.0f;
			}
		}

		else if (wParam == 'A') {
			if (isAttack == false&&weaponTwoStatus == 1) {
				isAttack = true;
				isAtk1 = true;
				atkIncrement = 9.0f;
				atkIncrement2 = 15.0f;
				isHammerTop = true;
			}
		}

		else if (wParam == VK_SPACE)
		{
			changeCamera = true;
			orThoView = !orThoView;
			perspectiveView = !perspectiveView;
			reset = true;
			/*if (orThoView == 0 && perspectiveView == 1)
			{
				 orThoView = 1;
				 reset = true;
			}
			else if (orThoView == 1 && perspectiveView == 0)
			{
				 perspectiveView = 1;
				 reset = true;
			}*/

		}

		break;


	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0; //for shadow

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE; //1 layer
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------







void drawBackground(string getFileName) {

	LPCSTR GET;
	GET = getFileName.c_str();

	//Step 3: Initial texture Info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), GET,
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	//Step 4: Assign texture to polygon
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture); //Generate texture
	glBindTexture(GL_TEXTURE_2D, texture); //can bind with the texture using 2D array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
	glTexCoord2f(0.0f, 0.0f);

	//glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);


	// Draw a textured quad
	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, -6.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f, -6.0f, -3.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(8.0f, -6.0f, -3.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, 6.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f, 6.0f, -3.5f);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);


	DeleteObject(hBMP);
	glDeleteTextures(1, &texture);
}


void TextureCubeORRectangle(float resizes, float height, float thickness, string getFileName)
{
	LPCSTR GET;
	GET = getFileName.c_str();

	//Step 3: Initial texture Info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP2 = (HBITMAP)LoadImage(GetModuleHandle(NULL), GET,
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP2, sizeof(BMP2), &BMP2);

	//Step 4: Assign texture to polygon
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture2); //Generate texture
	glBindTexture(GL_TEXTURE_2D, texture2); //can bind with the texture using 2D array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP2.bmWidth, BMP2.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP2.bmBits);
	glTexCoord2f(0.0f, 0.0f);


	glPushMatrix();
	glBegin(GL_QUADS);
	// Face 1
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, thickness);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(resizes, 0.0f, thickness);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(resizes, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	// Face 2
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, height, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, height, thickness);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, thickness);
	// Face 3 
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, thickness);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, height, thickness);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(resizes, height, thickness);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(resizes, 0.0f, thickness);
	// Face 4
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(resizes, 0.0f, thickness);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(resizes, height, thickness);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(resizes, height, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(resizes, 0.0f, 0.0f);
	// Face 5
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(resizes, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(resizes, height, 0.0f);
	// Face 6 …
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(resizes, height, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(resizes, height, thickness);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, height, thickness);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	glEnd();

	glPopMatrix();

	//Step5: Remove Texture Info
	glDisable(GL_TEXTURE_2D);
	DeleteObject(hBMP2);
	glDeleteTextures(1, &texture2);
}


void TextureCylinderORCorn(float baseRadius, float topRadius, float height, int slices, int stacks, string getFileName)
{
	LPCSTR GET;
	GET = getFileName.c_str();

	//Step 3: Initial texture Info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP3 = (HBITMAP)LoadImage(GetModuleHandle(NULL), GET,
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP3, sizeof(BMP3), &BMP3);

	//Step 4: Assign texture to polygon
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture3); //Generate texture
	glBindTexture(GL_TEXTURE_2D, texture3); //can bind with the texture using 2D array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP3.bmWidth, BMP3.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP3.bmBits);
	glTexCoord2f(0.0f, 0.0f);

	GLUquadricObj *cylinder = NULL;
	cylinder = gluNewQuadric();
	glPushMatrix();
	gluQuadricTexture(cylinder, TRUE);
	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
	glPopMatrix();

	gluDeleteQuadric(cylinder); //clear memory

								//Step5: Remove Texture Info
	glDisable(GL_TEXTURE_2D);
	DeleteObject(hBMP3);
	glDeleteTextures(1, &texture3);
}

void TextureSphere(float radius, int slices, int stacks, string getFileName)
{
	LPCSTR GET;
	GET = getFileName.c_str();

	//Step 3: Initial texture Info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP4 = (HBITMAP)LoadImage(GetModuleHandle(NULL), GET,
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP4, sizeof(BMP4), &BMP4);

	//Step 4: Assign texture to polygon
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture4); //Generate texture
	glBindTexture(GL_TEXTURE_2D, texture4); //can bind with the texture using 2D array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP4.bmWidth, BMP4.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP4.bmBits);
	glTexCoord2f(0.0f, 0.0f);

	GLUquadricObj * sphere = NULL;
	sphere = gluNewQuadric();
	//glRotatef(0.2f, 0.5f, 0.5f, 0.5f);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, radius, slices, stacks);
	gluDeleteQuadric(sphere);


	glDisable(GL_TEXTURE_2D);
	DeleteObject(hBMP4);
	glDeleteTextures(1, &texture4);
}


void drawHead() {
	//neck
	glPushMatrix();
	glTranslatef(-0.75f, 1.55f, 0.4f);
	TextureCubeORRectangle(0.5f, 0.75f, 0.5f, "yellow.bmp");
	glPopMatrix();
	//	face
	glPushMatrix();
	glTranslatef(-1.05f, 2.25f, 0.4f);
	TextureCubeORRectangle(1.25f, 1.25f, 0.75f, "yellow.bmp");
	glPopMatrix();

	//eye
	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(3.0f, 0.12f, 1.15f);
	TextureSphere(0.20f, 30.0f, 30.0f, "black.bmp");
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(3.0f, 0.15f, 1.35f);
	TextureSphere(0.075f, 30.0f, 30.0f, "white.bmp");
	glPopMatrix();



	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(3.0f, 0.83f, 1.15f);
	TextureSphere(0.20f, 30.0f, 30.0f, "black.bmp");
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(3.0f, 0.81f, 1.35f);
	TextureSphere(0.075f, 30.0f, 30.0f, "white.bmp");
	glPopMatrix();

	//nose
	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(2.7f, 0.50f, 1.15f);
	TextureSphere(0.05f, 30.0f, 30.0f, "black.bmp");
	glPopMatrix();


	//red
	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(2.5f, 0.80f, 1.15f);
	TextureSphere(0.15f, 30.0f, 30.0f, "red.bmp");
	glPopMatrix();

	//red
	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(2.5f, 0.10f, 1.15f);
	TextureSphere(0.15f, 30.0f, 30.0f, "red.bmp");
	glPopMatrix();

	glPushMatrix();
	drawEars();
	glPopMatrix();






}

void drawEars() {
	//left ear
	glPushMatrix();
	glTranslatef(-1.2, 3.5, 0.65);
	/*glPushMatrix();*/
	glRotatef(90.0, 1.00, 0.0, 0.0);
	glRotatef(45.0, 0.00, 1.0, 0.0);
	//glPushMatrix();
	glTranslatef(0.0, 0.0, -1.0);
	TextureCylinderORCorn(0.005, 0.25, 0.75, 20, 25, "black.bmp");
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.75);
	TextureCylinderORCorn(0.25, 0.25, 0.75, 20, 25, "yellow.bmp");
	glPopMatrix();
	glPopMatrix();
	//right ear

	glPushMatrix();
	glTranslatef(0.2, 3.5, 0.65);
	/*glPushMatrix();*/
	glRotatef(90.0, 1.00, 0.0, 0.0);
	glRotatef(-45.0, 0.00, 1.0, 0.0);
	//glPushMatrix();
	glTranslatef(0.0, 0.0, -1.0);
	TextureCylinderORCorn(0.005, 0.25, 0.75, 20, 25, "black.bmp");
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.75);
	TextureCylinderORCorn(0.25, 0.25, 0.75, 20, 25, "yellow.bmp");
	glPopMatrix();
	glPopMatrix();
}



void drawBody() {


	glPushMatrix();
	glTranslatef(-1.2f, 0.55f, 0.0f);
	TextureCubeORRectangle(1.5f, 1.0f, 1.5f, file[i]);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.80f, -0.46f, 0.0f);
	TextureCubeORRectangle(0.75f, 1.0f, 1.5f, "jetengine.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.185f, -0.46f, 0.0f);
	TextureCubeORRectangle(0.41f, 1.0f, 1.5f, "black.bmp");
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.05f, -0.46f, 0.0f);
	TextureCubeORRectangle(0.35f, 1.0f, 1.5f, "black.bmp");
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1.2f, -1.44f, 0.0f); //1.35
	TextureCubeORRectangle(1.5f, 1.0f, 1.5f, file[i]);
	glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-3.5f, -0.5f, 0.0f);

	//TextureCubeORRectangle(1.5f, 1.5f, 1.5f, "brick_window.bmp");
	//glPopMatrix();

}


void drawLeftHand() {
	glPushMatrix();
	//hand move angle
	if (isRun == true) {

		if (isRunUpper == true)
		{
			runAngle += increment;
		}
		else if(isRunUpper == false) {
			runAngle -= increment;
		}
		if (runAngle >= 35 && isRunUpper == true) {
			runAngle -= increment;
			isRunUpper = false;
		}
		if (runAngle <= -35 && isRunUpper == false) {
			runAngle += increment;
			isRunUpper = true;
		}


	}
	if (isRun == 1) {
		glTranslatef(0.84f, 0.8f, 0.43f);
		glRotatef(runAngle, 1.0, 0.0, 0.0);
		glTranslatef(-0.84f, -0.8f, -0.43f);

	}
	
	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.94f, -0.7f, 0.23f);
	TextureCubeORRectangle(0.50f, 0.50f, 0.55f, "metal.bmp");
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.9f, -1.45f, 0.25f);
	TextureCubeORRectangle(0.75f, 0.75f, 0.75f, "gold_metal.bmp");
	glPopMatrix();
	//lower hand movement
	glPushMatrix();
	if (isRun == 1) {
		glTranslatef(0.54f, 0.8f, 0.23f);
		glRotatef(runAngle-30, 1.0, 0.0, 0.0);
		glTranslatef(-0.54f, -0.8f, -0.23f);

	}
	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.25f, -1.2f, 0.31f);
	TextureCubeORRectangle(0.75f, 0.25f, 0.55f, "metal.bmp");
	glPopMatrix();
	

	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.5f, -1.5f, 0.25f);
	TextureCubeORRectangle(0.75f, 0.75f, 0.75f, "gold_metal.bmp");
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.2f, -1.2f, 0.28f);
	TextureCubeORRectangle(0.75f, 0.25f, 0.65f, "metal.bmp");
	glPopMatrix();
	glPushMatrix();
	drawFinger();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void drawRightHand() {

	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.9f, 1.20f, 0.25f);
	TextureCubeORRectangle(0.50f, 0.50f, 0.55f, "metal.bmp");
	glPopMatrix();





	if (changeWeapon = true) {
		if (weaponOneStatus == 1) {

			glPushMatrix();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.9f, 1.80f, 0.55f);
			TextureSphere(0.50f, 30.0f, 30.0f, "pokeball.bmp");
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-3.1, -0.35, 0.65);

			glRotatef(90.0, 1.00, 0.0, 0.0);
			glRotatef(90.0, 0.00, 1.00, 0.0);
			glRotatef(45.0, 0.00, 1.00, 0.0);
			//glPushMatrix();
			glTranslatef(0.0, 0.0, -1.0);
			TextureCylinderORCorn(0.015, 0.45, 2.75, 20, 25, "metalSword.bmp");

			glPopMatrix();
		}

		if (weaponTwoStatus == 1) {
			glPushMatrix();
			if (isAttack == true) {

				glTranslatef(0.84f, 0.8f, 0.43f);
				glRotatef(attackAngle1, 1.0, 0.0, 0.0);
				glTranslatef(-0.84f, -0.8f, -0.43f);

				glTranslatef(-0.94f, 0.5f, 1.13f);
				glRotatef(attackAngle2, 0.0, 0.0, 1.0);
				glTranslatef(0.94f, -0.5f, -1.13f);


				glTranslatef(-0.64f, 0.4f, 0.63f);
				glRotatef(attackAngle3, 1.0, 0.0, 0.0);
				glTranslatef(0.64f, -0.4f, -0.63f);


				glTranslatef(-1.24f, 1.1f, 1.4f);
				glRotatef(attackAngle4, 0.0, 0.0, 1.0);
				glTranslatef(1.24f, -1.1f, -1.4f);

				if (isAtk1 == true) {
					attackAngle1 = attackAngle1 - atkIncrement;
				}
				else if (isAtk2 == true) {
					attackAngle2 = attackAngle2 + atkIncrement;
				}

				else if (isAtk3 == true) {
					attackAngle3 = attackAngle3 - atkIncrement;
				}

				else if (isAtk4 == true) {
					if (isHammerTop == false) {
						attackAngle4 = attackAngle4 - atkIncrement2;
					}
					else if (isHammerTop == true) {

						attackAngle4 = attackAngle4 + atkIncrement2;
					}

					if (attackAngle4 <= -135 && isHammerTop == false) {
						isHammerTop = true;
						attackAngle4 = attackAngle4 + atkIncrement2;
					}

					if (attackAngle4 >= 0 && isHammerTop == true) {
						isHammerTop = false;
						attackAngle4 = attackAngle4 - atkIncrement2;
					}
				}

			}


			if (attackAngle1 <= -180) {
				isAtk1 = false;
				isAtk2 = true;
			}

			if (attackAngle2 >= 45) {
				isAtk2 = false;
				isAtk3 = true;
			}

			if (attackAngle3 <= -55) {
				isAtk3 = false;
				isAtk4 = true;
			}

			if (attackAngle3 >= 55) {
				isAtk4 = false;
			}

			if (attackAngle4 <= -45&&isHammerBottom == false) {
				isHammerTop = true;
				attackAngle4 = attackAngle4 + atkIncrement2;
			}

			if (attackAngle4 >= 0 && isHammerTop == true) {
				isHammerBottom = true;
				attackAngle4 = attackAngle4 - atkIncrement2;
			}


		

			glPushMatrix();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.9f, 1.80f, 0.55f);
			TextureSphere(0.50f, 30.0f, 30.0f, "pokeball.bmp");
			glPopMatrix();


			glPushMatrix();
			glTranslatef(-2.5, 0.15, 0.65);
			/*glPushMatrix();*/
			glRotatef(90.0, 1.00, 0.0, 0.0);
			glRotatef(90.0, 0.00, 1.00, 0.0);
			glRotatef(45.0, 0.00, 1.00, 0.0);
			//glPushMatrix();
			glTranslatef(0.0, 0.0, -1.0);
			TextureCylinderORCorn(0.15, 0.15, 1.75, 20, 25, "handle.bmp");
			glPopMatrix();

			glPushMatrix();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glTranslatef(-1.0f, 2.5f, 0.25f);
			glRotatef(45, 0.0f, 1.0f, 1.0f);
			glTranslatef(-0.5, 0.0, 0.45f);
			TextureCubeORRectangle(2.0f, 1.0f, 0.55f, "hammermetal.bmp");
			glPopMatrix();
			glPopMatrix();
		}
		changeWeapon = false;
	}
}


void drawFinger() {

	//top is lower finger btm is upper finger
	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.3f, -1.2f, 0.31f);
	TextureCubeORRectangle(0.10f, 0.25f, 0.10f, "gold_metal.bmp");
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.4f, -1.2f, 0.43f);
	TextureCubeORRectangle(0.25f, 0.25f, 0.10f, "gold_metal.bmp");
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.5f, -1.2f, 0.54f);
	TextureCubeORRectangle(0.30f, 0.25f, 0.10f, "gold_metal.bmp");
	glPopMatrix();




	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.4f, -1.2f, 0.67f);
	TextureCubeORRectangle(0.25f, 0.25f, 0.10f, "gold_metal.bmp");
	glPopMatrix();



	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.4f, -1.2f, 0.78f);
	TextureCubeORRectangle(0.20f, 0.25f, 0.10f, "gold_metal.bmp");
	glPopMatrix();



	glPushMatrix();



	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.425f, -1.2f, 0.31f);
	TextureCubeORRectangle(0.15f, 0.25f, 0.10f, "metal.bmp");
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.65f, -1.2f, 0.43f);
	TextureCubeORRectangle(0.25f, 0.25f, 0.10f, "metal.bmp");
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.7f, -1.2f, 0.54f);
	TextureCubeORRectangle(0.30f, 0.25f, 0.10f, "metal.bmp");
	glPopMatrix();



	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.65f, -1.2f, 0.67f);
	TextureCubeORRectangle(0.25f, 0.25f, 0.10f, "metal.bmp");
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-1.6f, -1.2f, 0.78f);
	TextureCubeORRectangle(0.20f, 0.25f, 0.10f, "metal.bmp");
	glPopMatrix();
	glPopMatrix();

}

void drawWholeBody() {

	drawBody();
	drawLeftHand();
	drawRightHand();

}


void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	glClearColor(255.0f, 222 / 255.0f, 95 / 255.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and depth buffer
	glEnable(GL_DEPTH_TEST);


	if (timerCountDown = true) {
		ultimateTimer--;
		if (ultimateTimer <= 0)
		{
			timerCountDown = false;
		}
	}

	//gluPerspective(90.0, 1.0 , -1.0, 2.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);

	if (changeCamera == true) {
		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();
		if (orThoView == 1) {
			//glMatrixMode(GL_PROJECTION);
			//glLoadIdentity();
			glOrtho(viewLeft, viewRight, bottom, top, nearOrtho, farOrtho); //(X1, X2, Y1. Y2, Z1, Z1)
			changeCamera = false;
		}
		else if (perspectiveView == 1) {
			//glMatrixMode(GL_PROJECTION);
			//glLoadIdentity();
			gluPerspective(90.0, 1.0, -1.0, 2.0);

			glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);
			changeCamera = false;
		}
	}

	if (backgroundStatus == 0) {

		glPushMatrix();
		drawBackground("pokeballclose.bmp");
		glPopMatrix();

	}

	if (backgroundStatus == 1) {

		glPushMatrix();
		drawBackground("pokeballopen.bmp");
		glPopMatrix();

	}

	if (ultimateTimer <= 0 && backgroundStatus == 1) {
		 
			backgroundStatus = 2;
			//sounds->playSoundTrack();
		}


	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);

	if (backgroundStatus == 2) {
		//glPushMatrix();
		//drawBackground("galaxy.bmp");
		//glPopMatrix();

		glPushMatrix();
		if (perspectiveView == 1) {
			glTranslatef(0.0, 0.0, z);
		}
		else if (orThoView == 1) {
			glRotatef(rotateCam2, 1.0, 0.0, 0.0);

			glRotatef(rotateCam, 0.0, 1.0, 0.0);
		}
		drawHead();
		drawWholeBody();


		glPopMatrix();

	}


	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800,
		NULL, NULL, wc.hInstance, NULL);

	//WS = window style
	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));



	//sounds = new AudioManager;
	//sounds->initSound();
	//sounds->load();
	//sounds->playSoundTrack();


	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	//gluPerspective(90.0, 1.0 , -1.0, 2.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);



	//if (orThoView == 1) {
	//	//glMatrixMode(GL_PROJECTION);
	//	//glLoadIdentity();
	glOrtho(viewLeft, viewRight, bottom, top, nearOrtho, farOrtho); //(X1, X2, Y1. Y2, Z1, Z1)
//}
//else if (perspectiveView == 1) {
//	//glMatrixMode(GL_PROJECTION);
//	//glLoadIdentity();
//	gluPerspective(90.0, 1.0, -1.0, 2.0);
//	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);
//}


////	gluPerspective(fovy, aspect, near, far);
//	gluPerspective(60.0, 1.0, 1.0, 5.0);

//////	glFrustum(xmin, xmax, ymin, ymax, -zmin, -zmax);
//	glFrustum(-3.0, 3.0, -3.0, 3.0, 3.0, 5.0); //(make it bigger in z-axis 1-10)




	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------