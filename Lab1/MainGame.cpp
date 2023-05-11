#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Shader geoShader();
	Shader rippleShader();
	//Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\Ball.obj");
	mesh2.loadModel("..\\res\\monkey3.obj");
	mesh3.loadModel("..\\res\\cone.obj");
	eMappingShader.init("..\\res\\eMapping.vert", "..\\res\\eMapping.frag");
	geoShader.initGeo("..\\res\\\shaderGeoText.vert", "..\\res\\shaderGeoText.geom", "..\\res\\shaderGeoText.frag");
	rippleShader.init("..\\res\\rippleShader.vert", "..\\res\\rippleShader.frag");

	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	skybox.init(faces);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

//void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
//{
//	
//	ALint state; 
//	alGetSourcei(Source, AL_SOURCE_STATE, &state);
//	/*
//	Possible values of state
//	AL_INITIAL
//	AL_STOPPED
//	AL_PLAYING
//	AL_PAUSED
//	*/
//	if (AL_PLAYING != state)
//	{
//		audioDevice.playSound(Source, pos);
//	}
//}

void MainGame::linkGeo()
{
	float randX = ((float)rand() / (RAND_MAX));
	float randY = ((float)rand() / (RAND_MAX));
	float randZ = ((float)rand() / (RAND_MAX));
	// Frag: uniform float randColourX; uniform float randColourY; uniform float randColourZ;
	geoShader.setFloat("randColourX", randX);
	geoShader.setFloat("randColourY", randY);
	geoShader.setFloat("randColourZ", randZ);
	// Geom: uniform float time;
	geoShader.setFloat("time", counter);
}

void MainGame::linkeMapping(Texture* t) 
{
	GLuint texLoc = glGetUniformLocation(eMappingShader.getID(), "diffuse"); 

	eMappingShader.setVec3("camPos", myCamera.getPos());
	eMappingShader.setMat4("model", transform.GetModel());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t->getID());
	glUniform1i(texLoc, 1);
}

void MainGame::linkRipple(Texture* t) 
{
	GLuint texLoc = glGetUniformLocation(rippleShader.getID(), "hologramTexture");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t->getID());

	rippleShader.setFloat("timeFragment", counter);
	rippleShader.setFloat("timeVertex", counter);
	float randomIntensity = ((float)rand() / (RAND_MAX));
	rippleShader.setFloat("intensity", randomIntensity);
	rippleShader.setFloat("fogDensity", 0.5);
	rippleShader.setFloat("colorShiftSpeed", 1.5);
	rippleShader.setFloat("flickerSpeed", 0.5);
	rippleShader.setFloat("transparency", 1);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour

	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture

	rippleShader.Bind();
	linkRipple(&texture);
	texture1.Bind(0);

	transform.SetPos(glm::vec3(0.0, -sinf(counter), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(1.0, 1.0, 1.0));
	rippleShader.Update(transform, myCamera);

	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.62f);

	transform.SetPos(glm::vec3(3.0, -sinf(counter), 3.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(1.0, 1.0, 1.0));
	rippleShader.Update(transform, myCamera);

	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.82f);

	transform.SetPos(glm::vec3(-3.0, -sinf(counter), 3.0));
	transform.SetRot(glm::vec3(0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(1.0, 1.0, 1.0));
	rippleShader.Update(transform, myCamera);

	mesh3.draw();
	mesh3.updateSphereData(*transform.GetPos(), 0.82f);

	counter = counter + 0.02f;

	skybox.draw(&myCamera);	
	

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();


	_gameDisplay.swapBuffer();	
	

} 