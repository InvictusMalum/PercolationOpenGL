#include"shaderClass.h"

std::string Shader::get_file_contents(const char* filename) {
	std::string stringOut;

	std::string line;
	std::ifstream file(filename);
	if (file.is_open())
	{
		while ( getline (file,line) )
		{
			stringOut += line + '\n';
		}
		file.close();
	}
	else std::cout <<"Unable to open file";
	return stringOut;
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader()
{

}

void Shader::Generate(const char* vertexFile, const char* fragmentFile) {
	// create strings to store the code
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	vertexSource = vertexCode.c_str();
	fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the Vertex Shader source to the Vertex Shader Object 
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader (into machine code)
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach the Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Fragment Shader (into machine code)
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap up and link all the shaders together into the Shader Program
	glLinkProgram(ID);

	// Delete the useless shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}// Activates the Shader Program

void Shader::Regenerate(const char* vertexFile, const char* fragmentFile) {
	// // Read vertexFile and fragmentFile and store the strings
	// std::string vertexCode = get_file_contents(vertexFile);
	// std::string fragmentCode = get_file_contents(fragmentFile);

	// // Convert the shader source strings into character arrays
	// const char* vertexSource = vertexCode.c_str();
	// const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the Vertex Shader source to the Vertex Shader Object 
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader (into machine code)
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach the Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Fragment Shader (into machine code)
	glCompileShader(fragmentShader);

	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap up and link all the shaders together into the Shader Program
	glLinkProgram(ID);

	// Delete the useless shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}// Activates the Shader Program

void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}