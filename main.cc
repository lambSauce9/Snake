#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

#include <iostream>
#include <string>
#include <sstream>

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "image.h"
#include "snake.h"

Snake* player;


double minX = -1.0, maxX = 1.0;
double minY = -1.0, maxY = 1.0;


double width = 640;
double height = 480;



double unitSize = 10;

int unitsPerRow = width / unitSize;
int unitsPerCol = height / unitSize;



int limitX = unitsPerRow;
int limitY = unitsPerCol;





int score = 0;
int scoreMultiplier = 1;


double timerTick = 65;
double timerMultiplier = 0.8;
double speed = 1.0;


int dirX = 1;
int dirY = 0; 


bool showMap = false;


int snakeShape = 0;


static GLuint texName[36];


typedef enum {
    SPEED1, SPEED2, SPEED3,
    PERSP1, PERSP2, SNAKE1, SNAKE2, SALIR
} opcionesMenu;

bool showSplashScreen = true;



int appleX, appleY;


int appleAngle = 0;




int crece = 0;



void loadTexture(Image* image, int k) {
    glBindTexture(GL_TEXTURE_2D, texName[k]); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,               
        0,                           
        GL_RGB,                      
        image->width, image->height, 
        0,                          
        GL_RGB,                      
        GL_UNSIGNED_BYTE,            
                                    
        image->pixels);              
}


Image* loadBMP(const char* filename);

void onMenu(int opcion) {
    switch (opcion) {

        
    case SPEED1:
        speed = 1.0;
        break;
    case SPEED2:
        speed = 2.0;
        break;
    case SPEED3:
        speed = 3.0;
        break;

        
    case PERSP1:
        showMap = false;
        break;
    case PERSP2:
        showMap = true;
        break;

       
    case SNAKE1:
        snakeShape = 0;
        break;
    case SNAKE2:
        snakeShape = 1;
        break;

        
    case SALIR:
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void initMenu(void) {
    int menuVelocidad, menuPerspectiva, menuSerpiente, menuPrincipal;

    menuVelocidad = glutCreateMenu(onMenu);
    glutAddMenuEntry("Principiante", SPEED1);
    glutAddMenuEntry("Intermedio", SPEED2);
    glutAddMenuEntry("Avanzado", SPEED3);

    menuPerspectiva = glutCreateMenu(onMenu);
    glutAddMenuEntry("3D", PERSP1);
    glutAddMenuEntry("2D", PERSP2);

    menuSerpiente = glutCreateMenu(onMenu);
    glutAddMenuEntry("Esférica", SNAKE1);
    glutAddMenuEntry("Cubica", SNAKE2);

    menuPrincipal = glutCreateMenu(onMenu);
    glutAddSubMenu("Nivel", menuVelocidad);
    glutAddSubMenu("Perspectiva (2D/3D)", menuPerspectiva);
    glutAddSubMenu("Serpiente", menuSerpiente);
    glutAddMenuEntry("Salir", SALIR);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



static void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_FLAT);

    player = new Snake(unitsPerRow / 2, unitsPerCol / 2, 100);            


    srand((int)time(NULL));

    
    appleX = rand() % unitsPerRow + 1;
    appleY = rand() % unitsPerCol + 1;

   
    initMenu();

    
    glGenTextures(2, texName);
    Image* image;

    image = loadBMP("C:/Users/Thanh Tung/Desktop/Project3/snake.bmp");
    loadTexture(image, 0);

    image = loadBMP("C:/Users/Thanh Tung/Desktop/Project3//apple.bmp");
    loadTexture(image, 1);

    delete image;
}



double xPos2d(int x) {
    double wide = maxX - minX;
    double mappedX = x * (wide / unitsPerRow);

    return minX + mappedX;
}


double yPos2d(int y) {
    double tall = maxY - minY;
    double mappedY = y * (tall / unitsPerCol);

    return minY + mappedY;
}


void drawString(void* font, const char* s, float x, float y) {
    unsigned int i;
    glRasterPos2f(x, y);

    for (i = 0; i < strlen(s); i++)
        glutBitmapCharacter(font, s[i]);
}

void draw3dString(void* font, const char* s, float x, float y, float z) {
    unsigned int i;

    glPushMatrix();

    glTranslatef(x, y, z);
    glScaled(0.0005, 0.0005, 0.0005);

    for (i = 0; i < strlen(s); i++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);

    glPopMatrix();
}

void drawSplashScreen() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 225, 225);
    glLineWidth(1);

    std::stringstream ss; 

    //Co viet them gi o man hinh thi viet o day nhe

    ss << "COMPUTER GRAPHIC PROJECT";
    drawString(GLUT_BITMAP_8_BY_13, ss.str().c_str(), -1.5, 1.08);
    ss.str("");
    ss.clear();

    ss << "W A S D to moving";
    drawString(GLUT_BITMAP_8_BY_13, ss.str().c_str(), -1.5, 1.0);
    ss.str("");
    ss.clear();

    ss << "E to end program";
    drawString(GLUT_BITMAP_8_BY_13, ss.str().c_str(), -1.5, 0.92);
    ss.str("");
    ss.clear();

    ss << "M to switch to 2DView";
    drawString(GLUT_BITMAP_8_BY_13, ss.str().c_str(), -1.5, 0.68);
    ss.str("");
    ss.clear();

    ss << "ENTER TO START";
    drawString(GLUT_BITMAP_8_BY_13, ss.str().c_str(), -1.5, 0.60);
    ss.str("");
    ss.clear();
}

