#include "Mesh.h"

Mesh::Mesh():vao(0),vertexBuffer(0),indexBuffer(0),vertexCount(0){};

bool Mesh::load(std::string sourceFile) 
{
	std::ifstream File(sourceFile);
	if (!File)
		return false;
	
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	std::vector<glm::vec3> vert;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> tex;
	std::vector<glm::ivec3> fPoints;
	std::unordered_map<glm::ivec3, GLuint> vertexToIndexTable;
	GLuint vertexIndex = 0;

	int vertCount = 0, normCount = 0, texCount = 0, facesCount = 0;
	enum Field { VERTEXPOS, NORMAL, TEXPOS, FACE, NONE } currentField;

	while (!File.eof())
	{
		std::string str, currentFieldName;
		std::getline(File, str, '\n');
		std::istringstream currentLine(str);

		currentField = NONE;
		currentLine >> currentFieldName;

		if (currentFieldName == "v")  currentField = VERTEXPOS;
		if (currentFieldName == "vn") currentField = NORMAL;
		if (currentFieldName == "vt") currentField = TEXPOS;
		if (currentFieldName == "f")  currentField = FACE;

		if (currentField == NONE) continue;
		switch (currentField)
		{
		case VERTEXPOS:
			vert.emplace_back(); ++vertCount;
			currentLine >> vert.back().x
				>> vert.back().y
				>> vert.back().z;
			break;
		case NORMAL:
			norm.emplace_back(); ++normCount;
			currentLine >> norm.back().x
				>> norm.back().y
				>> norm.back().z;
			break;
		case TEXPOS:
			float discard;
			tex.emplace_back(); ++texCount;
			currentLine >> tex.back().x
				>> tex.back().y >> discard;
			break;
		case FACE:
			while (!currentLine.eof())
			{
				fPoints.emplace_back();
				char discard; // used to store '/' symbol 
				currentLine >> fPoints.back()[0] >> discard
					>> fPoints.back()[1] >> discard
					>> fPoints.back()[2];
	
				if (fPoints.back() == glm::ivec3())
				{
					fPoints.pop_back();
					continue;
				}
				//Check if this new vertice is already present in the vertices array
				auto vertIter = vertexToIndexTable.find(fPoints.back());
				if (vertIter != vertexToIndexTable.end())
				{
					indices.emplace_back(vertIter->second);
					continue;
				}
				vertexToIndexTable.insert(std::make_pair(fPoints.back(), vertexIndex));
				indices.emplace_back(vertexIndex++);
				vertices.emplace_back();

				vertices.back().coord[0] = vert[fPoints.back()[0] - 1][0];
				vertices.back().coord[1] = vert[fPoints.back()[0] - 1][1];
				vertices.back().coord[2] = vert[fPoints.back()[0] - 1][2];

				vertices.back().texCoord[0] = tex[fPoints.back()[1] - 1][0];
				//vertices.back().texCoord[1] = 1.0 - tex[fPoints.back()[1] - 1][1];
				vertices.back().texCoord[1] = tex[fPoints.back()[1] - 1][1];

				vertices.back().normal[0] = norm[fPoints.back()[2] - 1][0];
				vertices.back().normal[1] = norm[fPoints.back()[2] - 1][1];
				vertices.back().normal[2] = norm[fPoints.back()[2] - 1][2];
			}
			++facesCount;
			break;
		}
	}
	File.close();
	printf("%d vertices, %d vertex normals, %d texture coords, %d faces loaded.\n",vertCount,normCount ,texCount,facesCount);
	
	this->vertexCount = indices.size();

	glGenBuffers(1, &this->vertexBuffer);
	glGenBuffers(1, &this->indexBuffer);
	glGenVertexArrays(1,&this->vao);

	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coord));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}

void Mesh::drawOne() 
{
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES,this->vertexCount,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}