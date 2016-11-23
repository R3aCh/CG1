/* ----------------------------------------------------------------
   name:           Mesh.cpp
   purpose:        cg1_ex2 2014 triangle mesh for OpenGL rendering
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <fstream>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Mesh.hpp"

using namespace glm;
using namespace std;

// NVIDIA wants it like this
// see https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
const GLuint Mesh::attribVertex= 0;
const GLuint Mesh::attribNormal= 2;
const GLuint Mesh::attribColor= 3;
const GLuint Mesh::attribTexCoord= 8;

Mesh::Mesh(){
  
  drawNormals= false;
  drawTexCoords= false;
  winding= CW;
}

Mesh::Mesh(const string& name){
  
  doCenter= true;
  doUnitize= true;
  drawNormals= true;
  drawTexCoords=false;
  winding= CW;
  load(name);
}

Mesh::~Mesh(){
}

void Mesh::setWinding(PolygonWinding winding){
  this->winding= winding;
}

void Mesh::computeBoundingSphere(bool compute){
  doComputeBoundingSphere= compute;
}

void Mesh::computeBoundingBox(bool compute){
  doComputeBoundingBox= compute;
}

// normalize to bounding sphere radius 1
void Mesh::unitize(bool doUnitize){
  this->doUnitize= doUnitize;
}

// center model
void Mesh::center(bool doCenter){
  this->doCenter= doCenter;
}

// load a model	
void Mesh::load(const string& fileName){
  
  positions.clear();
  faces.clear();
  groups.clear();
  materials.clear();
  normals.clear();
  boundingSphereRadius= 0;
  boundingBoxMin= vec3(0);
  boundingBoxMax= vec3(0);
  
  name= fileName;
  loadOBJ(fileName);
  
  if(doComputeBoundingSphere)computeBoundingSphere();
  if(doComputeBoundingBox)computeBoundingBox();
  
  if(doCenter) center();
  if(doUnitize) unitize();
}


int Mesh::getNumFaces(void){
  return faces.size();
}

// load an OBJ model
void Mesh::loadOBJ(const string& filename){
  
  /**
   * .obj file format
   *
   * '#'  -  comment
   * 'v'  -  vertex position (3 floats separated by whitespace)
   * 'vt' -  texture coordinates (2 floats separated by whitespace)
   * 'vn' -  vertex normal (3 floats separated by whitespace)
   * 'f'  -  faces, n times 3 v/vt/vn indices separated by /  and whitespace: v/t/n v/t/n ... v/t/n
   * 'g'  -  group, 
   */
  
  const string POSITION= "v";
  const string TEX_COORD= "vt";
  const string NORMAL= "vn";
  const string FACE= "f";
  const string GROUP= "g";
  const string USE_MTL= "usemtl";
  const string MTL_GROUP= "group";
  const string MTL_LIB= "mtllib";
  
  // end last (and possibly only) group
  Group begin= {"default", 0, ""};
  groups.push_back(begin);
  
  string group= "";
  
  ifstream file;
  file.open(filename.c_str());
  
  vector<GLuint> pi;
  vector<GLuint> ti;
  vector<GLuint> ni;
  string l;
  
  while(getline(file, l)){  
    
    stringstream line(l);
    string type;
    line >> type;
    
    if(type == POSITION){
      
      float x, y, z;
      line >> x >> y >> z;
      positions.push_back(vec3(x,y,z));	
    }
    else if(type == TEX_COORD){
      
      float u, v;
      line >> u >> v;
      texCoordsRaw.push_back(vec2(u,v));
    }
    else if(type == NORMAL){
      
      float nx, ny, nz;
      line >> nx >> ny >> nz;
      normalsRaw.push_back(vec3(nx, ny, nz));
    }
    else if(type == FACE){
      
      pi.clear();
      ti.clear();
      ni.clear();
      GLuint positionIndex, texCoordIndex, normalIndex;
      char slash;
      
      while(!line.eof()){
	
	// v v v
	if(line.str().find('/') == string::npos && line.str().length() > 5){
	  line >> positionIndex;
	  pi.push_back(positionIndex-1);
	}
	// v//n v//n v//n
	else if(line.str().find("//") != string::npos){
	  line >> positionIndex >> slash >> slash >> normalIndex;
	  pi.push_back(positionIndex-1);
	  ni.push_back(normalIndex-1);
	}
	else{
	  // v/t v/t v/t
	  line >> positionIndex >> slash >> texCoordIndex;
	  pi.push_back(positionIndex-1);
	  ti.push_back(texCoordIndex-1);
	  // v/t/n v/t/n v/t/n
	  if(line.peek() == '/'){
	    line >> slash >> normalIndex;
	    ni.push_back(normalIndex-1);
	  }
	}
      } // line end
      // FIX ME!!!
      if(pi.size()>15 && pi[(int)pi.size()-1]==pi[(int)pi.size()-2]) pi.pop_back();
      //		for(int i= 0; i<pi.size(); i++) cout << pi[i] << " " ;
      triangulate(pi,ti,ni);
    } // end face
    else if(type == GROUP){
      string name;
      line >> name;
      Group group= {name, (GLuint)faces.size(), ""};
      groups.push_back(group);
    } // end group
    else if(type == MTL_GROUP){ // materials separrately specified
      
      line >> group;
    }
    else if(type == USE_MTL){
      
      string material;
      line >> material;
      if(group != "")
	addMaterial(group, material);
      else groups.back().material= material;
    }
    else if(type == MTL_LIB){
      
      string dir, library;
      stringstream name(filename);
      getline(name, dir, '/');
      line >> library;
      name << dir << '/' << library;
      
      parseMaterials(name.str());
    }
    
  } // eof
  // end last (and possibly only) group
  Group end= {"default", (GLuint)faces.size(), ""};
  groups.push_back(end);
  
  file.close();	    
  
  // bring to format opengl eats
  // this means possible duplication of normals 
  // and / or texture coordinates
  clean();
  
  //--------------#CHANGED-----------------
  parseMaterials(filename.substr(0, filename.find_last_of(".")) + ".mtl");
  //--------------#CHANGED-----------------

  cout << "loaded " << filename << ": " 
       << positions.size() << " vertices, " << texCoordsRaw.size() << " texture coordinates, " << normalsRaw.size() << " normals, "
       << faces.size() << " faces, " << ((int)groups.size()-2) << " groups, " << materials.size() << " materials" << endl;
  
}

