/* ----------------------------------------------------------------
   name:           Context.cpp
   purpose:        GL context creation and GLUT callbacks
   version:	   SKELETON CODE
   TODO:           Context::createWindows() Context::reshape() Context::redisplayAll() mouseMoved()
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include <iostream>

#include <vector>
// for pair
#include <utility>

#include "Projection.hpp"
#include "Context.hpp"

#include "GLIncludes.hpp"

#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

// gap between subwindows
const int Context::GAP= 25;

// initial window position
const vec2 Context::POSITION(100, 100);

// size of (upper) subwindows
const vec2 Context::SIZE(256, 256);

// window title
const std::string Context::TITLE= "cg1 assignment 2 - projection";

// mouse position and mouse pressed info
Context::Mouse Context::mouse;

// glut bitmap font 
// see setFont
GLvoid * Context::font= GLUT_BITMAP_HELVETICA_10;

Window Context::Main::window, Context::World::window, Context::Screen::window, Context::Command::window;

void Context::createWindows(void){

  Main::window= Window(NULL, TITLE, POSITION, vec2(2*SIZE.x+3*GAP, 2*SIZE.y+3*GAP));
  glutDisplayFunc(Projection::display);
  glutReshapeFunc(Context::reshape);
  glutKeyboardFunc(Projection::keyPressed);

  World::window= Window(&Main::window, "World-space view", vec2(GAP, GAP), SIZE);
  glutDisplayFunc(Projection::World::display);
  glutKeyboardFunc(Projection::keyPressed);
  glutCreateMenu(Projection::World::menu);
  vector< pair < int, string > > menuEntries= Projection::World::getMenuEntries();
  for(int i= 0; i<menuEntries.size(); i++) glutAddMenuEntry(menuEntries[i].second.c_str(), menuEntries[i].first);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  Screen::window= Window(&Main::window, "Screen-space view", vec2(SIZE.x+2*GAP, GAP), SIZE);
  glutDisplayFunc(Projection::Screen::display);
  glutKeyboardFunc(Projection::keyPressed);
  glutCreateMenu(Projection::Screen::menu);
  menuEntries= Projection::Screen::getMenuEntries();
  for(int i= 0; i<menuEntries.size(); i++) glutAddMenuEntry(menuEntries[i].second.c_str(), menuEntries[i].first);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  Command::window= Window(&Main::window, "Command manipulation window", vec2(GAP, SIZE.y+2*GAP), vec2(2*SIZE.x+GAP, SIZE.y));
  glutDisplayFunc(Projection::Command::display);
  glutMouseFunc(Context::mouseButton);
  glutMotionFunc(Context::mouseMoved);
  glutKeyboardFunc(Projection::keyPressed);
  glutCreateMenu(Projection::Command::menu);
  menuEntries= Projection::Command::getMenuEntries();
  for(int i= 0; i<menuEntries.size(); i++) glutAddMenuEntry(menuEntries[i].second.c_str(), menuEntries[i].first);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Context::init(int argc, char **argv){

  // create window with glut
  glutInit(&argc, argv);

  createWindows();

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
}

void Context::redisplayAll(void){

  World::window.makeCurrent();
  glutPostRedisplay();
  Screen::window.makeCurrent();
  glutPostRedisplay();
  Command::window.makeCurrent();
  glutPostRedisplay();
}

// select glut bitmap font
void Context::setFont(string name, int size){  
  
  if(name.compare("helvetica") == 0){
    if(size == 10)
      font= GLUT_BITMAP_HELVETICA_10;
    else if(size == 12) 
      font= GLUT_BITMAP_HELVETICA_12;
    else if(size == 18)
      font= GLUT_BITMAP_HELVETICA_18;
  } 
  else if(name.compare("times") == 0){
    if(size == 10)
      font= GLUT_BITMAP_TIMES_ROMAN_10;
    if(size == 24)
      font= GLUT_BITMAP_TIMES_ROMAN_24;
  } 
  else if(name.compare("8x13") == 0){
    font= GLUT_BITMAP_8_BY_13;
  }
  else if(name.compare("9x15") == 0) {
    font= GLUT_BITMAP_9_BY_15;
  }
}

// draw a string in 2D
void Context::drawString(float x, float y, string s){
  glRasterPos2f(x, y);
  for (size_t i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}

// draw a string in 3D
void Context::drawString(float x, float y, float z, string s){
  glRasterPos3f(x, y, z);
  for (size_t i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}

// reshape-Callback for GLUT
void Context::reshape(int width, int height){

  Projection::reshape(width, height);

  float subWidth= (width-3*GAP)/2.0f;
  float subHeight= (height-3*GAP)/2.0f;
  
  World::window.makeCurrent();
  World::window.reposition(GAP, GAP);
  World::window.reshape(subWidth, subHeight);
  Projection::World::reshape(subWidth, subHeight);

  Screen::window.makeCurrent();
  Screen::window.reposition(subWidth+2*GAP, GAP);
  Screen::window.reshape(subWidth, subHeight);
  Projection::Screen::reshape(subWidth, subHeight);

  Command::window.makeCurrent();
  Command::window.reposition(GAP, subHeight+2*GAP);
  Command::window.reshape(width-2*GAP, subHeight);
  Projection::Command::reshape(width-2*GAP, subHeight);
}

void Context::drawTitles(void){

  float subWidth= (Main::window.width()-3*GAP)/2.0f;
  float subHeight= (Main::window.height()-3*GAP)/2.0f;

  setFont("helvetica", 12);
  
  drawString(GAP, GAP-5, World::window.getTitle().c_str());

  drawString(GAP+subWidth+GAP, GAP-5, Screen::window.getTitle());
  
  drawString(GAP, subHeight+2*GAP-5, Command::window.getTitle());
}
  
// mouse button callback
void Context::mouseButton(int button, int state, int x, int y){

  if(state == GLUT_DOWN){    
    mouse.pressed= true;
    if(Command::window.isCurrent()) Projection::Command::mousePressed(vec2(x,y));
  }
  else if(state == GLUT_UP){
    mouse.pressed= false;
    if(Command::window.isCurrent()) Projection::Command::mouseReleased(vec2(x,y));
  }
    
  mouse.position= vec2(x,y);
}

// command mouse moved callback
void Context::mouseMoved(int x, int y){

  if(mouse.pressed){
    if(Command::window.isCurrent()) Projection::Command::mouseDragged(mouse.position, vec2(x,y));
  }
 
  mouse.position= vec2(x,y);
}
