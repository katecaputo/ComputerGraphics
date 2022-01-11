#include "SDLGraphicsProgram.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

// inserts a breakpoint in code
#define GLCall(x) GLClearErrorStates(); x ; GLCheckError(#x,__LINE__);

// error handling routine
static void GLClearErrorStates(){
    // return all of the errors that occur.
    while(glGetError() != GL_NO_ERROR){
        ;
    }
}

// Returns false if an error occurs
static bool GLCheckError(const char* function, int line){
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error]" << error << "|" << function << " Line: " << line << "\n";
        return false;
    }
    return true;
}

// Initialization function
// Returns a true or false value based on successful completion of setup.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h):m_screenWidth(w),m_screenHeight(h){

	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Final",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_screenWidth,
                                m_screenHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	GetOpenGLVersionInfo();

	// OBJECTS FOR SCENE 1
	m_objectsS1.push_back(new OBJ("./../common/textures/bluecircle.png"));
	m_objectsS1.push_back(new OBJ("./../common/textures/redcircle.png"));
	m_objectsS1.push_back(new OBJ("./../common/textures/yellowcircle.png"));

	// OBJECTS FOR SCENE 2
	m_objectsS2.push_back(new OBJ("./../common/textures/4panewindow.png"));
	m_objectsS2.push_back(new OBJ("./../common/textures/sunflowers.png"));

	// OBJECTS FOR SCENE 3
	m_objectsS3.push_back(new OBJ("./../common/textures/sky.png"));
	m_objectsS3.push_back(new OBJ("./../common/textures/back_mtn.png"));
	m_objectsS3.push_back(new OBJ("./../common/textures/mid_mtn.png"));
	m_objectsS3.push_back(new OBJ("./../common/textures/front_mtn.png"));
	m_objectsS3.push_back(new OBJ("./../common/textures/back_hill.png"));
	m_objectsS3.push_back(new OBJ("./../common/textures/front_hill.png"));

	// set default scene to scene #1
	m_objects = m_objectsS1;

	// initializes the scene's camera
	m_camera = new Camera();
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    // Reclaim all of our objects and camera
	for (int i = 0; i < m_objects.size(); i++) {
		delete m_objects[i];
	}

	for (int i = 0; i < m_objectsS1.size(); i++) {
		delete m_objectsS1[i];
	}

	for (int i = 0; i < m_objectsS2.size(); i++) {
		delete m_objectsS2[i];
	}

	for (int i = 0; i < m_objectsS3.size(); i++) {
		delete m_objectsS3[i];
	}

	delete m_camera;

    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

    // Setup our OpenGL State machine
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); 
	return success;
}


// Update OpenGL
void SDLGraphicsProgram::Update(){

	// OBJ0's transformations
	m_objects[0]->SetTranslationLocation(1.2f, 0.0f, -5.0f);
    m_objects[0]->GetTransform().LoadIdentity();
    m_objects[0]->GetTransform().Translate(m_objects[0]->GetTranslationLocation().x,
									   m_objects[0]->GetTranslationLocation().y,
									   m_objects[0]->GetTranslationLocation().z);
    m_objects[0]->GetTransform().Scale(1.8f,1.8f,1.8f);
    m_objects[0]->Update(m_screenWidth,m_screenHeight, m_camera);

	// OBJ1's transformations
	m_objects[1]->SetTranslationLocation(-1.5f, 0.3f, -7.0f);
	m_objects[1]->GetTransform().LoadIdentity();
    m_objects[1]->GetTransform().Translate(m_objects[1]->GetTranslationLocation().x,
									   m_objects[1]->GetTranslationLocation().y,
									   m_objects[1]->GetTranslationLocation().z);
    m_objects[1]->GetTransform().Scale(2.16f,2.16f,2.16f);
    m_objects[1]->Update(m_screenWidth,m_screenHeight, m_camera);

	// OBJ2;s transformations
	m_objects[2]->SetTranslationLocation(0.0f, -2.0f, -9.0f);
	m_objects[2]->GetTransform().LoadIdentity();
    m_objects[2]->GetTransform().Translate(m_objects[2]->GetTranslationLocation().x,
									   m_objects[2]->GetTranslationLocation().y,
									   m_objects[2]->GetTranslationLocation().z);
    m_objects[2]->GetTransform().Scale(2.52f,2.52f,2.52f);
    m_objects[2]->Update(m_screenWidth,m_screenHeight, m_camera);
}



