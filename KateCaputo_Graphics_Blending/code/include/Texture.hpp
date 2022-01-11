#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <string>

class Texture{
public:
    // Constructor
    Texture();

    // Destructor
    ~Texture();

	// Loads and sets up an actual texture
    void LoadTexture(const std::string filepath);

	// slot tells us which slot we want to bind to.
    void Bind(unsigned int slot=0) const;

    // Be done with our texture
    void Unbind();

	// Loads a PNG from memory.
    void LoadPNG();
    
    // Return the width
    inline int GetWidth(){
        return m_width;
    }

    // Return the height
    inline int GetHeight(){
        return m_height;
    }

private:
    // Store a unique ID for the texture
    GLuint m_textureID;

	// Filepath to the image loaded
    std::string m_filepath;

	// Raw pixel data
    unsigned char* m_pixelData;

	// Size and format of image
    int m_width{0}; // Width of the image
    int m_height{0}; // Height of the image

};



#endif
