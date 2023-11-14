/****************************************************************************
** $Id: qt/glbox.cpp   3.3.6   edited Aug 31 2005 $
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
** The OpenGL code is mostly borrowed from Brian Pauls "spin" example
** in the Mesa distribution
**
****************************************************************************/

#include "glbox.h"

#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif

/*!
  Create a GLBox widget
*/

GLBox::GLBox( QWidget* parent, const char* name )
    : QGLWidget( parent, name )
{
    xRot = yRot = zRot = 0.0;		// default object rotation
    //scale = 1.25;			// default object scale
    scale = 1.0;			// default object scale
    object = 0;
}


/*!
  Release allocated resources
*/

GLBox::~GLBox()
{
    makeCurrent();
    glDeleteLists( object, 1 );
}


/*!
  Paint the box. The actual openGL commands for drawing the box are
  performed here.
*/

void GLBox::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );

    glLoadIdentity();
    
    //glCallList( object );

    glRotatef( xRot, 1.0, 0.0, 0.0 ); 
    glRotatef( yRot, 0.0, 1.0, 0.0 ); 
    glRotatef( zRot, 0.0, 0.0, 1.0 );

    glTranslatef( 0.0, 0.0, -10.0 );
    glScalef( scale, scale, scale );

    drawPaper();
    drawObstacles();
    drawAllBoids();
    updateAllBoids();

}


/*!
  Set up the OpenGL rendering state, and define display list
*/

void GLBox::initializeGL()
{
    qglClearColor( black ); 		// Let OpenGL clear to black

    //buildFont();
    
   // Set up lighting.
   /*
   float light1_ambient[4]  = { 1.0, 1.0, 1.0, 1.0 };
   float light1_diffuse[4]  = { 1.0, 0.9, 0.9, 1.0 };
   float light1_specular[4] = { 1.0, 0.7, 0.7, 1.0 };
   float light1_position[4] = { -1.0, 1.0, 1.0, 0.0 };
   glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
   glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
   glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
   glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
   glEnable(GL_LIGHT1);

   float light2_ambient[4]  = { 0.2, 0.2, 0.2, 1.0 };
   float light2_diffuse[4]  = { 0.9, 0.9, 0.9, 1.0 };
   float light2_specular[4] = { 0.7, 0.7, 0.7, 1.0 };
   float light2_position[4] = { 1.0, -1.0, -1.0, 0.0 };
   glLightfv(GL_LIGHT2, GL_AMBIENT,  light2_ambient);
   glLightfv(GL_LIGHT2, GL_DIFFUSE,  light2_diffuse);
   glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
   glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

   float front_emission[4] = { 0.3, 0.2, 0.1, 0.0 };
   float front_ambient[4]  = { 0.2, 0.2, 0.2, 0.0 };
   float front_diffuse[4]  = { 0.95, 0.95, 0.8, 0.0 };
   float front_specular[4] = { 0.6, 0.6, 0.6, 0.0 };
   glMaterialfv(GL_FRONT, GL_EMISSION, front_emission);
   glMaterialfv(GL_FRONT, GL_AMBIENT, front_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, front_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, front_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 16.0);
   glColor4fv(front_diffuse);

   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
   glColorMaterial(GL_FRONT, GL_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   glEnable(GL_LIGHTING);
   */

    glShadeModel( GL_FLAT );
    
    //defining the boundaries of the paper
    boundaryXMin = -500.0;
    boundaryXMax = 500.0;
    
    boundaryYMin = -500.0;
    boundaryYMax = 500.0;

    //object = drawPaper();		// Generate an OpenGL display list
    setUpFonts();
    loadText("sample.txt");
}


/*!
  Set up the OpenGL view port, matrix mode, etc.
*/

void GLBox::resizeGL( int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
   // glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );
   // glFrustum( -100.0, 100.0, -100.0, 100.0, 5.0, 15.0 );
    glOrtho( -1000.0, 1000.0, -1000.0, 1000.0, -1000, 1000.0 );
    glMatrixMode( GL_MODELVIEW );
}


/*!
  Generate an OpenGL display list for the object to be shown, i.e. the box
*/

