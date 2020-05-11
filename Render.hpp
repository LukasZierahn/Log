//
//  render.hpp
//  PLG
//
//  Created by Lukas Zierahn on 02.06.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//

#ifndef render_hpp
#define render_hpp

#include "Include.h"
#include "ShaderConfigs.h"

class ColoredObject;
class DrawableObject;
class Camera;
class ModelLoader;

class Render {
private:
    int width = 1000;
    int height = 1000;
    bool show;
    
    ModelLoader* modelLoader;
    Camera* camera;
    
    vector<GLuint> programs;
    vector<vector<DrawableObject*>> drawVector;

    GLFWwindow* window;
    
    GLuint vertexArrayID;
    
    void InitGL();
    GLuint LoadShader(const char* path, GLenum type);
    
public:
    Render(): Render(true) { }
    Render(bool show);

    GLFWwindow* getWindow() { return window; }
    
    void Draw();
    void setShow(bool show) { this->show = show; }
    
    void addDrawableObject(DrawableObject* drawObject);
    void removeDrawableObject(DrawableObject* drawObject);

    int getHeight() { return height; }
    int getWidth() { return width; }
    GLuint getProgram(int program) { return programs[program]; }
    ModelLoader* getModelLoader() { return modelLoader; }
    
    Camera* getCamera() { return camera; }
    
    ~Render();
};

#endif /* render_hpp */
