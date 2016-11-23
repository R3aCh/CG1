/* ----------------------------------------------------------------
   name:           Context.hpp
   purpose:        GL context class declaration, prototypes of GLUT callbacks
   version:	   SKELETON CODE
   TODO:           nothing (see context.cpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <string>

#include "glm/glm.hpp"

#include "Window.hpp"

class Context{

public:

  // intialization                                             
  static void init(int argc, char **argv);

  // select glut bitmap font
  static void setFont(std::string name, int size);

  // draw a string on the screen
  static void drawString(float x, float y, std::string s);
  static void drawString(float x, float y, float z, std::string s);

  // draw window titles above windows
  static void drawTitles(void);
  
  // redisplay all windows
  static void redisplayAll(void);

private:

  class Main{ public: static Window window; };
  
  class World{ public: static Window window; };

  class Screen{ public: static Window window; };

  class Clip{ public: static Window window; };

  class Command{ public: static Window window; };
  
  // create windows
  static void createWindows(void);
  
  // reshape callback
  static void reshape(int width, int height);

  // mouse pressed/released callback
  static void mouseButton(int button, int state, int x, int y);
  
  // mouse motion callback
  static void mouseMoved(int x, int y);
  
  // window title                                                              
  static const std::string TITLE;

  // initial window position;
  static const glm::vec2 POSITION;

  // initial window size;
  static const glm::vec2 SIZE;
  
  struct Mouse{
    glm::vec2 position;
    bool pressed;
  };
  
  //previous mouse positions
  static Mouse mouse;

  static const int GAP;
  
  static GLvoid * font;

  static Window window;
};
