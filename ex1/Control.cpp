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

#define _USE_MATH_DEFINES
#include <cmath>

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
string Control::menuText[]= {"quit", "reset", "Euler rotation", "Trackball rotation"};
unsigned int Control::menuOptions[]= {1, 2, 3, 4};

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

enum RotationMode Control::rotationMode;

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

  // enable transparency support
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
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

    case 'r':
    case 'R':
      sceneGraph->reset();
      redisplay= true;
      break;

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

  case 2:
    sceneGraph->reset();
    break;

    // END XXX

    
    // XXX: add more options (optional)

  case 3:
    rotationMode = EULER;
    break;

  case 4:
    rotationMode = TRACKBALL;
    break;

    
    // END XXX

  default:
    break;
  }

  return redisplay;
}


// mouse pressed control
bool Control::mousePressed(vec2 mouse, vec2 windowSize){
  unsigned char index;
  glReadPixels(mouse.x, windowSize.y - mouse.y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &index);
  sceneGraph->selectById(index);
  return true; //always redisplay
}

// Sources:
// http://www.gukewen.sdu.edu.cn/panrj/courses/4-AngelCGE2-Virtual-Trackball.pdf
// http://nehe.gamedev.net/tutorial/arcball_rotation/19003/
static void mousePositionToSpherePoint(float x, float y, float width, float height, vec3 &v) {
  float square_len;

  v.x = (2.0f*x - width) / width;
  v.y = (height - 2.0f*y) / height;
  square_len = v.x*v.x + v.y*v.y;

  if (square_len > 1.0f) {
    float norm = 1.0f / sqrt(square_len);
    v.x *= norm;
    v.y *= norm;
    v.z = 0.0f;
  }
  else
    v.z = sqrt(1.0f - square_len);
}

// Stolen and adapted from this tutorial:
// http://nehe.gamedev.net/tutorial/arcball_rotation/19003/
void quaterionToEulerianAngle(const vec4& q, float& pitch, float& roll, float& yaw)
{
  float ysqr = q.y * q.y;
  float t0 = -2.0f * (ysqr + q.z * q.z) + 1.0f;
  float t1 = +2.0f * (q.x * q.y - q.w * q.z);
  float t2 = -2.0f * (q.x * q.z + q.w * q.y);
  float t3 = +2.0f * (q.y * q.z - q.w * q.x);
  float t4 = -2.0f * (q.x * q.x + ysqr) + 1.0f;

  t2 = t2 > 1.0f ? 1.0f : t2;
  t2 = t2 < -1.0f ? -1.0f : t2;

  pitch = std::asin(t2);
  roll = std::atan2(t3, t4);
  yaw = std::atan2(t1, t0);
}

// mouse dragged control
bool Control::mouseDragged(vec2 previousMouse, vec2 mouse, vec2 windowSize){

  if (rotationMode == EULER)
    sceneGraph->rotate(mouse.y-previousMouse.y, mouse.x-previousMouse.x, 0);
  else {
    // transform mouse coordinates into points on our virtual trackball sphere
    vec3 previousPos, pos;
    mousePositionToSpherePoint(previousMouse.x, previousMouse.y, windowSize.x, windowSize.y, previousPos);
    mousePositionToSpherePoint(mouse.x, mouse.y, windowSize.x, windowSize.y, pos);

    // our rotation axis and angle
    vec3 axis = cross(previousPos, pos);

    // fill a quaternion with the rotation axis and the rotation angle
    vec4 quat;
    quat.x = axis.x;
    quat.y = axis.y;
    quat.z = axis.z;
    quat.w = dot(previousPos, pos);

    // now transform the quaternion into Euler angles
    float pitch, roll, yaw;
    quaterionToEulerianAngle(quat, pitch, roll, yaw);

    // apply the rotation to the currently selected object
    sceneGraph->rotate(true ? -roll * 180.0f/M_PI : 0.0f,  true ? -pitch * 180.0f/M_PI : 0.0f, true ? -yaw * 180.0f/M_PI : 0.0f);
  }

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
