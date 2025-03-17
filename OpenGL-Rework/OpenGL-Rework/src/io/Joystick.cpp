#include "Joystick.h"
#include <iostream>

Joystick::Joystick(int i) {
  id = getId(i);

  update();
}

void Joystick::update() {
  present = glfwJoystickPresent(id);
  if (present) {
    name = glfwGetJoystickName(id);
    axes = glfwGetJoystickAxes(id, &axesCount);
    buttons = glfwGetJoystickButtons(id, &buttonCount);
  }
}

float Joystick::axesState(int axies) {
  if (present) {
    return axes[axies];
  }
  return 0.0f;
}

unsigned char Joystick::buttonState(int button) {
  if (present) {
    return buttons[button];
  }
}

int Joystick::getAxesCount() { return axesCount; }

int Joystick::getButtonCount() { return buttonCount; }

bool Joystick::isPresent() { return present; }

const char *Joystick::getName() { return name; }

int Joystick::getId(int i) {
  switch (i) {
  case 0:
    return GLFW_JOYSTICK_1;
  default:
    return -1;
  };
}
