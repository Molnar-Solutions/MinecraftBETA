#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint width,
		GLint height);

	int Initialise();

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();

	bool GetShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}
	bool* getKeys() { return keys; }

	void swapBuffers()
	{
		glfwSwapBuffers(mainWindow);
	}

	~Window();
private:
	GLFWwindow* mainWindow;

	GLint width, height,
		bufferWidth, bufferHeight;

	bool keys[1024];
	bool mousedFirstMoved;

	GLfloat lastX, lastY, xChange, yChange;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code,
		int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};