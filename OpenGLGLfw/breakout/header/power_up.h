/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef POWERUP_H
#define POWERUP_H

#include <glm/glm.hpp>

#include <iostream>
#include "game_object.h"
#include "texture.h"
#include "shader_b.h"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
    // 道具类型
    std::string Type;
    GLfloat     Duration;
    GLboolean   Activated;
    // 构造函数
    PowerUp(std::string type, glm::vec3 color, GLfloat duration,glm::vec2 position, Texture2D texture)
        : GameObject(position, SIZE, texture, color, VELOCITY),Type(type), Duration(duration), Activated(){ }
};

#endif