TEMPLATE	= app
TARGET		= box

CONFIG		+= qt opengl warn_on release
CONFIG -= dlopen_opengl

DEPENDPATH	= ../include

REQUIRES        = opengl

HEADERS		= glbox.h \
		  globjwin.h \
		  Boid.h \
		  Vector3d.h \
		  Obstacle.h

SOURCES		= glbox.cpp \
		  globjwin.cpp \
		  Boid.cpp \
		  Vector3d.cpp \
		  Obstacle.cpp \
		  main.cpp
