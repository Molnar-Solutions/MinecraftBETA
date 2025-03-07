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
#include "Camera.h"
#include "EntityShader.h"
#include "TerrainShader.h"
#include "Chunk.h"

Camera* camera;

/* Shaders */
EntityShader* entityShader;
TerrainShader* terrainShader;

Chunk* chunk;

double deltaTime = 0.0f;
double lastTime = 0.0f;

/* Entities */
std::vector<Model*> entities;

/* Uniforms */
glm::mat4 transformationMatrix(1.0f);
glm::mat4 projectionMatrix(1.0f);
glm::mat4 viewMatrix(1.0f);

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

    /* Initialization */
    GLfloat startCameraMoveSpeed = 5.0f;
    GLfloat startCameraTurnSpeed = 0.25f;

    camera = new Camera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f, 0.0f, startCameraMoveSpeed, startCameraTurnSpeed
    );

    /* Create shaders */
    entityShader = new EntityShader();
    entityShader->CreateFromFiles(
        "shaders/shader.vert",
        "shaders/shader.frag"
    );

    terrainShader = new TerrainShader();
    terrainShader->CreateFromFiles(
        "shaders/terrainShader.vert",
        "shaders/terrainShader.frag"
    );

	/* Create chunk */
	chunk = new Chunk(0.0f, 0.0f);

    /* Add some models */
    Model* tree = new Model();
    tree->LoadModel("Models/Box/Box.obj", "Box");
    tree->setPosition(0.0f, -2.15f, -0.5f);
    tree->setScale(1.0f, 1.0f, 1.0f);
    tree->calculateBoundingBox();
    entities.push_back(tree);

    GLenum status{};
    GLenum error{};

    projectionMatrix = glm::perspective(glm::radians(86.0f),
        (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(),
        0.1f, 100.0f);

    /* Main game loop */
    while (!mainWindow.GetShouldClose())
    {
        GLfloat now = glfwGetTime(); // SDL: SDL_GetPerformanceCounter();
        deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
        lastTime = now;

        PreDraw(status, error);

        camera->keyControl(mainWindow.getKeys(), deltaTime);
        camera->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        viewMatrix = camera->calculateViewMatrix();



		/* Render entities */
        entityShader->UseShader();
        entityShader->Validate();

        entityShader->SetProjectionMatrix(projectionMatrix);
        entityShader->SetViewMatrix(viewMatrix);

        for (auto e : entities)
        {
            transformationMatrix = glm::mat4(1.0f);
            transformationMatrix = glm::translate(transformationMatrix, tree->GetPosition());
            transformationMatrix = glm::scale(transformationMatrix, tree->GetScale());

            entityShader->SetTransformationMatrix(transformationMatrix);

            tree->RenderModel();
        }

        entityShader->StopShader();



		/* Render terrain */
        terrainShader->UseShader();
		terrainShader->Validate();

		terrainShader->SetProjectionMatrix(projectionMatrix);
		terrainShader->SetViewMatrix(viewMatrix);
        terrainShader->SetSideTexture(1);
		terrainShader->SetTopTexture(2);

		chunk->Render();

        terrainShader->StopShader();

		/* Swap buffers */
        mainWindow.swapBuffers();
    }

    return 0;
}