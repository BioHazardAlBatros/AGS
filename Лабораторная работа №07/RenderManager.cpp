#include "RenderManager.h"

bool RenderManager::init() 
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab7");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return false;
	}
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	Shader shader;
	if (!shader.load(R"(assets\shaders\DiffuseTextureInstanced.vsh)", R"(assets\shaders\DiffuseTextureInstanced.fsh)"))
		return false;
	this->shaders.push_back(shader);
	return true;
}
void RenderManager::start() 
{
	glClearColor(0.7, 0.8, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	this->shaders[0].activate();

	this->shaders[0].setUniform("projectionMatrix", this->camera->getProjectionMatrix());
	this->shaders[0].setUniform("texture_0", 0);

	this->shaders[0].setUniform("fogColor", { 0.7, 0.8, 1.0, 1.0 });
	this->shaders[0].setUniform("fogStartDistance", 70.0f);
	this->shaders[0].setUniform("fogEndDistance", 150.0f);
	this->shaders[0].setUniform("fogDensity", 1.0f);
	glActiveTexture(GL_TEXTURE0);
}
void RenderManager::setCamera(Camera* camera) 
{
	this->camera = camera;
}
void RenderManager::addToRenderQueue(GraphicObject& graphicObject) 
{
	this->graphicObjects.push_back(graphicObject);
}
void RenderManager::finish()
{
    Material* prevMat = nullptr;
    Texture* prevTex = nullptr;
    Mesh* prevMesh = nullptr;
    materialSwaps = 0;
    textureSwaps = 0;
    drawCalls = 0;

    Light sun;
    glm::mat4 viewMatrix = this->camera->getViewMatrix();

    this->shaders[0].setUniform("lAmbient", sun.getAmbient());
    this->shaders[0].setUniform("lDiffuse", sun.getDiffuse());
    this->shaders[0].setUniform("lSpecular", sun.getSpecular());
    this->shaders[0].setUniform("lPosition", viewMatrix * sun.getDirection());

    std::vector<glm::mat4> instanceMatrix;

    for (auto& graphObj : this->graphicObjects)
    {
        Material* material = rm.getMaterial(graphObj.getMaterialId());
        Texture* texture = rm.getTexture(graphObj.getTextureId());
        Mesh* mesh = rm.getMesh(graphObj.getMeshId());

        if ((material != prevMat) || (texture != prevTex) || (mesh != prevMesh))
        {
            if (!instanceMatrix.empty())
            {
                this->shaders[0].setUniform("modelViewMatrix[0]", instanceMatrix);
                prevMesh->drawMany(instanceMatrix.size());
                drawCalls++;
                instanceMatrix.clear();
            }
            prevMesh = mesh;

            if (material!=prevMat)
            {
                prevMat = material;
                this->shaders[0].setUniform("mAmbient", material->getAmbient());
                this->shaders[0].setUniform("mDiffuse", material->getDiffuse());
                this->shaders[0].setUniform("mSpecular", material->getSpecular());
                this->shaders[0].setUniform("mShininess", material->getShininess());
                materialSwaps++;
            }

            if (texture!=prevTex)
            {
                prevTex = texture;
                texture->bind();
                textureSwaps++;
            }
        }
 
        instanceMatrix.emplace_back(viewMatrix * graphObj.getModelMatrix());
        if (instanceMatrix.size() == 20 || (mesh!=prevMesh && !instanceMatrix.empty()))
        {
            this->shaders[0].setUniform("modelViewMatrix[0]", instanceMatrix);
            if(prevMesh)
            prevMesh->drawMany(instanceMatrix.size());
            drawCalls++;
            instanceMatrix.clear();
        }
    }
    //last batch
    if (!instanceMatrix.empty())
    {
        this->shaders[0].setUniform("modelViewMatrix[0]", instanceMatrix);
        prevMesh->drawMany(instanceMatrix.size());
        drawCalls++;
    }
    this->graphicObjects.clear();
}