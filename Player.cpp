#include "Player.h"

bool Player::Moved() const {
    if (coords.x == old_coords.x && coords.y == old_coords.y) {
        return false;
    } else {
        return true;
    }
}

void Player::ProcessInput(MovementDir dir) {
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

void Player::Draw(Image &env_screen, Image &screen, MovementDir move) {
    if (Moved() or move != MovementDir::IDLE) {
        for (int y = old_coords.y; y < old_coords.y + tileSize; ++y) {
            for (int x = old_coords.x; x < old_coords.x + tileSize; ++x) {
                screen.PutPixel(x, y, env_screen.GetPixel(x, y));
            }
        }
        old_coords = coords;
    }
    if (cond == (int) move * 10) {
        step++;
        step %= (int) steps[(int) move];
    } else {
        cond = (int) move * 10;
        step = 0;
    }
    for (int y = coords.y; y < coords.y + tileSize; ++y) {
        for (int x = coords.x; x < coords.x + tileSize; ++x) {
            Pixel pix = hero[cond + step]->GetPixel(x - coords.x, tileSize - y - 1 + coords.y);
            screen.PutPixel(x, y, blend(env_screen.GetPixel(x, y), pix));
        }
    }
}