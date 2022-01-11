#ifndef VERTEX_BUFFER_LAYOUT_HPP
#define VERTEX_BUFFER_LAYOUT_HPP

// The glad library helps setup OpenGL extensions.
#include <glad/glad.h>


class VertexBufferLayout{ 
public:
    // Generates a new buffer
    VertexBufferLayout();

    // Destroys all of our buffers.
    ~VertexBufferLayout();

    // Selects the buffer to bind
    void Bind();
    
    // Unbind our buffers
    void Unbind();

    // Creates a vertex and index buffer object
    // Format is: x,y,z, xn, yn, zn, s,t
    // vcount: the number of vertices
    // icount: the number of indices
    // vdata: A pointer to an array of data for vertices
    // idata: A pointer to an array of data for indices
    void CreateNormalBufferLayout(unsigned int vcount,unsigned int icount, float* vdata, unsigned int* idata );

private:
    // Vertex Array Object
    GLuint m_VAOId;

    // Vertex Buffer
    GLuint m_vertexPositionBuffer;

    // Index Buffer Object
    GLuint m_indexBufferObject;

    // Stride of data
    unsigned int m_stride{0};
};


#endif