GLuint GLBox::drawPaper()
{	
    GLuint list;

//    list = glGenLists( 1 );

 //   glNewList( list, GL_COMPILE );

    qglColor( white );		      // Shorthand for glColor3f or glIndex

    
    glBegin(GL_QUADS);
    glVertex3f(boundaryXMin-200, boundaryYMin-200, 0.0);
    glVertex3f(boundaryXMin-200, boundaryYMax+200, 0.0);
    glVertex3f(boundaryXMax+200, boundaryYMax+200, 0.0);
    glVertex3f(boundaryXMax+200, boundaryYMin-200, 0.0);
    glEnd();

    int numLines = (boundaryYMax-boundaryYMin+400)/50; 
    
    glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_LINES);
    //draw the horizonatal lines
    for(int i=0; i<numLines; i++)
    {
	glVertex3f(boundaryXMin-200, boundaryYMin-200 + i*50, 0.0);
	glVertex3f(boundaryXMax+200, boundaryYMin-200 + i*50, 0.0);
    }

    //draw the vertical line

    	glColor3f(0.75, 0.0, 0.0);

    	glVertex3f(boundaryXMin, boundaryYMin-200, 0.0);
    	glVertex3f(boundaryXMin, boundaryYMax+200, 0.0);

    glEnd();
    
  //  glEndList();

    return list;
}

void GLBox::setUpFonts()
{
	char fontFile[] = "/usr/share/fonts/truetype/freefont/FreeSans.ttf";
	font = new FTGLExtrdFont(fontFile);

	if(font->Error())
		printf("Could not open font:  %s\n",fontFile);
	if(!font->FaceSize(40))
		printf("Failed to set font size\n");

	font->Depth(20);
	
	printf("font setup successful\n");
}

void GLBox::drawObstacles()
{
	Vector3d center1(100.0, 50.0, 0.0);
	Obstacle newObs(center1, 150.0);
	//obstacleList.append(newObs);		
	//obstacleList.append(Obstacle(Vector3d(-150.0, -200.0, 0.0), 100.0));
	
	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);	

	glColor3f(1.0, 0.0, 0.0);

	for(int i=0; i<obstacleList.size(); i++)
	{
		Obstacle obs = obstacleList[i];

		glPushMatrix();
		glTranslatef(obs.center.x, obs.center.y, obs.center.z);
		gluDisk(quadratic, obs.radius-50.0, obs.radius, 32, 32);	
		glPopMatrix();
	}
}

/*!
  Set the rotation angle of the object to \e degrees around the X axis.
*/

void GLBox::setXRotation( int degrees )
{
    xRot = (GLfloat)(degrees % 360);
    updateGL();
}


/*!
  Set the rotation angle of the object to \e degrees around the Y axis.
*/

void GLBox::setYRotation( int degrees )
{
    yRot = (GLfloat)(degrees % 360);
    updateGL();
}


/*!
  Set the rotation angle of the object to \e degrees around the Z axis.
*/

void GLBox::setZRotation( int degrees )
{
    zRot = (GLfloat)(degrees % 360);
    updateGL();
}

void GLBox::buildFont() 
{
    Display *dpy;
    XFontStruct *fontInfo;  // storage for our font.

    base = glGenLists(96);                      // storage for 96 characters.
    
    // load the font.  what fonts any of you have is going
    // to be system dependent, but on my system they are
    // in /usr/X11R6/lib/X11/fonts/*, with fonts.alias and
    // fonts.dir explaining what fonts the .pcf.gz files
    // are.  in any case, one of these 2 fonts should be
    // on your system...or you won't see any text.
    
    // get the current display.  This opens a second
    // connection to the display in the DISPLAY environment
    // value, and will be around only long enough to load 
    // the font. 
    dpy = XOpenDisplay(NULL); // default to DISPLAY env.   

    fontInfo = XLoadQueryFont(dpy, "-*-helvetica-bold-r-normal-*-*-120-*-*-*-*-iso8859-1");
    if (fontInfo == NULL) {
	fontInfo = XLoadQueryFont(dpy, "fixed");
	if (fontInfo == NULL) {
	    fprintf(stderr,"no X font available?\n");
	}
    }

    // after loading this font info, this would probably be the time
    // to rotate, scale, or otherwise twink your fonts.  

    // start at character 32 (space), get 96 characters (a few characters past z), and
    // store them starting at base.
    glXUseXFont(fontInfo->fid, 32, 96, base);

    // free that font's info now that we've got the 
    // display lists.
    XFreeFont(dpy, fontInfo);

    // close down the 2nd display connection.
    XCloseDisplay(dpy);
}

