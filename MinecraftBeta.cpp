#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "Window.h"

int main()
{
    Window mainWindow = Window(1200, 800);
    mainWindow.Initialise();

    while (!mainWindow.GetShouldClose())
    {
        glfwPollEvents();

        mainWindow.swapBuffers();
    }

    return 0;
}