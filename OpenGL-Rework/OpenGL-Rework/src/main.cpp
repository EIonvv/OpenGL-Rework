#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/model.h"
#include "graphics/light.h"

#include "graphics/models/cube.hpp"

#include "graphics/models/lamp.hpp"

#include "io/keyboard/keyboard.h"
#include "io/mouse/mouse.h"
#include "io/joystick/joystick.h"
#include "io/screen/screen.h"
#include "io/camera/camera.h"

void processInput(double deltaTime);

float mixVal = 0.5f;

Screen screen;

Joystick mainJ(0);
Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 3.0f));

double deltaTime = 0.0f; // tme btwn frames
double lastFrame = 0.0f; // time of last frame

int main() {
	int success;
	char infoLog[512];

	std::cout << "Hello, OpenGL!" << std::endl;

	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif

	if( !screen.init() ) {
		std::cout << "Could not open window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();

	// SHADERS ===============================
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/object.vs", "assets/lamp.fs");
	// End of SHADERS ===============================

	// MODELS ==============================
	Cube cube(Material::mix(Material::emerald, Material::gold, 0.7), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	cube.init();

	//DirLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(0.4f), glm::vec3(0.75f) };
	// End of MODELS ==============================
	Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(-1.0f, -0.5f, 0.5f), glm::vec3(0.25f));
	lamp.init();
	
	SpotLight spotLight = {
		Camera::defaultCamera.cameraPos,Camera::defaultCamera.cameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)
	};
	// End of MODELS ==============================

	mainJ.update();
	if( mainJ.isPresent() ) {
		std::cout << mainJ.getName() << " is present." << std::endl;
	}

	while( !screen.shouldClose() ) {
		// calculate dt
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// process input
		processInput(deltaTime);

		// render
		screen.update();

		// draw shapes
		shader.activate();
		shader.set3Float("viewPos", Camera::defaultCamera.cameraPos);

		//dirLight.direction = glm::vec3(
		//	glm::rotate(glm::mat4(1.0f),
		//							glm::radians(0.5f),
		//							glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dirLight.direction, 1.0f));
		//dirLight.render(shader);
		// set light strengths
		//lamp.pointLight.render(shader);

		spotLight.position = Camera::defaultCamera.cameraPos;
		spotLight.direction = Camera::defaultCamera.cameraFront;
		spotLight.render(shader);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// create transformation
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = Camera::defaultCamera.getViewMatrix();
		projection = glm::perspective(
			glm::radians(Camera::defaultCamera.zoom),
			(float)Screen::SCR_WIDTH / (float)Screen::SCR_HEIGHT, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		cube.render(shader);

		lampShader.activate();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lamp.render(lampShader);

		// send new frame to window
		screen.newFrame();
		glfwPollEvents();
	}

	cube.cleanup();
	lamp.cleanup();

	glfwTerminate();
	return 0;
}

void processInput(double deltaTime) {
	if( Keyboard::key(GLFW_KEY_ESCAPE) ) {
		screen.setShouldClose(true);
	}

	// change mix value
	if( Keyboard::key(GLFW_KEY_UP) ) {
		mixVal += .05f;
		if( mixVal > 1 ) {
			mixVal = 1.0f;
		}
	}
	if( Keyboard::key(GLFW_KEY_DOWN) ) {
		mixVal -= .05f;
		if( mixVal < 0 ) {
			mixVal = 0.0f;
		}
	}

	// move camera
	if( Keyboard::key(GLFW_KEY_W) ) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::FORWARD, deltaTime);
	}
	if( Keyboard::key(GLFW_KEY_S) ) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::BACKWARD, deltaTime);
	}
	if( Keyboard::key(GLFW_KEY_D) ) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
	}
	if( Keyboard::key(GLFW_KEY_A) ) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::LEFT, deltaTime);
	}
	if( Keyboard::key(GLFW_KEY_SPACE) ) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::UP, deltaTime);
	}
	if( Keyboard::key(GLFW_KEY_LEFT_SHIFT) ) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::DOWN, deltaTime);
	}
}