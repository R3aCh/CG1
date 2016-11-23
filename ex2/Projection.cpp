/* ----------------------------------------------------------------
   name:           Projection.cpp
   purpose:        projection tutorial 
   based on tutorial by Nate Robins, 1997
   modified for clip-space viewing in the context 
   of 'introduction to computer graphics' 
   winter term 2014/2015, assignment 2
   version:	   SKELETON CODE
   TODO:           clip-space implementation (see header file) and init()
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include <iostream>

#include "GLIncludes.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Context.hpp"
#include "Projection.hpp"

// use this with care
// might cause name collisions
using namespace std;
using namespace glm;

mat4 Projection::projectionMatrix;
mat4 Projection::viewMatrix;

Mesh Projection::mesh(std::string(TMP_PATH) + std::string("al.obj"));

// directional light in positve z-direction
vec4 Projection::lightPosition= glm::vec4(0.0, 0.0, 1.0, 0.0);

void Projection::init(void){

  // some output to console
  cout << "--------------------------------------------\n";
  cout << " cg1_ex2 projection tutorial                \n";
  cout << "                                            \n";
  cout << " keyboard:                                  \n";
  cout << " p: perspective projection                  \n";
  cout << " o: orthographic projection                 \n";
  cout << " f: frustum mode                            \n";
  cout << " r: reset                                   \n";
  cout << " q/Q: quit program                          \n";
  cout << "                                            \n";
  cout << " mouse:                                     \n";
  cout << " left click+drag: rotate clip view          \n";
  cout << " right click: window-specific menus         \n";
  cout << "--------------------------------------------\n";
}

void Projection::reshape(int width, int height){
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(&glm::ortho(0.0f, (float)width, (float)height, 0.0f, -0.1f, 0.1f)[0][0]);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// draw base vectors
void Projection::drawAxes(void){

  glColor3ub(255, 0, 0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 0.0, 0.0);
  glVertex3f(0.75, 0.25, 0.0);
  glVertex3f(0.75, -0.25, 0.0);
  glVertex3f(1.0, 0.0, 0.0);
  glVertex3f(0.75, 0.0, 0.25);
  glVertex3f(0.75, 0.0, -0.25);
  glVertex3f(1.0, 0.0, 0.0);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, 0.75, 0.25);
  glVertex3f(0.0, 0.75, -0.25);
  glVertex3f(0.0, 1.0, 0.0);
  glVertex3f(0.25, 0.75, 0.0);
  glVertex3f(-0.25, 0.75, 0.0);
  glVertex3f(0.0, 1.0, 0.0);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 1.0);
  glVertex3f(0.25, 0.0, 0.75);
  glVertex3f(-0.25, 0.0, 0.75);
  glVertex3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.25, 0.75);
  glVertex3f(0.0, -0.25, 0.75);
  glVertex3f(0.0, 0.0, 1.0);
  glEnd();
    
  glColor3ub(255, 255, 0);
  Context::setFont("helvetica", 12);
  Context::drawString(1.1, 0.0, 0.0, "x");
  Context::drawString(0.0, 1.1, 0.0, "y");
  Context::drawString(0.0, 0.0, 1.1, "z");
}

void Projection::display(void){

  // clear color and depth buffer
  glClearColor(0.8, 0.8, 0.8, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor4f(0, 0, 0, 1);

  Context::drawTitles();

  glutSwapBuffers();
}

void Projection::keyPressed(unsigned char key, int x, int y){
  
  switch (key) {
  case 'p':
    Command::setProjectionMode(PERSPECTIVE);
    Context::redisplayAll();
    break;
  case 'o':
    Command::setProjectionMode(ORTHO);
    Context::redisplayAll();
    break;
  case 'f':
    Command::setProjectionMode(FRUSTUM);
    Context::redisplayAll();
    break;
  case 'r':
    Command::reset();
    Context::redisplayAll();
    break;
  case 'q':
  case 'Q':
    exit(0);
    break;
  default:
    break;
  }
}

// -------------------------------------------------------
// WORLD-SPACE VIEW
// -------------------------------------------------------

bool Projection::World::drawMesh= true;

void Projection::World::reshape(int width, int height){

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(&glm::perspective(radians(60.0f), (float) width / (float) height, 1.0f, 256.0f)[0][0]);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // this defines a camera matrix
  glTranslatef(0.0, 0.0, -5.0);
  glRotatef(-45.0, 0.0, 1.0, 0.0);
}

void Projection::World::display(void){

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  glm::vec3 viewDirection= Command::computeViewDirection();

  glClearColor(0.0, 0.0, 0.0, 1.0);    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  // draw current mesh if toggled
  if(drawMesh) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0]);
    // smooth shading
    glShadeModel(GL_SMOOTH);

    mesh.draw();
    glDisable(GL_LIGHTING);
  }

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  // apply inverse view transformation to axes and frustum
  // this moves the camera position and frustum into world space
  // coordinates
  glMultMatrixf(&glm::inverse(viewMatrix)[0][0]);
    
  /* draw the axis and eye vector */
  glPushMatrix();
  glColor3ub(0, 0, 255);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, -1.0 * length(viewDirection));
  glVertex3f(0.1, 0.0, -0.9 * length(viewDirection));
  glVertex3f(-0.1, 0.0, -0.9 * length(viewDirection));
  glVertex3f(0.0, 0.0, -1.0 * length(viewDirection));
  glVertex3f(0.0, 0.1, -0.9 * length(viewDirection));
  glVertex3f(0.0, -0.1, -0.9 * length(viewDirection));
  glVertex3f(0.0, 0.0, -1.0 * length(viewDirection));
  glEnd();
  glColor3ub(255, 255, 0);
  Context::setFont("helvetica", 12);
  Context::drawString(0.0, 0.0, -1.1 * length(viewDirection), "e");
  glColor3ub(255, 0, 0);
  glScalef(0.4, 0.4, 0.4);
  drawAxes();
  glPopMatrix();
    
  // apply inverse projection transformation to unit-frustum
  glMatrixMode(GL_MODELVIEW);
  glMultMatrixf(&glm::inverse(projectionMatrix)[0][0]);
    
  /* draw the canonical viewing frustum */
  // back clip plane
  glColor3f(0.2, 0.2, 0.2);
  glBegin(GL_QUADS);
  glVertex3i(1, 1, 1);
  glVertex3i(-1, 1, 1);
  glVertex3i(-1, -1, 1);
  glVertex3i(1, -1, 1);
  glEnd();
    
  // four corners of frustum
  glColor3ub(128, 196, 128);
  glBegin(GL_LINES);
  glVertex3i(1, 1, -1);
  glVertex3i(1, 1, 1);
  glVertex3i(-1, 1, -1);
  glVertex3i(-1, 1, 1);
  glVertex3i(-1, -1, -1);
  glVertex3i(-1, -1, 1);
  glVertex3i(1, -1, -1);
  glVertex3i(1, -1, 1);
  glEnd();
    
  // front clip plane
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.2, 0.2, 0.4, 0.5);
  glBegin(GL_QUADS);
  glVertex3i(1, 1, -1);
  glVertex3i(-1, 1, -1);
  glVertex3i(-1, -1, -1);
  glVertex3i(1, -1, -1);
  glEnd();
  glPopAttrib();
    
  glPopMatrix();
    
  glutSwapBuffers();

  glPopAttrib();
}

