//
//  Texture.cpp
//  
//
//  Created by Lukas Zierahn on 23.06.19.
//

#include <stdio.h>
#include <stdlib.h>
#include "Texture.hpp"

Texture::Texture(string key, string path) {
    this->key = key;
    glGenTextures(1, &texture);
    LoadBMPFromPath(path);
}

void Texture::LoadBMPFromPath(string path) {
    unsigned char header[54];
    
    FILE* file = fopen(path.c_str(), "rb");
    if (!file) {
        printf("Texture: %s not found\n", path.c_str());
        return;
    }
    
    if (fread(header, 1, 54, file) != 54){
        printf("Not a correct BMP file\n");
        fclose(file);
        return;
    }
    
    if (header[0] != 'B' || header[1] != 'M'){
        printf("Tried to open a none BMP-file as a BMP-file\n");
        fclose(file);
        return;
    }
    
    if (*(int*)&(header[0x1E]) != 0 || *(int*)&(header[0x1C]) != 24) {
        printf("Not a 24bpp BMP file\n");
        fclose(file);
        return;
    }
    
    unsigned int dataPos;
    unsigned int imageSize;
    
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    
    printf("Reading in Texture %s, %d/%d %f mb\n", path.c_str(), width, height, width * height * 3 / 1000000.0f);

    if (imageSize == 0)    imageSize = width * height * 3;
    if (dataPos == 0)      dataPos = 54;
    
    data = new unsigned char [imageSize];
    
    fread(data, 1, imageSize, file);
    fclose (file);
    
    GenerateTexture();
}

void Texture::GenerateTexture() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::EditPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    long absolutePosition = (width * y + x) * 3;
    
    //this is stored BGR so be aware
    data[absolutePosition] = b;
    data[absolutePosition + 1] = g;
    data[absolutePosition + 2] = r;
}

Texture::~Texture() {
    delete [] data;
    glDeleteTextures(1, &texture);
}
