#include "Env.h"

void Env::DrawBackground(Image &screen) {
    for (int i = 0; i < blockHeight; i++) {
        for (int j = 0; j < blockWidth; j++) {
            for (int x = coords.x; x < coords.x + tileSize; x++) {
                for (int y = coords.y; y < coords.y + tileSize; y++) {
                    Pixel pix = env[EnvDir::FLOOR]->GetPixel(x - coords.x, tileSize - y - 1 + coords.y);
                    screen.PutPixel(x, y, pix);
                }
            }
            coords.x += tileSize;
        }
        coords.y += tileSize;
        coords.x = 0;
    }
    coords.y = 0;
}

Point Env::ProcessResources(void) {
    coords.y = 0;
    coords.x = 0;
    Point hero_coords{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    for (int i = 0; i < blockHeight; i++) {
        for (int j = 0; j < blockWidth; j++) {
            char c = maps[current_level][i][j];
            auto mask = pos_mapper[c];
            for (int x = coords.x; x < coords.x + tileSize; x++) {
                for (int y = coords.y; y < coords.y + tileSize; y++) {
                    mask_map[y][x] = mask;
                }
            }
            int key;
            switch (c) {
                case '@':
                    hero_coords.x = j * tileSize;
                    hero_coords.y = i * tileSize;
                    break;
                case 'E':
                    key = i * blockWidth + j;
                    enemies.push_back(Enemy({j * tileSize, i * tileSize}));
                    break;
                case 'D':
                    key = i * blockWidth + j;
                    doors.insert(std::make_pair(key, Door()));
                    break;
                case '#':
                    key = i * blockWidth + j;
                    walls.insert(std::make_pair(key, Wall()));
                    break;
                case ' ':
                    key = i * blockWidth + j;
                    spaces.insert(std::make_pair(key, Space()));
                    break;
                case 'x':
                    key = i * blockWidth + j;
                    targets.insert(std::make_pair(key, Target()));
                    break;
                case 'T':
                    key = i * blockWidth + j;
                    torches.insert(std::make_pair(key, Torch()));
                    break;
                case '.':
                    key = i * blockWidth + j;
                    grass.insert(std::make_pair(key, Grass()));
                    break;
                default:
                    break;
            }
            coords.x += tileSize;
        }
        coords.y += tileSize;
        coords.x = 0;
    }
    coords.y = 0;
    return hero_coords;
}

void Env::ProcessEnemies(Image &env_screen, Image &screen, Player &player) { // enemy's speed equals player's speed, speed = 32 (_-_)
    Point coords = player.GetCoords();
    for (auto &enemy : enemies) {
        if (enemy.isAlive()) {
            int move_dist = enemy.GetSpeed();
            Point pos_coords = enemy.GetCoords();
            MovementDir k = enemy.GetNextCond();
            switch (k) {
                case MovementDir::UP:
                    pos_coords.y += move_dist;
                    break;
                case MovementDir::DOWN:
                    pos_coords.y -= move_dist;
                    break;
                case MovementDir::RIGHT:
                    pos_coords.x += move_dist;
                    break;
                case MovementDir::LEFT:
                    pos_coords.x -= move_dist;
                    break;
                default:
                    break;
            }
            Point pb = {coords.x / tileSize, coords.y / tileSize};
            Point eb = {pos_coords.x / tileSize, pos_coords.y / tileSize};
            if (pb == eb) {
                enemy.MovePos(false);
            } else {
                auto mask = mask_map[eb.y * tileSize][eb.x * tileSize];
                if (mask == PosDir::NON or mask == PosDir::ACT or mask == PosDir::DEATH or mask == PosDir::TARGET) {
                    if (mask == PosDir::DEATH) {
                        enemy.Dead();
                    }
                    enemy.MovePos(false);
                } else {
                    Point coords = enemy.GetCoords();
                    for (int x = coords.x; x < coords.x + tileSize; ++x) {
                        for (int y = coords.y; y < coords.y + tileSize; ++y) {
                            mask_map[y][x] = PosDir::POS;
                        }
                    }
                    for (int x = pos_coords.x; x < pos_coords.x + tileSize; ++x) {
                        for (int y = pos_coords.y; y < pos_coords.y + tileSize; ++y) {
                            mask_map[y][x] = PosDir::NON;
                        }
                    }
                    enemy.MovePos(true);
                }
            }
        }
        enemy.Draw(env_screen, screen);
    }
}

void Env::DrawResource(Image &env_screen, Image &screen, std::unordered_map<int, Resource> &objs) {
    for (auto &obj : objs) {
        Point coords{(obj.first % blockWidth) * tileSize, (obj.first / blockWidth) * tileSize};
        auto &img = env[obj.second.getStep[obj.second.step]];
        for (int x = coords.x; x < coords.x + tileSize; x++) {
            for (int y = coords.y; y < coords.y + tileSize; y++) {
                Pixel pix = img->GetPixel(
                        x - coords.x, tileSize - y - 1 + coords.y);
                screen.PutPixel(x, y, blend(env_screen.GetPixel(x, y), pix));
            }
        }
        obj.second.step++;
        if (obj.second.step == obj.second.steps) {
            obj.second.step = 0;
        }
    }
}

void Env::GetBloomResource(std::unordered_map<int, Resource> &objs) {
    for (auto &obj : objs) {
        Point coords{(obj.first % blockWidth) * tileSize, (obj.first / blockWidth) * tileSize};
        if (obj.second.blooming) {
            int x_l = coords.x + obj.second.shapes[0].x;
            int y_l = coords.y + obj.second.shapes[0].y;
            int x_r = coords.x + obj.second.shapes[1].x;
            int y_r = coords.y + obj.second.shapes[1].y;
            for (int x_ = x_l; x_ <= x_r; ++x_) {
                for (int y_ = y_l; y_ <= y_r; ++y_) {
                    int x_max = x_ + RADIUS_BLOOM < WINDOW_WIDTH ? x_ + RADIUS_BLOOM : WINDOW_WIDTH - 1;
                    int y_max = y_ + RADIUS_BLOOM < WINDOW_HEIGHT ? y_ + RADIUS_BLOOM : WINDOW_HEIGHT - 1;
                    for (int x = x_ - RADIUS_BLOOM + 1 > 0 ? x_ - RADIUS_BLOOM + 1 : 0, j = 0; x < x_max; ++x, ++j) {
                        for (int y = y_ - RADIUS_BLOOM + 1 > 0 ? y_ - RADIUS_BLOOM + 1 : 0, i = 0; y < y_max; ++y, ++i) {
                            auto add = Add.find(y * WINDOW_WIDTH + x);
                            if (add == Add.end()) {
                                Add.insert(std::make_pair(y * WINDOW_WIDTH + x, bloom_matrix[i][j]));
                            } else {
                                add->second = add->second + bloom_matrix[i][j] > MAX_BLOOM ? MAX_BLOOM : add->second + bloom_matrix[i][j];
                            }
                        }
                    }
                }
            }
        }
    }
}

void Env::ApplyBloom(Image &env_screen, Image &screen) {
    for (auto &item : Add) {
        Point coords{item.first % WINDOW_WIDTH, item.first / WINDOW_WIDTH};
        int x = coords.x;
        int y = coords.y;
        screen.PutPixel(x, y, blend(env_screen.GetPixel(x, y), screen.GetPixel(x, y) + item.second));
    }
}

void Env::GetBloomResources(void) {
    GetBloomResource(targets);
    GetBloomResource(spaces);
    GetBloomResource(walls);
    GetBloomResource(torches);
    GetBloomResource(grass);
}

void Env::DrawResources(Image &env_screen, Image &screen) {
    DrawResource(env_screen, screen, walls);
    DrawResource(env_screen, screen, spaces);
    DrawResource(env_screen, screen, targets);
    DrawResource(env_screen, screen, torches);
    DrawResource(env_screen, screen, grass);
}

void Env::ApplyHazeResource(Image &env_screen, Image &screen, std::unordered_map<int, Resource> &objs) {
    for (auto &obj : objs) {
        if (obj.second.haze) {
            Point coords = {(obj.first % blockWidth) * tileSize, (obj.first / blockWidth) * tileSize};
            int y_l = obj.second.haze_zone[0].y;
            int y_r = obj.second.haze_zone[1].y;
            int x_l = obj.second.haze_zone[0].x;
            int x_r = obj.second.haze_zone[1].x;
            for (int y = coords.y + y_l; y < coords.y + y_r; ++y) {
                double shift = (haze_shifts[(y + obj.second.haze_step) % WINDOW_HEIGHT] * HAZE_STRENGTH);
                if (shift > 0) {
                    for (int x = coords.x + x_r - 1; x >= coords.x + x_l; --x) {
                        int x_shift = x + (int) shift < WINDOW_WIDTH ? x + (int) shift : WINDOW_WIDTH - 1;
                        screen.PutPixel(x_shift, y, blend(env_screen.GetPixel(x_shift, y), screen.GetPixel(x, y)));
                    }
                } else {
                    for (int x = coords.x + x_l; x < coords.x + x_r; ++x) {
                        int x_shift = x + (int) shift > 0 ? x + (int) shift : 0;
                        screen.PutPixel(x_shift, y, blend(env_screen.GetPixel(x_shift, y), screen.GetPixel(x, y)));
                    }
                }
            }
            obj.second.haze_step++;
            obj.second.haze_step %= WINDOW_HEIGHT;
        }
    }
}

void Env::ApplyHaze(Image &env_screen, Image &screen) {
    ApplyHazeResource(env_screen, screen, walls);
    ApplyHazeResource(env_screen, screen, spaces);
    ApplyHazeResource(env_screen, screen, targets);
    ApplyHazeResource(env_screen, screen, torches);
    ApplyHazeResource(env_screen, screen, grass);
}

/*
void Env::ApplyHazeFull(Image &env_screen, Image &screen) {
        static int haze_step = std::rand() % WINDOW_WIDTH;
        static double haze_strength = 10;
        int y_l = 0;
        int y_r = WINDOW_HEIGHT;
        int x_l = 0;
        int x_r = WINDOW_WIDTH;
        for (int x = x_l; x < x_r; ++x) {
            double shift = (haze_shifts[(x + haze_step) % WINDOW_WIDTH] * HAZE_STRENGTH);
            if (shift > 0) {
                for (int y = y_r - 1; y >= y_l; --y) {
                    int y_shift = y + (int) shift < WINDOW_HEIGHT ? y + (int) shift : WINDOW_HEIGHT - 1;
                    screen.PutPixel(x, y_shift, blend(env_screen.GetPixel(x, y_shift), screen.GetPixel(x, y)));
                }
            } else {
                for (int y = 0; y < y_r; ++y) {
                    int y_shift = y + (int) shift > 0 ? y + (int) shift : 0;
                    screen.PutPixel(x, y_shift, blend(env_screen.GetPixel(x, y_shift), screen.GetPixel(x, y)));
                }
            }
        }
        haze_step++;
        haze_step %= WINDOW_WIDTH;
}
*/

bool Env::NotValidDoors(void) {
    bool any_process{};
    for (auto &door: doors) {
        Point coords{(door.first % blockWidth) * tileSize, (door.first / blockWidth) * tileSize};
        if (door.second.need_process) {
            any_process = true;
            PosDir mask = PosDir::NON;
            if (door.second.status == EnvDir::DOOR_0) {
                door.second.step++;
                if (door.second.step == door.second.steps) {
                    door.second.step = 0;
                    door.second.need_process = false;
                    door.second.status = EnvDir::DOOR_3;
                    mask = PosDir::DEACT;
                }
            } else {
                door.second.step++;
                if (door.second.step == door.second.steps) {
                    door.second.step = 0;
                    door.second.need_process = false;
                    door.second.status = EnvDir::DOOR_0;
                    mask = PosDir::ACT;
                }
            }
            if (mask != PosDir::NON) {
                for (int x = coords.x; x < coords.x + tileSize; x++) {
                    for (int y = coords.y; y < coords.y + tileSize; y++) {
                        mask_map[y][x] = mask;
                    }
                }
            }
        }
    }
    return any_process;
}

void Env::DrawDoors(Image &env_screen, Image &screen) {
    for (auto &door: doors) {
        Point coords{(door.first % blockWidth) * tileSize, (door.first / blockWidth) * tileSize};
        auto &img = env[door.second.getStep[10 * (int) door.second.status + door.second.step]];
        for (int x = coords.x; x < coords.x + tileSize; x++) {
            for (int y = coords.y; y < coords.y + tileSize; y++) {
                Pixel pix = img->GetPixel(
                        x - coords.x, tileSize - y - 1 + coords.y);
                screen.PutPixel(x, y, blend(env_screen.GetPixel(x, y), pix));
            }
        }
    }
}

void Env::DrawTransit(EnvDir n, Image &env_screen, Image &screen) {
    auto &img = env[n];
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            Pixel pix = img->GetPixel(x, WINDOW_HEIGHT - y - 1);
            screen.PutPixel(x, y, blend(env_screen.GetPixel(x, y), pix));
        }
    }
}