vector< pair < int, string > > Projection::World::getMenuEntries(void){

  vector< pair < int, string > > menuEntries;
  
  menuEntries.push_back(pair<int, string>('m', "Toggle model"));

  return menuEntries;
}

void Projection::World::menu(int value){
  
  switch (value) {
  case 'm':
    drawMesh = !drawMesh;
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

// -------------------------------------------------------
// SCREEN-SPACE VIEW
// -------------------------------------------------------

// screen_reshape sets the current projection and view matrix
// in 'projectionMatrix[16]' and 'viewMatrix[16]'
void Projection::Screen::reshape(int width, int height){

  glViewport(0, 0, width, height);
}

void Projection::Screen::display(void){

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(&projectionMatrix[0][0]);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(&viewMatrix[0][0]);

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // directional light in positve z-direction
  // must have modelview transform applied to it in order
  // to have correct light position in eye coordinates
  glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0]);

  glShadeModel(GL_SMOOTH);

  mesh.draw();
  
  glPopAttrib();

  glutSwapBuffers();
}


vector< pair < int, string > > Projection::Screen::getMenuEntries(void){

  vector< pair < int, string > > menuEntries;
  
  menuEntries.push_back(pair<int, string>(0, "Model"));
  menuEntries.push_back(pair<int, string>(0, ""));
  menuEntries.push_back(pair<int, string>('a', "Al Capone"));
  menuEntries.push_back(pair<int, string>('s', "Soccerball"));
  menuEntries.push_back(pair<int, string>('d', "Dolphins"));
  menuEntries.push_back(pair<int, string>('f', "Flowers"));
  menuEntries.push_back(pair<int, string>('j', "F-16"));
  menuEntries.push_back(pair<int, string>('p', "Porsche"));
  menuEntries.push_back(pair<int, string>('r', "Rose"));

  return menuEntries;
}

