//
//  Texture.hpp
//  
//
//  Created by Lukas Zierahn on 23.06.19.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "Include.h"

class Texture {
    string key;
    
    unsigned char* data = nullptr;
    
    int width;
    int height;
    
    GLuint texture;
    
public:
    Texture(string key, string path);
    
    void LoadBMPFromPath(string path);
    void GenerateTexture();
    
    void EditPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    
    GLuint getTextureId() { return texture; }
    
    int getWidth() { return width; }
    int getHeight() { return height; }

    ~Texture();
};

#endif /* Texture_hpp */
