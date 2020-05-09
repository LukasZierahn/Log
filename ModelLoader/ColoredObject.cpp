//
//  ColoredObject.cpp
//  PLG
//
//  Created by Lukas Zierahn on 23.06.19.
//  Copyright © 2019 Lukas Zierahn. All rights reserved.
//
#include "ModelLoader.hpp"
#include "ModelData.hpp"
#include "Render.hpp"
#include "ColorData.hpp"

#include "ColoredObject.hpp"

ColoredObject::ColoredObject(Render* render): render(render), DrawableObject(render) {
    render->addColoredObject(this);
    
    colorData = new ColorData(0);
}

void ColoredObject::setModelDataByKey(string key) {
    modelData = render->getModelLoader()->getModel(key);
    colorData->count = modelData->vertexCount;
}

void ColoredObject::Draw() {
    if (!colorData->initialized) {
        throw new runtime_error("Tried to draw a ColordObject without initializing ColorData");
    }
    
    ComputeMVPMatrix();
    
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVPmatrix[0][0]);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, modelData->vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorData->colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glDrawArrays(GL_TRIANGLES, 0, modelData->vertexCount);

}

ColoredObject::~ColoredObject() {
    render->removeColordObject(this);
}
