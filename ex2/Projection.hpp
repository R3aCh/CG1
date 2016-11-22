/* ----------------------------------------------------------------
   name:           Projection.hpp
   purpose:        projection tutorial - prototypes
   based on tutorial by Nate Robins, 1997
   modified for clip-space viewing in the context 
   of 'introduction to computer graphics' 
   winter term 2012/2013, assignment 2
   version:	   SKELETON CODE
   TODO:           clip-space view callbacks
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <string>

#include "Cell.hpp"
#include "Mesh.hpp"

class Projection{
  
public:

  enum ProjectionMode{
    PERSPECTIVE, FRUSTUM, ORTHO
  }; 
  
  //initialization
  static void init(void);

  static void reshape(int width, int height);

  static void display(void);
  
  static void keyPressed(unsigned char key, int x, int y);

  // content of world view sub-window
  class World{

    friend class Projection;
    
  public:

    // display scene
    static void display(void);
  
    // redisplay scene after window reshape
    static void reshape(int width, int height);

    // mouse menu
    static void menu(int id);

    // return menu entries
    static std::vector< std::pair < int, std::string > >  getMenuEntries(void);

  private:
    
    // draw mesh?
    static bool drawMesh;
  };
  
  //content of screen view sub-window
  class Screen{

    friend class Projection;
  
  public:
  
    // display scene
    static void display(void);
  
    // redisplay scene after window reshape
    static void reshape(int width, int height);

    // mouse menu
    static void menu(int id);

    // return menu entries
    static std::vector< std::pair < int, std::string > >  getMenuEntries(void);
  };

  
  // content of clip view sub-window
  class Clip{

    friend class Projection;
  
  public:

    // display scene
    static void display(void);
  
    // redisplay scene after window reshape
    static void reshape(int width, int height);
 
    // mouse motion callback
    static void mouseDragged(glm::vec2 previousMouse, glm::vec2 mouse);

    // mouse menu
    static void menu(int id);

    // return menu entries
    static std::vector< std::pair < int, std::string > >  getMenuEntries(void);

  private:

    static float angle;

    // draw mesh?
    static bool drawMesh;
  
    // draw clip planes?
    static bool drawClipPlanes;

    #ifdef EXTRA
    
    // correct normals?
    static bool correctNormals;

    // show normals
    static bool showNormals;

    #endif

    // additional clip planes
    static glm::dvec4 clipPlane1;
    static glm::dvec4 clipPlane2;
    static glm::dvec4 clipPlane3;
    static glm::dvec4 clipPlane4;
    static glm::dvec4 clipPlane5;
    static glm::dvec4 clipPlane6;

    // menu entries
    static std::vector < std::pair < int, std::string > >  menuEntries;
  };


  // content of command view sub-window
  class Command{

    friend class Projection;
  
  public:

    // adjust to new window size
    static void reshape(int width, int height);
  
    // display
    static void display(void);
  
    // mouse pressed
    static void mousePressed(glm::vec2 mouse);
 
    // mouse dragged
    static void mouseDragged(glm::vec2 previousMouse, glm::vec2 mouse);

    // mouse released
    static void mouseReleased(glm::vec2 mouse);
  
    // mouse menu
    static void menu(int id);

    // reset all cells to default
    static void reset(void);

    // set projection mode
    static void setProjectionMode(ProjectionMode mode);

    // compute view direction
    static glm::vec3 computeViewDirection(void);

    // return menu entries
    static std::vector< std::pair < int, std::string > >  getMenuEntries(void);

  private:

    // compute matrices from cell content
    static glm::mat4 computeViewMatrix(void);
    static glm::mat4 computeProjectionMatrix(void);

    static Cell lookat[];
    static Cell perspective[];
    static Cell frustum[];
    static Cell ortho[];

    static ProjectionMode projectionMode;
  };

private:

  static void drawAxes(void);
  
  static glm::mat4 projectionMatrix, viewMatrix;

  static Mesh mesh;
  
  static  glm::vec4 lightPosition;
};
