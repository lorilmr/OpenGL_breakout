/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "text_renderer.h"

#include <algorithm>
#include <sstream>
#include <iostream>

#include <irrklang/irrKlang.h>
using namespace irrklang;

//游戏相关的状态数据
SpriteRenderer* Renderer;//渲染精灵
GameObject* Player;//挡板
BallObject* BallFirst;//球
ParticleGenerator* Particles;//粒子
PostProcessor* Effects;//后处理
ISoundEngine* SoundEngine = createIrrKlangDevice();//音频
TextRenderer* Text;//文本渲染
int probability = 3;//生成球的概率

float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(),KeysProcessed(), Destroyed(false), Width(width), Height(height),Level(0), Lives(1)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete BallFirst;
    delete Particles;
    delete Effects;
    delete Text;
    SoundEngine->drop();
}

void Game::Init()
{
    // 加载着色器
    ResourceManager::LoadShader("breakout/header/shaders/sprite.vs", "breakout/header/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("breakout/header/shaders/particle.vs", "breakout/header/shaders/particle.fs", nullptr, "particle");
    ResourceManager::LoadShader("breakout/header/shaders/post_processing.vs", "breakout/header/shaders/post_processing.fs", nullptr, "postprocessing");
    // 配置着色器
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // 设置专用于渲染的控制
    Shader spriteShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(spriteShader);
    // 加载纹理
    ResourceManager::LoadTexture("breakout/header/textures/awesomeface.png", GL_TRUE, "face");//球纹理
    ResourceManager::LoadTexture("breakout/header/textures/background.jpg", GL_FALSE, "background");//背景纹理
    ResourceManager::LoadTexture("breakout/header/textures/block.png", GL_FALSE, "block");//普通砖块
    ResourceManager::LoadTexture("breakout/header/textures/block_solid.png", GL_FALSE, "block_solid");//坚硬砖块
    ResourceManager::LoadTexture("breakout/header/textures/paddle.png", true, "paddle");//挡板纹理
    ResourceManager::LoadTexture("breakout/header/textures/particle.png", GL_TRUE, "particle");//粒子纹理
    ResourceManager::LoadTexture("breakout/header/textures/powerup_speed.png", true, "powerup_speed");
    ResourceManager::LoadTexture("breakout/header/textures/powerup_sticky.png", true, "powerup_sticky");
    ResourceManager::LoadTexture("breakout/header/textures/powerup_increase.png", true, "powerup_increase");
    ResourceManager::LoadTexture("breakout/header/textures/powerup_confuse.png", true, "powerup_confuse");
    ResourceManager::LoadTexture("breakout/header/textures/powerup_chaos.png", true, "powerup_chaos");
    ResourceManager::LoadTexture("breakout/header/textures/powerup_passthrough.png", true, "powerup_passthrough");
    // 加载关卡
    GameLevel one; one.Load("breakout/header/levels/0.lvl", this->Width, this->Height * 0.5);
    GameLevel two; two.Load("breakout/header/levels/1.lvl", this->Width, this->Height * 0.5);
    GameLevel three; three.Load("breakout/header/levels/2.lvl", this->Width, this->Height * 0.5);
    GameLevel four; four.Load("breakout/header/levels/3.lvl", this->Width, this->Height * 0.5);
    GameLevel five; five.Load("breakout/header/levels/4.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Levels.push_back(five);
    this->Level = 0;//当前关卡
    // 加载音频
    SoundEngine->play2D("breakout/header/audio/breakout.mp3", true);//循环播放背景音乐
    //初始化挡板的位置大小速率
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2,this->Height - PLAYER_SIZE.y);//坐标原点是左上角
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    //初始化球的位置半径初速度
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,-BALL_RADIUS * 2.0f);
    BallFirst = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture("face"));
    this->Balls.push_back(BallFirst);
    //初始化粒子
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"),ResourceManager::GetTexture("particle"),500);
    //初始化后期处理
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"),this->Width,this->Height);
    //初始化文本渲染器
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("breakout/header/fonts/OCRAExtended.ttf", 24);
}

