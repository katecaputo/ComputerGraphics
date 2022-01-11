#include "OBJ.hpp"
#include "Triangle.hpp"
#include "vec2.hpp"
#include "Texture.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// a struct to hold vertex data
struct VertexData { 
	float x;
    float y;
    float z;
    float xn;
    float yn;
    float zn;
	float s;
    float t;

	VertexData(float _x, float _y, float _z, float _xn, float _yn, float _zn, float _s, float _t): 
    x(_x), y(_y), z(_z), xn(_xn), yn(_yn), zn(_zn), s(_s), t(_t) {}
 
	// are two VertexData structs equal
	bool operator== (const VertexData &v2) {
		if( (x == v2.x) 
        && (y == v2.y) 
        && (z == v2.z) 
        && (xn == v2.xn)
        && (yn == v2.yn)
        && (zn == v2.zn)
        && (s == v2.s) 
        && (t == v2.t)) {
			return true;
		}
		return false;
	}
};

// gets the texutre (ppm) file name from the material file
std::string getPPMFileName(std::string mtlFileName, std::string objFilePath) {

    int sz = objFilePath.length();
    char* c = const_cast<char*>(objFilePath.c_str());

    int i = objFilePath.length();
    bool ongoing = true;

    // string to hold the file path of the directory that the obj, mtl, 
    // and ppm file should be in
    std::string begFilePath;

    // iterates backwards through the full filepath until reaching the last '/' of the string
    // the file path of the mtl will be the same as that of the obj until this '/', but the full
    // path is needed when opening the mtl and ppm file
    while(ongoing) {
        if (c[i] == '/') {
            begFilePath = objFilePath.substr(0, i + 1);
            ongoing = false;
        } else {
            i--;
        }
    }

    //opening the file
    std::ifstream file2;
    // merges the rest of the filepath we found with the actual name of the mtl
    // previously found in the obj file
    file2.open(begFilePath + mtlFileName); 

    std::string line;
    std::string finalName;
    // check to see if the file was able to be opened
    if (file2) {
        while (getline(file2, line) && !file2.eof()) {
            //std::cout << line << std::endl;
            //we are only interested in the line that starts with map_Kd currently
            if (line.substr(0, 6) == "map_Kd") {
                // finalName is the name of the PPM file (without the full filepath)
                finalName = line.substr(7, line.length() - 1);   
            }
        }
        
        //closes the .mtl file
        file2.close();
    } else {
        std::cout << "couldn't open file with given name" << std::endl;
    }

    if (finalName == "") {
        std::cout << "error finding PPM file name" << std::endl;
    }

    // returns the full filepath, using the same beginning part found earlier
    // (since the PPM file should also be in the same directory) and the name of the PPM file
    return begFilePath + finalName;
}


