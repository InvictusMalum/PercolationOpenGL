#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>

#include"glfwClass.h"

#include"shaderClass.h"
#include"EBO.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const unsigned int WIDTH_PIXELS = 99;
const unsigned int HEIGHT_PIXELS = 99;

const float p = .20;

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

    float percentage_values[HEIGHT_PIXELS][WIDTH_PIXELS];
    for (int i = 0; i < HEIGHT_PIXELS; i++)
	{
		for (int j = 0; j < WIDTH_PIXELS; j++)
		{
            percentage_values[i][j] = (rand() % 100) / (float)100;
        }
    }

    int grid_values[HEIGHT_PIXELS][WIDTH_PIXELS];
    for (int i = HEIGHT_PIXELS-1; i >= 0; i--)
	{
		for (int j = 0; j < WIDTH_PIXELS; j++)
		{
            if (percentage_values[i][j] < p) {
                grid_values[i][j] = 1;
            } else {
                if (i == HEIGHT_PIXELS-1) {
                    grid_values[i][j] = 2;
                } else if (grid_values[i+1][j] == 2) {
                    grid_values[i][j] = 2;
                } else {
                    grid_values[i][j] = 0;
                }
            }
        }
    }

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
    
    GLuint closed_indices[(WIDTH_PIXELS) * (HEIGHT_PIXELS)*6];
    for (int i = 0; i < HEIGHT_PIXELS; i++)
	{
		for (int j = 0; j < WIDTH_PIXELS; j++)
		{
            if (grid_values[i][j] == 1) {
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 0) = (i*(WIDTH_PIXELS+1) + j)*3;
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 1) = (i*(WIDTH_PIXELS+1) + j + 1)*3;
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 2) = (i*(WIDTH_PIXELS+1) + j + 1 + (WIDTH_PIXELS+1))*3;

                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 3) = (i*(WIDTH_PIXELS+1) + j)*3;
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 4) = (i*(WIDTH_PIXELS+1) + j + 1 + (WIDTH_PIXELS+1))*3;
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 5) = (i*(WIDTH_PIXELS+1) + j + (WIDTH_PIXELS+1))*3;
            }
		}
	}

    GLuint liquid_indices[(WIDTH_PIXELS) * (HEIGHT_PIXELS)*6];
    for (int i = 0; i < HEIGHT_PIXELS; i++)
	{
		for (int j = 0; j < WIDTH_PIXELS; j++)
		{
            if (grid_values[i][j] == 2) {
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 0) = (i*(WIDTH_PIXELS+1) + j)*3;
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 1) = (i*(WIDTH_PIXELS+1) + j + 1)*3;
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 2) = (i*(WIDTH_PIXELS+1) + j + 1 + (WIDTH_PIXELS+1))*3;

                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 3) = (i*(WIDTH_PIXELS+1) + j)*3;
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 4) = (i*(WIDTH_PIXELS+1) + j + 1 + (WIDTH_PIXELS+1))*3;
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 5) = (i*(WIDTH_PIXELS+1) + j + (WIDTH_PIXELS+1))*3;
            }
		}
	}

    Shader closedShader;
    Shader liquidShader;
    VBO vbo;

    VAO closedVAO;
    EBO closedEBO;

    VAO liquidVAO;
    EBO liquidEBO;

    vbo.Generate();

    // Generates Vertex Buffer Object and links it to vertices
	closedShader.Generate("default.txt", "closed.txt");
    liquidShader.Generate("default.txt", "liquid.txt");
    
    closedVAO.Generate();
    closedVAO.Bind();
    closedEBO.Generate(closed_indices, sizeof(closed_indices)/sizeof(GLuint));
    closedVAO.LinkVBO(vbo, 0, vertices, sizeof(vertices)/sizeof(GLfloat));

    closedVAO.Unbind();
	closedEBO.Unbind();

    liquidVAO.Generate();
    liquidVAO.Bind();
    liquidEBO.Generate(liquid_indices, sizeof(liquid_indices)/sizeof(GLuint));
    liquidVAO.LinkVBO(vbo, 0, vertices, sizeof(vertices)/sizeof(GLfloat));

    liquidVAO.Unbind();
	liquidEBO.Unbind();

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        // processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.4f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        closedShader.Activate();
        closedVAO.Bind();
        glPointSize(2);
        glDrawElements(GL_TRIANGLES, sizeof(closed_indices)/sizeof(GLfloat), GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        closedVAO.Unbind();

        liquidShader.Activate();
        liquidVAO.Bind();
        glPointSize(2);
        glDrawElements(GL_TRIANGLES, sizeof(liquid_indices)/sizeof(GLfloat), GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        liquidVAO.Unbind();
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    closedVAO.Delete();
    closedEBO.Delete();
    closedShader.Delete();

    liquidVAO.Delete();
    liquidEBO.Delete();
    liquidShader.Delete();

    vbo.Delete();

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