#ifndef SDLGRAPHICSPROGRAM_HPP
#define SDLGRAPHICSPROGRAM_HPP

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

// The glad library helps setup OpenGL extensions.
#include <glad/glad.h>

#include "OBJ.hpp"
#include "Camera.hpp"

// This class sets up a full graphics program using SDL
class SDLGraphicsProgram{
public:

    // Constructor
    SDLGraphicsProgram(int w, int h);

    // Desctructor
    ~SDLGraphicsProgram();

    // Setup OpenGL
    bool InitGL();

    // Per frame update
    void Update();
    void Update4Pane(bool rotateScene);
    void UpdateFarmScene();

    // Renders shapes to the screen
    void Render();

    // loop that runs forever
    void Loop();

    // Get Pointer to Window
    SDL_Window* GetSDLWindow();

    // Helper Function to Query OpenGL information.
    void GetOpenGLVersionInfo();

private:
    // Screen dimension constants
    int m_screenHeight;
    int m_screenWidth;

    // The window we'll be rendering to
    SDL_Window* m_window ;

    // OpenGL context
    SDL_GLContext m_openGLContext;

    // Object in our scene contained in a vector
    std::vector<OBJ*> m_objects;

    // Object in our scene contained in a vector
    std::vector<OBJ*> m_objectsS1;
    std::vector<OBJ*> m_objectsS2;
    std::vector<OBJ*> m_objectsS3;

    // the single camera of our scene
    Camera* m_camera;
};

#endif