// parse a material library	
void Mesh::parseMaterials(string filename){
  
  const string NEW_MATERIAL= "newmtl";
  const string MATERIAL_AMBIENT= "Ka";
  const string MATERIAL_DIFFUSE= "Kd";
  const string MATERIAL_SPECULAR= "Ks";
  const string SPECULAR_EXPONENT= "Ns";
  
  ifstream file;
  file.open(filename.c_str());
  
  string l;
  
  while(getline(file, l)){  
    
    stringstream line(l);
    string type;
    line >> type;
    
    if(type == NEW_MATERIAL){
      
      string name;
      line >> name;
      Material m;
      m.name= name;
      materials.push_back(m);
    }
    else if(type == MATERIAL_AMBIENT){
      
      float r, g, b;
      line >> r >> g >> b;
      vec3 ambient= vec3(r,g,b);
      materials.back().ambient= ambient;
    }
    else if(type == MATERIAL_DIFFUSE){
      
      float r, g, b;
      line >> r >> g >> b;
      vec3 diffuse= vec3(r,g,b);
      materials.back().diffuse= diffuse;
    }
    else if(type == MATERIAL_SPECULAR){
      
      float r, g, b;
      line >> r >> g >> b;
      vec3 specular= vec3(r,g,b);
      materials.back().specular= specular;
    }
    else if(type == SPECULAR_EXPONENT){
      
      float exponent;
      line >> exponent;
      materials.back().shininess= exponent;
    }
  } // eof
  file.close();
}

// find material
unsigned int Mesh::materialIndex(string name){
  for(unsigned int i= 0; i<materials.size(); ++i){
    
    if(materials[i].name == name) return i;
  }
  return 0;
}

// add material to group
void Mesh::addMaterial(string group, string material){
  for(unsigned int i= 0; i<groups.size(); ++i){
    if(groups[i].name == group) groups[i].material= material;
  }
}

// triangulate a polygon
void Mesh::triangulate(vector<GLuint> pi, vector<GLuint> ti, vector<GLuint> ni){
  
  for(unsigned int v= 2; v<pi.size(); v++){
    
    if(winding==CW)
      faces.push_back(uvec3(pi[0], pi[v-1], pi[v]));
    else
      faces.push_back(uvec3(pi[v], pi[v-1], pi[0]));
  }
  
  for(size_t v= 2; v<ti.size(); v++){
    
    if(winding==CW)
      texCoordIndices.push_back(uvec3(ti[0], ti[v-1], ti[v]));
    else
      texCoordIndices.push_back(uvec3(ti[v], ti[v-1], ti[0]));
  }
    
  for(unsigned int v= 2; v<ni.size(); v++){
    
    if(winding==CW)
      normalIndices.push_back(uvec3(ni[0], ni[v-1], ni[v]));
    else
      normalIndices.push_back(uvec3(ni[v], ni[v-1], ni[0]));
  }
  
  pi.clear();
  ti.clear();
  ni.clear();
}

