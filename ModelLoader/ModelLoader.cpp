//
//  ModelLoader.cpp
//  PLG
//
//  Created by Lukas Zierahn on 03.06.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//

#include <stdio.h>
#include <vector>

#include "Texture.hpp"
#include "ModelData.hpp"

#include "ModelLoader.hpp"

ModelLoader::ModelLoader(Render* render) {
    this->render = render;
    LoadHardcodedModels();
}

ModelData* ModelLoader::getModel(string key) {
    if (modelDataMap.count(key) != 0) {
        return modelDataMap.at(key);
    } else {
        LoadModelFromFile(key);
        return modelDataMap.at(key);
    }
}

Texture* ModelLoader::getTexture(string key) {
    if (textureMap.count(key) != 0) {
        return textureMap.at(key);
    } else {
        LoadTextureFromFile(key);
        return textureMap.at(key);
    }
}

void ModelLoader::LoadModelFromFile(string key) {
    LoadModelFromFile(key, "data/" + key + ".cng");
}

void ModelLoader::LoadModelFromFile(string key, string path) {
    ifstream modelStream;
    char input;
    
    //open the document
    modelStream.open(path.c_str(), ifstream::in);
    
    if (modelStream.fail()) {
        modelStream.close();
        ConvertFileToCng(key);
        modelStream.open(path.c_str(), ifstream::in);
    }
    
    if (modelStream.fail()) {
        throw new runtime_error("failed to load cng-Model: " + key);
        return;
    }
    
    vector<vec3> pos;
    vector<vec2> uv;
    bool hasUVs = false;
    vector<vec3> normal;
    bool hasNormals = false;

    vector<unsigned short> faces;
    
    modelStream >> hasUVs;
    modelStream.get();
    modelStream >> hasNormals;
    
    //Load the Indexes and Vertexes
    while (!modelStream.eof())
    {
        //get the first char of the line
        modelStream.get(input);

        //Vertecies
        if (input == 'v')
        {
            modelStream.get(input);
            if (input == ' ')
            {
                vec3 newPos;
                modelStream >> newPos.x >> newPos.y >> newPos.z;
                pos.push_back(newPos);
                
                if (hasUVs) {
                    vec2 newUv;
                    modelStream >> newUv.x >> newUv.y;
                    uv.push_back(newUv);
                }
                
                if (hasNormals) {
                    vec3 newNormal;
                    modelStream >> newNormal.x >> newNormal.y >> newNormal.z;
                    normal.push_back(newNormal);
                }

                modelStream.get(input);
            }
        }
        //Indecies
        else if (input == 'f')
        {
            modelStream.get(input);
            if (input == ' ')
            {
                for (int i = 0; i < 3; i++)
                {
                    int newIndex;
                    modelStream >> newIndex;
                    faces.push_back(newIndex);
                    
                    modelStream.get(input);
                }
            }
        }
        //skip to the next line
        else if (input != '\n')
        {
            modelStream.get(input);

        }
    }
    
    ModelData* newModelData = new ModelData();
    newModelData->key = key;
    glGenBuffers(1, &newModelData->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newModelData->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(vec3), &pos[0], GL_STATIC_DRAW);
    newModelData->vertexCount = pos.size();
    
    if (hasUVs) {
        glGenBuffers(1, &newModelData->uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, newModelData->uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(vec2), &uv[0], GL_STATIC_DRAW);
    }
    newModelData->hasUVs = hasUVs;

    if (hasNormals) {
        glGenBuffers(1, &newModelData->normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, newModelData->normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(vec3), &normal[0], GL_STATIC_DRAW);
    }
    newModelData->hasNormals = hasNormals;
    
    glGenBuffers(1, &newModelData->indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newModelData->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned short), &faces[0] , GL_STATIC_DRAW);
    newModelData->indexCount = faces.size();
    
    modelDataMap[key] = newModelData;
}

void ModelLoader::ConvertFileToCng(string key) {
    ConvertFileToCng(key, "./datasrc/" + key + ".obj", "./data/" + key + ".cng");
}