void Projection::Screen::menu(int value){
  
  string name;
    
  switch(value){
  case 'a':
    mesh.load(std::string(TMP_PATH) + std::string("al.obj"));
    Context::redisplayAll();
    break;
  case 's':
    mesh.load(std::string(TMP_PATH) + std::string("soccerball.obj"));
    Context::redisplayAll();
    break;
  case 'd':
    mesh.load(std::string(TMP_PATH) + std::string("dolphins.obj"));
    Context::redisplayAll();
    break;
  case 'f':
    mesh.load(std::string(TMP_PATH) + std::string("flowers.obj"));
    Context::redisplayAll();
    break;
  case 'j':
    mesh.load(std::string(TMP_PATH) + std::string("f-16.obj"));
    Context::redisplayAll();
    break;
  case 'p':
    mesh.load(std::string(TMP_PATH) + std::string("porsche.obj"));
    Context::redisplayAll();
    break;
  case 'r':
    mesh.load(std::string(TMP_PATH) + std::string("rose+vase.obj"));
    Context::redisplayAll();
    break;
  default:
    break;
  }
}



// -------------------------------------------------------
// CLIP-SPACE VIEW
// -------------------------------------------------------

// draw mesh?
bool Projection::Clip::drawMesh= true;
// draw clip planes?
bool Projection::Clip::drawClipPlanes= false;

float Projection::Clip::angle= 0.0;

void Projection::Clip::reshape(int width, int height){
	//glViewport(0, 0, width, height);
}    

// this is where all the clip-space 'magic' happens
void Projection::Clip::display(void){

}

void Projection::Clip::mouseDragged(glm::vec2 previousMouse, glm::vec2 mouse){

}

void Projection::Clip::menu(int value){

	switch (value){
	case 0:
		cout << 0 << endl;
		Context::redisplayAll();
		break;
	case 1:
		cout << 1 << endl;
		Context::redisplayAll();
		break;
	default:
		break;
	}
}

vector< pair < int, string > > Projection::Clip::getMenuEntries(void){
	vector< pair < int, string > > menuEntries;

	menuEntries.push_back(pair<int, string>(0, "Toggle Model"));
	menuEntries.push_back(pair<int, string>(1, "Toggle Planes"));

	return menuEntries;
}

// -------------------------------------------------------
// COMMAND WINDOW
// -------------------------------------------------------