void Game::Update(float dt)
{
    for (BallObject* Ball : this->Balls) {
        //更新球位置
        Ball->Move(dt, this->Width);
        // 更新粒子的速度和颜色，以及找到消失的第一个粒子并重新生成
        Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
        //if (Ball->Position.y >= this->Height)
        //    delete Ball;
    }
    this->Balls.erase(std::remove_if(this->Balls.begin(), this->Balls.end(),
    [](BallObject* a) { return a->Position.y >= 600.0f; }
    ), this->Balls.end());
    //std::cout << "UP:" << this->Balls.size() << " ";
    // 检测碰撞
    this->DoCollisions();
    // update PowerUps
    this->UpdatePowerUps(dt);
    // reduce shake time
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }
    if (this->Balls.size() <= 0) {
        this->ResetLevel();
        this->State = GAME_MENU;
        this->ResetPlayer();
    }
    if (this->Level == 3) {
        probability = 2;
    }
    //// 球是否接触底部边界? 游戏结束：游戏继续
    //if (Ball->Position.y >= this->Height) 
    //{
    //    --this->Lives;//失去一条生命值
    //    // 玩家是否已失去所有生命值? 游戏结束：游戏继续
    //    if (this->Lives == 0)
    //    {
    //        this->ResetLevel();
    //        this->State = GAME_MENU;
    //    }
    //    this->ResetPlayer();
    //}
    //游戏胜利
    if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    {
        this->ResetLevel();
        this->ResetPlayer();
        Effects->Chaos = GL_TRUE;
        this->State = GAME_WIN;
    }
}

void Game::ProcessInput(float dt)
{
    //菜单选择
    if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
        }
        if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
        {
            this->Level = (this->Level + 1) % this->Levels.size();
            this->KeysProcessed[GLFW_KEY_W] = true;
        }
        if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
        {
            if (this->Level > 0)
                --this->Level;
            else
                this->Level = this->Levels.size()-1;
            //this->Level = (this->Level - 1) % 4;
            this->KeysProcessed[GLFW_KEY_S] = true;
        }
    }
    //游戏中
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            //std::cout << GLFW_KEY_A;等于65
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
            //球固定的时候跟着板子动
            for (BallObject* Ball : this->Balls) {
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
            for (BallObject* Ball : this->Balls) {
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            for (BallObject* Ball : this->Balls) {
                if (Ball->Stuck)
                    Ball->Stuck = false;
            }
    }
    //游戏结束（胜利）
    if (this->State == GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])//按下Enter键进去菜单
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
            Effects->Chaos = GL_FALSE;
            this->State = GAME_MENU;
        }
    }
}

void Game::Render()
{
    //Texture2D faceTex = ResourceManager::GetTexture("face");
    //Renderer->DrawSprite(faceTex, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));//纹理，位置，大小，旋转，颜色
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU|| this->State == GAME_WIN)
    {
        // begin rendering to postprocessing framebuffer
        Effects->BeginRender();//MSFBO
            // 绘制背景
            Texture2D backgroundTex = ResourceManager::GetTexture("background");
            Renderer->DrawSprite(backgroundTex,glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);//渲染背景
            // 绘制关卡
            this->Levels[this->Level].Draw(*Renderer);//渲染砖块
            // 绘制挡板
            Player->Draw(*Renderer);
            // 绘制道具
            for (PowerUp& powerUp : this->PowerUps)
                if (!powerUp.Destroyed)
                    powerUp.Draw(*Renderer);
            // 绘制粒子  
            Particles->Draw();
            // 绘制球
            for (auto Ball : this->Balls) {
                Ball->Draw(*Renderer);
            }
        // end rendering to postprocessing framebuffer
        Effects->EndRender();//MSFBO==>FBO
        // render postprocessing quad
        Effects->Render(glfwGetTime());//把FBO的纹理附件渲染到默认的帧缓冲区

        //渲染生命值文本
        std::stringstream ss,s1; ss << this->Lives;
        //Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
        s1<< this->Level+1;
        Text->RenderText("Level:" + s1.str(), 5.0f, 5.0f, 1.0f);
    }
    //渲染菜单栏
    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
        Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
    }
    //渲染游戏胜利
    if (this->State == GAME_WIN)
    {
        Text->RenderText(
            "You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
        );
        Text->RenderText(
            "Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
    }
}

void Game::ResetLevel()
{
    std::string levelName = "breakout/header/levels/";
    levelName += +(char)(this->Level + '0');
    levelName += +".lvl";
    //std:: cout << levelName;
    this->Levels[this->Level].Load(levelName.c_str(), this->Width, this->Height * 0.5f);
    //if (this->Level == 0)
    //    this->Levels[0].Load("breakout/header/levels/one.lvl", this->Width, this->Height * 0.5f);
    //else if (this->Level == 1)
    //    this->Levels[1].Load("breakout/header/levels/two.lvl", this->Width, this->Height * 0.5f);
    //else if (this->Level == 2)
    //    this->Levels[2].Load("breakout/header/levels/three.lvl", this->Width, this->Height * 0.5f);
    //else if (this->Level == 3)
    //    this->Levels[3].Load("breakout/header/levels/four.lvl", this->Width, this->Height * 0.5f);
    //else if (this->Level == 4)
    //    this->Levels[4].Load("breakout/header/levels/five.lvl", this->Width, this->Height * 0.5f);
    //this->Lives = 3;//重置生命值
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    //Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
    //初始化球的位置半径初速度
    this->Balls.clear();
    glm::vec2 ballPos = Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    BallFirst = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
    this->Balls.push_back(BallFirst);
    // also disable all active powerups
    Effects->Chaos = Effects->Confuse = false;
    //Ball->PassThrough = Ball->Sticky = false;
    PowerUps.clear();//清空所有道具
    Player->Color = glm::vec3(1.0f);
    //Ball->Color = glm::vec3(1.0f);
}



