#ifndef SCREEN_H
#define SCREEN_H

//clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//clang-format on

class Screen {
public:
  static unsigned int SCR_WIDTH;
  static unsigned int SCR_HEIGHT;

  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
};

#endif