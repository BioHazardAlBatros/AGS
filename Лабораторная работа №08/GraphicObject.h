#pragma once
#include "Data.h"

enum class GraphicObjectType {none,road,building,vehicle,big_nature,small_nature,big_prop,medium_prop,small_prop};
const std::map<std::string, GraphicObjectType> typeMap = 
  { {"none",GraphicObjectType::none},{"road",GraphicObjectType::road},
	{"building",GraphicObjectType::building},{"vehicle",GraphicObjectType::vehicle},
	{"big nature",GraphicObjectType::big_nature},{"small nature",GraphicObjectType::small_nature},
	{"big prop",GraphicObjectType::big_prop},{"medium prop",GraphicObjectType::medium_prop},{"small prop",GraphicObjectType::small_prop}};

class GraphicObject
{
private:
	float angle;
	glm::vec3 position;
	glm::vec4 color;
	glm::mat4 modelMatrix;

	GraphicObjectType type;
	glm::vec3 dimensions; //(WxHxD)

	int meshId;
	int materialId;
	int textureId;

public:
	GraphicObject();
	glm::vec4& getColor();
	glm::mat4& getModelMatrix();
	glm::vec3& getPosition();
	float getAngle();

	GraphicObjectType getType();
	glm::vec3 getDimensions();

	int getMeshId();
	int getMaterialId();
	int getTextureId();

	void setColor(glm::vec4 color);
	void setPosition(glm::vec3 position);
	void setAngle(float degree);
	void setType(GraphicObjectType type);
	void setDimensions(glm::vec3 dimensions);

	void setMeshId(int newId);
	void setMaterialId(int newId);
	void setTextureId(int newId);
};