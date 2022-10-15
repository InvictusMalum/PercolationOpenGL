#include"VAO.h"

// Constructor that generates a VAO ID
VAO::VAO()
{
	
}

void VAO::Generate()
{
	glGenVertexArrays(1, &ID);
}

void VAO::Regenerate()
{
	glGenVertexArrays(1, &ID);
}

// Binds the VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkVBO(VBO& VBO, GLuint layout, GLfloat* vertices, int length)
{
	VBO.Bind(vertices, length);
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Unbinds the VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}