#include"shaderClass.h"

const char *vertexSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *fragmentSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";


// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader()
{

}

void Shader::Generate(const char* vertexFile, const char* fragmentFile) {
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
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

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