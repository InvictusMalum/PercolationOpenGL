#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO();
	void Generate();
	void Regenerate();
	// Binds the VBO
	void Bind(GLfloat* vertices, int length);
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif