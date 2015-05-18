/*
 *  GpuParticles.cpp
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
#include "GpuParticles.h"

namespace itg
{
    const string GpuParticles::UNIFORM_PREFIX = "particles";
    const string GpuParticles::UPDATE_SHADER_NAME = "update";
    const string GpuParticles::DRAW_SHADER_NAME = "draw";
    
    GpuParticles::GpuParticles() : currentReadFbo(0), textureLocation(0)
    {
    }
    
    void GpuParticles::init(unsigned width, unsigned height, ofPrimitiveMode primitive, bool loadShaders, unsigned numDataTextures)
    {
        this->width = width;
        this->height = height;
        numFloats = width * height * FLOATS_PER_TEXEL;
        
        // fbos
        ofFbo::Settings s;
        s.internalformat = GL_RGBA32F_ARB;
        s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        s.minFilter = GL_NEAREST;
        s.maxFilter = GL_NEAREST;
        s.wrapModeHorizontal = GL_CLAMP;
        s.wrapModeVertical = GL_CLAMP;
        s.width = width;
        s.height = height;
        s.numColorbuffers = numDataTextures;
        for (unsigned i = 0; i < 2; ++i)
        {
            fbos[i].allocate(s);
        }
        
        // mesh
        mesh.clear();
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                mesh.addVertex(ofVec3f(200.f * x / (float)width - 100.f, 200.f * y / (float)height - 100.f, -500.f));
                mesh.addTexCoord(ofVec2f(x, y));
            }
        }
        mesh.setMode(primitive);
        
        // shaders
        if (loadShaders)
        {
            updateShader.load(UPDATE_SHADER_NAME);
            drawShader.load(DRAW_SHADER_NAME);
        }
    }
    
    void GpuParticles::update()
    {
        fbos[1 - currentReadFbo].begin(false);
        glPushAttrib(GL_ENABLE_BIT);
        // we set up no camera model and ignore the modelview and projection matrices
        // in the vertex shader, we make a viewport large enought to ensure the shader
        // is executed for each pixel
        glViewport(0, 0, width, height);
        glDisable(GL_BLEND);
        ofSetColor(255, 255, 255);
        fbos[1 - currentReadFbo].activateAllDrawBuffers();
        
        updateShader.begin();
        ofNotifyEvent(updateEvent, updateShader, this);
        setUniforms(updateShader);
        texturedQuad(-1, -1, 2, 2, width, height);
        updateShader.end();
        glPopAttrib();
        
        fbos[1 - currentReadFbo].end();
        
        currentReadFbo = 1 - currentReadFbo;
    }
    
    void GpuParticles::draw()
    {
        drawShader.begin();
        ofNotifyEvent(drawEvent, drawShader, this);
        setUniforms(drawShader);
        mesh.draw();
        drawShader.end();
    }
    
    void GpuParticles::setUniforms(ofShader& shader)
    {
        for (unsigned i = 0; i < fbos[currentReadFbo].getNumTextures(); ++i)
        {
            ostringstream oss;
            oss << UNIFORM_PREFIX << ofToString(i);
            shader.setUniformTexture(oss.str().c_str(), fbos[currentReadFbo].getTextureReference(i), i + textureLocation);
        }
    }
    
    void GpuParticles::loadDataTexture(unsigned idx, float* data,
                                       unsigned x, unsigned y, unsigned width, unsigned height)
    {
        if (idx < fbos[currentReadFbo].getNumTextures())
        {
            if (!width) width = this->width;
            if (!height) height = this->height;
            fbos[currentReadFbo].getTextureReference(idx).bind();
            glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, x, y, width, height, GL_RGBA, GL_FLOAT, data);
            fbos[currentReadFbo].getTextureReference(idx).unbind();
        }
        else ofLogError() << "Trying to load data from array into non-existent buffer.";
    }
    
    void GpuParticles::zeroDataTexture(unsigned idx,
                                       unsigned x, unsigned y, unsigned width, unsigned height)
    {
        if (!width) width = this->width;
        if (!height) height = this->height;
        float* zeroes = new float[width * height * FLOATS_PER_TEXEL];
        memset(zeroes, 0, sizeof(float) * width * height * FLOATS_PER_TEXEL);
        loadDataTexture(idx, zeroes, x, y, width, height);
        delete[] zeroes;
    }
    
    void GpuParticles::texturedQuad(float x, float y, float width, float height, float s, float t)
    {
        // TODO: change to triangle fan/strip
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, 0);
        
        glTexCoord2f(s, 0);
        glVertex3f(x + width, y, 0);
        
        glTexCoord2f(s, t);
        glVertex3f(x + width, y + height, 0);
        
        glTexCoord2f(0, t);
        glVertex3f(x, y + height, 0);
        glEnd();
    }
    
    void GpuParticles::save(const string& fileName)
    {
        ofstream fileStream(ofToDataPath(fileName, true).c_str());
        if (fileStream.is_open())
        {
            for (unsigned i = 0; i < fbos[currentReadFbo].getNumTextures(); ++i)
            {
                if (i) fileStream << "|";
                ofFloatPixels pixels;
                fbos[currentReadFbo].getTextureReference(i).readToPixels(pixels);
                for (unsigned j = 0; j < pixels.size(); ++j)
                {
                    if (j) fileStream << ",";
                    fileStream << pixels[j];
                }
            }
            fileStream.close();
        }
        else ofLogError() << "Could not save particle data to " << ofToDataPath(fileName, true);
    }
    
    void GpuParticles::load(const string& fileName)
    {
        ifstream fileStream(ofToDataPath(fileName, true).c_str());
        if (fileStream.is_open())
        {
            string data((istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
            vector<string> textureData = ofSplitString(data, "|");
            for (unsigned i = 0; i < textureData.size(); ++i)
            {
                if (i < fbos[currentReadFbo].getNumTextures())
                {
                    vector<string> floatsAsText = ofSplitString(textureData[i], ",");
                    vector<float> floats(floatsAsText.size(), 0);
                    for (unsigned j = 0; j < floats.size(); ++j)
                    {
                        floats[j] = atof(floatsAsText[j].c_str());
                    }
                    loadDataTexture(i, &floats[0]);
                }
                else ofLogError() << "Trying to load data from file into non-existent buffer.";
            }
            fileStream.close();
        }
        else ofLogError() << "Could not load particle data from " << ofToDataPath(fileName, true);
    }
}