#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <gl_shader.h>
#include "Mesh.h"


//#include<map>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
	//model data
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	int numVertices;
	int numFaces;

	//constructor, expects a filepath to a 3D model.
	Model(const std::string& path, bool gamma = false);

	//darws the model
	void Render(GLShader& shader, glm::mat4& model, const glm::mat4 view, const glm::mat4 projection, bool whetherResetModelValue);

private:
	/*loads a model with supported Assimp extensions from file
	  and stores the resulting meshes in the meshes vector.*/
	void LoadModel(const std::string& path);

	/*processes a node in a recursive fashion. processes each individual mesh located
	  at the node and repeats this process on its children nodes(if any).*/
	void ProcessNode(aiNode* node, const aiScene* scene);

	//processes a mesh 
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	//checks all material textures of a given type and loads the textures if they're not loaded yet.
	//the required info is returned as a Texture struct.
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

