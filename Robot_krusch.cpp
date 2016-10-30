/* ----------------------------------------------------------------
   name:           Robot.cpp
   purpose:        implementation of the scenegraph construction
   version:	   SKELETON CODE
   TODO:           buildRobot
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "Robot.hpp"

// nodes of the scenegraph

// XXX: CURRENTLY ONLY TORSO. INSERT OTHER BODY PARTS HERE.
//      SEE EXAMPLE BINARY cg1_ex1.exe (win32) and cg1_ex1 (linux).
//      EXPERIMENTATION IS ENCOURAGED. THE FINAL MODEL/SCENEGRAPH
//      NEED NOT BE IDENTICAL TO THE PROPOSED SOLUTION.

// XXX: EXTEND THE SCENEGRAPH i.e.
// Node *part1, *part2, *part3, etc...

// build the robot scenegraph
Node *Robot::buildRobot(){
  
  // torso attributes and initial position.
  // for parameters of constructor and their meanings,
  // see file node.h
  Node *torso= new Node(0, 0, 0,
			200, 300, 100,
			0, 0, 0,
			0, 0, 0);
  
  
  // XXX: implement more body/robot/whatever parts here
  
  Node *head = new Node(0, 195, 0,
	  120, 50, 75,
	  0, -40, 0,
	  0, 0, 0);

  Node *hat = new Node(0, 45, 0,
	  100, 5, 100,
	  0, 0, 0,
	  0, 0, 0);

  hat->setParent(head);
  // INSERT YOUR CODE HERE
  Node *left_arm = new Node(-120, 20, 0,
	  20, 150, 30,
	  0, 85, 0,
	  0, 0, 0);

  Node *right_arm = new Node(120, 20, 0,
	  20, 150, 30,
	  0, 85, 0,
	  0, 0, 0);

  Node *left_leg = new Node(-40, -270, 0,
	  50, 200, 50,
	  0, 110, 0,
	  0, 0, 0);

  Node *right_leg = new Node(40, -270, 0,
	  50, 200, 50,
	  0, 110, 0,
	  0, 0, 0);
  
  Node *left_foot = new Node(0 ,-115, 20,
	  50, 10, 100,
	  0, 5, -20,
	  0, 0, 0);

  Node *right_foot = new Node(0, -115, 20,
	  50, 10, 100,
	  0, 5, -20,
	  0, 0, 0);
  // END XXX
  
  // finally, attach the robot/model parts
  // example: leftArm->setParent(torso), etc...
  // BUILD THE SCENEGRAPH
  
  // XXX: attach other body parts here

  // INSERT YOUR CODE HERE
  head->setParent(torso);
  
  right_arm->setParent(torso);
  left_arm->setParent(torso);

  right_leg->setParent(torso);
  left_leg->setParent(torso);

  left_foot->setParent(left_leg);
  right_foot->setParent(right_leg);
  // END XXX
  
  // return root node
  return torso;
}
