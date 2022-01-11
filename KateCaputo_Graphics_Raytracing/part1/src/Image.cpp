#include "Image.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// a helper for checking for comments in a ppm file
bool checkComment(ifstream &inFile) {
    char hashtag;
    while (hashtag = inFile.peek(), hashtag == '\n') {
        inFile.get();
    }
    if (hashtag == '#') {
        char buffer[1000];
        inFile.getline(buffer, 999);
        return true;
    } else {
        return false;
    }
}

// is this pixel inside the circle?
bool isInside(int xPoint, int yPoint, int rad, int x, int y)
{
    // dist from center pt
    int d = ((x - xPoint) * (x - xPoint)) + ((y - yPoint) * (y - yPoint));
    d = sqrt(d);

    // if the distance btwn the point and the center of the circle is larger
    // than the circle's radius, the point is not inside the circle
    if (d <= rad)
        return true;
    else
        return false;
}

// constructor that just returns a black background
PPM::PPM() {
    m_width = 400;
    m_height = 400;
    stringPPM = "P3";
    maxColor = 255;

    needToDelete = true;

    // assignes all items in pixel data to be 0 to get a black background color
    int arraysize = m_width * m_height * 3;
    m_PixelData = new unsigned char[arraysize];

    for (int i = 0; i < arraysize; i++) {
        m_PixelData[i] = 0;
    }
}

// constructor that just returns a black background
PPM::PPM(int w, int h, int c) {
    m_width = w;
    m_height = h;
    stringPPM = "P3";
    maxColor = c;

    needToDelete = true;

    // assignes all items in pixel data to be 0 to get a black background color
    int arraysize = m_width * m_height * 3;
    m_PixelData = new unsigned char[arraysize];

    for (int i = 0; i < arraysize; i++) {
        m_PixelData[i] = 0;
    }
}

// Constructor loads a filename with the ppm extension
PPM::PPM(std::string fileName, uint8_t* &refArray) {

    needToDelete = false;

    // opening the file
    ifstream inFile;
    inFile.open(fileName);

    // check to see if the file was able to be opened
    if (! inFile) {
        cout << "couldn't open ppm file with given name" << endl;
    }

    // variables to keep track of the first 4 elements read in
    int countHeader = 0;
    int* arrayHeader = new int[3];

    // reads in the header lines, ignoring comments
    std::string line;
    while (countHeader < 4) {
        if (checkComment(inFile)) {
        } else if (countHeader == 0) {
            inFile >> stringPPM;
            countHeader++;
        } else {
            inFile >> arrayHeader[countHeader - 1];
            countHeader++;
        }
    }

    // checks for invalid PPM type
    if (stringPPM != "P3" && stringPPM != "P6") {
        cout << "not a valid type of PPM" << endl;
        inFile.close();
    }

    // checks for invalid width and height, else assigns variables
    if (arrayHeader[0] < 0 || arrayHeader[1] < 0) {
        cout << "invalid height or width" << endl;
        inFile.close();
    } else {
        m_width = arrayHeader[0];
        m_height = arrayHeader[1];
    }

    // checks for invalid number of bits, else assigns variable
    if (stringPPM == "P6" && arrayHeader[2] > 255) {
        cout << "invalid max bits" << endl;
        inFile.close();
    } else {
        maxColor = arrayHeader[2];
    }  

    // size of array
    int arraysize = m_width * m_height * 3;
    
    // variable to store all RBG values for all pixels in an array
    refArray = new unsigned char[arraysize];

    // a temporary int array to hold pixel RBG vales
    int* temp = new int[arraysize];

    int count = 0;

    // reads in the RBG data, checking for comments in between
    while (count < arraysize && !inFile.eof()) {
        if (!checkComment(inFile)) {
            inFile >> temp[count];
            count++;
        }
    }

    // assignes all items in the temporary int array into the uint8_t array
    for (int i = 0; i < arraysize; i++) {
        refArray[i] = temp[i];
    }

    inFile.close();
}

// deletes allocated memory
PPM::~PPM(){
    // reclaims allocated memory
    if (needToDelete) {
        delete [] m_PixelData;
    }
}

// Saves a PPM Image to a new file. P3 format
void PPM::savePPM(std::string outputFileName){

    // opens a file to output
    ofstream outFile;

    // writes to a file by the given name
    outFile.open(outputFileName);

    // writes the header
    outFile << stringPPM << endl;
    outFile << m_width << endl;
    outFile << m_height << endl;
    outFile << maxColor << endl;

    // writes the pixel data
    std::string line = "";
    for (int j = 0; j < m_height * m_width * 3; j++) {
        string cur = std::to_string(pixelData()[j]);
        line.append(cur);
        line.append(" ");
    }

    // removes trailing space
    outFile << line.substr(0, line.length() - 1) << endl;

    //closes the output file
    outFile.close();
}

// Saves a PPM Image to a new file. P6 format
void PPM::saveP6(std::string outputFileName){

    // opens a file to output
    ofstream outFile2;

    // writes to a file by the given name
    outFile2.open(outputFileName);

    // writes the header
    outFile2 << "P6" << endl;
    outFile2 << m_width << endl;
    outFile2 << m_height << endl;
    outFile2 << maxColor << endl;

    for (int j = 0; j < m_height * m_width * 3; j++) {
        outFile2 << (char) (m_PixelData[j] % (maxColor + 1));
    }

    //closes the output file
    outFile2.close();
}

// Darkens the image (/2 all RBG values)
void PPM::darken(){
    for (int i = 0; i < m_height * m_width * 3; i++) {
        uint8_t old = pixelData()[i];
        if (old != 0) {
            pixelData()[i] = old / 2;
        }
    }
}

// Lighten the image (x2 all RGB values)
void PPM::lighten(){
    for (int i = 0; i < m_height * m_width * 3; i++) {
        uint8_t old = pixelData()[i];
        if (old * 2 <= 255) {
            pixelData()[i] = old * 2;
        } else {
            pixelData()[i] = 255;
        }
    }
}

// sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B){
    // finding the index of the given pixel
    int i = (((y - 1) * m_width) + (x - 1)) * 3;

    pixelData()[i] = R;
    pixelData()[i + 1] = G;
    pixelData()[i + 2] = B;
}

// draws a circle onto an image
void PPM::drawCircle() {

    // center of circle is in center of image
    int xPoint = m_width / 2;
    int yPoint = m_height / 2;
    int rad;

    // default setting radius to be 1/4 the size of the image's smallest dimestion
    if (m_width > m_height) {
        rad = m_height / 4;
    } else {
        rad = m_width / 4;
    }

    // iterates over pixel grid
    for (int i = 1; i <= m_height; i++) {
        for (int j = 1; j <= m_width; j++) {
            // if the pixel should be in the circle, changes its color to red
            if (isInside(xPoint, yPoint, rad, j, i)) {
                setPixel(j, i, 255, 0, 0);
            }
        }
    }
}