Cell Projection::Command::lookat[9] = {
  Cell(180, 120, -5.0, 5.0, 0.0, 0.1,
       "Specifies the X position of the eye point."),
  Cell(240, 120, -5.0, 5.0, 0.0, 0.1,
       "Specifies the Y position of the eye point."),
  Cell(300, 120, -5.0, 5.0, 2.0, 0.1,
       "Specifies the Z position of the eye point."),
  Cell(180, 160, -5.0, 5.0, 0.0, 0.1,
       "Specifies the X position of the reference point."),
  Cell(240, 160, -5.0, 5.0, 0.0, 0.1,
       "Specifies the Y position of the reference point."),
  Cell(300, 160, -5.0, 5.0, 0.0, 0.1,
       "Specifies the Z position of the reference point."),
  Cell(180, 200, -2.0, 2.0, 0.0, 0.1,
       "Specifies the X direction of the up vector."),
  Cell(240, 200, -2.0, 2.0, 1.0, 0.1,
       "Specifies the Y direction of the up vector."),
  Cell(300, 200, -2.0, 2.0, 0.0, 0.1,
       "Specifies the Z direction of the up vector."),
};

Cell Projection::Command::perspective[4] = {
  Cell(180, 80, 1.0, 179.0, 60.0, 1.0,
       "Specifies field of view angle (in degrees) in y direction."),
  Cell(240, 80, -3.0, 3.0, 1.0, 0.01,
       "Specifies field of view in x direction (width/height)."),
  Cell(300, 80, 0.1, 10.0, 1.0, 0.05,
       "Specifies distance from viewer to near clipping plane."),
  Cell(360, 80, 0.1, 10.0, 10.0, 0.05,
       "Specifies distance from viewer to far clipping plane."),
};

Cell Projection::Command::frustum[6] = {
  Cell(120, 80, -10.0, 10.0, -0.575, 0.1,
       "Specifies coordinate for left vertical clipping plane."),
  Cell(180, 80, -10.0, 10.0, 0.575, 0.1,
       "Specifies coordinate for right vertical clipping plane."),
  Cell(240, 80, -10.0, 10.0, -0.575, 0.1,
       "Specifies coordinate for bottom vertical clipping plane."),
  Cell(300, 80, -10.0, 10.0, 0.575, 0.1,
       "Specifies coordinate for top vertical clipping plane."),
  Cell(360, 80, 0.1, 10, 1.0, 0.05,
       "Specifies distance to near clipping plane."),
  Cell(420, 80, 0.1, 10, 10, 0.05,
       "Specifies distance to far clipping plane."),
};

Cell Projection::Command::ortho[6] = {
  Cell(120, 80, -10.0, 10.0, -1.0, 0.1,
       "Specifies coordinate for left vertical clipping plane."),
  Cell(180, 80, -10.0, 10.0, 1.0, 0.1,
       "Specifies coordinate for right vertical clipping plane."),
  Cell(240, 80, -10.0, 10.0, -1.0, 0.1,
       "Specifies coordinate for bottom vertical clipping plane."),
  Cell(300, 80, -10.0, 10.0, 1.0, 0.1,
       "Specifies coordinate for top vertical clipping plane."),
  Cell(360, 80, -5.0, 5.0, 1.0, 0.01,
       "Specifies distance to near clipping plane."),
  Cell(420, 80, -5.0, 5.0, 3.5, 0.01,
       "Specifies distance to far clipping plane."),
};

void Projection::Command::reset(void){

  perspective[0].setValue(60.0);
  perspective[1].setValue(1.0);
  perspective[2].setValue(1.0);
  perspective[3].setValue(10.0);
  ortho[0].setValue(-1.0);
  ortho[1].setValue(1.0);
  ortho[2].setValue(-1.0);
  ortho[3].setValue(1.0);
  ortho[4].setValue(1.0);
  ortho[5].setValue(3.5);
  frustum[0].setValue(-0.575);
  frustum[1].setValue(0.575);
  frustum[2].setValue(-0.575);
  frustum[3].setValue(0.575);
  frustum[4].setValue(1.0);
  frustum[5].setValue(10);
  lookat[0].setValue(0.0);
  lookat[1].setValue(0.0);
  lookat[2].setValue(2.0);
  lookat[3].setValue(0.0);
  lookat[4].setValue(0.0);
  lookat[5].setValue(0.0);
  lookat[6].setValue(0.0);
  lookat[7].setValue(1.0);
  lookat[8].setValue(0.0);

  viewMatrix= computeViewMatrix();
  projectionMatrix= computeProjectionMatrix();
}