void GLBox::loadText(QString fileName) 
{
	QFile file( fileName );
	if ( file.open( IO_ReadOnly ) ) 
	{
	
		float startX = boundaryXMin + 250;	//start of characters on a line
		float endX = boundaryXMax -200;	//end of characters on a line
		float startY = boundaryYMax;
		float endY = boundaryYMin;
		float nextX = startX;
		float nextY = startY;

		while(!file.atEnd())
		{
			QTextStream stream( &file );
			QString line;
			while ( !stream.atEnd() ) {
				line = stream.readLine(); // line of text excluding '\n'

				nextX = startX;	//position X of next Boid
				nextY -= 50;	//position Y of next Boid

				//display text and create Boids
				for(int j=0; j<line.length(); j++)
				{
					char c = line.at(j).latin1();

					float posX = nextX;
					float posY = nextY;

					nextX += 50.0;

					if((nextX > endX) || (c == '\n'))
					{
						nextX = startX;
						nextY -= 50.0;
					}

					if(c == ' ')
						continue;

					//create a Boid
					Vector3d initialVel(10.0, 10.0, 0.0);
					Boid *b = new Boid(c, posX, posY, 0.0, 
							initialVel.x, initialVel.y, initialVel.z);

					boidList.append(b);

		//			drawRect(posX, posY);
				}
			}
			
		}
		fflush(stdout);
		file.close();
	}
	else
		printf("File not found: %s\n",fileName.latin1());
}

void GLBox::drawRect(QString str, float x, float y, float z, float angle)
{
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();

	glTranslatef(x,y,z);
	//glRotatef(angle, 0.0, 0.0, 1.0);

	font->Render(str.latin1());	
/*
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, 0.0);	
		glVertex3f(40.0, 0.0, 0.0);	
		glVertex3f(20.0, 20.0, 0.0);	
	glEnd();
*/

	glPopMatrix();
}

void GLBox::drawAllBoids()
{
	//draws all the boids
	for(int i=0; i<boidList.count(); i++)
	{
		Boid *b = boidList.at(i);

		float angle;

//		glPushMatrix();
		if(b->vel.length() == 0.0)
			angle = 90.0;
		else
			angle = acos(dot(b->vel,Vector3d(1.0,0.0,0.0))/b->vel.length()) * 180/3.14;
		//printf("rotate angle: %f\n",90 - angle);
//		glRotatef(angle, 0.0, 0.0, 1.0);

		drawRect(QString(QChar(b->c)), b->pos.x, b->pos.y, b->pos.z, 90.0 - angle);
//		glPopMatrix();
	}
	
}

void GLBox::updateAllBoids()
{
	QValueList<Vector3d> newPosList1;
	QValueList<Vector3d> newPosList2;
	QValueList<Vector3d> newVelList3;
	
	//compute new position for all the boids
	int numBoids = boidList.count();
	for(int i=0; i<numBoids; i++)
	{
		newPosList1.append(rule1(i));
		newPosList2.append(rule2(i));
		newVelList3.append(rule3(i));
	}

	//apply the new positions
	for(int i=0; i<numBoids; i++)
	{
		Boid *b;
		b = boidList.at(i);
		
		Vector3d oldPos = b->pos;
		Vector3d newPos1 = newPosList1[i];
		Vector3d newPos2 = newPosList2[i];
		Vector3d newVel3 = newVelList3[i];

		float angle = avoidBoundaries(i);
//		float angle2 = avoidObstacles(i);
	//	printf("steer angles:%f %f\n", angle*180/3.14, angle2*180/3.14);

		//printf("(%f %f %f) (%f %f %f) (%f %f %f)\n", newPos1.x, newPos1.y, newPos1.z,
		//		newPos2.x, newPos2.y, newPos2.z,
		//		newVel3.x, newVel3.y, newVel3.z);
		//printf("(%f %f %f) (%f %f %f)\n\n",b->pos.x, b->pos.y, b->pos.z,
		//		b->vel.x, b->vel.y, b->vel.z);

		//update velocity and position
		//Vector3d vel = b->vel;
		b->vel = b->vel.rotate(angle);
		//b->vel = b->vel.rotate(angle2);

		b->vel = b->vel + newPos1 + newPos2 + newVel3;

	//	b->vel = b->vel + newPos1 + newVel3;
		b->pos = b->pos + b->vel;


//		b->pos = (oldPos + newPos1);
	}
}

