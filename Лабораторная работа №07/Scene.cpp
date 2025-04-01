#include "Scene.h"

Scene::Scene() {};

int cmp(const void* a, const void* b)
{
	if (((GraphicObject*)a)->getMaterialId() < ((GraphicObject*)b)->getMaterialId())
		return -1;
	else if (((GraphicObject*)a)->getMaterialId() > ((GraphicObject*)b)->getMaterialId())
		return 1;

	if (((GraphicObject*)a)->getTextureId() < ((GraphicObject*)b)->getTextureId())
		return -1;
	else if (((GraphicObject*)a)->getTextureId() > ((GraphicObject*)b)->getTextureId())
		return 1;

	if (((GraphicObject*)a)->getMeshId() < ((GraphicObject*)b)->getMeshId())
		return -1;
	else if (((GraphicObject*)a)->getMeshId() > ((GraphicObject*)b)->getMeshId())
		return 1;

	return 0;
}

GraphicObject Scene::createGraphicObject(std::string model) 
{
	GraphicObject newObject;
	ResourceManager& rm = ResourceManager::instance();
	
	auto& modelData = this->document[model.c_str()];
	auto tempArr = modelData["dimensions"].GetArray();
//	printf("%s\n%s\ns\n%s\n", modelData["type"].GetString(), modelData["model"].GetString(), modelData["material"].GetString(), modelData["texture"].GetString());

	newObject.setType(typeMap.at(modelData["type"].GetString()));	
	newObject.setDimensions( { tempArr[0].GetFloat(),tempArr[1].GetFloat(), tempArr[2].GetFloat() });
	newObject.setMeshId(rm.loadMesh(modelData["mesh"].GetString()));
	newObject.setMaterialId(rm.loadMaterial(modelData["material"].GetString()));
	newObject.setTextureId(rm.loadTexture(modelData["texture"].GetString()));
	return newObject;
}

bool Scene::lodTest(GraphicObject& graphicObject) 
{
	float distanceToCamera = glm::length(graphicObject.getPosition() - camera->getPosition());
	if (distanceToCamera > 1000) return false;
	
	switch (graphicObject.getType())
	{
	case GraphicObjectType::vehicle: //<500
		if (distanceToCamera >= 500) return false;
		break;

	case GraphicObjectType::big_prop://<400
		if (distanceToCamera >= 400) return false;
		break;

	case GraphicObjectType::big_nature://<350
		if (distanceToCamera >= 350) return false;
		break;

	case GraphicObjectType::medium_prop://<300
		if (distanceToCamera >= 300) return false;
		break;

	case GraphicObjectType::small_prop://<200
		if (distanceToCamera >= 200) return false;
		break;

	case GraphicObjectType::small_nature://<200
		if (distanceToCamera >= 200) return false;
		break;
	default: break;
	}
	return true;
}
bool Scene::frustumCullingTest(GraphicObject& graphicObject)
{
//	float test = 1.0 / 1;
//	float x = boundBox.x * test, y = boundBox.y * test, z = boundBox.z * test;
	glm::vec3 boundBox = graphicObject.getDimensions();
	float x = boundBox.x / 2, y = boundBox.y / 2, z = boundBox.z / 2;
	glm::mat4 PVM = camera->getProjectionMatrix() * camera->getViewMatrix() * graphicObject.getModelMatrix();
	
	glm::vec4 vertices[8] = {
		PVM * glm::vec4(x,  y,  z, 1.0f),
		PVM * glm::vec4(x,  y, -z, 1.0f),
		PVM * glm::vec4(x, -y,  z, 1.0f),
		PVM * glm::vec4(x, -y, -z, 1.0f),
		PVM * glm::vec4(-x,  y,  z, 1.0f),
		PVM * glm::vec4(-x,  y, -z, 1.0f),
		PVM * glm::vec4(-x, -y,  z, 1.0f),
		PVM * glm::vec4(-x, -y, -z, 1.0f)
	};

	bool seenA=false, seenB=false, 
		 seenC=false, seenD=false,
		 seenE=false, seenF=false;
	
	for (auto& v : vertices) 
	{
		if (!seenA) seenA = (v.x <= v.w);
		if (!seenB) seenB = (v.x >= -v.w);
		if (!seenC) seenC = (v.y <= v.w);
		if (!seenD) seenD = (v.y >= -v.w);
		if (!seenE) seenE = (v.z <= v.w);
		if (!seenF) seenF = (v.z >= -v.w);
	}

	return seenA && seenB && seenC && seenD && seenE && seenF;
}

