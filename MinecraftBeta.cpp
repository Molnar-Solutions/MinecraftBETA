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

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

struct Mesh
{
    GLuint VAO, VBO, IBO;

    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    unsigned int strideLength;

    void Create()
    {
        /* Create object */
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices[0]) * indices.size(),
            &indices.front(),
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertices[0]) * vertices.size(),
            &vertices.front(),
            GL_STATIC_DRAW
        );

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(vertices[0]) * strideLength,
            0
        );
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void Render()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        glDrawElements(
            GL_TRIANGLES,
            indices.size(),
            GL_UNSIGNED_INT,
            0
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void CleanUp()
    {
        glDeleteBuffers(1, &IBO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }
};

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
    Window mainWindow = Window(SCREEN_WIDTH, SCREEN_HEIGHT);
    mainWindow.Initialise();

    Shader* shader = new Shader();
    shader->CreateFromFiles(
        "shaders/shader.vert",
        "shaders/shader.frag"
    );

    /* Create meshes */
    Mesh* mesh = new Mesh();
    mesh->vertices = {
        -0.5f, 0.5f, 0.0f, // 0
        -0.5f, -0.5f, 0.0f, // 1
        0.5f, 0.5f, 0.0f, // 2
        0.5f, -0.5f, 0.0f // 3
    };
    mesh->indices = {
        0, 1, 3,
        3, 2, 0
    };
    mesh->strideLength = 3;
    mesh->Create();

    GLenum status;
    GLenum error;

    while (!mainWindow.GetShouldClose())
    {
        PreDraw(status, error);

        shader->UseShader();
        shader->Validate();

        /* Render elements */
        mesh->Render();

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    mesh->CleanUp();

    return 0;
}