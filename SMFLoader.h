#ifndef SMFLOADER_H
#define SMFLOADER_H

/*
   一個vertex 3個float  一個size
   若有3個vertex  vertexSize 為3  但是vertex中所有的float為9

   face也是一樣    */

class SMFLoader
{
public:
	SMFLoader();
	void load(char* filename);
	void clear();
	float* getVertices();
	int* getFaces();
	float* getColors();

	float* getFace(int index);
	float* getFaceColor(int index);

	int getVertexSize();
	int getFaceSize();
	int getColorSize();
private:
	float* vertex;
	int* face;
	float* color;

	int vertexSize;
	int faceSize;
	int colorSize;
};

#endif#pragma once