void ModelLoader::ConvertFileToCng(string key, string srcPath, string targetPath) {
    //open the document
    ifstream inputStream;
    char input;
    inputStream.open(srcPath.c_str(), ifstream::in);
    
    if (inputStream.fail()) {
        throw new runtime_error("failed to open Model: " + key);
        return;
    }
    
    //Load in the data
    struct Vertex {
        int posIndex = 0;
        int uvIndex = 0;
        int normalIndex = 0;
        
        int cngIndex = -1;
        
        bool isEqual(Vertex target) {
            return posIndex == target.posIndex && uvIndex == target.uvIndex && normalIndex == target.normalIndex;
        }
    };
    
    vector<vec3> pos;
    vector<vec2> uv;
    bool hasUVs = false;
    vector<vec3> normal;
    bool hasNormals = false;
    
    //the cng vertex are the vertexies consisting of all pos, uv and normal, the faces are the cng index of the corrosponding vertex in cngVertex
    vector<Vertex> cngVertex;
    vector<int> face;
    
    while (!inputStream.eof())
    {
        inputStream.get(input);
        if (input == '\n') {
            break;
        }
        
        //Vertecies
        if (input == 'v')
        {
            inputStream.get(input);
            if (input == ' ')
            {
                vec3 newVertex;
                inputStream >> newVertex.x >> newVertex.y >> newVertex.z;
                pos.push_back(newVertex);
                
                inputStream.ignore(1000, '\n');
            }
            else if (input == 't')
            {
                hasUVs = true;
                
                inputStream.get(input);
                
                vec2 newTexVertex;
                inputStream >> newTexVertex.x >> newTexVertex.y;
                newTexVertex.y = 1 - newTexVertex.y;
                uv.push_back(newTexVertex);
                
                inputStream.ignore(1000, '\n');
            }
            else if (input == 'n') {
                hasNormals = true;
                
                inputStream.get(input);
                
                vec3 newNormalVertex;
                inputStream >> newNormalVertex.x >> newNormalVertex.y >> newNormalVertex.z;
                normal.push_back(newNormalVertex);
                
                inputStream.ignore(1000, '\n');
            }
        }
        else if (input == 'f') { //Indecies
            inputStream.get(input);
            if (input == ' ')
            {
                for (int i = 0; i < 3; i++)
                {
                    Vertex newCngVertex;
                    //inputStream >> newCngVertex.posIndex >> newCngVertex.normalIndex >> newCngVertex.uvIndex;
                    inputStream >> newCngVertex.posIndex;
                    inputStream.get(input); //this is always removing the slashes between the indexies
                    
                    if (hasUVs) {
                        inputStream >> newCngVertex.uvIndex;
                        inputStream.get(input);
                    }
                    
                    if (!hasUVs && hasNormals) {
                        inputStream.get(input);
                        inputStream.get(input);
                    }
                    
                    if (hasNormals) {
                        inputStream >> newCngVertex.normalIndex;
                        inputStream.get(input);
                    }
                                        
                    //making these zero indexed
                    newCngVertex.posIndex--;
                    newCngVertex.uvIndex--;
                    newCngVertex.normalIndex--;

                    
                    //checking if we got a new one
                    for (int j = 0; j < cngVertex.size(); j++) {
                        if (cngVertex[j].isEqual(newCngVertex)) {
                            newCngVertex = cngVertex[j];
                            break;
                        }
                    }
                    
                    //if we did get a new one
                    if (newCngVertex.cngIndex == -1) {
                        newCngVertex.cngIndex = cngVertex.size();
                        cngVertex.push_back(newCngVertex);
                    }
                    
                    face.push_back(newCngVertex.cngIndex);
                }
            }
        }
        else if (input != '\n') { //skip to the next line
            inputStream.ignore(1000, '\n');
            
        }
    }
    
    inputStream.close();
    
    ofstream outputStream;
    outputStream.open(targetPath.c_str(), ifstream::out);
    outputStream << fixed;
    
    outputStream << hasUVs << " " << hasNormals << "\n";

    for (int i = 0; i < cngVertex.size(); i++) {
        Vertex vertex = cngVertex[i];
        outputStream << "v " << pos[vertex.posIndex].x << " " << pos[vertex.posIndex].y << " " << pos[vertex.posIndex].z;
        
        if (hasUVs) {
            outputStream << " " << uv[vertex.uvIndex].x << " " << uv[vertex.uvIndex].y;
        }
        
        if (hasNormals) {
            outputStream << " " << normal[vertex.normalIndex].x << " " << normal[vertex.normalIndex].y << " " << normal[vertex.normalIndex].z << "\n";
        }
    }
    
    for (int i = 0; i < face.size(); i += 3) {
        outputStream << "f " << face[i] << " " << face[i+1] << " " << face[i+2] << "\n";
    }

    outputStream.close();
}

void ModelLoader::LoadTextureFromFile(string key) {
    LoadTextureFromFile(key, "data/" + key + ".bmp");
}

void ModelLoader::LoadTextureFromFile(string key, string path) {
    textureMap[key] = new Texture(key, path);
}

ModelLoader::~ModelLoader() {
    for (auto& entry: modelDataMap) {
        glDeleteBuffers(1, &entry.second->vertexBuffer);
        
        if (entry.second->hasNormals) {
            glDeleteBuffers(1, &entry.second->normalBuffer);
        }
        
        if (entry.second->hasUVs) {
            glDeleteBuffers(1, &entry.second->uvBuffer);
        }
                
        delete entry.second;
    }
    
    for (auto& entry: textureMap) {
        delete entry.second;
    }
}

void ModelLoader::LoadHardcodedModels() {
    ModelData* cube = new ModelData();
    cube->key = "cube";
    
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    
    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };
    
    glGenBuffers(1, &cube->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cube->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    cube->vertexCount = 12 * 3;
    
    modelDataMap["cube"] = cube;
    
    ModelData* triangle = new ModelData();
    triangle->key = "triangle";
    
    static const GLfloat vertexBufferDataTriangle[] = {
        -0.7f, 0.0f, -0.5f,
        0.7f, 0.0f, -0.5f,
        0.0f, 0.0f, 0.5f,
    };
    
    glGenBuffers(1, &triangle->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangle->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferDataTriangle), vertexBufferDataTriangle, GL_STATIC_DRAW);
    triangle->vertexCount = 3;
        
    modelDataMap["triangle"] = triangle;
}