// powerups
bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type);//检查是否有同类道具处于激活状态

//根据道具的速度移动它，并减少已激活道具的持续时间，每当时间减少至小于0时，我们令其失效，并恢复相关变量的状态
void Game::UpdatePowerUps(float dt)
{
    for (PowerUp& powerUp : this->PowerUps)
    {
        powerUp.Position += powerUp.Velocity * dt;//道具速度固定
        if (powerUp.Activated)//如果道具被激活
        {
            powerUp.Duration -= dt;

            if (powerUp.Duration <= 0.0f)
            {
                // remove powerup from list (will later be removed)
                powerUp.Activated = false;
                // deactivate effects
                if (powerUp.Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))//如果没有其他同类道具被激活
                    {	// only reset if no other PowerUp of type sticky is active
                        for (BallObject* Ball : this->Balls) {
                            Ball->Sticky = false;
                        }
                        Player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "pass-through")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {	// only reset if no other PowerUp of type pass-through is active
                        for (BallObject* Ball : this->Balls) {
                            Ball->PassThrough = false;
                            Ball->Color = glm::vec3(1.0f);
                        }
                    }
                }
                else if (powerUp.Type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {	// only reset if no other PowerUp of type confuse is active
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp.Type == "chaos")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
                    {	// only reset if no other PowerUp of type chaos is active
                        Effects->Chaos = false;
                    }
                }
            }
        }
    }
    // Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
    // Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());
}


bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}
void Game::SpawnPowerUps(GameObject& block)
{
    if (ShouldSpawn(75)) // 1 in 75 chance
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
    if (ShouldSpawn(15)) // Negative powerups should spawn more often
        this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
    if (ShouldSpawn(15))
        this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

//激活道具后的实现效果
void Game::ActivatePowerUp(PowerUp& powerUp)
{
    if (powerUp.Type == "speed")
    {
        for (BallObject* Ball : this->Balls) {
            Ball->Velocity *= 1.2;//加速       
        }
    }
    else if (powerUp.Type == "sticky")
    {
        for (BallObject* Ball : this->Balls) {
            Ball->Sticky = true;//球黏在挡板上
        }
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);//挡板变色
    }
    else if (powerUp.Type == "pass-through")
    {
        for (BallObject* Ball : this->Balls) {
            Ball->PassThrough = true;//球可以穿过非实心砖块
            Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);//球变色       
        }
    }
    else if (powerUp.Type == "pad-size-increase")
    {
        Player->Size.x += 50;//挡板长度增加
    }
    else if (powerUp.Type == "confuse")
    {
        if (!Effects->Chaos)
            Effects->Confuse = true; // only activate if chaos wasn't already active
    }
    else if (powerUp.Type == "chaos")
    {
        if (!Effects->Confuse)
            Effects->Chaos = true;
    }
}

bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
    // Check if another PowerUp of the same type is still active
    // in which case we don't disable its effect (yet)
    for (const PowerUp& powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return true;
    }
    return false;
}


// Collision detection
// 碰撞检测
GLboolean CheckCollision(GameObject& one, GameObject& two);
Collision CheckCollision(BallObject& one, GameObject& two);
Direction VectorDirection(glm::vec2 closest);

//计算碰撞方向
Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // 上
        glm::vec2(1.0f, 0.0f),  // 右
        glm::vec2(0.0f, -1.0f), // 下
        glm::vec2(-1.0f, 0.0f)  // 左
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

GLboolean CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // x轴方向碰撞？
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // y轴方向碰撞？
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // 只有两个轴向都有碰撞时才碰撞
    return collisionX && collisionY;
}

