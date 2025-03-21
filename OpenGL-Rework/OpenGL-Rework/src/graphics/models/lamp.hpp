#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "../light.h"


class Lamp : public Cube {
public:
	glm::vec3 lightColor;

	// light strength values
	PointLight pointLight;

	Lamp(glm::vec3 lightColor = glm::vec3(1.0f),
		glm::vec3 ambient = glm::vec3(1.0f),
		glm::vec3 diffuse = glm::vec3(1.0f),
		glm::vec3 specular = glm::vec3(1.0f),
		glm::vec3 pos = glm::vec3(0.0f),
		glm::vec3 size = glm::vec3(1.0f))
		: lightColor(lightColor),
		pointLight({ pos, ambient, diffuse, specular}),
		Cube(Material::white_plastic, pos, size) {}

	void render(Shader shader) {
		// set light color
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader);
	}

};

#endif