static void drawMap(void) {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    
    glColor3f(0.0, 225, 225);
    glLineWidth(1);

    glBegin(GL_LINES);

    for (int i = 0; i <= unitsPerRow; i += 2) {
        glVertex2d(minX, xPos2d(i));
        glVertex2f(maxX, xPos2d(i));
        glVertex2d(xPos2d(i), minY);
        glVertex2f(xPos2d(i), maxY);
    }

    glEnd();

    
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    glVertex2f(minX, minY);
    glVertex2f(minX, maxY);
    glVertex2f(maxX, maxY);
    glVertex2f(maxX, minY);
    glEnd();

    glLineWidth(1);

    
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(6);

    glBegin(GL_POINTS);
    glVertex2f(xPos2d(appleX), yPos2d(appleY));
    glEnd();

    glPointSize(1);

   
    glColor3f(1.0, 1.0, 0.0);
    glLineWidth(unitSize);

    glBegin(GL_LINE_STRIP);
    for (int i = player->length - 1; i >= 0; i--) {
        glVertex2f(xPos2d(player->xAt(i)), yPos2d(player->yAt(i)));
    }
    glEnd();

    glLineWidth(1);

   
    glColor3f(1.0, 1.0, 1.0);

    std::stringstream ss; 
    ss << "Score: " << std::to_string(score);
    drawString(GLUT_BITMAP_9_BY_15, ss.str().c_str(), -0.85, -0.85);
}

