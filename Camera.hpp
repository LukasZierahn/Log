//
//  Camera.hpp
//  PLG
//
//  Created by Lukas Zierahn on 03.06.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>

class Render;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


class Camera {
private:
    Render* render = nullptr;
    
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 jointMatrix;

    vec3 position = vec3(4, 3, 3);
    vec3 direction = vec3(-1, -1, -1);
    vec3 up = vec3(0, 1, 0);

    float fov = 45.0f;
    
public:
    Camera(Render* render);
    
    void UpdateMatricies();
    
    mat4 getViewMatrix() { return viewMatrix; }
    mat4 getProjectionMatrix() { return projectionMatrix; }
    mat4 getJointMatrix() { return jointMatrix; }

    void setPosition(vec3 pos) { position = pos; }
    void setDirection(vec3 dir) { direction = dir; }
    void setUp(vec3 up) { this->up = up; }
    
    void addPosition(vec3 pos) { position += pos; }
    void addDirection(vec3 dir) { direction += dir; }

};

#endif /* Camera_hpp */
