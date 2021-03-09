#ifndef MAIN_PROCESS_C
#define MAIN_PROCESS_C

#include "Image.h"
#include "common.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>

enum class MovementDir {
    IDLE,
    UP,
    RIGHT,
    DOWN,
    LEFT,
    ENEMY_DEAD
};

enum class StepDir {
    HERO_IDLE = 8,
    HERO_UP = 5,
    HERO_DOWN = 5,
    HERO_LEFT = 6,
    HERO_RIGHT = 6,
    ENEMY_IDLE = 3,
    ENEMY_UP = 3,
    ENEMY_DOWN = 3,
    ENEMY_LEFT = 3,
    ENEMY_RIGHT = 3,
    ENEMY_DEAD = 4,
    SPACE = 4,
    TARGET = 4,
    DOOR = 4,
    WALL = 4,
    TORCH = 3,
    GRASS = 1
};

enum class PosDir {
    POS,
    NON,
    ACT,
    DEACT,
    DEATH,
    TARGET
};

enum class EnvDir {
    FLOOR,
    WALL_0,
    WALL_1,
    WALL_2,
    WALL_3,
    DOOR_0,
    DOOR_1,
    DOOR_2,
    DOOR_3,
    TARGET_0,
    TARGET_1,
    TARGET_2,
    TARGET_3,
    SPACE_0,
    SPACE_1,
    SPACE_2,
    SPACE_3,
    TORCH_0,
    TORCH_1,
    TORCH_2,
    NEXT,
    RESET,
    GG
};

enum class CondDir {
    WELL_TRUE,
    WELL_FALSE
};

struct Point {
    int x;
    int y;
    bool operator ==(const Point &other) const {
        return x == other.x and y == other.y;
    }
};

const int levelCount = 2;
const int blockWidth = 50;
const int blockHeight = 32;
const uint8_t COEF_NORM = 10;
const uint8_t RADIUS_BLOOM = 10;
const uint8_t CENTER_BLOOM = 3;
const uint8_t CENTER_STRENGTH = 7;
const uint8_t MAX_BLOOM = 80;
const double HAZE_FREQ = 1000.0;
const double HAZE_STRENGTH = 4;
const int MOVE_SPEED = 32;
const int ENEMY_PATH_LEN = 12;
constexpr GLsizei WINDOW_WIDTH = tileSize * blockWidth, WINDOW_HEIGHT = tileSize * blockHeight;

std::unordered_map<EnvDir, std::unique_ptr<Image>> get_env(void);
std::unordered_map<int, std::unique_ptr<Image>> get_hero(void);
std::unordered_map<int, std::unique_ptr<Image>> get_enemy(void);
std::unordered_map<int, std::vector<std::string>> get_map(int n = levelCount);
std::array<std::array<uint8_t, 2*RADIUS_BLOOM-1>, 2*RADIUS_BLOOM-1> make_bloom_matrix(void);

#endif //MAIN_PROCESS_C
