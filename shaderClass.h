#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>



class Shader
{
	public:
		GLuint ID;
		const char* vertexSource;
		const char* fragmentSource;

		std::string get_file_contents(const char* filename);

		Shader();

		void Generate(const char* vertexFile, const char* fragmentFile);
		void Regenerate(const char* vertexFile, const char* fragmentFile);

		void Activate();
		void Delete();
};


#endif