// bring to format opengl eats
// this means possible duplication of normals
// and / or texture coordinates
void Mesh::clean(void){
  
  if(normalIndices.size()>0) normals.resize(positions.size());
  for(unsigned int i= 0; i<normalIndices.size(); i++){
    normals[faces[i].x]= normalsRaw[normalIndices[i].x];
    normals[faces[i].y]= normalsRaw[normalIndices[i].y];
    normals[faces[i].z]= normalsRaw[normalIndices[i].z];
  }
  normalsRaw.clear();
  normalIndices.clear();
  
  //normalize
  for(size_t i= 0; i< normals.size(); i++){
    normals[i]= normalize(normals[i]);
  } 
  
  if(texCoordIndices.size()>0) texCoords.resize(positions.size());
  for(unsigned int i= 0; i<texCoordIndices.size(); i++){
    texCoords[faces[i].x]= texCoordsRaw[texCoordIndices[i].x];
    texCoords[faces[i].y]= texCoordsRaw[texCoordIndices[i].y];
    texCoords[faces[i].z]= texCoordsRaw[texCoordIndices[i].z];
  }
  texCoordsRaw.clear();
  texCoordIndices.clear();
}

// center the mesh at its origin
void Mesh::center(void){
  
  computeBoundingBox();
  
  vec3 center= (boundingBoxMin + boundingBoxMax) * vec3(0.5);
  
  for(unsigned int i= 0; i<positions.size(); i++){
    positions[i]-= center;
  }
  boundingBoxMin-= center;
  boundingBoxMax-= center;
}

// scale to size 1
void Mesh::unitize(void){
  
  computeBoundingSphere();
  
  for(unsigned int i= 0; i<positions.size(); ++i){
    positions[i]/= boundingSphereRadius;
  }
  boundingSphereRadius= 1;
  boundingBoxMin= vec3(-1);
  boundingBoxMax= vec3(1);
}

// compute bounding sphere
void Mesh::computeBoundingSphere(void){
  
  if(boundingSphereRadius!=0) return;
  for(unsigned int i= 0; i<positions.size(); i++){
    vec3 v= positions[i];
    if(length(v) > boundingSphereRadius) boundingSphereRadius= length(v);
  }
}

// compute bounding box
void Mesh::computeBoundingBox(void){

  if(boundingBoxMin!=vec3(0) && boundingBoxMin!=vec3(0)) return;
  
  boundingBoxMin= vec3(numeric_limits<float>::max());
  boundingBoxMax= vec3(numeric_limits<float>::min());
  for(unsigned int i= 0; i<positions.size(); i++){
    if(positions[i].x < boundingBoxMin.x) boundingBoxMin.x= positions[i].x;
    if(positions[i].x > boundingBoxMax.x) boundingBoxMax.x= positions[i].x;
    if(positions[i].y < boundingBoxMin.y) boundingBoxMin.y= positions[i].y;
    if(positions[i].y > boundingBoxMax.y) boundingBoxMax.y= positions[i].y;
    if(positions[i].z < boundingBoxMin.z) boundingBoxMin.z= positions[i].z;
    if(positions[i].z > boundingBoxMax.z) boundingBoxMax.z= positions[i].z;
  }
}

void Mesh::draw(void){
  
  if(positions.size()==0) return;
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &positions[0]);
  if(normals.size()>0){
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);
  }
  
  unsigned int begin, end;
  for(unsigned int g= 0; g+1<groups.size(); ++g){
    begin= groups[g].index;
    end= groups[g+1].index;
    
    if(materials.size() > 0){
      unsigned int material= materialIndex(groups[g].material);
      glMaterialfv(GL_FRONT, GL_AMBIENT, &materials[material].ambient[0]);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, &materials[material].diffuse[0]);
      glMaterialfv(GL_FRONT, GL_SPECULAR, &materials[material].specular[0]);
      glMaterialf(GL_FRONT, GL_SHININESS, materials[material].shininess);
    }
    glDrawElements(GL_TRIANGLES, (end-begin)*3, GL_UNSIGNED_INT, &faces[begin]);    
  }
  
  // deactivate vertex arrays after drawing
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}
