//
//  DrawableObject.hpp
//  
//
//  Created by Lukas Zierahn on 23.06.19.
//

#ifndef DrawableObject_hpp
#define DrawableObject_hpp

class ModelData;
class Render;
class Texture;

#include "Include.h"
#include "ShaderConfigs.h"

class DrawableObject {
protected:
    Render* render;
    
    ModelData* modelData = nullptr;
    
    vec3 position = vec3(0, 0, 0);
    vec3 rotation = vec3(0, 0, 0);
    vec3 scale = vec3(1, 1, 1);
    mat4 MVPmatrix;
    GLuint MVPID;
    
public:
    DrawableObject(Render* render);
    
    virtual void Draw() { cout << "Drawable Object Draw function called!" << endl; }
    virtual shaderConfig getShaderConfig() { cout << "Drawable Object getShaderConfig function called!" << endl; return shaderConfig::coloredObject; }
    
    Render* getRender() { return render; }
    
    void setPosition(vec3 newPosition) { position = newPosition; }
    void addPosition(vec3 position) { this->position += position; }
    void setScale(vec3 newScale) { scale = newScale; }
    void setScale(float newScale) { scale = vec3(newScale, newScale, newScale); }
    
    void setRotation(vec3 newRotation) { rotation = newRotation; }
    
    vec3 getPosition() { return position; }

    void ComputeMVPMatrix();
};

#endif /* DrawableObject_hpp */
