//
//  ColourData.hpp
//  PLG
//
//  Created by Lukas Zierahn on 04.01.20.
//  Copyright © 2020 Lukas Zierahn. All rights reserved.
//

#ifndef ColorData_hpp
#define ColorData_hpp

#include "Include.h"

#include <stdio.h>

class ColorData {
public:
    GLuint colorBuffer;
    
    int count;
    bool initialized = false;
    
    ColorData(int count);
    
    void SetUnicodeColour(GLubyte r, GLubyte g, GLubyte b);
    void SetUnicodeColour(GLfloat r, GLfloat g, GLfloat b);
    
    ~ColorData();
};


#endif /* ColourData_hpp */
