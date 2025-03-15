#include "ResourceManager.h"

int ResourceManager::loadMesh(std::string sourceFile) 
{
	printf("\nLoading mesh data from %s\n", sourceFile.c_str());
	
	auto meshPair = this->meshes_id.find(sourceFile);
	if (meshPair != this->meshes_id.end())
	{
		printf("This file is already loaded.\n");
		printf("Mesh ID - %d\n", meshPair->second);
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
	printf("Mesh ID - %d\n", idCounter);

	return idCounter++;
}

Mesh* ResourceManager::getMesh(int index) 
{
	if (index!=-1 && index < this->meshes.size())
		return &this->meshes[index];
	return nullptr;
	//for (auto& mapItem : this->meshes_id)
	//	if(mapItem.second == index)
	//		return &(this->meshes[index]);
	//return nullptr;
}