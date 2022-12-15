/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_level.h"
#include <iostream>

#include <fstream>
#include <sstream>


void GameLevel::Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight)
{
    // Clear old data
    // 清空过期数据
    this->Bricks.clear();
    // Load from file
    // 从文件中加载
    GLuint tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLuint>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // Read each line from level file读取关卡文件的每一行
        {
            std::istringstream sstream(line);
            std::vector<GLuint> row;
            while (sstream >> tileCode) // Read each word seperated by spaces读取被空格分隔的每个数字
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
    //std::cout << Bricks.size();
    for (GameObject& tile : this->Bricks) {
        if (!tile.Destroyed) {
            tile.Draw(renderer);
        }
    }   
}

GLboolean GameLevel::IsCompleted()
{
    for (GameObject& tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return GL_FALSE;
    return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
    // Calculate dimensions
    // 计算每个维度的大小
    GLuint height = tileData.size();//有几行砖块
    GLuint width = tileData[0].size(); // 注意，我们可以在[0]处索引向量，因为只有当高度>0时才调用此函数。有几列砖块
    GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);// static_cast<GLfloat>(width)可以保证width被填满
    GLfloat unit_height = levelHeight / height;
    // Initialize level tiles based on tileData		
    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            // Check block type from level data (2D level array)
            if (tileData[y][x] == 1) // Solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);//砖的位置
                glm::vec2 size(unit_width, unit_height);//砖的大小
                GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));//创建object
                obj.IsSolid = GL_TRUE;//是坚硬不可销毁的砖块
                this->Bricks.push_back(obj);//加入GameObject数组（Bricks）
            }
            else if (tileData[y][x] > 1)	// Non-solid; now determine its color based on level data非实心的：现在根据关卡数据来确定它的颜色。
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
            }
        }
    }
}