#pragma once

/* ----------------------------------------------------------------
   name:           Context.hpp
   purpose:        GL context creation and handling, prototypes of GLUT callbacks
   version:	   SKELETON CODE
   TODO:           nothing (see Context.cpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include <string>

#include "glm/glm.hpp"

/* ------------------------------------------------ 
   implemented glut callbacks:
   display            - display scene
   reshape            - reshape window
   keyPressed         - keyboard callbacks
   menu               - right mouse button menu
   mouseMoved         - apply rotation when dragging
   mousePressed       - register mouse clicks
   registerCallbacks  - register all callbacks with GLUT
   ------------------------------------------------ */

class Context{

public:
  
  // intialization                                             
  static void init(int argc, char **argv);

  static void loop();

  static void addMenu(std::string text[], unsigned int options[], size_t numOptions);
  
protected:

  //register callbacks with GLUT
  static void registerCallbacks(void);

  // display scene
  // register with glutDisplayFunc()
  static void display(void);
 
  // redisplay scene after window reshape
  // register with glutReshapeFunc()
  static void reshape(int width, int height);

  // mouse callback
  // register with glutMouseFunc()
  static void mousePressed(int btn, int state, int x, int y);

  // keyboard callback
  // register with glutKeyboardFunc()
  static void keyPressed(unsigned char key, int x, int y);
 
  // keyboard callback for special keys 
  // register with glutSpecialFunc()
  static void specialKeys(int key, int x, int y);

  // mouse motion callback
  // register with glutMotionFunc()
  static void mouseMoved(int x, int y);
 
  // mouse menu                                                                
  // register with glutCreateMenu()
  // attach on right mouse button with glutAttachMenu();
  static void menu(int id);

  // window size                                                        
  static glm::vec2 windowSize;

  // initial window position                                                   
  static glm::vec2 windowPosition;
                  
  // window title                                                              
  static std::string title;

  // left mouse button pressed?                                                
  static bool leftButton;

  // mouse position in previous frame                                          
  static glm::vec2 mouse;
};
