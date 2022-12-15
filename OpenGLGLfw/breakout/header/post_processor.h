/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader_b.h"


// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or 
// Shake boolean. 
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
//后处理器为突破游戏托管所有后处理效果。
//它在一个有纹理的四边形上渲染游戏，之后可以通过启用“混乱”、“混乱”或“抖动”布尔值来启用特定效果。
//需要在渲染游戏之前调用BeginRender（），在渲染游戏之后调用EndRender（），类才能工作。
class PostProcessor
{
public:
    // state
    Shader PostProcessingShader;
    Texture2D Texture;
    unsigned int Width, Height;
    // options
    bool Confuse, Chaos, Shake;
    // constructor
    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    // prepares the postprocessor's framebuffer operations before rendering the game
    // 在渲染游戏之前准备后处理器的帧缓冲区操作
    void BeginRender();
    // should be called after rendering the game, so it stores all the rendered data into a texture object
    // 应在渲染游戏后调用，以便将所有渲染的数据存储到纹理对象中。
    void EndRender();
    // renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    // 渲染后处理器纹理的四边形纹理
    void Render(float time);
private:
    // render state
    unsigned int MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    unsigned int RBO; // RBO is used for multisampled color buffer
    unsigned int VAO;
    // initialize quad for rendering postprocessing texture
    void initRenderData();
};

#endif