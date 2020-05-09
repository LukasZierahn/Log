//
//  ColourData.cpp
//  PLG
//
//  Created by Lukas Zierahn on 04.01.20.
//  Copyright © 2020 Lukas Zierahn. All rights reserved.
//

#include "ColorData.hpp"


ColorData::ColorData(int count): count(count) {
    glGenBuffers(1, &colorBuffer);
}

void ColorData::SetUnicodeColour(GLubyte r, GLubyte g, GLubyte b) {
    SetUnicodeColour(r / 255.0f, g / 255.0f, b / 255.0f);
}

void ColorData::SetUnicodeColour(GLfloat r, GLfloat g, GLfloat b) {    
    vector<GLfloat> buffer(count * 3, 0.0f);
    for (int i = 0; i < count * 3; i += 3) {
        buffer[i] = r;
        buffer[i + 1] = g;
        buffer[i + 2] = b;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(GLfloat), buffer.data(), GL_STATIC_DRAW);
    initialized = true;
}

ColorData::~ColorData() {
    glDeleteBuffers(1, &colorBuffer);
}
