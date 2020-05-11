//
//  render.cpp
//  PLG
//
//  Created by Lukas Zierahn on 02.06.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//

#include "Render.hpp"

#include "ColoredObject.hpp"
#include "TexturedObject.hpp"
#include "ModelLoader/ModelLoader.hpp"
#include "Camera.hpp"


void Render::InitGL() {
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "PLG", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create Window GLFW\n");

        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");

        glfwTerminate();
        return;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    
    programs = vector<GLuint>(shaderConfigCount, 0);
    for (int i = 0; i < shaderConfigCount; i++) {
        programs[i] = glCreateProgram();
        GLuint VS_Id = LoadShader(vertexShader[i].c_str(), GL_VERTEX_SHADER);
        GLuint FS_Id = LoadShader(fragmentShader[i].c_str(), GL_FRAGMENT_SHADER);

        glAttachShader(programs[i], VS_Id);
        glAttachShader(programs[i], FS_Id);
        glLinkProgram(programs[i]);
        
        GLint result = GL_FALSE;
        int infoLogLength;
        glGetProgramiv(programs[i], GL_LINK_STATUS, &result);
        glGetProgramiv(programs[i], GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            vector<char> ProgramErrorMessage(infoLogLength+1);
            glGetProgramInfoLog(programs[i], infoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }
        
        
        glDetachShader(programs[i], VS_Id);
        glDetachShader(programs[i], FS_Id);
        
        glDeleteShader(VS_Id);
        glDeleteShader(FS_Id);
    }
}

void Render::addDrawableObject(DrawableObject *drawObject) {
    int shaderConfig = drawObject->getShaderConfig();
    for (int i = 0; i < drawVector[shaderConfig].size(); i++) {
        if (drawVector[shaderConfig][i] == drawObject) {
            return;
        }
    }
    
    drawVector[shaderConfig].push_back(drawObject);
}

void Render::removeDrawableObject(DrawableObject *drawObject) {
    int shaderConfig = drawObject->getShaderConfig();
    for (int i = drawVector[shaderConfig].size() - 1; i >= 0; i--) {
        if (drawVector[shaderConfig][i] == drawObject) {
            drawVector[shaderConfig].erase(drawVector[shaderConfig].begin() + i);
            return;
        }
    }
}

Render::Render(bool show): show(show) {
    InitGL();
    drawVector = vector<vector<DrawableObject*>>(shaderConfigCount);

    this->show = false;
    
    modelLoader = new ModelLoader(this);
    camera = new Camera(this);
}

void Render::Draw() {
    if (!show) {
        return;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera->UpdateMatricies();
    
    for (int i = 0; i < drawVector.size(); i++) {
        glUseProgram(programs[i]);
        for (DrawableObject* drawObject : drawVector[i]) {
            drawObject->Draw();
        }
    }
    
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

GLuint Render::LoadShader(const char* path, GLenum type) {
    string shaderCode;
    ifstream shaderStream(path, ios::in);
    if (shaderStream.is_open()){
        stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    } else {
        printf("Failed to load shader from path %s\n", path);
        return 0;
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    GLuint shaderId = glCreateShader(type);
    char const* sourcePointer = shaderCode.c_str();
    glShaderSource(shaderId, 1, &sourcePointer, NULL);
    glCompileShader(shaderId);
    
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0){
        vector<char> errorMessage(InfoLogLength+1);
        glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &errorMessage[0]);
        printf("%s\n", &errorMessage[0]);
    }
    
    return shaderId;
}

Render::~Render() {
    for (int i = 0; i < drawVector.size(); i++) {
        glDeleteProgram(programs[i]);
    }
    glDeleteVertexArrays(1, &vertexArrayID);
    
    delete modelLoader;
    delete camera;
}
