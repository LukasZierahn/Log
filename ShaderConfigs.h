//
//  ShaderConfigs.h
//  Log
//
//  Created by Lukas on 09.05.20.
//  Copyright © 2020 Lukas. All rights reserved.
//

#ifndef ShaderConfigs_h
#define ShaderConfigs_h

#include <string>

enum shaderConfig {texturedObject, coloredObject};
const int shaderConfigCount = 2;

const string vertexShader[] = {
    "shader/VS.GLSL",
    "shader/Colord_VS.GLSL"
};

const string fragmentShader[] = {
    "shader/FS.GLSL",
    "shader/Colord_FS.GLSL"
};

#endif /* ShaderConfigs_h */
