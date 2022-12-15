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
//�����������ʾ��ײ���ݵ�tupe����
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;//�Ƿ�����ײ��������ײ�ķ���

// Initial size of the player paddle
// ��ʼ������Ĵ�С
const glm::vec2 PLAYER_SIZE(100, 20);
// ��ʼ�����������
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
// ��ʼ���������
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
// ��ʼ����İ뾶
const float BALL_RADIUS = 12.5f;


// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;//��Ϸ״̬
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    bool                    Destroyed;
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;//�ؿ�
    std::vector<PowerUp>    PowerUps;//����
    std::vector<BallObject*> Balls;//��
    unsigned int            Level;//��ǰ�ؿ�
    unsigned int            Lives;//�������ֵ
    // constructor/destructor���캯��/��������
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)��ʼ����Ϸ״̬���������е���ɫ��/����/�ؿ���
    void Init();
    // game loop��Ϸѭ��
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();//��ײ���
    // Reset ������Ϸ
    void ResetLevel();
    void ResetPlayer();
    // powerups����
    void SpawnPowerUps(GameObject& block);//�ڸ�����ש��λ������һ������
    void UpdatePowerUps(float dt);//�������е�ǰ������ĵ���
    void ActivatePowerUp(PowerUp& powerUp);//�������Ч��
};

#endif