// Constructor loads a filename with the .obj extension 
OBJ::OBJ(std::string fileName){

    // opening the file
    std::ifstream inFile;
    inFile.open(fileName);  

    // check to see if the file was able to be opened
    if (!inFile) {
        std::cout << "couldn't open obj file with given name" << std::endl;
    }

    std::string line;

    //stores the .mtl and .ppm file names (not full filepath)
    std::string texFileName; 
    std::string mtlFileName;

    // the vertex, normal, and texture data read into vectors from the 'v' 'vn' and 'vt' lines
    std::vector<float> tempXYZdata;
    std::vector<float> tempSTdata;
    std::vector<float> tempNORMALdata;

    // all the VertexData structs (from the 'f' lines) without repetitions
    std::vector<VertexData> allVertexDatas;
    
    // parses file
    while (getline(inFile, line) && !inFile.eof()) {

        //turns single line into a stringstream and creates a temporary
        //string that will represent the characters between delimeters (' ');
        std::string temp; 
        int c = 0;

        // handles material line
        if (line.substr(0, 6) == "mtllib") {
            mtlFileName = line.substr(7, line.length());
            texFileName = getPPMFileName(mtlFileName, fileName);
            if (texFileName.substr(line.length() - 4,line.length() - 1) != ".mtl") {
                texFileName = "";
            }
        }

        // handles vertex lines
        else if (line.substr(0,2) == "v ") {
            std::string str = line.substr(2, line.length() - 3);
            std::stringstream ss(str);

            while (getline(ss, temp, ' ')) {
                tempXYZdata.push_back(std::stof(temp));
            }
        }

        // handles normal lines
        else if (line.substr(0,2) == "vn") {
            std::string str = line.substr(3, line.length() - 4);
            std::stringstream ss(str);

            while (getline(ss, temp, ' ')) {
                tempNORMALdata.push_back(std::stof(temp));
            }
        }       

        // handles texture lines
        else if (line.substr(0,2) == "vt") {
            std::string str = line.substr(3, line.length() - 2);
            std::stringstream ss(str);

            while (getline(ss, temp, ' ')) {
                tempSTdata.push_back(std::stof(temp));
            }
        } 
        
        // handles face lines
        else if (line.substr(0,1) == "f") {
            std::string str = line.substr(2, line.length() - 1);
            int sz = str.length();
            char* c = const_cast<char*>(str.c_str());

            std::string a;
            for (int i = 0; i < sz; i++) {
                if (c[i] != '/') {
                    // appends characters to number (as string) until reaching '/'
                    a += c[i];
                } else {

                    // temporarily stores the face data for one vertex (x/x/x)
                    unsigned long texInd;
                    unsigned long xyzInd;
                    unsigned long normalInd;

                    // stores the full vertex index number
                    if (a!= "") {
                        xyzInd = std::stoul(a) - 1;
                        a= "";
                    }
                    i++;

                    // deals with texture buffer argument
                    while (c[i] != '/') {
                        a+= c[i];
                        i++;
                    }

                    // stores the full texture index number
                    if (a != "") {
                        texInd = std::stoul(a) - 1;
                        a = "";
                    }
                    i++;

                    // appends characters to number (as string) until reaching ' '
                    while(!isspace(c[i])) {
                        a += c[i];
                        i++;
                    };
                    // stores the full normal index
                    normalInd = std::stoul(a) - 1;
                    a = "";

                    float sData;
                    float tData;

                    if (texFileName == "") {
                        sData = 0;
                        tData = 0;
                    } else {
                        sData = tempSTdata[texInd * 2];
                        tData = tempSTdata[texInd * 2 + 1];
                    }

                    //creates an instance of VertexData using the vertex and normal indices
                    VertexData v {tempXYZdata[xyzInd * 3],
                    tempXYZdata[xyzInd * 3 + 1],
                    tempXYZdata[xyzInd * 3 + 2],
                    0,
                    0,
                    0,
                    sData,
                    tData};

                    // determines if an equivalent VertexData already exists
                    bool add = true;
                    for (int i = 0; i < allVertexDatas.size(); i++) {
                        // if so, does not add this new one to the full list and instead
                        // adds the index of the equivalent one to the index buffer
                        if (v == allVertexDatas[i]) {
                            add = false;
                            m_indices.push_back(i);
                        } 
                    }

                    // however, if this VertexData is new, it gets added to the full list
                    // of VertexDatas and it's index is added to the index buffer
                    if (add) {
                        allVertexDatas.push_back(v);
                        m_indices.push_back(allVertexDatas.size() - 1);
                    }
                    
                }
            }
        }
    }

    // closes the file
    inFile.close();

    std::cout << "CLOSE" << std::endl;

    // ------------------------ NEW FOR THIS ASSIGNMENT ------------------------

    vec3 gray_color = vec3(0.5, 0.5, 0.5);

    // creats a texture of the ppm image, if one exists    
    std::shared_ptr<Texture> ppmTex;
    if (texFileName == "") {
        ppmTex = std::make_shared<SolidColor>(gray_color);
    } else {
        ppmTex = std::make_shared<Image_Tex>(texFileName);
    }

    std::shared_ptr<Material> ppmMat = std::make_shared<Lambertian>(ppmTex);

    int j = 0;
    while (j < m_indices.size()) {
        // gets the indices where the triangle's vertex data is stored
        unsigned int ind1 = m_indices[j];
        unsigned int ind2 = m_indices[j + 1];
        unsigned int ind3 = m_indices[j + 2];

        // gets the vertex data at those indices
        vec3 triPoint1 = vec3(allVertexDatas[ind1].x, allVertexDatas[ind1].y, allVertexDatas[ind1].z);
        vec3 triPoint2 = vec3(allVertexDatas[ind2].x, allVertexDatas[ind2].y, allVertexDatas[ind2].z);
        vec3 triPoint3 = vec3(allVertexDatas[ind3].x, allVertexDatas[ind3].y, allVertexDatas[ind3].z);

        vec2 uv1 = vec2(allVertexDatas[ind1].s, allVertexDatas[ind1].t);
        vec2 uv2 = vec2(allVertexDatas[ind2].s, allVertexDatas[ind2].t);
        vec2 uv3 = vec2(allVertexDatas[ind3].s, allVertexDatas[ind3].t);
        j = j + 3;

        // creates a triangle given the vertices loaded in
        Hittable* tempTri = new Triangle(triPoint1, triPoint2, triPoint3, uv1, uv2, uv3, gray_color, ppmMat);

        // adds the triangle to the object's list of them
        m_triangles.push_back(tempTri);
    }
}

// destructor deletes allocated memory
OBJ::~OBJ(){
    for (int i = 0; i < m_triangles.size(); i++) {
        delete m_triangles[i];
    }
}