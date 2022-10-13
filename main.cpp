#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include"glfwClass.h"

#include"shaderClass.h"
#include"EBO.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

Shader shaderProgram;
VBO VBO;
VAO VAO;
EBO EBO;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const unsigned int WIDTH_PIXELS = 99;
const unsigned int HEIGHT_PIXELS = 99;

//https://stackoverflow.com/questions/63616715/opengl-draw-points-with-different-colors-vertex-location-and-vertex-color-in-di

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    GLFWwindow* window = GLFWClass::StartUp(SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    GLfloat vertices[(WIDTH_PIXELS+1) * (HEIGHT_PIXELS+1) * 3];

    for (int i = 0; i <= HEIGHT_PIXELS; i++)
	{
		for (int j = 0; j <= WIDTH_PIXELS; j++)
		{
			*(vertices + ((int64_t)i * (WIDTH_PIXELS+1) + j) * 3) = (GLfloat)(j) / (WIDTH_PIXELS) * 2 - 1;
			*(vertices + ((int64_t)i * (WIDTH_PIXELS+1) + j) * 3 + 1) = (GLfloat)(i) / (HEIGHT_PIXELS) * 2 - 1;
			*(vertices + ((int64_t)i * (WIDTH_PIXELS+1) + j) * 3 + 2) = 0;
		}
	}
    
    GLuint indices[(WIDTH_PIXELS) * (HEIGHT_PIXELS)*6];
    for (int i = 0; i < HEIGHT_PIXELS; i++)
	{
		for (int j = 0; j < WIDTH_PIXELS; j++)
		{
			*(indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 0) = (i*(WIDTH_PIXELS+1) + j)*3;
			*(indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 1) = (i*(WIDTH_PIXELS+1) + j + 1)*3;
			*(indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 2) = (i*(WIDTH_PIXELS+1) + j + 1 + (WIDTH_PIXELS+1))*3;

            *(indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 3) = (i*(WIDTH_PIXELS+1) + j)*3;
			*(indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 4) = (i*(WIDTH_PIXELS+1) + j + 1 + (WIDTH_PIXELS+1))*3;
			*(indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 5) = (i*(WIDTH_PIXELS+1) + j + (WIDTH_PIXELS+1))*3;
		}
	}


    //void DrawingData::Generate(VBO VBO, const char* vertShader, const char* fragShader, int sizeMult)
    //sizeMult is for EBO 

    // Generates Vertex Buffer Object and links it to vertices
	shaderProgram.Generate("default.txt", "black.txt");
    
    VAO.Generate();
    VBO.Generate();

    VAO.Bind();
    
    std::cout << sizeof(indices)/sizeof(GLuint);

    EBO.Generate(indices, sizeof(indices)/sizeof(GLuint));
    VAO.LinkVBO(VBO, 0, vertices, sizeof(vertices)/sizeof(GLfloat));

    VAO.Unbind();
	EBO.Unbind();

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        // processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        shaderProgram.Activate();
        VAO.Bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glPointSize(2);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLfloat), GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        VAO.Unbind(); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    VAO.Delete();
    VBO.Delete();
    shaderProgram.Delete();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}