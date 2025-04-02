#include "RenderManager.h"

bool RenderManager::init() 
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab8");

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

    if (!shader.load(R"(assets\shaders\PostProcessing.vsh)", R"(assets\shaders\SimplePostProcessing.fsh)"))
        return false;
    this->shaders.push_back(shader);

    if (!shader.load(R"(assets\shaders\PostProcessing.vsh)", R"(assets\shaders\GreyPostProcessing.fsh)"))
        return false;
    this->shaders.push_back(shader);

    if (!shader.load(R"(assets\shaders\PostProcessing.vsh)", R"(assets\shaders\SepiaPostProcessing.fsh)"))
        return false;
    this->shaders.push_back(shader);

    //Eti shaderi ne po zadaniu, ya sdelal ih sam
    //if (!shader.load(R"(assets\shaders\PostProcessing.vsh)", R"(assets\shaders\NightPostProcessing.fsh)"))
    //    return false;
    //this->shaders.push_back(shader);
    //if (!shader.load(R"(assets\shaders\PostProcessing.vsh)", R"(assets\shaders\DawnPostProcessing.fsh)"))
    //    return false;
    //this->shaders.push_back(shader);

    this->postProcessingMode = 1 % (this->shaders.size()-1);
    updateViewport(800, 600);
    
    this->fbos.emplace_back();
    this->fbos.emplace_back();

    this->fbos[0].init(this->windowWidth,this->windowHeight,true);
    this->fbos[1].init(this->windowWidth, this->windowHeight);

    return true;
}
void RenderManager::start() 
{
    this->fbos[0].bind();
	glClearColor(0.7, 0.8, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
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
      drawAllObjects();
      this->fbos[0].resolveToFbo(this->fbos[1]);

      //static bool test1 = true;
      //if (test1)
      //{   //Saves buffer contents to .bmp file to verify copying
      //    FBO::ScreenshotFBO(this->fbos[0], "TEST.bmp");
      //    FBO::ScreenshotFBO(this->fbos[1], "TEST1.bmp");
      //    test1 = false;
      //}

      FBO::unbind();
      glViewport(0, 0, this->windowWidth, this->windowHeight);
      
      glDisable(GL_MULTISAMPLE);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);

      this->shaders[postProcessingMode + 1].activate();
      this->shaders[postProcessingMode + 1].setUniform("texture_0", 0);
      this->fbos[1].bindColorTexture(GL_TEXTURE0);
      drawQuad();

      //static bool test2 = true;
      //if (test2)
      //{
      //    //Saves final image to check output
      //    FBO::ScreenshotFBO(this->fbos[1], "TEST2.bmp");
      //    test2 = false;
      //}
}

void RenderManager::updateViewport(int w, int h)
{
    this->windowHeight = h;
    this->windowWidth = w;
    //cringe
    for (FBO& fbo : this->fbos)
        fbo.init(w, h, fbo.isMultisample());
}

void RenderManager::nextPostProcessing()
{
    this->postProcessingMode = (this->postProcessingMode + 1) % (this->shaders.size() - 1);
}

int	RenderManager::getPostProcessing()
{
    return this->postProcessingMode;
}

void RenderManager::drawAllObjects()
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

            if (material != prevMat)
            {
                prevMat = material;
                this->shaders[0].setUniform("mAmbient", material->getAmbient());
                this->shaders[0].setUniform("mDiffuse", material->getDiffuse());
                this->shaders[0].setUniform("mSpecular", material->getSpecular());
                this->shaders[0].setUniform("mShininess", material->getShininess());
                materialSwaps++;
            }

            if (texture != prevTex)
            {
                prevTex = texture;
                texture->bind();
                textureSwaps++;
            }
        }

        instanceMatrix.emplace_back(viewMatrix * graphObj.getModelMatrix());
        if (instanceMatrix.size() == 20 || (mesh != prevMesh && !instanceMatrix.empty()))
        {
            this->shaders[0].setUniform("modelViewMatrix[0]", instanceMatrix);
            if (prevMesh)
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

void RenderManager::drawQuad()
{
	static bool init = true;
	static GLuint VAO_Index = 0;
	static GLuint VBO_Index = 0;
	static int VertexCount = 0;
	if (init) {
		init = false;
		glGenBuffers(1, &VBO_Index);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		GLfloat	Verteces[] = {
			-0.5,	+0.5,
			-0.5,	-0.5,
			+0.5,	+0.5,
			+0.5,	+0.5,
			-0.5,	-0.5,
			+0.5,	-0.5
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);
		glGenVertexArrays(1, &VAO_Index);
		glBindVertexArray(VAO_Index);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		VertexCount = 6;
	}
	glBindVertexArray(VAO_Index);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
