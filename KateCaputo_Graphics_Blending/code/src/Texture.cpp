#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif


#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <memory>

// Default Constructor
Texture::Texture(){

}


// Default Destructor
Texture::~Texture(){
	// Delete our texture from the GPU
	glDeleteTextures(1,&m_textureID);

	// Delete our pixel data.	
	if(m_pixelData!=NULL){
		delete[] m_pixelData;
	}
}

void Texture::LoadTexture(const std::string filepath){
	// Set member variable
    m_filepath = filepath;

	// Load our actual image data from PNG
    // this allows for a 4th alpha value unlike in a ppm file
    // (RBGA instead of RBG)
    LoadPNG();

    glEnable(GL_TEXTURE_2D); 

    // ENABLE TRANSPARANCY
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Generate a buffer for our texture
    glGenTextures(1,&m_textureID);
    // select texture to bind to
    glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

    // IMPORTANT : note GL_RGBA instead of GL_RGB
	glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixelData);

	glBindTexture(GL_TEXTURE_2D, 0);
}

// Binds a texture
void Texture::Bind(unsigned int slot) const{
	// sets the active texture slot that we want to occupy
    glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

// unbinds when we are done
void Texture::Unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Loads the data from a PNG image
void Texture::LoadPNG(){
    
    // ... x = width, y = height, n = # 8-bit components per pixel ...
    // ... replace '0' with '1'..'4' to force that many components per pixel
    // ... but 'n' will always be the number that it would have been if you said 0
    int x,y,n;
    std::cout << "Reading in PNG file: " << m_filepath << std::endl;
    unsigned char *data = stbi_load(m_filepath.c_str(), &x, &y, &n, 0);
    m_pixelData = data;
    m_width = x;
    m_height = y;
    
    // ... process data if not NULL ..
    if (data != nullptr && x > 0 && y > 0)
    {
        std::cout << "PNG sucessfully loaded" << std::endl;
    }
    else
    {
        std::cout << "Some error\n";
    }

    stbi_image_free(data);
}