void Env::ApplyCoef(Image &screen, uint8_t coef, uint8_t norm) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            Pixel pix = (screen.GetPixel(x, y) / norm) * coef;
            screen.PutPixel(x, y, pix);
        }
    }
}

CondDir Env::checkPosMove(Player &player, MovementDir k) {
    int move_dist = player.GetSpeed();
    Point pos_coords = player.GetCoords();
    Point *shapes = player.GetShapes();
    switch (k) {
        case MovementDir::UP:
            pos_coords.y += move_dist;
            break;
        case MovementDir::DOWN:
            pos_coords.y -= move_dist;
            break;
        case MovementDir::RIGHT:
            pos_coords.x += move_dist;
            break;
        case MovementDir::LEFT:
            pos_coords.x -= move_dist;
            break;
        default:
            break;
    }
    // death_crossover = 0;
    // wall_crossover = 0
    // target_crossover = 0
    bool dead{};
    bool non{};
    for (int y = pos_coords.y + shapes[0].y; y < pos_coords.y + shapes[1].y; y++) {
        for (int x = pos_coords.x + shapes[0].x; x < pos_coords.x + shapes[1].x; x++) {
            auto mask = mask_map[y][x];
            level_completed |= mask == PosDir::TARGET;
            dead |= mask == PosDir::DEATH;
            non |= mask == PosDir::NON || mask == PosDir::ACT;
        }
    }
    if (level_completed) {
        return CondDir::WELL_TRUE;
    } else if (dead) {
        player.Dead();
        return CondDir::WELL_TRUE;
    } else if (non) {
        return CondDir::WELL_FALSE;
    }
    return CondDir::WELL_TRUE;
}
    /*
            if (mask == PosDir::ACT) {
                return CondDir::WELL_FALSE;
            } else if (mask == PosDir::DEATH) {
                death_crossover++;
            }
            wall_crossover += mask == PosDir::NON;
            target_crossover += mask == PosDir::TARGET;
        }
    }
    if (death_crossover > death_thrsh) {
        player.Dead();
        return CondDir::WELL_FALSE;
    }
    wall_crossover += target_crossover;
    level_completed = target_crossover > thrsh ? true : false;
    return wall_crossover < thrsh ? CondDir::WELL_TRUE : CondDir::WELL_FALSE;
     */

void Env::checkPosDoor(Player &player) {
    std::unordered_map<int, int> posPositions;
    Point coords = player.GetCoords();
    Point *shapes = player.GetShapes();
    for (int y = coords.y; y < coords.y + tileSize; ++y) {
        for (int x = coords.x; x < coords.x + tileSize; ++x) {
            int i = y / tileSize;
            int j = x / tileSize;
            auto pos = posPositions.find(i * blockWidth + j);
            if (pos == posPositions.end()) {
                posPositions.insert(std::make_pair(i * blockWidth + j, 1));
            } else {
                pos->second++;
            }
        }
    }
    int key;
    int val = 0;
    for (const auto &item : posPositions) {
        if (item.second > val) {
            val = item.second;
            key = item.first;
        }
    }
    int i = key / blockWidth;
    int j = key % blockWidth;
    if (i == 0) ++i;
    if (j == 0) ++j;
    int hitbox[] = {(i + 1) * blockWidth + j,
                    (i - 1) * blockWidth + j,
                    i * blockWidth + (j + 1),
                    i * blockWidth + (j - 1)};
    for (int key: hitbox) {
        auto door = doors.find(key);
        if (door != doors.end()) {
            door->second.need_process = true;
        }
    }
}