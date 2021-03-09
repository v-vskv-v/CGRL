#include "Enemy.h"
#include <utility>

std::unordered_map<int, std::unique_ptr<Image>> Enemy::enemy(std::move(get_enemy()));
std::array<MovementDir, ENEMY_PATH_LEN> Enemy::path({MovementDir::UP,
                                                     MovementDir::UP,
                                                     MovementDir::UP,
                                                     MovementDir::RIGHT,
                                                     MovementDir::RIGHT,
                                                     MovementDir::RIGHT,
                                                     MovementDir::DOWN,
                                                     MovementDir::DOWN,
                                                     MovementDir::DOWN,
                                                     MovementDir::LEFT,
                                                     MovementDir::LEFT,
                                                     MovementDir::LEFT});

std::unordered_map<MovementDir, StepDir> Enemy::MoveToStep{
    {MovementDir::UP, StepDir::ENEMY_UP},
    {MovementDir::DOWN, StepDir::ENEMY_DOWN},
    {MovementDir::LEFT, StepDir::ENEMY_LEFT},
    {MovementDir::RIGHT, StepDir::ENEMY_RIGHT}};

void Enemy::ProcessInput(MovementDir dir) {
    int move_dist = move_speed;
    switch(dir) {
        case MovementDir::UP:
            old_coords.y = coords.y;
            coords.y += move_dist;
            break;
        case MovementDir::DOWN:
            old_coords.y = coords.y;
            coords.y -= move_dist;
            break;
        case MovementDir::LEFT:
            old_coords.x = coords.x;
            coords.x -= move_dist;
            break;
        case MovementDir::RIGHT:
            old_coords.x = coords.x;
            coords.x += move_dist;
            break;
        default:
            break;
    }
}

void Enemy::Draw(Image &env_screen, Image &screen) {
    if (!isAlive()) {
        if (cond != (int) MovementDir::ENEMY_DEAD) {
            cond = (int) MovementDir::ENEMY_DEAD;
            cond_step = 0;
        } else if (cond_step < (int) StepDir::ENEMY_DEAD - 1) {
            cond_step++;
        }
    } else if (can_move and GetNextCond() != MovementDir::IDLE) {
        ProcessInput(GetNextCond());
        for (int y = old_coords.y; y < old_coords.y + tileSize; ++y) {
            for (int x = old_coords.x; x < old_coords.x + tileSize; ++x) {
                screen.PutPixel(x, y, env_screen.GetPixel(x, y));
            }
        }
        old_coords = coords;
    }

    for (int y = coords.y; y < coords.y + tileSize; ++y) {
        for (int x = coords.x; x < coords.x + tileSize; ++x) {
            Pixel pix = enemy[10 * cond + cond_step]->GetPixel(x - coords.x, tileSize - y - 1 + coords.y);
            screen.PutPixel(x, y, blend(env_screen.GetPixel(x, y), pix));
        }
    }
    Update();
}

void Enemy::Update() {
    if (cond == (int) MovementDir::IDLE) {
        cond_step++;
        if (cond_step == (int) StepDir::ENEMY_IDLE) {
            step = 0;
            cond_step = 0;
            cond = (int) path[0];
        }
    } else if (can_move) {
        cond_step++;
        cond_step %= (int) MoveToStep[path[step]];
        step++;
        step %= path.size();
        int new_cond = (int) path[step];
        if (new_cond != cond) {
            cond = new_cond;
            cond_step = 0;
        }
    }
}