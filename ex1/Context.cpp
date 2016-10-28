/* ----------------------------------------------------------------
   name:           Context.cpp
   purpose:        GL context creation and handling, glut callbacks
   version:	   SKELETON CODE
   TODO:           addMenu, registerCallbacks
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream> 

#include "Context.hpp"
#include "Control.hpp"
#include "RotationMode.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

// a bunch of variables
// window dimensions
vec2 Context::windowSize= vec2(600,600);
// initial window position
vec2 Context::windowPosition= vec2(100,100);
// window title
string Context::title= "cg1 assignment 1 - robot scenegraph";
// left mouse button pressed?
bool Context::leftButton= false;
// mouse position in previous frame
vec2 Context::mouse= vec2(0,0);

// set parameters to your own liking 
// (or leave them as they are)

void Context::init(int argc, char **argv){

  // create window with glut
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize(windowSize.x, windowSize.y);
  glutInitWindowPosition(windowPosition.x, windowPosition.y);
  glutCreateWindow(title.c_str());

  // register callbacks
  registerCallbacks();
}

// display callback for GLUT
void Context::display(void){
  
  // clear color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  Control::display();
}

// reshape-Callback for GLUT
void Context::reshape(int w, int h){

  windowSize= vec2(w,h);
  
  Control::reshape(windowSize);
}

// keyboard callback
void Context::keyPressed(unsigned char key, int x, int y){
	
  if(Control::keyPressed(key)) glutPostRedisplay();
}

// keyboard callback for special keys
// (arrow keys for node selection)
void Context::specialKeys(int key, int x, int y){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(key) {

  case GLUT_KEY_UP:
    Control::up();
    glutPostRedisplay();
    break;
  case GLUT_KEY_DOWN:
    Control::down();
    glutPostRedisplay();
    break;
  case GLUT_KEY_LEFT:
    Control::left();
    glutPostRedisplay();
    break;
  case GLUT_KEY_RIGHT:
    Control::right();
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

// mouse pressed callback
void Context::mousePressed(int button, int state, int x, int y){
  
  if (button == GLUT_LEFT) {
    if (state == GLUT_UP) {
      leftButton= false;
    }
    else if (state == GLUT_DOWN) {
      
      leftButton= true; 
      mouse= vec2(x, y);     
      
      if(Control::mousePressed(mouse, windowSize)) glutPostRedisplay();
    }
  }
}


// mouse motion callback
void Context::mouseMoved(int x, int y){

  // rotate selected node when left mouse button is pressed
  if(leftButton){
    if(x==mouse.x && y==mouse.y) return;
    if(Control::mouseDragged(mouse, vec2(x,y), windowSize)) glutPostRedisplay();
    mouse= vec2(x,y);
  }
}

// mouse menu callback
void Context::menu(int id){

  if(Control::menu(id)) glutPostRedisplay();
}

// start GLUT event loop
void Context::loop(){

  glutMainLoop();
}

// register callbacks with GLUT
void Context::registerCallbacks(void){
  
  glutDisplayFunc(display);
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeys);
  glutReshapeFunc(reshape);
  glutMotionFunc(mouseMoved);
  glutMouseFunc(mousePressed);

  // XXX: register menu callback and attach to right mouse button

  glutCreateMenu(menu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // END XXX
  
  return;
}

void Context::addMenu(std::string text[], unsigned int options[], size_t numOptions){

  for(int i= 0; i<numOptions; i++){
  
    // XXX: add menu entry
    
    glutAddMenuEntry(text[i].c_str(), options[i]);

    // END XXX
    
  }
}