static void drawPerspective(void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat ambientLight[] = { 0.5f, 0.4f, 0.4f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    GLfloat directedLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat directedLightPos[] = { -10.0f, 15.0f, 20.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
    glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

   
    glColor3f(0, 225, 225);
    glLineWidth(1);

    glBegin(GL_LINES);

    for (double i = 0; i <= unitsPerRow; i += 2) {
        glVertex2d(minX, xPos2d(i));
        glVertex2f(maxX, xPos2d(i));
        glVertex2d(xPos2d(i), minY);
        glVertex2f(xPos2d(i), maxY);
    }

    glEnd();

   
    glColor3f(0.6, 0.0, 0.6);

    glPushMatrix();
    glTranslated(maxX + 0.05, 0.0, 0.0);
    glScaled(1.0, 1.0 * 41, 1.0);
    glutSolidCube(0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslated(minX - 0.05, 0.0, 0.0);
    glScaled(1.0, 1.0 * 41, 1.0);
    glutSolidCube(0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, minY - 0.05, 0.0);
    glScaled(1.0 * 43, 1.0, 1.0);
    glutSolidCube(0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, maxY + 0.05, 0.0);
    glScaled(1.0 * 43, 1.0, 1.0);
    glutSolidCube(0.05);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

    
    glColor3f(1.5, 0.0, 0.0);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glBindTexture(GL_TEXTURE_2D, texName[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPushMatrix();
    glTranslated(xPos2d(appleX), yPos2d(appleY), 0.025);
    glRotated(appleAngle, 0.3, 1.0, 0.0);
    glutSolidCube(0.05);
    glPopMatrix();

    
    glColor3f(0.8, 1.0, 0.0);

    glBindTexture(GL_TEXTURE_2D, texName[0]);

    for (int i = player->length - 1; i >= 0; i--) {
        glPushMatrix();
        glTranslated(xPos2d(player->xAt(i)), yPos2d(player->yAt(i)), 0.025);

        if (snakeShape == 0) {
            glutSolidSphere(0.05, 10, 10);
        }
        else {
            glutSolidCube(0.05);
        }

        glPopMatrix();
    }

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);

   
    glColor3f(1.0, 1.0, 1.0);

    std::stringstream ss; 
    ss << "Score: " << std::to_string(score);
    draw3dString(GLUT_STROKE_MONO_ROMAN, ss.str().c_str(), -0.85, -0.85, 0.0);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
}

static void display(void) {
    double snakeX, snakeY;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (showSplashScreen) {

        gluLookAt(0.0, 0.0, 2.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

        drawSplashScreen();

    }
    else if (showMap) {

        gluLookAt(0.0, 0.0, 2.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

        drawMap();

    }
    else {

        snakeX = xPos2d(player->x());
        snakeY = yPos2d(player->y());

        snakeY = snakeY <= -0.5 ? -0.5 : snakeY;
        snakeY = snakeY >= 1.5 ? 1.5 : snakeY;

        gluLookAt(0.0, -2.0, 1,
            0.0, snakeY, 0.0,
            0.0, 0.5, 0.0);

        drawPerspective();

    }


    glutSwapBuffers();
}


bool snakeHits(float x, float y) {
    double nextX, nextY;

    nextX = player->x();
    nextY = player->y();

    return nextX == x && nextY == y;
}

void resetGame() {
    player->reset();
    dirX = 1;
    dirY = 0;
}

void myTimer(int valor) {
    int nextX, nextY;

    if (showSplashScreen) {
        glutTimerFunc(timerTick / speed, myTimer, 1);
        return;
    }

   
    if (dirX == 1 && player->x() >= unitsPerRow) {
        dirX = 0;
        dirY = 1;
    }
    else if (dirX == -1 && player->x() <= 0) {
        dirX = 0;
        dirY = -1;
    }
    else if (dirY == 1 && player->y() >= unitsPerCol) {
        dirY = 0;
        dirX = -1;
    }
    else if (dirY == -1 && player->y() <= 0) {
        dirY = 0;
        dirX = 1;
    }

    nextX = player->x() + dirX;
    nextY = player->y() + dirY;

    appleAngle = (appleAngle >= 360) ? 0 : appleAngle + 5;

  
    if (crece == 1 || snakeHits(appleX, appleY)) {

        
        score += (1 * scoreMultiplier * speed);

        if (!player->full()) {
            player->eat();
        }
        else {

            
            resetGame();

            
            scoreMultiplier++;

           
            timerTick *= timerMultiplier;
        }

        appleX = rand() % unitsPerRow + 1;
        appleY = rand() % unitsPerCol + 1;

        crece = 0;
    }

    if (!player->moveTo(dirX, dirY)) {
        resetGame();
        score = 0;
        scoreMultiplier = 1;
        speed = 1.0;
        timerMultiplier = 0.8;
    }

    glutPostRedisplay();
    glutTimerFunc(timerTick / speed, myTimer, 1);
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
   
    switch (theKey) {
        
    case 'w': case 'W':
        if (dirY != -1) { dirX = 0; dirY = 1; }
        break;
    case 's': case 'S':
        if (dirY != 1) { dirX = 0; dirY = -1; }
        break;
    case 'a': case 'A':
        if (dirX != 1) { dirX = -1; dirY = 0; }
        break;
    case 'd': case 'D':
        if (dirX != -1) { dirX = 1; dirY = 0; }
        break;

        
    case 'c': case 'C':
        crece = 1;
        break;

        
    case 'm': case 'M':
        showMap = !showMap;
        break;

        
    case 'v':
        speed *= 1.1;
        break;

        
    case 'V':
        speed *= 0.9;
        break;

    case 13:
        showSplashScreen = false;
        break;

        
    case 27: case 'e': case 'E':
        exit(-1);
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(120, 120);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Computer GRAPHIC PROJECT");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(myKeyboard);
    glutTimerFunc(2000, myTimer, 1);

    glutMainLoop();
    return EXIT_SUCCESS;
}