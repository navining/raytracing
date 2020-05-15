# Makefile for raytracing
# Author: Navi Ning (xning5@illinois.edu)

CPP=g++
CPPFLAGS=-O3

all: raytracing

smallpt: raytracing.cpp 
	$(CPP) $(CPPFLAGS) -o raytracing raytracing.cpp 

clean:
	-rm raytracing result.* 
