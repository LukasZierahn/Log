//
//  DrawableObject.hpp
//  PLG
//
//  Created by Lukas Zierahn on 02.06.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//

#ifndef TexturedObject_hpp
#define TexturedObject_hpp

class ModelData;
class Render;
class Texture;

#include "Include.h"
#include "DrawableObject.hpp"

class TexturedObject : public DrawableObject {
private:    
    Texture* texture = nullptr;
    
public:
    TexturedObject(Render* render);
    
    void setModelDataByKey(string key);
    void setTextureByKey(string key);
    Texture* getTexture() { return texture; };

    void Draw();
};

#endif /* TexturedObject_hpp */
