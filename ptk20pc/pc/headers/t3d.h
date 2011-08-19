/** \file t3d.h
 *  \version 1.0
 *  \author Jean-Yves Lamoureux <jylam@lnxscene.org>
 *  \brief Tiny 3d file loader and animator
 *
 *  This file contains data structures and definitions of the 3d engine
 */
/*
  (c) 2005 Jean-Yves Lamoureux.
      All rights reserved.
*/

#include	"KGraphic.h"

#ifndef __T3D_H_
#define __T3D_H_
#include <GL/gl.h>
#ifndef PI
# define PI 3.14159265358979323846
#endif

typedef struct _sColor
{
  float r;
  float g;
  float b;
  float a;

}sColor;

typedef struct _sMaterial
{
  int isTextured;
  char *filename;
  KGraphic	*kgraphicPtr ;
  GLuint textureId;
  sColor color;
}sMaterial;

typedef struct _sVertex
{
  float x,y,z;     
  float nx,ny,nz;   
  float u, v; 

}sVertex;

typedef struct _sFace
{
  int a,b,c;
  sVertex normal;
  sVertex an, bn, cn;
  int materialId;

}sFace;

typedef struct _sObject
{
  int vertexCount;
  sVertex *vertex;
  int faceCount;
  sFace *faces;
  sVertex bbox[2];

}sObject;

typedef struct _sCamera
{
  sVertex position;
  sVertex rotation;
}sCamera;

typedef struct _sScene
{
  int objectCount;
  sObject *objects;
  int materialCount;
  sMaterial *material;

  sVertex bbox[2];
  sVertex center;

  float originalScale;

  int firstFrame;
  int lastFrame;
  int frameSpeed;

  sVertex position;
  sVertex rotation;

  sCamera camera;


  int clearZBuffer;
  int clearPixelBuffer;
  int allowLights;

  
}sScene;
void T3dManageTransformations(sScene *scene);

/*! \fn sScene *T3dLoadASEObject(char *filename); 
  \brief Loads an ASE file and corresponding textures
  \param filename the name of the file
  \return sScene*
  \sa T3dDisplayObjectByTime T3dDisplayObjectByFrame
    */
sScene *T3dLoadASEObject(char *filename);


/*! \fn sScene *T3dLoadObject(char *filename); 
  \brief Loads a T3d file and corresponding textures
  \param filename the name of the file
  \return sScene*
  \sa T3dDisplayObjectByTime T3dDisplayObjectByFrame
    */
sScene *T3dLoadObject(char *filename);


/*! \fn void T3dDisplayObjectByTime(sScene *scene, float p_x, float p_y, float width, float height, float time);
  \brief Displays animated object
  \param scene sScene structure returned by T3dLoadObject()
  \param p_x X coordinate of sprite
  \param p_y Y coordinate of sprite
  \param width Width of sprite
  \param height Height of sprite
  \param time Animation seek, between 0 and 1 
  \return nothing
  \sa T3dLoadASEObject
    */
void T3dDisplayObjectByTime(sScene *scene, float p_x, float p_y, float width, float height, float time);
/*! \fn void T3dDisplayObjectByFrame(sScene *scene, float p_x, float p_y, float width, float height, int frame);
  \brief Displays animated object
  \param scene sScene structure returned by T3dLoadObject()
  \param p_x X coordinate of sprite
  \param p_y Y coordinate of sprite
  \param width Width of sprite
  \param height Height of sprite
  \param frame Frame number
  \return nothing
  \sa T3dLoadASEObject
    */
void T3dDisplayObjectByFrame(sScene *scene, float p_x, float p_y, float width, float height, int frame);


/*! \fn void T3dGetBBox(sScene *scene);
  \brief Calculate smallest bounding box
  \param scene sScene structure returned by T3dLoadObject()
  \return nothing
  \sa T3dLoadASEObject T3dDrawBBox
    */
void T3dGetBBox(sScene *scene);

/*! \fn  void T3dNormalizeObject(sScene *scene)
  \brief Rescale object to 0..1 range
  \param scene sScene structure returned by T3dLoadObject()
  \return nothing
  \sa T3dLoadASEObject T3dGetBBox
    */
void T3dNormalizeObject(sScene *scene);

/*! \fn void T3dDrawBBox(sScene *scene, int obj);
  \brief Draw bounding box of a specific animation step
  \param scene sScene structure returned by T3dLoadObject()
  \param obj Object ID
  \return nothing
  \sa T3dLoadASEObject T3dGetBBox
    */
void T3dDrawBBox(sScene *scene, int obj);
/*! \fn void T3dFree(sScene *scene);
  \brief Free previously loaded scene
  \param scene sScene structure returned by T3dLoadObject()
  \return nothing
  \sa T3dLoadASEObject
    */
void T3dFree(sScene *scene);