// Render - called once per loop
void SDLGraphicsProgram::Render(){
    // Initialize background color of the screen
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.2f, 0.2f, 0.2f, 1.f );

    // Clear color buffer and Depth Buffer
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // sorts objects, using a map, according to their current distance from the camera
	std::map<float, OBJ*> sorted;
	glm::vec3 camPos = glm::vec3(m_camera->GetEyeXPosition(),
								 m_camera->GetEyeYPosition(),
								 m_camera->GetEyeZPosition());

    for (unsigned int i = 0; i < m_objects.size(); i++) {
        float distance = glm::length(camPos - m_objects[i]->GetTranslationLocation());
        sorted[distance] = m_objects[i];
    }

	// renders all objects in the scene from back to front
	// (furthest from camera to closest)
	// this is necessary for transparency to work
	for(std::map<float, OBJ*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
		it->second->Render();
	} 

    SDL_Delay(50); 
}


// loops forever
void SDLGraphicsProgram::Loop(){
	// flag for if program is quit or not
    bool quit = false;

	float cameraSpeed = 0.4f;

	// determines which scene we are looking at; default is #1, the 3 circles
	int curScene = 1;
	
	// (for scene #2) - are we rotating the window panel? (toggle)
	bool rotScene = false;

    SDL_Event e;
    SDL_StartTextInput();

    while(!quit){
		while(SDL_PollEvent( &e ) != 0){
			if(e.type == SDL_QUIT){
                quit = true;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
				quit = true;
			} 
            // Handle keyboad input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
             m_camera->MouseLook(mouseX, mouseY);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_LEFT:
                            m_camera->MoveLeft(cameraSpeed);
                            break;
                        case SDLK_RIGHT:
                            m_camera->MoveRight(cameraSpeed);
                            break;
                        case SDLK_UP:
                            m_camera->MoveForward(cameraSpeed);
                            break;
                        case SDLK_DOWN:
                            m_camera->MoveBackward(cameraSpeed);
                            break;
                        case SDLK_RSHIFT:
                            m_camera->MoveUp(cameraSpeed);
                            break;
                        case SDLK_LSHIFT:
                            m_camera->MoveDown(cameraSpeed);
                            break;
                    }
                break;
			}

			// Depending on keyboard presses (1, 2 or 3), determines what
			// scene we are looking at
			// also resets the camera position to (0,0,0) when switching scenes
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_2) {
				curScene = 2;
				m_camera->SetCameraEyePosition(0.0f, 0.0f, 0.0f);
			} else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_1) {
				curScene = 1;
				m_camera->SetCameraEyePosition(0.0f, 0.0f, 0.0f);
			} else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_3) {
				curScene = 3;
				m_camera->SetCameraEyePosition(0.0f, 0.0f, 0.0f);
			}

			// when 'r' is pressed in scene 2, toggles roation on or off
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
				rotScene = !rotScene;
			} 
      	} 

		// objects are filled, not wireframe
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		// sets our list of objects (which scene) based on key presses
		// and updates (appopriate translations for each scene)
		// NOTE: all objects for all scenes are defined in constructor,
		//       not here in Loop or Update so that the objects aren't 
		//       created over and over again, only once.
		if (curScene == 1) {
			m_objects = m_objectsS1;
			Update();
		} else if (curScene == 2) {
			m_objects = m_objectsS2;
			Update4Pane(rotScene);
		} else if (curScene == 3) {
			m_objects = m_objectsS3;
			UpdateFarmScene();
		}

		Render();
      	SDL_GL_SwapWindow(GetSDLWindow());
    }
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

// Update OpenGL for the 4 window pane scene
void SDLGraphicsProgram::Update4Pane(bool rotScene){

	static float rot = M_PI / 2;
	if (rotScene) {
    	rot+=0.05;
	}
	if(rot>360){rot=0;}

	// OBJ0's transformations
	m_objects[0]->SetTranslationLocation(0.0f, 0.0f, -4.0f);
    m_objects[0]->GetTransform().LoadIdentity();
    m_objects[0]->GetTransform().Translate(m_objects[0]->GetTranslationLocation().x,
									   m_objects[0]->GetTranslationLocation().y,
									   m_objects[0]->GetTranslationLocation().z);
    m_objects[0]->GetTransform().Scale(2.0f,2.0f,2.0f);
	m_objects[0]->GetTransform().Rotate(rot,0.0f,0.0f,1.0f);
    m_objects[0]->Update(m_screenWidth,m_screenHeight, m_camera);

	// OBJ1's transformations
	m_objects[1]->SetTranslationLocation(-1.0f, 1.0f, -4.1f);
	m_objects[1]->GetTransform().LoadIdentity();
    m_objects[1]->GetTransform().Translate(m_objects[1]->GetTranslationLocation().x,
									   m_objects[1]->GetTranslationLocation().y,
									   m_objects[1]->GetTranslationLocation().z);
    m_objects[1]->GetTransform().Scale(1.0f,1.0f,1.0f);
	m_objects[1]->GetTransform().Rotate(M_PI,1.0f,0.0f,0.0f);
    m_objects[1]->Update(m_screenWidth,m_screenHeight, m_camera);
}

