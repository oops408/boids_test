/****************************************************************************
** $Id: qt/glbox.h   3.3.6   edited Aug 31 2005 $
**
** Copyright (C) 1992-2005 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

/****************************************************************************
**
** This is a simple QGLWidget displaying an openGL wireframe box
**
****************************************************************************/

#ifndef GLBOX_H
#define GLBOX_H

#include <qgl.h>
#include <qfile.h>
#include <GL/glx.h>
#include <qptrlist.h>
#include <qvaluelist.h>
#include <math.h>

#include <FTGL/FTGLExtrdFont.h>

using namespace std;

#include "Boid.h"
#include "Vector3d.h"
#include "Obstacle.h"

class GLBox : public QGLWidget
{
    Q_OBJECT

public:

    GLBox( QWidget* parent, const char* name );
    ~GLBox();
    void		loadText(QString str);
    void drawRect(QString str, float x, float y, float z, float angle);
    void drawAllBoids();
    Vector3d rule1(int boidNum);
    Vector3d rule2(int boidNum);
    Vector3d rule3(int boidNum);
    void updateAllBoids();
    Vector3d flockCenter();
    void findFlockBoundingBox(float *box);
    Vector3d findFlockOrientation();
    float avoidBoundaries(int boidNum);
    float avoidObstacles(int boidNum);
    float dot(Vector3d vec1, Vector3d vec2);
    void setUpFonts();

public slots:

    void		setXRotation( int degrees );
    void		setYRotation( int degrees );
    void		setZRotation( int degrees );
    void 		buildFont();
    void drawObstacles();

protected:

    void		initializeGL();
    void		paintGL();
    void		resizeGL( int w, int h );

    virtual GLuint 	drawPaper();

private:

    GLuint object;
    GLuint base;
    GLfloat xRot, yRot, zRot, scale;

    //boundaries of the paper
    float boundaryXMin, boundaryXMax, boundaryYMin, boundaryYMax;

    //list of boids
    QPtrList<Boid> boidList;

    //list of obstacles
    QValueList<Obstacle> obstacleList;

    FTGLExtrdFont *font;
};


#endif // GLBOX_H