Collision CheckCollision(BallObject& one, GameObject& two)
{   
    //AABB - 圆碰撞检测
    // 获取圆的中心 
    glm::vec2 center(one.Position + one.Radius);//点C
    // 计算AABB的信息（中心、半边长）
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x,
        two.Position.y + aabb_half_extents.y
    );//点B
    // 获取两个中心的差矢量
    glm::vec2 difference = center - aabb_center;//向量D
    //接下来用AABB的半边长(half-extents)w和h¯来限制(clamp)矢量D¯。
    //长方形的半边长是指长方形的中心到它的边的距离；简单的说就是它的尺寸除以2。
    //这一过程返回的是一个总是位于AABB的边上的位置矢量（除非圆心在AABB内部）。
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
    glm::vec2 closest = aabb_center + clamped;//最近的点P
    // 获得圆心center和最近点closest的矢量并判断是否 length <= radius
    difference = closest - center;//获得新的向量V=P-C
    if (glm::length(difference) <= one.Radius)
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}
void Game::DoCollisions()
{
    //std::cout << "Do:" << this->Balls.size() << " ";
    for (int i = 0; i < this->Balls.size();++i) {
        BallObject* Ball = this->Balls[i];
        if (Ball->Position.x < 0 || Ball->Position.x > this->Width || Ball->Position.y < 0 || Ball->Position.y > this->Height)
            continue;
        //球和砖块的碰撞
        for (GameObject& box : this->Levels[this->Level].Bricks)
        {
            if (!box.Destroyed)
            {
                Collision collision = CheckCollision(*Ball, box);
                if (std::get<0>(collision))
                {
                    // 如果砖块不是实心就销毁砖块
                    if (!box.IsSolid) {
                        box.Destroyed = GL_TRUE;
                        this->SpawnPowerUps(box);//以一定几率生成道具
                        SoundEngine->play2D("breakout/header/audio/bleep.mp3", false);//播放小球撞击非实心砖块时的音效 
                        if (ShouldSpawn(probability)) {
                            //初始化球的位置半径初速度
                            glm::vec2 ballPos = box.Position+ glm::vec2(box.Size/2.0f);
                            if (ballPos.x > 0 && ballPos.x < 800.0f && ballPos.y>0 && ballPos.y < 600.0f) {
                                BallObject* newBall = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
                                newBall->Stuck = false;
                                this->Balls.push_back(newBall);
                            }
                        }

                    }
                    else
                    {   // if block is solid, enable shake effect
                        ShakeTime = 0.05f;
                        Effects->Shake = true;
                        SoundEngine->play2D("breakout/header/audio/solid.wav", false);//播放小球撞击实心砖块时的音效
                    }
                    // 碰撞处理
                    Direction dir = std::get<1>(collision);
                    glm::vec2 diff_vector = std::get<2>(collision);
                    if (!(Ball->PassThrough && !box.IsSolid)) // don't do collision resolution on non-solid bricks if pass-through is activated
                    {
                        if (dir == LEFT || dir == RIGHT) // 水平方向碰撞
                        {
                            Ball->Velocity.x = -Ball->Velocity.x; // 反转水平速度
                            // 重定位
                            GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);//为得到R¯我们用球的半径减去V¯
                            if (dir == LEFT)
                                Ball->Position.x += penetration; // 将球右移
                            else
                                Ball->Position.x -= penetration; // 将球左移
                        }
                        else // 垂直方向碰撞
                        {
                            Ball->Velocity.y = -Ball->Velocity.y; // 反转垂直速度
                            // 重定位
                            GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                            if (dir == UP)
                                Ball->Position.y -= penetration; // 将球上移
                            else
                                Ball->Position.y += penetration; // 将球下移
                        }
                    }
                }
            }
        }
        //球和挡板的碰撞
        Collision result = CheckCollision(*Ball, *Player);
        if (!Ball->Stuck && std::get<0>(result))
        {
            // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
            GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
            GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
            GLfloat percentage = distance / (Player->Size.x / 2);
            // 依据结果移动
            GLfloat strength = 2.0f;
            glm::vec2 oldVelocity = Ball->Velocity;
            Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
            //Ball->Velocity.y = -Ball->Velocity.y;
            Ball->Velocity.y = -1 * abs(Ball->Velocity.y);//解决粘板问题
            Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);//速度大小和原来保持一致

            // if Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
            Ball->Stuck = Ball->Sticky;//此时球和挡板接触
            SoundEngine->play2D("breakout/header/audio/bleep.wav", false);//小球在挡板上反弹时的音效
        }
    }

    //道具碰撞检测
    for (PowerUp& powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            if (powerUp.Position.y >= this->Height) {
                //与屏幕底面接触就销毁
                 powerUp.Destroyed = GL_TRUE;           
            }
            if (CheckCollision(*Player, powerUp))
            {   // 道具与挡板接触，激活它！
                ActivatePowerUp(powerUp);//开启效果
                powerUp.Destroyed = GL_TRUE;//道具被吃掉
                powerUp.Activated = GL_TRUE;//效果被激活
                SoundEngine->play2D("breakout/header/audio/powerup.wav", false);//播放获得道具时的音效
            }
        }
    }

}


