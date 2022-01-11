#ifndef PPM_HPP
#define PPM_HPP

#include <string>
#include <cmath>

class PPM{
public:

    // default constructor
    PPM();
    
    // constructor for when theres no given file; gray background
    PPM(int w, int h, int col);

    // constructor for when we are loading in a ppm file
    PPM(std::string fileName, uint8_t* &refArray);

    // destructor
    ~PPM();

    // saves a PPM image file as P3 format
    void savePPM(std::string outputFileName);

    // saves a PPM image file as P6 format
    void saveP6(std::string p6fileName);

    // darkens a ppm image
    void darken();

    // lightens a ppm image
    void lighten();

    // returns the raw pixel data
    inline unsigned char* pixelData() const { 
        return m_PixelData; 
    }

    // returns image width
    inline int getWidth() const {
        return m_width;
    }

    // returns image height
    inline int getHeight() const {
        return m_height;
    }

    // returns image max color bytes
    inline int getMaxBytes() const {
        return maxColor;
    }

    // Sets a pixel to a specific R,G,B value 
    void setPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B);
    
    // draws a red circle onto an image
    void drawCircle();

private:    
    // raw pixel data as RBG
    uint8_t* m_PixelData;

    // image width and height
    int m_width{0};
    int m_height{0};

    // variables to store the given maximum color value and type of PPM
    std::string stringPPM;
    int maxColor;

    // do i need to deallocate memory?
    //      m_PixelData is only allocated in some constructors
    bool needToDelete;
};



#endif
