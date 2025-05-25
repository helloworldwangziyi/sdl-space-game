//
// Created by Administrator on 25-5-11.
//
// SceneMain.cpp
#include "SceneMain.h"
#include "Game.h"
#include <SDL_image.h>
#include <iostream>

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Update(float deltaTime)
{
    keyboardControl(deltaTime);
    updateBullet(deltaTime);
    spawnEnemy();           // 生成敌机
    updateEnemy(deltaTime); // 更新敌机
    updateEnemyBullet(deltaTime); // 更新敌机子弹
    updatePlayer(deltaTime); // 更新玩家
}

void SceneMain::Render()
{
    // 渲染子弹
    renderBullet();

    // 渲染敌机
    renderEnemy();

    // 渲染敌机子弹
    renderEnemyBullet();

    // 渲染玩家
    if(!isDead)
    {
        SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
        SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
    }
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::Init()
{
    // 初始化
    std::random_device rd;                                   // 获取真随机数作为种子
    gen = std::mt19937(rd());                                // 用真随机数初始化随机数生成器
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f); // 均匀分布

    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/SpaceShip.png");
    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", SDL_GetError());
    }
    //
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化子弹
    bullet.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/laser-1.png");
    SDL_QueryTexture(bullet.texture, NULL, NULL, &bullet.width, &bullet.height);
    bullet.width /= 4;
    bullet.height /= 4;

    // 初始化敌机
    enemy.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/insect-2.png");
    SDL_QueryTexture(enemy.texture, NULL, NULL, &enemy.width, &enemy.height);
    enemy.width /= 4;
    enemy.height /= 4;

    // 初始化敌机子弹
    enemyBullet.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/bullet-1.png");
    SDL_QueryTexture(enemyBullet.texture, NULL, NULL, &enemyBullet.width, &enemyBullet.height);
    enemyBullet.width /= 4;
    enemyBullet.height /= 4;
}

void SceneMain::clean()
{
    // 清理子弹
    for (auto &it : bullets)
    {
        if (it != nullptr)
        {
            delete it;
        }
    }
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
    if (bullet.texture != nullptr)
    {
        SDL_DestroyTexture(bullet.texture);
        bullet.texture = nullptr;
    }

    // 清理敌机
    for (auto &enemy : enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy;
        }
    }
    enemies.clear();
    // 清理敌机纹理
    if (enemy.texture != nullptr)
    {
        SDL_DestroyTexture(enemy.texture);
    }

    // 清理敌机子弹
    for(auto &it : enemyBullets)
    {
        if(it != nullptr)
        {
            delete it;
        }
    }
    enemyBullets.clear();

    // 清理敌机子弹纹理
    if (enemyBullet.texture != nullptr)
    {
        SDL_DestroyTexture(enemyBullet.texture);
        enemyBullet.texture = nullptr;
    }
}

void SceneMain::keyboardControl(float deltaTime)
{
    auto keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_W])
    {
        std::cout << "W" << std::endl;
        player.position.y -= player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        std::cout << "S" << std::endl;
        player.position.y += player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        std::cout << "A" << std::endl;
        player.position.x -= player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        std::cout << "D" << std::endl;
        player.position.x += player.speed * deltaTime;
    }

    // 限制玩家在窗口内移动
    if (player.position.x < 0)
    {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width)
    {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0)
    {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height)
    {
        player.position.y = game.getWindowHeight() - player.height;
    }

    // 用空格表示发射子弹
    if (keyboardState[SDL_SCANCODE_J])
    {
        std::cout << "space" << std::endl;
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastFireTime >= player.coolDown)
        {
            shootBullet();
            player.lastFireTime = currentTime;
        }
    }
}

void SceneMain::shootBullet()
{
    Bullet *newBullet = new Bullet(bullet);
    newBullet->position.x = player.position.x + player.width / 2 - newBullet->width / 2;
    newBullet->position.y = player.position.y;
    bullets.push_back(newBullet);
}

void SceneMain::updateBullet(float deltaTime)
{
    int margin = 32;
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        auto cur_bullet = *it;

        // 更新子弹的位置
        cur_bullet->position.y -= cur_bullet->speed * deltaTime;

        if (cur_bullet->position.y + margin < 0)
        {
            it = bullets.erase(it);
            delete cur_bullet;
        }
        else
        {
            bool hit = false;
            for(auto enemy : enemies)
            {
                SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
                
                SDL_Rect bulletRect = {static_cast<int>(cur_bullet->position.x), static_cast<int>(cur_bullet->position.y), cur_bullet->width, cur_bullet->height};

                if(SDL_HasIntersection(&bulletRect, &enemyRect))
                {
                    enemy->currentHealth -= cur_bullet->damage;
                    delete cur_bullet;
                    it = bullets.erase(it);
                    hit = true;
                    break;
                }
            }
            if(!hit)
            {
                ++it;
            }
        }
    }
}

void SceneMain::renderBullet()
{
    for (auto it : bullets)
    {
        SDL_Rect bulletRect = {static_cast<int>(it->position.x), static_cast<int>(it->position.y), it->width, it->height};
        SDL_RenderCopy(game.getRenderer(), it->texture, nullptr, &bulletRect);
    }
}