//all the boids try to move towards the center of the mass
Vector3d GLBox::rule1( int boidNum )
{
	int numNeighbours = 0;
	int numBoids = boidList.count();
	Vector3d center(0.0,0.0,0.0);

	Boid * b = boidList.at(boidNum);

	for(int i=0; i<numBoids; i++)
	{
		if(i!= boidNum)
		{
			Boid *bI = boidList.at(i);
			Vector3d distVec = b->pos - bI->pos;
			float dist = distVec.length();
			
			if(dist < 200.0)
			{
				center = center + bI->pos;
				numNeighbours ++;
			}
		}
	}

	if(numNeighbours != 0)
	{
		center = center / numNeighbours; 
		center = (center - (b->pos))/20;
		return center;
	}
	else
		return Vector3d(0.0, 0.0, 0.0); //no adjustment is needed
}

Vector3d GLBox::rule2( int boidNum )
{
	int numBoids = boidList.count();
	Vector3d disp(0.0, 0.0, 0.0);

	Boid * b = boidList.at(boidNum);

	//printf("rule2\n");
	for(int i=0; i<numBoids; i++)
	{
		if(i!= boidNum)
		{
			Boid *bI = boidList.at(i);
			Vector3d distVec = b->pos - bI->pos;
			float dist = distVec.length();
		
			if(dist < 50.0)
			{
				Vector3d unitVect = distVec / dist;

	//			printf("%f\n",dist);
				disp =  disp + unitVect * (50-dist);
			}
		}
	}
	//printf("end\n");

	return disp;
}

Vector3d GLBox::rule3( int boidNum )
{
	int numNeighbours = 0;
	int numBoids = boidList.count();
	Vector3d totalVel(0.0, 0.0, 0.0);

	Boid * b = boidList.at(boidNum);

	for(int i=0; i<numBoids; i++)
	{
		if(i!= boidNum)
		{
			Boid *bI = boidList.at(i);
			Vector3d distVec = b->pos - bI->pos;
			float dist = distVec.length();
		
			if(dist < 200.0)
			{
				totalVel = totalVel + bI->vel;
				numNeighbours++;
			}
		}
	}

	if(numNeighbours != 0)
	{
		totalVel = totalVel / numNeighbours;
		return (totalVel - b->vel);
	}
	else
		return Vector3d(0.0, 0.0, 0.0); //no adjustment is needed
}

//returns the center of the flock
Vector3d GLBox::flockCenter()
{
	int numBoids = boidList.count();
	Vector3d center(0.0,0.0,0.0);

	for(int i=0; i<numBoids; i++)
	{
			Boid *b = boidList.at(i);

			center = center + b->pos;
	}

	center = center / numBoids; 

	return center;
}

//finds the bounding box of the flock
void GLBox::findFlockBoundingBox(float *box)
{
	int numBoids = boidList.count();

	float xMin=INT_MAX, xMax=INT_MIN, yMin=INT_MAX, yMax=INT_MIN,
	      zMin=INT_MAX, zMax=INT_MIN;

	for(int i=0; i<numBoids; i++)
	{
		Vector3d pos = boidList.at(i)->pos;
		if(pos.x > xMax)
			xMax = pos.x;
		if(pos.x < xMin)
			xMin = pos.x;

		if(pos.y > yMax)
			yMax = pos.y;
		if(pos.y < yMin)
			yMin = pos.y;

		if(pos.z > zMax)
			zMax = pos.z;
		if(pos.z < zMin)
			zMin = pos.z;
	}

	box[0] = xMin;
	box[1] = xMax;
	box[2] = yMin;
	box[3] = yMax;
	box[4] = zMin;
	box[5] = zMax;

	//printf("bounding box: %f %f %f %f\n",box[0],box[1],box[2],box[3]);
}

//returns a unit vector in the orientation of the flock
//average the velocity vectors
Vector3d GLBox::findFlockOrientation()
{
	int numBoids = boidList.count();
	Vector3d orientation(0.0,0.0,0.0);

	for(int i=0; i<numBoids; i++)
		orientation = orientation + boidList.at(i)->vel;

	//convert to unit vector
	float len = orientation.length();
	orientation = orientation /len;

	return orientation;
}

