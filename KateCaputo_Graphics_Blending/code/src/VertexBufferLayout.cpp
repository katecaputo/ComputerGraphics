#include "VertexBufferLayout.hpp"
#include <iostream>


VertexBufferLayout::VertexBufferLayout(){
}

VertexBufferLayout::~VertexBufferLayout(){
    // Delete our buffers that we have previously allocated
    glDeleteBuffers(1,&m_vertexPositionBuffer);
    glDeleteBuffers(1,&m_indexBufferObject);
}


void VertexBufferLayout::Bind(){
    // Bind to our vertex array
    glBindVertexArray(m_VAOId);
    // Bind to our vertex information
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    // Bind to the elements we are drawing
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
}

// unbind when necessary
void VertexBufferLayout::Unbind(){
        // Bind to our vertex array
        glBindVertexArray(0);
        // Bind to our vertex information
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Bind to the elements we are drawing
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// A normal map layout needs the following attributes
//
// positions: x,y,z
// normals:  x,y,z
// texcoords: s,t
// tangent: t_x,t_y,t_z
// bitangent b_x,b_y,b_z
void VertexBufferLayout::CreateNormalBufferLayout(unsigned int vcount,unsigned int icount, float* vdata, unsigned int* idata ){
		m_stride = 14;
        
        static_assert(sizeof(GLfloat)==sizeof(float), "GLFloat and gloat are not the same size on this architecture");
       
        // VertexArrays
        glGenVertexArrays(1, &m_VAOId);
        glBindVertexArray(m_VAOId);

        // Vertex Buffer Object (VBO)

        glGenBuffers(1, &m_vertexPositionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
        glBufferData(GL_ARRAY_BUFFER, vcount*sizeof(float), vdata, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        // pass in our vertex data
        glVertexAttribPointer(  0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*m_stride, 0);

        // Add three floats for normal coordinates
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,sizeof(float)*m_stride,(char*)(sizeof(float)*3));

        // Add two floats for texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE,sizeof(float)*m_stride,(char*)(sizeof(float)*6));

        // Add three floats for tangent coordinates
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,3,GL_FLOAT, GL_FALSE,sizeof(float)*m_stride,(char*)(sizeof(float)*8));

        // Add three floats for bi-tangent coordinates
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4,3,GL_FLOAT, GL_FALSE,sizeof(float)*m_stride,(char*)(sizeof(float)*11));
        
		// VBO for index buffer
        static_assert(sizeof(unsigned int)==sizeof(GLuint),"Gluint not same size!");

		// Setup an index buffer
        glGenBuffers(1, &m_indexBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount*sizeof(unsigned int), idata,GL_STATIC_DRAW);
    }