/*! \fn int T3dSaveObject(const char *filename, sScene *scene);
  \brief Save scene in T3D file (smaller, faster,and safer than ASE)
  \param filename file to write. Will be overwritten.
  \param scene sScene structure returned by T3dLoadObject()
  \return 0 if it failed, 1 otherwise.
  \sa T3dLoadASEObject
    */
int T3dSaveObject(const char *filename, sScene *scene);


/*private*/
char *getTextureName(char *str);
void gotoChunk(const char *chunk);
void normalizeVector(sVertex *vect);
inline float  interpLinear(float start, float end, float timeseek);
inline float interpCosine(float start, float end, float timeseek);
inline void interVerticeCosine(sVertex *v1, sVertex *v2, sVertex *out, float timeseek);
inline float initInterpCosine(float timeseek);
inline void interTexCoord(float u1, float v1, float u2, float v2, float *ru, float *rv, float timeseek);
char *getBinaryTextureName(FILE *fp);
void Fread32(void  *a, FILE *fp);
#endif /*__T3D_H_*/


/*! \class  T3d
  \brief T3d class
*/
class T3d
{
public:
/*! \fn  T3d::T3d (char*);
  \brief Loads an ASE or T3D file and corresponding textures
  \param filename the name of the file
  \return object
    */
  T3d (char*);
/*! \fn T3d::~T3d ();
  \brief Free previously loaded scene
  \return nothing
  \sa T3d
    */
  ~T3d ();
/*! \fn int T3d::saveObject(const char *filename);
  \brief Save scene in T3D file (smaller, faster,and safer than ASE)
  \param filename file to write. Will be overwritten.
  \return 0 if it failed, 1 otherwise.
  \sa T3d
    */
  int saveObject(const char *filename);
/*! \fn int T3d::getFrameCount(void);
  \brief Get scene frame count (ie object count)
  \return frame count
  \sa T3d
    */
  int getFrameCount(void);
/*! \fn  void T3d::displayObjectByTime (float p_x, float p_y, float width, float height, float time);
  \brief Displays animated object
  \param p_x X coordinate of sprite
  \param p_y Y coordinate of sprite
  \param width Width of sprite
  \param height Height of sprite
  \param time Animation seek, between 0 and 1 
  \return nothing
  \sa T3d
    */
  void displayObjectByTime (float p_x, float p_y, float width, float height, float time);
/*! \fn  void T3d::displayObjectByFrame (float p_x, float p_y, float width, float height, int frame);
  \brief Displays animated object
  \param p_x X coordinate of sprite
  \param p_y Y coordinate of sprite
  \param width Width of sprite
  \param height Height of sprite
  \param frame frame number
  \return nothing
  \sa T3d
    */
  void displayObjectByFrame(float p_x, float p_y, float width, float height, int frame);
/*! \fn  void T3d::displayBBox(int obj);
  \brief Draw bounding box of a specific animation step
  \param obj Object ID
  \return nothing
  \sa T3d
    */
  void displayBBox(int obj);

/*! \fn  float T3d::getOriginalScale(void);
  \brief Get scale of object (as read in the file)
  \return scale
  \sa T3d
    */
  float getOriginalScale(void);

/*! \fn  float T3d::setPosition(float x, float y, float z);
  \brief Set position of the object
  \param x X coordinate
  \param y Y coordinate
  \param z Z coordinate
  \sa T3d
    */
  void setPosition(float x, float y, float z);
/*! \fn  float T3d::setRotation(float x, float y, float z);
  \brief Set rotation of the object
  \param x X angle
  \param y Y angle
  \param z Z angle
  \sa T3d
    */
  void setRotation( float x,float  y, float z);
/*! \fn  float T3d::setCameraPosition(float x, float y, float z);
  \brief Set position of the camera
  \param x X coordinate
  \param y Y coordinate
  \param z Z coordinate
  \sa T3d
    */
  void setCameraPosition(float x, float y, float z);
/*! \fn  float T3d::setCameraRotation(float x, float y, float z);
  \brief Set rotation of the camera
  \param x X angle
  \param y Y angle
  \param z Z angle
  \sa T3d
    */
  void setCameraRotation( float x,float  y, float z);
/*! \fn  void T3d::setZBufferClear(bool prop);
  \brief Set whenever the zbuffer must be cleared before drawing or not
  \param prop true or false
  \sa T3d
    */
  void setZBufferClear(bool prop);
/*! \fn  void T3d::setPixelBufferClear(bool prop);
  \brief Set whenever the pixel buffer must be cleared before drawing or not
  \param prop true or false
  \sa T3d
    */
  void setPixelBufferClear(bool prop);
/*! \fn  void T3d::setLighting(bool prop);
  \brief Set whenever the object will be affected by light or not
  \param prop true or false
  \sa T3d
    */
  void setLighting(bool prop);
  

private:
  sScene *scene;
};

