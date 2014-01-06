/*
 *  GpuParticles.h
 *
 *  Copyright (c) 2013, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
#pragma once

#include "ofMain.h"

namespace itg
{
    /**
     * For now, only uses RGBA and TEXTURE_RECTANGLE to make usage simpler
     */
    class GpuParticles
    {
    public:
        static const string UNIFORM_PREFIX;
        static const string UPDATE_SHADER_NAME;
        static const string DRAW_SHADER_NAME;
        static const unsigned FLOATS_PER_TEXEL = 4;
        
        // you don't have to use these but makes
        // code more readable
        enum DataTextureIndex
        {
            POSITION,
            VELOCITY
        };
        
        GpuParticles();
        
        void init(unsigned width, unsigned height,
                  ofPrimitiveMode primitive = OF_PRIMITIVE_POINTS, bool loadShaders = true, unsigned numDataTextures = 2);
        void update();
        void draw();
        
        void loadDataTexture(unsigned idx, float* data,
                             unsigned x = 0, unsigned y = 0, unsigned width = 0, unsigned height = 0);
        void zeroDataTexture(unsigned idx,
                             unsigned x = 0, unsigned y = 0, unsigned width = 0, unsigned height = 0);
        
        unsigned getWidth() const { return width; }
        unsigned getHeight() const { return height; }
        unsigned getNumFloats() const { return numFloats; }
        
        void setTextureLocation(unsigned textureLocation) { this->textureLocation = textureLocation; }
        
        // listen to these events to set custom uniforms
        ofEvent<ofShader> updateEvent;
        ofEvent<ofShader> drawEvent;
        
        ofVboMesh& getMeshRef() { return mesh; }
        
        // advanced
        ofShader& getUpdateShaderRef() { return updateShader; }
        ofShader& getDrawShaderRef() { return drawShader; }
        
        void save(const string& fileName);
        void load(const string& fileName);
        
    private:
        void texturedQuad(float x, float y, float width, float height, float s, float t);
        void setUniforms(ofShader& shader);
        
        ofFbo fbos[2];
        ofVboMesh mesh;
        ofShader updateShader, drawShader;
        unsigned currentReadFbo;
        unsigned textureLocation;
        unsigned width, height, numFloats;
    };
}
