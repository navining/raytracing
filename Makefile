# Makefile for raytracing
# Author: Navi Ning (xning5@illinois.edu)
# Date: 2020/05/10

CPP=g++
CPPFLAGS=-O3 -fopenmp 

all: raytracing

smallpt: raytracing.cpp 
	$(CPP) $(CPPFLAGS) -o raytracing raytracing.cpp 

clean:
	-rm raytracing result.ppm 

