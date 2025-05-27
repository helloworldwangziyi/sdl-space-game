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
    updateExplosions(deltaTime); // 更新爆炸动画
    updateItem(deltaTime); // 更新物品
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

    // 渲染爆炸动画
    renderExplosions();

    // 渲染物品
    renderItem();

    // 渲染UI
    renderUI();
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

    // 初始化爆炸动画
    explosion.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/effect/explosion.png");
    SDL_QueryTexture(explosion.texture, NULL, NULL, &explosion.width, &explosion.height);
    explosion.totalFrames = explosion.width / explosion.height;
    explosion.width = explosion.height;

    // 初始化物品
    item.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/bonus_life.png");
    SDL_QueryTexture(item.texture, NULL, NULL, &item.width, &item.height);
    item.width /= 4;
    item.height /= 4;

    // 读取并播放背景音乐
    bgm = Mix_LoadMUS("assets/assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if(bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load background music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);

    // 读取音效音源
    sounds["player_shoot"] = Mix_LoadWAV("assets/assets/sound/laser-shoot4.wav");
    sounds["enemy_shoot"] = Mix_LoadWAV("assets/assets/sound/xs_laser.wav");
    sounds["enemy_explode"] = Mix_LoadWAV("assets/assets/sound/explosion3.wav");
    sounds["player_explode"] = Mix_LoadWAV("assets/assets/sound/explosion1.wav");
    sounds["get_item"] = Mix_LoadWAV("assets/assets/sound/eff5.wav");
    sounds["hit"] = Mix_LoadWAV("assets/assets/sound/eff11.wav");

    uiHealth = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/Health UI Black.png");

    // 载入字体
    // 初始化 SDL_ttf
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    scoreFont = TTF_OpenFont("assets/assets/font/VonwaonBitmap-12px.ttf", 24);
    if (scoreFont == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s", TTF_GetError());
    }
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
    for(auto it : explosions)
    {
        if(it != nullptr)
        {
            delete it;
        }
    }

    explosions.clear();
    // 清理爆炸动画
    if(explosion.texture != nullptr)
    {
        SDL_DestroyTexture(explosion.texture);
        // explosion.texture = nullptr;
    }

    for(auto it : items)
    {
        if(it != nullptr)
        {
            delete it;
        }
    }
    items.clear();

    if(item.texture != nullptr)
    {
        SDL_DestroyTexture(item.texture);
    }

    for(auto it : sounds)
    {
        if(it.second != nullptr)
        {
            Mix_FreeChunk(it.second);
        }
    }
    sounds.clear();
    // 清理音乐资源
    if(bgm != nullptr)
    {
        Mix_FreeMusic(bgm);
        Mix_HaltMusic();
        bgm = nullptr;
    }

    // 清理ui
    if(uiHealth != nullptr)
    {
        SDL_DestroyTexture(uiHealth);
    }

    // 清理字体
    if(scoreFont != nullptr)
    {
        TTF_CloseFont(scoreFont);
        scoreFont = nullptr;
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
    Mix_PlayChannel(0, sounds["player_shoot"], 0);
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
                    Mix_PlayChannel(-1, sounds["hit"], 0);
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
    Mix_PlayChannel(-1, sounds["enemy_shoot"], 0);
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
                Mix_PlayChannel(-1, sounds["hit"], 0);
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
        playerExplode();
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
    auto currentTime = SDL_GetTicks();
    auto cur_explosion = new Explosion(explosion);
    cur_explosion->position.x = enemy->position.x + enemy->width / 2 - cur_explosion->width / 2;
    cur_explosion->position.y = enemy->position.y + enemy->height / 2 - cur_explosion->height / 2;
    cur_explosion->startTime = currentTime;
    explosions.push_back(cur_explosion);

    // 添加50%概率掉落物品
    if(dis(gen) < 0.5)
    {
        dropItem(enemy);
    }
    delete enemy;

    score += 10;

}


void SceneMain::updateExplosions(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for(auto it = explosions.begin(); it != explosions.end();)
    {
        auto cur_explosion = *it;

        // 根据时间差 和帧率计算当前索引帧
        cur_explosion->currentFrame = (currentTime - cur_explosion->startTime)  * cur_explosion->FPS / 1000;

        if(cur_explosion->currentFrame >= cur_explosion->totalFrames)
        {
            delete cur_explosion;
            it = explosions.erase(it);
        }
        else
        {
            ++it;
        }
        
    }
}

void SceneMain::renderExplosions()
{
    for(auto it : explosions)
    {
        SDL_Rect src = {
            it->width * it->currentFrame,
            0,
            it->width,
            it->height
        };

        SDL_Rect dst = 
        {
            static_cast<int>(it->position.x),
            static_cast<int>(it->position.y),
            it->width,
            it->height
        };

        SDL_RenderCopy(game.getRenderer(), it->texture, &src, &dst);
    }
}

void SceneMain::dropItem(Enemy* enemy)
{
    auto new_item = new Item(item);
    new_item->position.x = enemy->position.x + enemy->width / 2 - new_item->width / 2;
    new_item->position.y = enemy->position.y + enemy->height / 2 - new_item->height / 2;

    // 生成随机方向
    float angle = dis(gen) * 2 * M_PI;
    new_item->direction.x = cos(angle);
    new_item->direction.y = sin(angle);

    items.push_back(new_item);
}

void SceneMain::updateItem(float deltaTime)
{
    for(auto it = items.begin(); it != items.end();)
    {
        auto cur_item = *it;

        // 更新物品位置
        cur_item->position.x += cur_item->direction.x * cur_item->speed * deltaTime;
        cur_item->position.y += cur_item->direction.y * cur_item->speed * deltaTime;

        // 处理屏幕边缘反弹
        if(cur_item->position.x < 0 && cur_item->bounceCount > 0)
        {
            cur_item->direction.x = -cur_item->direction.x;
            cur_item->bounceCount--;
        }
        if(cur_item->position.x > game.getWindowWidth() - cur_item->width && cur_item->bounceCount > 0)
        {
            cur_item->direction.x = -cur_item->direction.x;
            cur_item->bounceCount--;
        }
        if(cur_item->position.y < 0 && cur_item->bounceCount > 0)
        {
            cur_item->direction.y = -cur_item->direction.y;
            cur_item->bounceCount--;
        }
        if(cur_item->position.y > game.getWindowHeight() - cur_item->height && cur_item->bounceCount > 0)
        {
            cur_item->direction.y = -cur_item->direction.y;
            cur_item->bounceCount--;
        }
        // 如果超出屏幕范围就删除
        if(cur_item->position.x + cur_item->width < 0 || cur_item->position.x > game.getWindowWidth() ||
        cur_item->position.y + cur_item->height < 0 || cur_item->position.y > game.getWindowHeight())
        {
            it = items.erase(it);
            delete cur_item;
        }
        else 
        {
            // 检测物体与玩家的碰撞
            SDL_Rect itemRect = {static_cast<int>(cur_item->position.x), static_cast<int>(cur_item->position.y), cur_item->width, cur_item->height};
            SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};

            if(SDL_HasIntersection(&itemRect, &playerRect))
            {
                playerGetItem(cur_item);
                it = items.erase(it);
                delete cur_item;
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::playerGetItem(Item* item)
{
    // 根据物品类型处理
    score += 5;
    if(item->type == ItemType::Life)
    {
        Mix_PlayChannel(-1, sounds["get_item"], 0);
        player.currentHealth += 1;
        if(player.currentHealth > player.maxHealth)
        {
            player.currentHealth = player.maxHealth;
        }
    }

}


void SceneMain::renderItem()
{
    for(auto it : items)
    {
        SDL_Rect itemRect = {static_cast<int>(it->position.x), static_cast<int>(it->position.y), it->width, it->height};
        SDL_RenderCopy(game.getRenderer(), it->texture, nullptr, &itemRect);
    }
}


void SceneMain::playerExplode()
{
    auto currentTime = SDL_GetTicks();
    Explosion* newExplosion = new Explosion(explosion);
    newExplosion->position = player.position;
    newExplosion->startTime = currentTime;
    explosions.push_back(newExplosion);
    

    Mix_PlayChannel(-1, sounds["player_explode"], 0);
    
}

void SceneMain::renderUI()
{
    int x = 10;
    int y = 10;
    int size = 32;
    int offset = 40;
    SDL_SetTextureColorMod(uiHealth, 100, 100, 100);

    for(int i = 0; i < player.maxHealth; i++)
    {
        SDL_Rect rect = {x + i * offset, y, size, size};
        SDL_RenderCopy(game.getRenderer(), uiHealth, nullptr, &rect);
    }

    SDL_SetTextureColorMod(uiHealth, 255, 255, 255);

    for(int i = 0; i < player.currentHealth; i++)
    {
        SDL_Rect rect = {x + i * offset, y , size, size};
        SDL_RenderCopy(game.getRenderer(), uiHealth, nullptr, &rect);
    }

    std::string text = "SCORE: " + std::to_string(score);
    SDL_Color color = {255, 255, 255, 255};
    
    SDL_Surface* surface = TTF_RenderUTF8_Solid(scoreFont, text.c_str(), color);
    if (surface == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to render text: %s", TTF_GetError());
        return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
    if (texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create texture: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect rect = {game.getWindowWidth() - 10 - surface->w, 10, surface->w, surface->h};
    SDL_RenderCopy(game.getRenderer(), texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    
}
    
