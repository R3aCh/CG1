#pragma once

/* ----------------------------------------------------------------
   name:           Control.hpp
   purpose:        robot control
   version:	   SKELETON CODE
   TODO:           nothing (see Control.cpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "glm/glm.hpp"

#include "SceneGraph.hpp"
#include "RotationMode.hpp"

class Control{

public:

  typedef struct {
    // ambient color
    glm::vec4 ambient;
    // diffuse color
    glm::vec4 diffuse;
    // specular color
    glm::vec4 specular;
    // shininess
    float shininess;
  } Material;

  typedef struct {
    //position in view space
    glm::vec4 position;
    // ambient color
    glm::vec4 ambient;
    // diffuse color
    glm::vec4 diffuse;
    // specular color
    glm::vec4 specular;
  } LightSource;
  
  // initialization
  static void init();
  
  // add a scenegraph
  static void addSceneGraph(SceneGraph *sceneGraph);

  // adjust to new window size
  static void reshape(glm::vec2 windowSize);
  
  // display robot
  static void display(void);

  // mouse pressed control
  static bool mousePressed(glm::vec2 mouse, glm::vec2 windowSize);

  // mouse dragged control
  static bool mouseDragged(glm::vec2 previousMouse, glm::vec2 mouse, glm::vec2 windowSize);
 
  // keyboard control
  static bool keyPressed(unsigned char key);
 
  // keyboard control for special keys 
  static void up();
  static void down();
  static void left();
  static void right();

  // mouse menu control                                                         
  static bool menu(int id);

  static enum RotationMode rotationMode;

protected:
  
  static Material material;
  static LightSource lightSource;
  
  // the scenegraph
  static SceneGraph *sceneGraph;

  static std::string menuText[];
  static unsigned int menuOptions[];

  // field of view (in degrees)                                               
  static GLfloat fov;

  // camera position                                                           
  static GLfloat cameraZ;

  // near and far plane                                                        
  static GLfloat nearPlane, farPlane;

};
