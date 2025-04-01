#include "Material.h"

Material::Material()
{
	this->ambient = { 0.0,0.0,0.0,0.0 };
	this->diffuse = { 0.0,0.0,0.0,0.0 };
	this->specular = { 0.0,0.0,0.0,0.0 };
	this->shininess = 1.0;
}
glm::vec4& Material::getAmbient() 
{
	return this->ambient;
}
glm::vec4& Material::getDiffuse() 
{
	return this->diffuse;
}
glm::vec4& Material::getSpecular() 
{
	return this->specular;
}
float Material::getShininess() 
{
	return this->shininess;
}

bool Material::load(std::string sourceFile) 
{
	std::ifstream File(sourceFile);
	if (!File)
		return false;

	rapidjson::IStreamWrapper buffer(File);
	rapidjson::Document jsonFile;
	jsonFile.ParseStream(buffer);
	File.close();

	if (jsonFile.GetParseError() != 0)
	{
		//https://rapidjson.org/group___r_a_p_i_d_j_s_o_n___e_r_r_o_r_s.html#ga7d3acf640886b1f2552dc8c4cd6dea60
		printf("JSON Parse error code - %d. View docs for more information.",int(jsonFile.GetParseError()));
		return false;
	}

	auto tempArr = jsonFile["ambient"].GetArray();
	this->ambient = { tempArr[0].GetDouble(),tempArr[1].GetDouble() ,tempArr[2].GetDouble() ,tempArr[3].GetDouble() };

	tempArr = jsonFile["diffuse"].GetArray();
	this->diffuse = { tempArr[0].GetDouble(),tempArr[1].GetDouble() ,tempArr[2].GetDouble() ,tempArr[3].GetDouble() };

	tempArr = jsonFile["specular"].GetArray();
	this->specular = { tempArr[0].GetDouble(),tempArr[1].GetDouble() ,tempArr[2].GetDouble() ,tempArr[3].GetDouble() };

//	tempArr = jsonFile["emission"].GetArray();
//	{ tempArr[0].GetDouble(),tempArr[1].GetDouble() ,tempArr[2].GetDouble() ,tempArr[3].GetDouble() };

	this->shininess = jsonFile["shininess"].GetDouble();

	return true;
}