void Projection::Command::setProjectionMode(ProjectionMode mode){

  projectionMode= mode;

  projectionMatrix= computeProjectionMatrix();
  viewMatrix= computeViewMatrix();
}

// fills in projection matrix
mat4 Projection::Command::computeProjectionMatrix(void){

  if(projectionMode == PERSPECTIVE)
    return glm::perspective(radians(perspective[0].getValue()), perspective[1].getValue(), 
			    perspective[2].getValue(), perspective[3].getValue());
  else if(projectionMode == ORTHO)
    return glm::ortho(ortho[0].getValue(), ortho[1].getValue(), ortho[2].getValue(),
		      ortho[3].getValue(), ortho[4].getValue(), ortho[5].getValue());
  else // projectionMode == FRUSTUM
    return glm::frustum(frustum[0].getValue(), frustum[1].getValue(), frustum[2].getValue(),
			frustum[3].getValue(), frustum[4].getValue(), frustum[5].getValue());
}

vec3 Projection::Command::computeViewDirection(void){

  vec3 viewDirection;
  // 'l' is the normalized viewing direction
  viewDirection[0]= lookat[3].getValue() - lookat[0].getValue(); 
  viewDirection[1]= lookat[4].getValue() - lookat[1].getValue(); 
  viewDirection[2]= lookat[5].getValue() - lookat[2].getValue();

  return viewDirection;
}

// fills in camera matrix
mat4 Projection::Command::computeViewMatrix(void){
  return glm::lookAt(glm::vec3(lookat[0].getValue(), lookat[1].getValue(), lookat[2].getValue()),
		     glm::vec3(lookat[3].getValue(), lookat[4].getValue(), lookat[5].getValue()),
		     glm::vec3(lookat[6].getValue(), lookat[7].getValue(), lookat[8].getValue()));
}

