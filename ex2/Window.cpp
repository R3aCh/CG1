/* ----------------------------------------------------------------
   name:           Window.cpp
   purpose:        GLUT (sub-) window implementation
   version:	   SKELETON CODE
   TODO:           constructor
   author:         katrin lang
		   computer graphics
		   tu berlin
   ------------------------------------------------------------- */

#include "Window.hpp"

using namespace std;
using namespace glm;

// Dummy constructor
Window::Window(){}

Window::Window(Window *parent, string title, vec2 position, vec2 size) :

  title(title){

  glutInitWindowPosition(position.x, position.y);
  glutInitWindowSize(size.x, size.y);

  // id = glutCreateWindow(title.c_str());
  //--------------#CHANGED-----------------
  //this->parent = parent;
  if (parent != NULL){
	  // get parent window position
	  id = glutCreateSubWindow(parent->id, position.x, position.y, size.x, size.y);
  }
  else{
	  id = glutCreateWindow(title.c_str());
  }
  //--------------#CHANGED-----------------
}

// get width
float Window::width(){

  return glutGet(GLUT_WINDOW_WIDTH);
}

// get height
float Window::height(){

  return glutGet(GLUT_WINDOW_HEIGHT);
}

// get size
vec2 Window::size(){

  return vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// make window current
void Window::makeCurrent(void){

  glutSetWindow(id);
}

// is current?
bool Window::isCurrent(void){

  return glutGetWindow()==id;
}

string Window::getTitle(void){
  return title;
}

// reshape window
void Window::reshape(int width, int height){

  glutReshapeWindow(width, height);
  glutPostRedisplay();
}

// reposition window
void Window::reposition(int x, int y){

  glutPositionWindow(x, y);
}
