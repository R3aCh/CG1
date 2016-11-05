/* ----------------------------------------------------------------
   name:           Node.cpp
   purpose:        implementation for a scenegraph node
   version:	   SKELETON CODE
   TODO:           transform, drawJoint, reset
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream>

#include "Node.hpp"
#include "Context.hpp"
#include "Debug.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtc/type_ptr.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

unsigned char id_count = 1;

// constructor
// x,y,z: position of node center of mass relative to parent node center
// length, height, and width of this body part
// jointx, jointy, jointz: x,y,z offset of  rotation axis from center of body part
// i.e. for rotation about a "shoulder" as opposed to the center of mass of a glutSolidCube
// x,y,z: rotation (in degrees) of node relative to parent node rotation
// radius: radius of virtual trackball
Node::Node(float x, float y, float z, 
	   float length, float height, float width,
	   float jointx, float jointy, float jointz,
	   float rotx, float roty, float rotz){
    
  selected= false;

  position= vec3(x,y,z);
    
  dimension= vec3(length, height, width);
    
  joint= vec3(jointx, jointy, jointz);
    
  rotation= vec3(rotx, roty, rotz);
    
  initialRotation= rotation;

  parent= NULL;
  child= NULL;
  previous= NULL;
  next= NULL;

  id= id_count++;
}

// destructor
// nothing to do so far
Node::~Node(){}

// sets the parent of this node
// and adds this node 
// to the list of the parent's children
void Node::setParent(Node* parent){
  
  this->parent= parent;
  if(parent->child==NULL){
    parent->child= this;
  }
  else{
    Node *sibling= parent->child;
    while(sibling->next != NULL) sibling= sibling->next;
    sibling->next= this;
    this->previous= sibling;
  }
}

// transform an individual node
// according to its position, 
// rotation, and rotation center
// and call draw() methods
// XXX: NEEDS TO BE IMPLEMENTED
void Node::render(){
 
  // note the order of the operations:
  // the transformations are applied in "reverse" order
  // of glRotate/glTranslate calls
  // (also see cg1 lecture notes on this topic)

  // translate to final position
  // XXX

  glTranslatef(position[0], position[1], position[2]);
  
  
  // END XXX

  // translate node center to joint position
  // XXX
  glTranslatef(joint[0], joint[1], joint[2]);

  // END XXX

  // apply this node's rotation
  // XXX

  glRotatef(rotation[0], 1, 0, 0);
  glRotatef(rotation[1], 0, 1, 0);
  glRotatef(rotation[2], 0, 0, 1);

  // END XXX

  // draw Joint (rotation center)
  drawJoint();

  // translate center of rotation into limb's origin
  // XXX

  glTranslatef(-joint[0], -joint[1], -joint[2]);

  // END XXX

  // draw the node
  draw();
}

// draw an individual node
void Node::draw(){

  // save lighting bit for red light
  // save enable bit for lighting
  // and current bit for color
  glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
	
  GLfloat red[]= {1.0, 0, 0};

  // if node is selected, shine a red light on it
  if(selected) glLightModelfv(GL_LIGHT_MODEL_AMBIENT, red);

  glPushMatrix();

  glStencilFunc(GL_ALWAYS, id, 0xFF); // ORDER MATTERS!

  glScalef(dimension.x, dimension.y, dimension.z);
  glutSolidCube(1.0);

  glPopMatrix();

  glPopAttrib();
}

// draw the joint (rotation center)
// of an individual node
// XXX: NEEDS TO BE IMPLEMENTED
void Node::drawJoint(){

  // save enable bit for lighting
  // and current bit for color
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

  // draw coordinate axes at center of rotation
  // note: lighting disabled for axis drawing
  glDisable(GL_LIGHTING);

  // draw a sphere at center of rotation
  // (with glutwiredphere...)
  // XXX

  glutWireSphere(15.0, 10, 10);

  // END XXX

  glBegin(GL_LINES);

  // XXX: DRAW X,Y AND Z AXES IN RED,GREEN AND BLUE
  //      SEE PROVIDED cg1_ex1.exe (win32) AND cg1_ex1 (linux)

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // x
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(50.0f, 0.0f, 0.0f);

  // y
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 50.0f, 0.0f);

  // z
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 50.0f);

  // END XXX

  glEnd();
  glPopAttrib();
}


// increment / decrement rotation
void Node::rotate(float x, float y, float z){
  rotation+= vec3(x,y,z);
}

// getter for next pointer
Node *Node::getNext(){
  return next;
}

// getter for previous pointer
Node *Node::getPrevious(){
  return previous;
}

// getter for parent pointer
Node *Node::getParent(){
  return parent;
}

// getter for child pointer
Node *Node::getChild(){
  return child;
}

// select node
void Node::select(){
  selected= true;
}

// deselect node
void Node::deselect(){
  selected= false;
}

// resets an individual node
// to the initial rotation 
// specified in constructor
// XXX: NEEDS TO BE IMPLEMENTED
void Node::reset(){

  // reset rotations
  // XXX

  rotation = initialRotation;

  // END XXX
}
