/* ----------------------------------------------------------------
   name:           Control.cpp
   purpose:        robot control 
   version:	   SKELETON CODE
   TODO:           menuText/menuOptions, menu, keyPressed
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream> 

#include "Control.hpp"
#include "Context.hpp"
#include "Robot.hpp"
#include "RotationMode.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

// TODO: add menu Option for reset
string Control::menuText[]= {"quit"};
unsigned int Control::menuOptions[]= {1};

// field of view (in degrees)
GLfloat Control::fov= radians(40.0);
// camera position
GLfloat Control::cameraZ;
// near and far plane
GLfloat Control::nearPlane; 
GLfloat Control::farPlane;

// set material parameters to your own liking 
// (or leave them as they are)

// material parameters
Control::Material Control::material= {vec4(0.5, 0.5, 0.5, 1.0),
				      vec4(0.3, 0.3, 0.3, 1.0),
				      vec4(0.5, 0.5, 0.5, 1.0),
				      3.0f};

// light parameters
Control::LightSource Control::lightSource= {vec4(5.0, 5.0, 5.0, 1.0),
					    vec4(0.3f, 0.3f, 0.3f, 1.0f),
					    vec4(1.0f, 1.0f, 1.0f, 1.0f),
					    vec4(1.0f, 1.0f, 1.0f, 1.0f)};

SceneGraph *Control::sceneGraph= NULL;

void Control::init(){

  Context::addMenu(menuText, menuOptions, sizeof(menuOptions)/sizeof(unsigned int));
   
  // light
  glLightfv(GL_LIGHT0, GL_POSITION, &lightSource.position[0]);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &lightSource.ambient[0]);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  //material
  glMaterialfv(GL_FRONT, GL_AMBIENT, &material.ambient[0]);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, &material.diffuse[0]);
  glMaterialfv(GL_FRONT, GL_SPECULAR, &material.specular[0]);
  glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
  
  // flat shading (if you want rounded limbs, set this to GL_SMOOTH)
  glShadeModel(GL_FLAT);
  
  // set background color to black
  glClearColor(0.0,0.0,0.0,1.0);
  
  // enable depth test (z-buffer)
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  
  // enable normalization of vertex normals
  glEnable(GL_NORMALIZE);
  
  // some output to console
  cout << "--------------------------------------------\n";
  cout << " cg1_ex1 opengl robot scenegraph            \n";
  cout << "                                            \n";
  cout << " keyboard:                                  \n";
  cout << " arrow keys: select node                    \n";
  cout << " x/X,y/Y,z/Z: rotate node                   \n";
  cout << " r: reset all rotations                     \n";
  cout << " q/Q: quit program                          \n";
  cout << "                                            \n";
  cout << " mouse:                                     \n";
  cout << " right click: config menu                   \n";
  cout << " left click+drag: rotate selected node      \n";
  cout << "--------------------------------------------\n";
}

// add a scenegraph
void Control::addSceneGraph(SceneGraph *sceneGraph){

  Control::sceneGraph= sceneGraph;
}

// adjust to new window size
void Control::reshape(vec2 windowSize){

  // reshaped window aspect ratio
  float aspect = (float) windowSize.x / (float) windowSize.y;
  
  // viewport
  glViewport(0,0, (GLsizei) windowSize.x, (GLsizei) windowSize.y);

  // compute z position of camera
  cameraZ= (windowSize.y/2.0f) / tan(fov/radians(180.0f));
  // compute near and far plane
  nearPlane=cameraZ/10.0f; 
  farPlane= cameraZ*10.0f;
  
  // restore view definition after window reshape
  glMatrixMode(GL_PROJECTION);
  // perspective projection
  glLoadMatrixf(&glm::perspective(fov, aspect, nearPlane, farPlane)[0][0]);
    
  // switch to opengl modelview matrix
  glMatrixMode(GL_MODELVIEW);
  // position the camera at (0,0,cameraZ) looking down the
  // negative z-axis at (0,0,0)
  glLoadMatrixf(&lookAt(vec3(0,0,cameraZ), vec3(0,0,0), vec3(0,1,0))[0][0]);
}

// this is where the drawing happens
void Control::display(void){
  
  // draw the scenegraph
  sceneGraph->traverse();
  
  // display back buffer
  glutSwapBuffers();
}


// keyboard control
bool Control::keyPressed(unsigned char key){

  // rotation units
  float step= 2.0;

  // redisplay Window?
  bool redisplay= false;
  
  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(key){
    
  case 'q':
  case 'Q': exit(0);
  case 'x':
    sceneGraph->rotate(step, 0, 0);
    redisplay= true;
    break;
  case 'X': 
    sceneGraph->rotate(-step, 0, 0);
    redisplay= true;
    break;
  case 'y':
    sceneGraph->rotate(0, step, 0);
    redisplay= true;
    break;
  case 'Y' :
    sceneGraph->rotate(0, -step, 0);
    redisplay= true;
    break;  
  case 'z':
    sceneGraph->rotate(0, 0, step);
    redisplay= true;
    break;
  case 'Z': 
    sceneGraph->rotate(0, 0, -step);
    redisplay= true;
    break;

    // XXX: reset rotations

    // INSERT YOUR CODE HERE

    
    // END XXX

  default:
    break;
  }
  
  return redisplay;
}

// keyboard functions for special keys
// (arrow keys for node selection)
void Control::up(){
  sceneGraph->up();
}

void Control::down(){
  sceneGraph->down();
}

void Control::left(){
  sceneGraph->left();
}

void Control::right(){
  sceneGraph->right();
}


// mouse menu control
// TODO: add a reset option
// for all rotations
// see also registerCallbacks()
// and addMenu() in Context.cpp.
// you may also add config options
// like selection  of different
// animations here (optional)
// XXX: NEEDS TO BE IMPLEMENTED
bool Control::menu(int id){

  // redisplay Window?
  bool redisplay= false;
  
  switch (id) {
  case 1: 
    delete sceneGraph;
    exit(0);
  
    // XXX: reset rotations

    // INSERT YOUR CODE HERE

    
    // END XXX

    
    // XXX: add more options (optional)

    // INSERT YOUR CODE HERE
    
    
    // END XXX

  default:
    break;
  }

  return redisplay;
}


// mouse pressed control
bool Control::mousePressed(vec2 mouse, vec2 windowSize){
  
  return true; //always redisplay
}

// mouse dragged control
bool Control::mouseDragged(vec2 previousMouse, vec2 mouse, vec2 windowSize){

  sceneGraph->rotate(mouse.y-previousMouse.y, mouse.x-previousMouse.x, 0);
  
  return true; //always redisplay
}

int main(int argc, char** argv){
    
  // initialize OpenGL context
  Context::init(argc, argv);

  // set OpenGL state
  Control::init();
  
  //  build the robot hierarchy (see robot.cpp)
  Node *root= Robot::buildRobot();
  //make scenegraph
  SceneGraph *sceneGraph= new SceneGraph(root);
  
  Control::addSceneGraph(sceneGraph);
  
  //start event loop
  Context::loop();
  
  return 0;
}
