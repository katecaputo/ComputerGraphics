#include "SDLGraphicsProgram.hpp"
#include "OBJ.hpp"

#include <iostream>
using namespace std;

int main(int argc, char** argv){

	SDL_Log("Beginning Intitialization");

	// Create an instance of a SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(1280, 720);

	// loops through the program
	mySDLGraphicsProgram.Loop();

	return 0;
}
