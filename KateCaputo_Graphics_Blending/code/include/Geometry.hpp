/** @file Geometry.hpp
 *  @brief Organizes vertex and triangle information.
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>

// Purpose of this class is to store vertice and triangle information
class Geometry{
public:
	// Constructor
	Geometry();

	// Destructor
	~Geometry();
	
	// Functions for working with individual vertices
	unsigned int GetBufferSizeInBytes();
    // Retrieve the buffer data size
	unsigned int GetBufferDataSize();

    // Retrieve the Buffer Data Size
	float* GetBufferDataPtr();

	// Add a new vertex 
	void AddVertex(float x, float y, float z, float s, float t);

    // pushes all attributes into a single vector
	void Gen();

	// Creates a triangle from 3 indicies
	void MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2); 

    // Retrieve how many indicies there are
	unsigned int GetIndicesSize();

    // Retrieve the pointer to the indices
	unsigned int* GetIndicesDataPtr();

private:
	// m_bufferData stores all of the vertexPositons, coordinates, normals, etc.
	std::vector<float> m_bufferData;

    // Individual components:
	std::vector<float> m_vertexPositions;
	std::vector<float> m_textureCoords;
	std::vector<float> m_normals;
	std::vector<float> m_tangents;
	std::vector<float> m_biTangents;

	// The indices for a indexed-triangle mesh
	std::vector<unsigned int> m_indices;
};

#endif
