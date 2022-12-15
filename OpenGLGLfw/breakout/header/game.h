/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"
#include "ball_object.h"
#include "power_up.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
//重命名定义表示碰撞数据的tupe类型
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;//是否发声碰撞，发声碰撞的方向，

// Initial size of the player paddle
// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化挡班的速率
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
// 初始化球的速率
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
// 初始化球的半径
const float BALL_RADIUS = 12.5f;


// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;//游戏状态
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    bool                    Destroyed;
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;//关卡
    std::vector<PowerUp>    PowerUps;//道具
    std::vector<BallObject*> Balls;//球
    unsigned int            Level;//当前关卡
    unsigned int            Lives;//玩家生命值
    // constructor/destructor构造函数/析构函数
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)初始化游戏状态（加载所有的着色器/纹理/关卡）
    void Init();
    // game loop游戏循环
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();//碰撞检测
    // Reset 重置游戏
    void ResetLevel();
    void ResetPlayer();
    // powerups道具
    void SpawnPowerUps(GameObject& block);//在给定的砖块位置生成一个道具
    void UpdatePowerUps(float dt);//管理所有当前被激活的道具
    void ActivatePowerUp(PowerUp& powerUp);//激活道具效果
};

#endif