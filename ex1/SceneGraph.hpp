#pragma once

/* ----------------------------------------------------------------
   name:           SceneGraph.hpp
   purpose:        scenegraph class declaration
   version:	   SKELETON CODE
   TODO:           nothing (see SceneGraph.cpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "Node.hpp"
#include "RotationMode.hpp"

class SceneGraph{

public:
  
  SceneGraph(Node *root);

  ~SceneGraph();

  // traverse and draw the scenegraph
  void traverse();

  // navigation in tree
  // (needed for node selection)
  void up();
  void down();
  void left();
  void right();

  void selectById(unsigned char id);

  // increment / decrement rotation of selected node
  void rotate(float x, float y, float z);

  // reset all rotations
  void reset();

private:

  // traverse and draw the scenegraph from a given node
  void traverse(Node *node);

  // reset all rotations below given node
  void reset(Node *node);

  // destroy all nodes
  void clear(Node *node);

  // find a node by its id
  Node* findNode(unsigned char id, Node *node);

  // root node
  Node *root; 

  // currently selected node
  Node *selected;
};
