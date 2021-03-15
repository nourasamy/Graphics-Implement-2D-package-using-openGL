/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#include<GL/gl.h>
//#include<GL/glut.h>
#include<stdio.h>
#endif
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;
static float Transformx = -2.4;
static float Transformy = -1.2;
static float TransformWheel=-1.9;
static float wheel1=-2.3;
static float wheel2=-1;


static int menu_id;
static int window;
static int submenu;
static int value=0;
static int h=0;
static int W=0;



void menu(int num){
  if(num == 0){
    glutDestroyWindow(window);
    exit(0);
  }else{
    value = num;
  }
  glutPostRedisplay();
}
void createmenu(void){
    submenu=glutCreateMenu(menu);
    glutAddMenuEntry("moving car", 1);
     menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Draw",submenu);
glutAttachMenu(GLUT_RIGHT_BUTTON);

}
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1, 2, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    h=height;
    W=width;
}

static void car(double a){
glPushMatrix();
    glTranslated(Transformx,Transformy,-6);
    glBegin(GL_POLYGON);
      glVertex2f(-0.7,-0.7);
      glVertex2f( 2.2,-0.7);
      glVertex2f( 2.2,0);
      glVertex2f(-0.7,0);
        glEnd();
    glPopMatrix();


  glPushMatrix();
        glTranslated(Transformx,Transformy,-6);
        glBegin(GL_LINE_LOOP);
      glVertex2f(-0.5,0);
      glVertex2f( 1.7,0);
      glVertex2f(1,0.6);
      glVertex2f(-0.1,0.6);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glTranslated(wheel1,TransformWheel,-6);
        glRotated(180,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus(0.03,0.3,16,16);
    glPopMatrix();

    glPushMatrix();
        glTranslated(Transformx,Transformy,-6);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0.6,0);
         glVertex2f(0.6,-0.7);
       glEnd();

    glPopMatrix();

    glPushMatrix();
        glTranslated(Transformx,Transformy,-6);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0.6,0.6);
        glVertex2f(0.6,0);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glTranslated(wheel2,TransformWheel,-6);
        //glRotated(60,1,0,0);
        glRotated(180,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus(0.03,0.3,16,16);
    glPopMatrix();
}

static void display(void)
{

const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
const double a = t*180.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(value==1){
    glColor3d(1,0,0);
    car(a);
    }

    glutSwapBuffers();
}


static void key( int key, int x, int y)
{
    switch (key)
    {

		case GLUT_KEY_UP:
            Transformx+=0.1;
            wheel1+=0.1;
            wheel2+=0.1;
              break;

        case GLUT_KEY_DOWN:
            Transformx-=0.1;
            wheel1-=0.1;
            wheel2-=0.1;
              break;
        case GLUT_KEY_LEFT:

            Transformy+=0.1;
            TransformWheel+=0.1;

              break;
         case GLUT_KEY_RIGHT:
            Transformy-=0.1;
            TransformWheel-=0.1;

              break;
    }

    glutPostRedisplay();
}

static void keyzoom( unsigned char KEY, int x, int y)
{
    switch (KEY)
    {
    case '+':
  resize(W+1, h+1);
   break;
   case '-':

    resize(W-1,h-1);
        break;

    }

    glutPostRedisplay();
}



static void idle(void)
{
    glutPostRedisplay();
}




const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(700,500);
    glutInitWindowPosition(150,50);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    window=glutCreateWindow("GLUT Shapes");
    createmenu();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyzoom);
    //glutMouseFunc(mouseClicks);
    glutSpecialFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


//glutCreateMenu(menu);
//glutReshapeFunc(resize);
   // glutDisplayFunc(display);
    glutMainLoop();

    return EXIT_SUCCESS;
}
