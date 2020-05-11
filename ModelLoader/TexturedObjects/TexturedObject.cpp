//
//  DrawableObject.cpp
//  PLG
//
//  Created by Lukas Zierahn on 02.06.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//

#include "Render.hpp"
#include "ModelLoader.hpp"
#include "ModelData.hpp"
#include "Texture.hpp"
#include "ShaderConfigs.h"

#include "TexturedObject.hpp"

TexturedObject::TexturedObject(Render* render) : DrawableObject(render) {
    render->addDrawableObject(this);
}

void TexturedObject::setModelDataByKey(string key) {
    modelData = render->getModelLoader()->getModel(key);
    if (!modelData->hasUVs) {
        throw new runtime_error("Tried to assign ModelData to TexturedObject without UV coordinates: " + key);
    }
}

void TexturedObject::setTextureByKey(string key) {
    texture = render->getModelLoader()->getTexture(key);
}

void TexturedObject::Draw() {
    if (modelData == nullptr || texture == nullptr) {
        return;
    }
    
    ComputeMVPMatrix();
    
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVPmatrix[0][0]);
    
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
    
    // 1rst attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, modelData->vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, modelData->uvBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, modelData->normalBuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData->indexBuffer);
    
    // Draw the triangles !
    glDrawElements(GL_TRIANGLES, modelData->indexCount, GL_UNSIGNED_SHORT, nullptr);
}

TexturedObject::~TexturedObject() {
    render->removeDrawableObject(this);
}
