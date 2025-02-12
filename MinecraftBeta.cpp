#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "Shader.h"
#include "Window.h"

#include "CommonValues.h"
#include "Model.h"

Shader* shader = new Shader();

double deltaTime = 0.0f;
double lastTime = 0.0f;

std::vector<Model*> entities;
glm::mat4 transformationMatrix(1.0f);

void PreDraw(GLenum status, GLenum error)
{
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glClearColor(0.89f, 0.25f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwPollEvents();

    /* Hunting for errors */
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer incomplete: %d\n", status);
    }

    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL Error: %d\n", error);
    }
}

int main()
{
    /* Initialise window */
    Window mainWindow = Window(SCREEN_WIDTH, SCREEN_HEIGHT);
    mainWindow.Initialise();

    /* Create shaders */
    shader->CreateFromFiles(
        "shaders/shader.vert",
        "shaders/shader.frag"
    );

    /* Add some models */
    Model* tree = new Model();
    tree->LoadModel("Models/Tree/Tree.obj", "Tree");
    tree->setPosition(0.0f, -2.15f, -0.5f);
    tree->setScale(1.0f, 1.0f, 1.0f);
    tree->calculateBoundingBox();
    entities.push_back(tree);

    GLenum status{};
    GLenum error{};

    /* Main game loop */
    while (!mainWindow.GetShouldClose())
    {
        GLfloat now = glfwGetTime(); // SDL: SDL_GetPerformanceCounter();
        deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
        lastTime = now;

        PreDraw(status, error);

        shader->UseShader();
        shader->Validate();

        /* Render entites */
        for (auto e : entities)
        {
            GLuint transformationMatrixLocation = shader->GetTransformationMatrixLocation();

            transformationMatrix = glm::mat4(1.0f);
            transformationMatrix = glm::translate(transformationMatrix, tree->GetPosition());
            transformationMatrix = glm::scale(transformationMatrix, tree->GetScale());

            glUniformMatrix4fv(transformationMatrixLocation, 1,
                GL_FALSE, glm::value_ptr(transformationMatrix));

            tree->RenderModel();
        }

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}