void SceneMain::spawnEnemy()
{
    // 生成敌机
    double probability = 1 / 60.0;
    double random = dis(gen);
    if (random > probability)
    {
        std::cout << "no spawn" << std::endl;
        return;
    }

    // 创建敌机
    Enemy *newEnemy = new Enemy(enemy);
    newEnemy->position.x = dis(gen) * (game.getWindowWidth() - newEnemy->width);
    newEnemy->position.y = -newEnemy->height;
    enemies.push_back(newEnemy);
}

void SceneMain::updateEnemy(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        auto cur_enemy = *it;

        // 更新敌机的位置
        cur_enemy->position.y += cur_enemy->speed * deltaTime;

        // 检出敌机位置是否飞出屏幕底部
        if (cur_enemy->position.y > game.getWindowHeight())
        {
            delete cur_enemy;
            it = enemies.erase(it);
        }
        else
        {
            // 检查冷却时间，决定是否发射子弹
            if(currentTime - cur_enemy->lastFireTime > cur_enemy->coolDown)
            {
                EnemyShootBullet(cur_enemy);
                cur_enemy->lastFireTime = currentTime;
            }
            if(cur_enemy->currentHealth <= 0)
            {
                enemyExplode(cur_enemy);
                it = enemies.erase(it);
            }
            else 
            {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemy()
{
    for (auto it : enemies)
    {
        SDL_Rect enemyRect = {static_cast<int>(it->position.x), static_cast<int>(it->position.y), it->width, it->height};
        SDL_RenderCopy(game.getRenderer(), it->texture, nullptr, &enemyRect);
    }
}

void SceneMain::EnemyShootBullet(Enemy* enemy)
{
    // 创建敌机子弹
    EnemyBullet* newEnemyBullet = new EnemyBullet(enemyBullet);
    
    // 设置子弹初始位置为敌机中心
    newEnemyBullet->position.x = enemy->position.x + enemy->width / 2 - newEnemyBullet->width / 2;
    newEnemyBullet->position.y = enemy->position.y + enemy->height / 2 - newEnemyBullet->height / 2;

    // 计算子弹方向
    newEnemyBullet->direction = getDirection(enemy);

    // 添加到敌机的子弹列表
    enemyBullets.push_back(newEnemyBullet);
     
}

SDL_FPoint SceneMain::getDirection(Enemy* enemy)
{
    auto x = (player.position.x + player.width / 2) - enemy->position.x - enemy->width / 2;
    auto y = (player.position.y + player.height / 2) - enemy->position.y - enemy->height / 2;
    auto distance = sqrt(x * x + y * y);

    // 归一化方向向量   
    x /= distance;
    y /= distance;

    return SDL_FPoint{x, y};
}

// 更新敌机子弹
void SceneMain::updateEnemyBullet(float deltaTime)
{
    auto margin = 32;
    for(auto it = enemyBullets.begin(); it != enemyBullets.end();)
    {
        auto cur_enemy_bullet = *it;

        // 根据方向更新子弹的位置
        cur_enemy_bullet->position.x += cur_enemy_bullet->direction.x * cur_enemy_bullet->speed * deltaTime;
        cur_enemy_bullet->position.y += cur_enemy_bullet->direction.y * cur_enemy_bullet->speed * deltaTime;

        // 检查子弹是否飞出屏幕
        if(cur_enemy_bullet->position.y - margin > game.getWindowHeight() || 
        cur_enemy_bullet->position.x - margin > game.getWindowWidth() ||
        cur_enemy_bullet->position.y + margin < 0 ||
        cur_enemy_bullet->position.x + margin < 0)
        {
            it = enemyBullets.erase(it);
            delete cur_enemy_bullet;
        }
        else
        {
            SDL_Rect bulletRect = {static_cast<int>(cur_enemy_bullet->position.x), static_cast<int>(cur_enemy_bullet->position.y), cur_enemy_bullet->width, cur_enemy_bullet->height};
            SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};

            if(SDL_HasIntersection(&bulletRect, &playerRect))
            {
                player.currentHealth -= cur_enemy_bullet->damage;
                delete cur_enemy_bullet;
                it = enemyBullets.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}


void SceneMain::renderEnemyBullet()
{
    for(auto it : enemyBullets)
    {
        SDL_Rect enemyBulletRect = {static_cast<int>(it->position.x), static_cast<int>(it->position.y), it->width, it->height};

        // 计算子弹旋转的角度
        float angle = atan2(it->direction.y, it->direction.x) * 180 / M_PI - 90;

        // 使用带旋转参数的渲染函数
        SDL_RenderCopyEx(game.getRenderer(), it->texture, nullptr, &enemyBulletRect, angle, nullptr, SDL_FLIP_NONE);
    }
}

void SceneMain::updatePlayer(float deltaTime)
{
    if(isDead)
    {
        return;
    }

    // 更新玩家位置
    if(player.currentHealth <= 0)
    {
        isDead = true;
    }

    for(auto enemy : enemies)
    {
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
        SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};

        if(SDL_HasIntersection(&enemyRect, &playerRect))
        {
            player.currentHealth -= 1;
            enemy->currentHealth = 0;
        }
    }
}


void SceneMain::enemyExplode(Enemy* enemy)
{
    
}