// Update OpenGL for the farm scene
void SDLGraphicsProgram::UpdateFarmScene(){

	// 'sky.png' transformations
	m_objects[0]->SetTranslationLocation(0.0f, 0.0f, -4.5f);
    m_objects[0]->GetTransform().LoadIdentity();
    m_objects[0]->GetTransform().Translate(m_objects[0]->GetTranslationLocation().x,
									   m_objects[0]->GetTranslationLocation().y,
									   m_objects[0]->GetTranslationLocation().z);
    m_objects[0]->GetTransform().Scale(2.0f,2.0f,2.0f);
	m_objects[0]->GetTransform().Rotate(M_PI,1.0f,0.0f,0.0f);
    m_objects[0]->Update(m_screenWidth,m_screenHeight, m_camera);

	// 'back mtn.png' transformations
	m_objects[1]->SetTranslationLocation(0.0f, 0.0f, -4.0f);
	m_objects[1]->GetTransform().LoadIdentity();
    m_objects[1]->GetTransform().Translate(m_objects[1]->GetTranslationLocation().x,
									   m_objects[1]->GetTranslationLocation().y,
									   m_objects[1]->GetTranslationLocation().z);
    m_objects[1]->GetTransform().Scale(1.75f,1.75f,1.75f);
	m_objects[1]->GetTransform().Rotate(M_PI,1.0f,0.0f,0.0f);
    m_objects[1]->Update(m_screenWidth,m_screenHeight, m_camera);

	// 'mid mtn.png' transformations
	m_objects[2]->SetTranslationLocation(0.0f, 0.0f, -3.5f);
	m_objects[2]->GetTransform().LoadIdentity();
    m_objects[2]->GetTransform().Translate(m_objects[2]->GetTranslationLocation().x,
									   m_objects[2]->GetTranslationLocation().y,
									   m_objects[2]->GetTranslationLocation().z);
    m_objects[2]->GetTransform().Scale(1.54f,1.54f,1.54f);
	m_objects[2]->GetTransform().Rotate(M_PI,1.0f,0.0f,0.0f);
    m_objects[2]->Update(m_screenWidth,m_screenHeight, m_camera);

	// 'front mtn.png' transformations
	m_objects[3]->SetTranslationLocation(0.0f, 0.0f, -3.0f);
	m_objects[3]->GetTransform().LoadIdentity();
    m_objects[3]->GetTransform().Translate(m_objects[3]->GetTranslationLocation().x,
									   m_objects[3]->GetTranslationLocation().y,
									   m_objects[3]->GetTranslationLocation().z);
    m_objects[3]->GetTransform().Scale(1.3f,1.3f,1.3f);
	m_objects[3]->GetTransform().Rotate(M_PI,1.0f,0.0f,0.0f);
    m_objects[3]->Update(m_screenWidth,m_screenHeight, m_camera);

	// 'back hill.png' transformations
	m_objects[4]->SetTranslationLocation(0.0f, 0.0f, -2.5f);
	m_objects[4]->GetTransform().LoadIdentity();
    m_objects[4]->GetTransform().Translate(m_objects[4]->GetTranslationLocation().x,
									   m_objects[4]->GetTranslationLocation().y,
									   m_objects[4]->GetTranslationLocation().z);
    m_objects[4]->GetTransform().Scale(1.08f,1.08f,1.08f);
	m_objects[4]->GetTransform().Rotate(M_PI,1.f,0.0f,0.0f);
    m_objects[4]->Update(m_screenWidth,m_screenHeight, m_camera);

	// 'front hill.png' transformations
	m_objects[5]->SetTranslationLocation(0.0f, 0.0f, -2.0f);
	m_objects[5]->GetTransform().LoadIdentity();
    m_objects[5]->GetTransform().Translate(m_objects[5]->GetTranslationLocation().x,
									   m_objects[5]->GetTranslationLocation().y,
									   m_objects[5]->GetTranslationLocation().z);
    m_objects[5]->GetTransform().Scale(0.86f,0.86f,0.86f);
	m_objects[5]->GetTransform().Rotate(M_PI,1.0f,0.0f,0.0f);
    m_objects[5]->Update(m_screenWidth,m_screenHeight, m_camera);
}