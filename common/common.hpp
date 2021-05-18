// Term--Fall 2020
// Chenyang Zhang

// this file includes some common functions

#pragma once

#include "cs488-framework/ObjFileDecoder.hpp"

#include <glm/glm.hpp>

#include <initializer_list>
#include <vector>
#include <unordered_map>
#include <string>

#include "GeometryNode.hpp"
#include "JointNode.hpp"


using namespace std;
using namespace glm;

// load the texture coordinate mapping
void getTexCoord(const char * objFilePath, vector<vec2> & uvCoords) {
	MeshId meshId;
	vector<vec3> positions;
	vector<vec3> normals;
	ObjFileDecoder::decode(objFilePath, meshId, positions, normals, uvCoords);
}


//----------------------------------------------------------------------------------------
// This function load a texture from a file.
unsigned int loadTexture(char const * path){

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nr;
	// use stb library to load the file
	unsigned char *data = stbi_load(path, &width, &height, &nr, 0);
	if (data) {
		GLenum format = GL_RGB;

		// generature texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// repeat the texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set interpolation method (filter) for minifying and magnifying operations
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data); // free the image file data
	}
	else {
		cout << "Not able to load the texture image file" << endl;
		stbi_image_free(data);
	}
	return textureID;
}


//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
void updateShaderUniforms(
	const ShaderProgram & shader,
	const GeometryNode & node,
	const glm::mat4 & viewMatrix,
	const glm::mat4 & transMatrix,
	const bool texture_mapping,
	const bool shadow_volume,
	const bool show_texture_mapping,
	glm::vec3 color,
	float opacity,
	const bool skybox
) {

	shader.enable();
	//-- Set ModelView matrix:
	GLint location = shader.getUniformLocation("ModelView");
	mat4 modelView = viewMatrix * transMatrix;
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
	CHECK_GL_ERRORS;

	if (!shadow_volume){
		//-- Set NormMatrix:
		location = shader.getUniformLocation("NormalMatrix");
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
		CHECK_GL_ERRORS;
	}

	if (!shadow_volume){
		//-- Set Material values:
		location = shader.getUniformLocation("material.kd");
		vec3 kd = node.material.kd;
		
		if (color[0] >= 0.0f) {
			kd = color;
		}

		glUniform3fv(location, 1, value_ptr(kd));
		CHECK_GL_ERRORS;

		location = shader.getUniformLocation("material.ks");
		vec3 ks = node.material.ks;
		glUniform3fv(location, 1, value_ptr(ks));

		location = shader.getUniformLocation("material.shininess");
		glUniform1f(location, node.material.shininess);

		location = shader.getUniformLocation("opacity");
		glUniform1f(location, opacity);

		location = shader.getUniformLocation("texture_mapping");
		glUniform1i(location, texture_mapping&&show_texture_mapping);
		CHECK_GL_ERRORS;

		location = shader.getUniformLocation("skybox");
		glUniform1i(location, skybox);
		CHECK_GL_ERRORS;
	}

	// location = shader.getUniformLocation("shadow_volume");
	// glUniform1i(location, shadow_volume);
	// CHECK_GL_ERRORS;

	shader.disable();

}