void Scene::init(std::string sourceFile) 
{
	std::ifstream File(sourceFile);
	if (!File)
		return;
	ResourceManager& rm = ResourceManager::instance();
	rapidjson::IStreamWrapper buffer(File);
	this->document.ParseStream(buffer);
	File.close();

	if (this->document.GetParseError() != 0)
	{
		//https://rapidjson.org/group___r_a_p_i_d_j_s_o_n___e_r_r_o_r_s.html#ga7d3acf640886b1f2552dc8c4cd6dea60
		printf("JSON Parse error code - %d. View docs for more information.", int(this->document.GetParseError()));
		return;
	}
	for (auto& object : this->document.GetObject())
	{
		rm.loadMesh(this->document[object.name]["mesh"].GetString());
		rm.loadTexture(this->document[object.name]["texture"].GetString());
		rm.loadMaterial(this->document[object.name]["material"].GetString());
	}
	return;
}

bool Scene::loadFromJson(std::string sourceFile)
{
	std::ifstream File(sourceFile);
	if (!File)
		return false;
	ResourceManager& rm = ResourceManager::instance();
	rapidjson::IStreamWrapper buffer(File);
	rapidjson::Document jsonFile;
	jsonFile.ParseStream(buffer);
	File.close();
	
	if (jsonFile.GetParseError() != 0)
	{
		//https://rapidjson.org/group___r_a_p_i_d_j_s_o_n___e_r_r_o_r_s.html#ga7d3acf640886b1f2552dc8c4cd6dea60
		printf("JSON Parse error code - %d. View docs for more information.", int(jsonFile.GetParseError()));
		return false;
	}

	int count = 0;
	for (auto& element : jsonFile.GetArray())
	{
		count++;
		const auto& object = element.GetObject();
		this->graphicObjects.emplace_back(createGraphicObject(object["model"].GetString()));
		this->graphicObjects.back().setAngle(object["angle"].GetFloat());
		auto tempArr = object["position"].GetArray();
		this->graphicObjects.back().setPosition({tempArr[0].GetFloat(), tempArr[1].GetFloat(), tempArr[2].GetFloat() });
	}
	printf("%d objects on scene. Sorting render queue...\n", count);

	std::qsort(this->graphicObjects.data(),this->graphicObjects.size(),sizeof(GraphicObject),cmp);	
//	for (auto& sortedGO: graphicObjects)
//		printf("%d %d %d\n", sortedGO.getMaterialId(), sortedGO.getTextureId(), sortedGO.getMeshId());

	return true;
}
void Scene::setCamera(Camera* camera) 
{
	this->camera = camera;
}
void Scene::setLight(Light* light) 
{
	this->light = light;
}
void Scene::draw() 
{
	RenderManager& rm = RenderManager::instance();
	this->renderedObjectCount = 0;
	for (auto& graphObj : this->graphicObjects)
	{
		if (!lodTest(graphObj)) continue;
		if (!frustumCullingTest(graphObj)) continue;
		rm.addToRenderQueue(graphObj);
		this->renderedObjectCount++;
	}
}
std::string Scene::getSceneDescription() 
{
	char buffer[35]{ 0 };
	sprintf_s(buffer,"[%d / %d objects rendered]", this->renderedObjectCount, this->graphicObjects.size());
	return buffer;
}
