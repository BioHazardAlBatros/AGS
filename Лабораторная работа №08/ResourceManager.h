#pragma once
#include "Data.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

class ResourceManager
{
private:
	ResourceManager() {};
	ResourceManager(const ResourceManager& v) = delete;
	ResourceManager& operator=(const ResourceManager& v) = delete;

	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<Texture> textures;
	std::map<std::string, int> meshes_id;
	std::map<std::string, int> materials_id;
	std::map<std::string, int> textures_id;

public:
	static ResourceManager& instance()
	{
		static ResourceManager ResourceManager;
		return ResourceManager;
	}
	int loadMesh(std::string sourceFile);
	Mesh* getMesh(int index);

	int loadMaterial(std::string sourceFile);
	Material* getMaterial(int index);

	int loadTexture(std::string sourceFile);
	Texture* getTexture(int index);

};