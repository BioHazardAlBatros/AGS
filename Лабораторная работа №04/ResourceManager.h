#pragma once
#include "Data.h"
#include "Mesh.h"

class ResourceManager
{
private:
	ResourceManager() {};
	ResourceManager(const ResourceManager& v) = delete;
	ResourceManager& operator=(const ResourceManager& v) = delete;

	std::vector<Mesh> meshes;
	std::map <std::string, int> meshes_id;

public:
	static ResourceManager& instance()
	{
		static ResourceManager ResourceManager;
		return ResourceManager;
	}
	int loadMesh(std::string filename);
	Mesh* getMesh(int index);

};