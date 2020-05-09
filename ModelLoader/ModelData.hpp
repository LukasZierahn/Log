//
//  ModelData.hpp
//  PLG
//
//  Created by Lukas Zierahn on 20.12.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//

#ifndef ModelData_hpp
#define ModelData_hpp

#include "Include.h"

#include <stdio.h>

class ModelData {
public:
    string key;
    GLuint vertexBuffer;
    GLuint uvBuffer;
    GLuint normalBuffer;
    GLuint indexBuffer;
    
    int vertexCount;
    int indexCount;
    
    bool hasNormals = false;
    bool hasUVs = false;
};


#endif /* ModelData_hpp */
