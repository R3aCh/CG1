/* ----------------------------------------------------------------
   name:           Mesh.cpp
   purpose:        cg1_ex2 2015 mesh class for OpenGL rendering
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "GLIncludes.hpp"

// OpenGL mathematics library
// http://glm.g-truc.net
#include "glm/glm.hpp"
#include "glm/gtx/integer.hpp"

/*
 * Class for a simple triangle mesh represented as an indexed face set
 * intended for processing with modern OpenGL in shaders
 */
class Mesh{

public:

  static const GLuint attribVertex;
  static const GLuint attribNormal;
  static const GLuint attribColor;
  static const GLuint attribTexCoord;
  
  // default constructor
  Mesh();

  Mesh(const std::string& filename);
  
  // destructor
  ~Mesh();

  // clockwise / counter-clockwise?
  enum PolygonWinding{
    CW,
    CCW
  };
  
  // set polygon winding
  void setWinding(PolygonWinding winding);
  
  // load the mesh from a file
  // currently .OBJ and .OFF are supported
  void load(const std::string& filename);
  
  // normalize to bounding sphere radius 1
  void unitize(bool doUnitize);
  // center model
  void center(bool doCenter);

  // tells libray whether to
  // calculate bounding sphere
  void computeBoundingSphere(bool compute);

  // tells libray whether to
  // calculate bounding box
  void computeBoundingBox(bool compute);
  
  // retrieve number of faces
  int getNumFaces(void);
  
  // draw the model
  void draw(void);
  
protected:
    
  // file name
  std::string name;

  void loadOBJ(const std::string& filename);
 
  //triangulate a polygon
  void triangulate(std::vector<GLuint> pi, std::vector<GLuint> ti, std::vector<GLuint> ni);
  // bring to format opengl eats
  // (no shared normals / texture coordinates)
  void clean(void);
  // parse a material library
  void parseMaterials(std::string filename);
  // find material
  unsigned int materialIndex(std::string name);
  // add material to group
  void addMaterial(std::string group, std::string material);

  // normalize to bounding sphere radius 1
  void unitize(void);
  // center model
  void center(void);

  void computeBoundingSphere();

  // tells libray whether to
  // calculate bounding box
  void computeBoundingBox();
    
  typedef struct group{
    std::string name;
    unsigned int index;
    std::string material;
  } Group;
    
  typedef struct material{
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
  } Material;

  // send to OpenGL or not?
  bool drawNormals;
  bool drawTexCoords;
  
  bool doCenter;
  bool doUnitize;
  
  PolygonWinding winding;

  bool doComputeBoundingSphere;
  bool doComputeBoundingBox;
  float boundingSphereRadius;
  // two opposite corners of bounding box
  glm::vec3 boundingBoxMin;
  glm::vec3 boundingBoxMax;
  
  // position of the vertices
  std::vector<glm::vec3> positions;
  // normals of the vertices
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> normalsRaw;
  // texture coordinates of the vertices
  std::vector<glm::vec2> texCoords;
  std::vector<glm::vec2> texCoordsRaw;
  // temporary storage for OBJ format
  std::vector<glm::uvec3> texCoordIndices;
  std::vector<glm::uvec3> normalIndices;
  // indices of the faces
  std::vector<glm::uvec3> faces;
  
  std::vector<Group> groups;
  std::vector<Material> materials;
};
