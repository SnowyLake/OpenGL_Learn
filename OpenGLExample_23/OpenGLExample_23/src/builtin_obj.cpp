#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include "../include/builtin_obj.h"
#include "../include/builtin_obj_data.h"

//---------------------------------------------------------------
//							public
//---------------------------------------------------------------
BuiltInObject::BuiltInObject(BuiltInObjectType objType)
{
	switch(objType)
	{
	case BuiltInObjectType::OBJ_CUBE:
		objVertices = cubeVertices;
		break;
	case BuiltInObjectType::OBJ_PLANE:
		objVertices = planeVertices;
		break;
	default:
		break;
	}
	SetVAO();
	this->texture = LoadBuiltInObjectTexture(objType);
}

void BuiltInObject::BuiltInObjRender(GLShader& shader, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
{
	glBindVertexArray(objVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	shader.SetMat4("model", model);
	shader.SetMat4("view", view);
	shader.SetMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void BuiltInObject::DeleteVAO()
{
	glDeleteVertexArrays(1, &this->objVAO);
}

void BuiltInObject::DeleteBuffers()
{
	glDeleteBuffers(1, &this->objVBO);
	glDeleteBuffers(1, &this->objEBO);
}

//---------------------------------------------------------------
//							private
//---------------------------------------------------------------
void BuiltInObject::SetVAO()
{
	glGenVertexArrays(1, &objVAO);
	glGenBuffers(1, &objVBO);
	glBindVertexArray(objVAO);
	glBindBuffer(GL_ARRAY_BUFFER, objVBO);
	glBufferData(GL_ARRAY_BUFFER, objVertices.size() * sizeof(float), &objVertices.at(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	objEBO = 0;
}

unsigned int BuiltInObject::LoadBuiltInObjectTexture(BuiltInObjectType objType)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	const char* path = nullptr;
	switch(objType)
	{
	case BuiltInObjectType::OBJ_CUBE:
		path = cubeTexturePath.c_str();
		break;
	case BuiltInObjectType::OBJ_PLANE:
		path = planeTexturePath.c_str();
		break;
	default:
		break;
	}
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	if(data)
	{
		GLenum format = NULL;
		if(nrComponents == 1)
			format = GL_RED;
		else if(nrComponents == 3)
			format = GL_RGB;
		else if(nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	} else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