//steers the flock to avoid a boundary wall
float GLBox::avoidBoundaries(int boidNum)
{
	Boid * b = boidList.at(boidNum);
	float steerAngle = 0.0;

	//for each boundary wall, find if flock is moving towards it
	
	float boidRelDirection;

	//y=Max boundary
	boidRelDirection = dot(b->vel/b->vel.length(), Vector3d(0.0,1.0,0.0));
	//boid in moving towards it, if dot product is greater than 0
	if(boidRelDirection > 0.0)
	{
		//check is the boid is coming close
		if((500.0 - b->pos.y) < 300.0)
		{
			//steer the boid away from the boundary
			steerAngle -= ((90*3.14/180) - acos(boidRelDirection))/10;	
		}
	}

	//x=Max boundary
	boidRelDirection = dot(b->vel/b->vel.length(), Vector3d(1.0,0.0,0.0));
	//boid in moving towards it, if dot product is greater than 0
	if(boidRelDirection > 0.0)
	{
		//check is the boid is coming close
		if((500.0 - b->pos.x) < 300.0)
		{
			//steer the boid away from the boundary
			steerAngle -= ((90*3.14/180) - acos(boidRelDirection))/10;	
		}
	}

	//x=Min boundary
	boidRelDirection = dot(b->vel/b->vel.length(), Vector3d(-1.0,0.0,0.0));
	//boid in moving towards it, if dot product is greater than 0
	if(boidRelDirection > 0.0)
	{
		//check is the boid is coming close
		if(fabs(-500.0 - b->pos.x) < 300.0)
		{
			//steer the boid away from the boundary
			steerAngle -= ((90*3.14/180) - acos(boidRelDirection))/10;	
		}
	}

	//y=Min boundary
	boidRelDirection = dot(b->vel/b->vel.length(), Vector3d(0.0,-1.0,0.0));
	//boid in moving towards it, if dot product is greater than 0
	if(boidRelDirection > 0.0)
	{
		//check is the boid is coming close
		if(fabs(-500.0 - b->pos.y) < 300.0)
		{
			//steer the boid away from the boundary
			steerAngle -= ((90*3.14/180) - acos(boidRelDirection))/10;	
		}
	}

	return steerAngle;
}

//steers a flock to avoid an obstacle
float GLBox::avoidObstacles(int boidNum)
{
	Boid * b = boidList.at(boidNum);

	float steerAngle = 0.0;
	
	for(int i=0; i<obstacleList.size(); i++)
	{
		Obstacle obs = obstacleList[i];

		//find if boid is near the obstacle
		Vector3d vec = obs.center - b->pos;	//vector joining the centers
		float distBetweenCenters = vec.length();
		if(distBetweenCenters < 2*obs.radius)
		{
			float dotProduct = dot(vec, b->vel);
			//find if boid is movind towards the obstacle
			if(dotProduct > 0.0)
			{
				//angle tangent from boid makes with the line joining the center
				//of the obstable
				float tangentAngle = asin(obs.radius/distBetweenCenters);
				//angle between direction of boid and line joining the center
				float directionAngle = acos(dotProduct/(vec.length()*b->vel.length()));

				if(directionAngle < tangentAngle)
					steerAngle += (tangentAngle - directionAngle);
				//printf("obstruction: %f\n", steerAngle * 180/3.14);
			}
		}
	}

	return steerAngle;
}

/*
float GLBox::avoidBoundaries()
{
	//find bounding box of the flock
	float boundingBox[6];
	findFlockBoundingBox(boundingBox);

	//find the flock orientation
	Vector3d flockOrientation = findFlockOrientation();
	//printf("flockOrientation: %f %f %f\n",flockOrientation.x, flockOrientation.y, flockOrientation.z);

	//direction to which the flock velocity should be changed
	Vector3d steerVec(0.0, 0.0, 0.0);

	if(fabs(boundingBox[0]-boundaryXMin) < 200)
		steerVec = steerVec + flockOrientation + Vector3d(0.0, 1.0, 0.0);
		
	if(fabs(boundaryXMax - boundingBox[1]) < 200)
		steerVec = steerVec + flockOrientation + Vector3d(0.0, 1.0, 0.0);

	//if(fabs(boundingBox[2]-boundaryYMin) < 200)
	//	steerVec = steerVec + flockOrientation + Vector3d(1.0, 0.0, 0.0);

	if(fabs(boundaryYMax - boundingBox[3]) < 200)
		steerVec = steerVec + flockOrientation + Vector3d(1.0, 0.0, 0.0);

	//don't need to change orientation
	float length = steerVec.length();
	if(length == 0.0)
		return acos(0.0);

	steerVec = steerVec / steerVec.length();

	//angle by which the flock is to be rotated
	float angle = dot(steerVec, flockOrientation);

	//printf("angle: %f\n",angle);

	return acos(angle);
}
*/

float GLBox::dot(Vector3d vec1, Vector3d vec2)
{
	return (vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z);
}
