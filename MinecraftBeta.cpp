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

#include "stb_image.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

struct Texture
{
    GLuint textureID;
    int width, height, bitDepth;

    void UseTexture()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    bool LoadTextureA(const char* fileLocation)
    {
        unsigned char* texData = stbi_load(
            fileLocation,
            &width,
            &height,
            &bitDepth,
            0
        );
        if (!texData)
        {
            printf("Failed to find: %s\n", fileLocation);
            return false;
        }

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            texData
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(texData);

        return true;
    }

    bool LoadTexture(const char* fileLocation)
    {
        unsigned char* texData = stbi_load(
            fileLocation,
            &width,
            &height,
            &bitDepth,
            0
        );
        if (!texData)
        {
            printf("Failed to find: %s\n", fileLocation);
            return false;
        }

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            texData
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(texData);

        return true;
    }

    void ClearTexture()
    {
        glDeleteTextures(1, &textureID);
    }
};

struct Mesh
{
    GLuint VAO, VBO, IBO;

    Texture* texture;

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

        /* Offset template: (void*)(sizeof(vertices[0]) * 5) */
        /* Save position coords to the VBO 0. */
        glVertexAttribPointer(
            0, // attribute list
            3, // x, y, z
            GL_FLOAT,
            GL_FALSE,
            sizeof(vertices[0]) * strideLength,
            0
        );
        glEnableVertexAttribArray(0);

        /* Save uv coords to the VBO 1. */
        glVertexAttribPointer(
            1,
            2, // x, y
            GL_FLOAT,
            GL_FALSE,
            sizeof(vertices[0]) * strideLength,
            (void*)(sizeof(vertices[0]) * 3)
        );
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO 
        glBindVertexArray(0); // VAO

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // IBO
    }

    void Render()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        /* Activate texture */
        texture->UseTexture();

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
    /* Initialise window */
    Window mainWindow = Window(SCREEN_WIDTH, SCREEN_HEIGHT);
    mainWindow.Initialise();

    /* Create shaders */
    Shader* shader = new Shader();
    shader->CreateFromFiles(
        "shaders/shader.vert",
        "shaders/shader.frag"
    );


    /* Create meshes */
    Mesh* mesh = new Mesh();
    
    /* Create textures */
    mesh->texture = new Texture();
    mesh->texture->LoadTextureA("textures/brick.png");

    /* Assign vertices */
    mesh->vertices = {
        -0.5f, 0.5f,  0.0f, 0, 0,
        -0.5f, -0.5f, 0.0f, 0, 1,
        0.5f, 0.5f,   0.0f, 1, 0,
        0.5f, -0.5f,  0.0f, 1, 1
    };
    /* How to connect vertices? */
    mesh->indices = { // IBO
        0, 1, 3,
        3, 2, 0
    };
    /* How long is a vertex? */
    mesh->strideLength = 5;

    /* Create a VAO & save it */
    mesh->Create();

    GLenum status{};
    GLenum error{};

    /* Main game loop */
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

    /* Clean up */
    mesh->texture->ClearTexture();
    mesh->CleanUp();

    return 0;
}