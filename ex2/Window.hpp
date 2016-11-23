/* ----------------------------------------------------------------
   name:           Window.hpp
   purpose:        GLUT (sub-) window class declaration
   version:	   SKELETON CODE
   TODO:           nothing (see Window.cpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <iostream>

#include "GLIncludes.hpp"

#include "glm/glm.hpp"

class Window{

public:

  // constructors
  Window();
  Window(Window *parent, std::string title, glm::vec2 position, glm::vec2 size);

  // make window current
  void makeCurrent(void);
  // is current?
  bool isCurrent(void);

  // redisplay window
  void redisplay(void);

  std::string getTitle(void);

  // get size
  glm::vec2 size();
  // get width
  float width();
  // get height
  float height();

  // reshape window
  void reshape(int width, int height);
  // reposition window
  void reposition(int x, int y);

private:

  // window id returned from glut
  GLuint id;
  // window title 
  std::string title;

  //--------------#CHANGED-----------------
  // parent window
  Window *parent;
  //--------------#CHANGED-----------------
};
