//
//  ModelLoader.hpp
//  PLG
//
//  Created by Lukas Zierahn on 03.06.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//

#ifndef ModelLoader_hpp
#define ModelLoader_hpp

#include "Include.h"
#include <map>

class Render;
class Texture;
class ModelData;

class ModelLoader {
private:
    Render* render;
    
    map<string, ModelData*> modelDataMap;
    map<string, Texture*> textureMap;

    void LoadHardcodedModels();
    
public:
    ModelLoader(Render* render);
    ModelData* getModel(string key);
    Texture* getTexture(string key);
    
    void LoadModelFromFile(string key);
    void LoadModelFromFile(string key, string path);
    
    void ConvertFileToCng(string key);
    void ConvertFileToCng(string key, string srcPath, string targetPath);
    
    void LoadTextureFromFile(string key);
    void LoadTextureFromFile(string key, string path);
    
    ~ModelLoader();
};

#endif /* ModelLoader_hpp */
