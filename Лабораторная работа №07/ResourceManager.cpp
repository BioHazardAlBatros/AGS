#include "ResourceManager.h"

int ResourceManager::loadMesh(std::string sourceFile) 
{
// SPEEDING UP LOADING PROCESS.
//	printf("\nLoading mesh data from %s\n", sourceFile.c_str());
	
	auto meshPair = this->meshes_id.find(sourceFile);
	if (meshPair != this->meshes_id.end())
	{
//		printf("This mesh is already loaded.\n");
//		printf("Mesh ID - %d\n", meshPair->second);
		return meshPair->second;
	}
	
	this->meshes.emplace_back();
	if (!this->meshes.back().load(sourceFile))
	{
		this->meshes.pop_back();
		printf("Failed to load from %s\n", sourceFile.c_str());
		return -1;
	}
	
	static int idCounter = 0;
	this->meshes_id.insert(std::make_pair(sourceFile, idCounter));
//	printf("Mesh ID - %d\n", idCounter);

	return idCounter++;
}

int ResourceManager::loadTexture(std::string sourceFile)
{
//	printf("\nLoading texture data from %s\n", sourceFile.c_str());

	auto texPair = this->textures_id.find(sourceFile);
	if (texPair != this->textures_id.end())
	{
//		printf("This texture is already loaded.\n");
//		printf("Texture ID - %d\n", texPair->second);
		return texPair->second;
	}

	this->textures.emplace_back();
	if (!this->textures.back().load(sourceFile))
	{
		this->textures.pop_back();
		printf("Failed to load from %s\n", sourceFile.c_str());
		return -1;
	}

	static int idCounter = 0;
	this->textures_id.insert(std::make_pair(sourceFile, idCounter));
//	printf("Texture ID - %d\n", idCounter);

	return idCounter++;
}
int ResourceManager::loadMaterial(std::string sourceFile)
{
//	printf("\nLoading material data from %s\n", sourceFile.c_str());

	auto matPair = this->materials_id.find(sourceFile);
	if (matPair != this->materials_id.end())
	{
//		printf("This material is already loaded.\n");
//		printf("Material ID - %d\n", matPair->second);
		return matPair->second;
	}

	this->materials.emplace_back();
	if (!this->materials.back().load(sourceFile))
	{
		this->materials.pop_back();
		printf("Failed to load from %s\n", sourceFile.c_str());
		return -1;
	}

	static int idCounter = 0;
	this->materials_id.insert(std::make_pair(sourceFile, idCounter));
//	printf("Material ID - %d\n", idCounter);

	return idCounter++;
}

Mesh* ResourceManager::getMesh(int index) 
{
	if (index!=-1 && index < this->meshes.size())
		return &this->meshes[index];
	return nullptr;
}

Material* ResourceManager::getMaterial(int index) 
{
	if (index != -1 && index < this->materials.size())
		return &this->materials[index];
	return nullptr;
}

Texture* ResourceManager::getTexture(int index) 
{
	if (index != -1 && index < this->textures.size())
		return &this->textures[index];
	return nullptr;
}