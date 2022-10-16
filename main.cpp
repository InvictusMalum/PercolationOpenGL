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

const unsigned int WIDTH_PIXELS = 199;
const unsigned int HEIGHT_PIXELS = 199;

float p = .40;
bool renderClosed = false;

GLfloat vertices[(WIDTH_PIXELS+1) * (HEIGHT_PIXELS+1) * 3];
GLuint liquid_indices[(WIDTH_PIXELS) * (HEIGHT_PIXELS)*6];
GLuint closed_indices[(WIDTH_PIXELS) * (HEIGHT_PIXELS)*6];

float percentage_values[HEIGHT_PIXELS][WIDTH_PIXELS];
int *grid_values[HEIGHT_PIXELS];

Shader closedShader;
Shader liquidShader;
VBO vbo;

VAO closedVAO;
EBO closedEBO;

VAO liquidVAO;
EBO liquidEBO;

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

void SpreadDown(int i, int j, int num, int** grid, int grid_width) {
    if (grid[i][j] == num && i > 0 && grid[i-1][j] == 0) {
        grid[i-1][j] = num;
        SpreadDown(i-1, j, num, grid, grid_width);
    }
    if (grid[i][j] == num && j > 0 && grid[i][j-1] == 0) {
        grid[i][j-1] = num;
        SpreadDown(i, j-1, num, grid, grid_width);
    }
    if (grid[i][j] == num && j < grid_width-1 && grid[i][j+1] == 0) {
        grid[i][j+1] = num;
        SpreadDown(i, j+1, num, grid, grid_width);
    }
}

void Render(GLFWwindow* window) {
    //Fill initial grid based on percent values and p
    for (int i = HEIGHT_PIXELS-1; i >= 0; i--)
    {
        grid_values[i] = new int[WIDTH_PIXELS];
        for (int j = 0; j < WIDTH_PIXELS; j++)
        {
            if (percentage_values[i][j] < p) {
                grid_values[i][j] = 1;
            } else {
                if (i == HEIGHT_PIXELS-1) {
                    grid_values[i][j] = 2;
                } else {
                    grid_values[i][j] = 0;
                }
            }
        }
    }

    //Percolate Grid
    for (int j = 0; j < WIDTH_PIXELS; j++)
    {
        SpreadDown(HEIGHT_PIXELS-1,j,2,grid_values,WIDTH_PIXELS);
    }

    //Generate closed nodes based
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
            } else {
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 0) = 0;
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 1) = 0;
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 2) = 0;

                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 3) = 0;
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 4) = 0;
                *(closed_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 5) = 0;
            }
        }
    }

    //Generate liquid nodes
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
            } else {
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 0) = 0;
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 1) = 0;
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 2) = 0;

                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 3) = 0;
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 4) = 0;
                *(liquid_indices + ((int64_t)i * WIDTH_PIXELS + j) * 6 + 5) = 0;
            }
        }
    }
    
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

    glClearColor(0.15f, 0.2f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (renderClosed) {
        closedShader.Activate();
        closedVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(closed_indices)/sizeof(GLfloat), GL_UNSIGNED_INT, 0);
        closedVAO.Unbind();
    }

    liquidShader.Activate();
    liquidVAO.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(liquid_indices)/sizeof(GLfloat), GL_UNSIGNED_INT, 0);
    liquidVAO.Unbind();

    closedVAO.Delete();
    closedEBO.Delete();
    closedShader.Delete();

    liquidVAO.Delete();
    liquidEBO.Delete();
    liquidShader.Delete();

    vbo.Delete();

    glfwSwapBuffers(window);
}

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

    //Fill vertices
    for (int i = 0; i <= HEIGHT_PIXELS; i++)
	{
		for (int j = 0; j <= WIDTH_PIXELS; j++)
		{
			*(vertices + ((int64_t)i * (WIDTH_PIXELS+1) + j) * 3) = (GLfloat)(j) / (WIDTH_PIXELS) * 2 - 1;
			*(vertices + ((int64_t)i * (WIDTH_PIXELS+1) + j) * 3 + 1) = (GLfloat)(i) / (HEIGHT_PIXELS) * 2 - 1;
			*(vertices + ((int64_t)i * (WIDTH_PIXELS+1) + j) * 3 + 2) = 0;
		}
	}

    int space_cool = 0;
    int p_cool = 0;
    int closed_cool = 0;
    bool initial = true;
    while (!glfwWindowShouldClose(window))
    {
        if (space_cool > 0) {
            space_cool--;
        }
        if (p_cool > 0) {
            p_cool--;
        }
        if (closed_cool > 0) {
            closed_cool--;
        }
        if (initial || (glfwGetKey(window, GLFW_KEY_SPACE) && space_cool == 0)) {
            space_cool = 50000;
            //Fill percent values
            for (int i = 0; i < HEIGHT_PIXELS; i++)
            {
                for (int j = 0; j < WIDTH_PIXELS; j++)
                {
                    percentage_values[i][j] = (rand() % 1000) / (float)1000;
                }
            }
            initial = false;

            Render(window);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) && p_cool == 0) {
            p += .001;
            if (p > 1) {
                p = 1;
            }
            p_cool = 1000;
            Render(window);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) && p_cool == 0) {
            p -= .001;
            if (p < 0) {
                p = 0;
            }
            p_cool = 1000;
            Render(window);
        }
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_LEFT)) && closed_cool == 0) {
            renderClosed = !renderClosed;
            closed_cool = 50000;
            Render(window);
        }
        processInput(window);
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