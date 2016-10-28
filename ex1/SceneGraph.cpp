/* ----------------------------------------------------------------
   name:           Scenegraph.cpp
   purpose:        implementation of the scenegraph functions
   version:	   SKELETON CODE
   TODO:           traverse reset
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream>

#include "SceneGraph.hpp"
#include "glm/glm.hpp"

// without this line, 
// printline debugging won't work
using namespace std;
 
// use this with care
// might cause name collisions
using namespace glm;

SceneGraph::SceneGraph(Node *root) : root(root), selected(root){

  selected->select();
}

SceneGraph::~SceneGraph(){

  clear(root);
}

// traverse and draw the scenegraph
// nothing to do here
// (see helper function)
void SceneGraph::traverse(){
  traverse(root);
}

// reset all rotations in the scenegraph
// nothing to do here
// (see helper function)
void SceneGraph::reset(){
  reset(root);
}

// navigation in tree
// (needed for node selection)
void SceneGraph::up(){
  if(selected->getParent() == NULL) return;
  selected->deselect();
  selected= selected->getParent();
  selected->select();
}
void SceneGraph::down(){
  if(selected->getChild() == NULL) return;
  selected->deselect();
  selected= selected->getChild();
  selected->select();
}
void SceneGraph::left(){
  if(selected->getPrevious() == NULL) return;
  selected->deselect();
  selected= selected->getPrevious();
  selected->select();
}
void SceneGraph::right(){ 
  if(selected->getNext() == NULL) return;
  selected->deselect();
  selected= selected->getNext();
  selected->select();
}

// increment / decrement rotation of selected node
void SceneGraph::rotate(float x, float y, float z){
  selected->rotate(x, y, z);
}

// traverse and draw the scenegraph from a given node
// XXX: NEEDS TO BE IMPLEMENTED
void SceneGraph::traverse(Node *node){

  if(node == NULL) return;

  // store current modelview (transformation)matrix
  // XXX
  
  glPushMatrix();

  
  // END XXX
  
  node->render();
	
  // continue concatenating transformations
  // if this node has children
  // XXX  

  traverse(node->getChild());

  
  // END XXX
  
  // after this scenegraph branch
  // restore previous transformation
  // XXX
  
  glPopMatrix();

  
  // END XXX
  
  // and traverse possible siblings
  // XXX

  traverse(node->getNext());

  // END XXX
}

void SceneGraph::clear(Node *node){

  if(node == NULL) return;
  delete node;
  clear(node->getChild());
  clear(node->getNext());
}

// reset the subtree corresponding 
// to a given node
// XXX: NEEDS TO BE IMPLEMENTED
void SceneGraph::reset(Node* node){

  // XXX

  if (node == NULL) return;
  node->reset();
  reset(node->getChild());
  reset(node->getNext());

  // END XXX
}