void Projection::Command::reshape(int width, int height){

  glViewport(0, 0, width, height);

  viewMatrix= computeViewMatrix();
  projectionMatrix= computeProjectionMatrix();

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(&glm::ortho(0.0f, (float) width, (float) height, 0.0f, -0.1f, 0.1f)[0][0]);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Projection::Command::display(void){

  glPushAttrib(GL_COLOR_BUFFER_BIT);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  glColor3ub(255,255,255);
    
  Context::setFont("helvetica", 18);
    
  if(projectionMode == PERSPECTIVE){
    Context::drawString(190, perspective[0].y-40, "fovy"); 
    Context::drawString(240, perspective[0].y-40, "aspect"); 
    Context::drawString(310, perspective[0].y-40, "zNear"); 
    Context::drawString(370, perspective[0].y-40, "zFar");
  } 
  else{
    Context::drawString(140, perspective[0].y-40, "left"); 
    Context::drawString(180, perspective[0].y-40, "right"); 
    Context::drawString(240, perspective[0].y-40, "bottom");
    Context::drawString(320, perspective[0].y-40, "top"); 
    Context::drawString(370, perspective[0].y-40, "near");
    Context::drawString(430, perspective[0].y-40, "far");
  }
    
  if(projectionMode == PERSPECTIVE){
    Context::drawString(50, perspective[0].y, "gluPerspective(");
    Context::drawString(230, perspective[0].y, ","); 
    Context::drawString(290, perspective[0].y, ",");
    Context::drawString(350, perspective[0].y, ",");
    Context::drawString(420, perspective[0].y, ");");
  } 
  else if(projectionMode == FRUSTUM){
    Context::drawString(30, frustum[0].y, "glFrustum(");
    Context::drawString(170, frustum[0].y, ",");
    Context::drawString(230, frustum[0].y, ","); 
    Context::drawString(290, frustum[0].y, ",");
    Context::drawString(350, frustum[0].y, ",");
    Context::drawString(410, frustum[0].y, ","); 
    Context::drawString(480, frustum[0].y, ");");
  } 
  else{
    Context::drawString(45, ortho[0].y, "glOrtho(");
    Context::drawString(170, ortho[0].y, ",");
    Context::drawString(230, ortho[0].y, ","); 
    Context::drawString(290, ortho[0].y, ",");
    Context::drawString(350, ortho[0].y, ",");
    Context::drawString(410, ortho[0].y, ","); 
    Context::drawString(480, ortho[0].y, ");");
  }
    
  Context::drawString(88, lookat[0].y, "gluLookAt(");
  Context::drawString(230, lookat[0].y, ","); 
  Context::drawString(290, lookat[0].y, ",");
  Context::drawString(350, lookat[0].y, ",");
  Context::drawString(380, lookat[0].y, "<- eye");
  Context::drawString(230, lookat[3].y, ","); 
  Context::drawString(290, lookat[3].y, ",");
  Context::drawString(350, lookat[3].y, ",");
  Context::drawString(380, lookat[3].y, "<- center");
  Context::drawString(230, lookat[6].y, ","); 
  Context::drawString(290, lookat[6].y, ",");
  Context::drawString(360, lookat[6].y, ");");
  Context::drawString(380, lookat[6].y, "<- up");
    
  if(projectionMode == PERSPECTIVE){
    for(int i= 0; i<4; i++){
      perspective[i].draw();
    }
  }
  else if(projectionMode == FRUSTUM){
    for(int i= 0; i<6; i++){
      frustum[i].draw();
    }
  } 
  else if(projectionMode == ORTHO){
    for(int i= 0; i<6; i++){
      ortho[i].draw();
    }
  }   
    
  for(int i= 0; i<9; i++){
    lookat[i].draw();
  }

  glPopAttrib();

  glutSwapBuffers();
}

void Projection::Command::mousePressed(glm::vec2 mouse){
 
  Cell::active= true;  

  if(projectionMode == PERSPECTIVE){
    for(int i= 0; i<4; i++){
      perspective[i].hit(mouse.x, mouse.y);
    }
  } 
  else if(projectionMode == FRUSTUM){
    for(int i= 0; i<6; i++){
      frustum[i].hit(mouse.x, mouse.y);
    }
  } 
  else if(projectionMode == ORTHO){

    for(int i= 0; i<6; i++){
      ortho[i].hit(mouse.x, mouse.y);
    }
  }
  for(int i= 0; i<9; i++){
    lookat[i].hit(mouse.x, mouse.y);
  }

  Context::redisplayAll();
}

void Projection::Command::mouseDragged(glm::vec2 previousMouse, glm::vec2 mouse){

  for(int i= 0; i<4; i++){
    perspective[i].update(previousMouse.y-mouse.y);
  }
  for(int i= 0; i<6; i++){
    frustum[i].update(previousMouse.y-mouse.y);
  }
  for(int i= 0; i<6; i++){
    ortho[i].update(previousMouse.y-mouse.y);
  }
  for(int i= 0; i<9; i++){
    lookat[i].update(previousMouse.y-mouse.y);
  }

  viewMatrix= computeViewMatrix();
  projectionMatrix= computeProjectionMatrix();

  Context::redisplayAll();
}

void Projection::Command::mouseReleased(vec2 mouse){
  
  Cell::active= false;  
  
  glutPostRedisplay();
}

Projection::ProjectionMode Projection::Command::projectionMode= PERSPECTIVE;

vector< pair < int, string > > Projection::Command::getMenuEntries(void){

  vector< pair < int, string > > menuEntries;
  
  menuEntries.push_back(pair<int, string>('p', "Perspective"));
  menuEntries.push_back(pair<int, string>('o', "Ortho")); 
  menuEntries.push_back(pair<int, string>('f', "Frustum"));
  menuEntries.push_back(pair<int, string>('r', "Reset"));
  menuEntries.push_back(pair<int, string>('q', "Quit"));

  return menuEntries;
}

void Projection::Command::menu(int value){
  keyPressed((unsigned char)value, 0, 0); 
}



// main function
int main(int argc, char** argv){
    
  // initialize OpenGL context
  Context::init(argc, argv);
  
  // initialize Projection class
  Projection::init();
  
  // start GLUT event loop
  glutMainLoop();
  
  return 0;
}
