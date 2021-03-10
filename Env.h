#ifndef MAIN_ENV_H
#define MAIN_ENV_H

#include "Image.h"
#include "Player.h"
#include "Enemy.h"
#include "Process.h"
#include <unordered_map>
#include <array>
#include <memory>
#include <cmath>

class Env {
    struct Resource {
        int steps;
        int step;
        bool blooming;
        bool haze;
        int haze_step{std::rand() % WINDOW_WIDTH};
        Point shapes[2]{};
        Point haze_zone[2]{};
        std::unordered_map<int, EnvDir> getStep;
        Resource(int steps_, int step_, bool blooming_, bool haze_, std::unordered_map<int, EnvDir> getStep_,
                 Point haze_l={0, 0}, Point haze_r={0, 0}, Point shape_l={0,0}, Point shape_r={0, 0}) :
                steps(steps_), step(step_), getStep(std::move(getStep_)), blooming(blooming_), haze(haze_) {
            shapes[0] = shape_l;
            shapes[1] = shape_r;
            haze_zone[0] = haze_l;
            haze_zone[1] = haze_r;
        }
    };
    struct Door {
        EnvDir status{EnvDir::DOOR_0};
        int step{};
        int steps{(int) StepDir::DOOR};
        bool need_process{};
        std::unordered_map<int, EnvDir> getStep {
                {10 * (int) EnvDir::DOOR_0, EnvDir::DOOR_0},
                {10 * (int) EnvDir::DOOR_0 + 1, EnvDir::DOOR_1},
                {10 * (int) EnvDir::DOOR_0 + 2, EnvDir::DOOR_2},
                {10 * (int) EnvDir::DOOR_0 + 3, EnvDir::DOOR_3},
                {10 * (int) EnvDir::DOOR_3, EnvDir::DOOR_3},
                {10 * (int) EnvDir::DOOR_3 + 1, EnvDir::DOOR_2},
                {10 * (int) EnvDir::DOOR_3 + 2, EnvDir::DOOR_1},
                {10 * (int) EnvDir::DOOR_3 + 3, EnvDir::DOOR_0}
        };
    };
    struct Torch : Resource {
        Torch() : Resource((int) StepDir::TORCH, std::rand() % (int) StepDir::TORCH, true, true, {
                {0, EnvDir::TORCH_0},
                {1, EnvDir::TORCH_1},
                {2, EnvDir::TORCH_2}},
                           {8, 28}, {24, 40}, {9, 22}, {23, 28}) {}
    };
    struct Space : Resource {
        Space() : Resource((int) StepDir::SPACE, std::rand() % (int) StepDir::SPACE, false, false, {
                {0, EnvDir::SPACE_0},
                {1, EnvDir::SPACE_1},
                {2, EnvDir::SPACE_2},
                {3, EnvDir::SPACE_3}}) {}
    };
    struct Target : Resource {
        Target() : Resource((int) StepDir::TARGET, std::rand() % (int) StepDir::TARGET, false, false, {
                {0, EnvDir::TARGET_0},
                {1, EnvDir::TARGET_1},
                {2, EnvDir::TARGET_2},
                {3, EnvDir::TARGET_3}}) {}
    };
    struct Wall : Resource {
        Wall() : Resource((int) StepDir::WALL, std::rand() % (int) StepDir::WALL, false, false, {
                {0, EnvDir::WALL_0},
                {1, EnvDir::WALL_1},
                {2, EnvDir::WALL_2},
                {3, EnvDir::WALL_3}}) {}
    };
    struct Grass : Resource {
        Grass() : Resource((int) StepDir::GRASS, 0, false, false, {
                {0, EnvDir::FLOOR}}) {}
    };
    std::array<std::array<uint8_t , 2*RADIUS_BLOOM-1>, 2*RADIUS_BLOOM-1>  bloom_matrix{make_bloom_matrix()};
    std::unordered_map<int, uint8_t> Add{};
    Point coords;
    int current_level{};
    bool level_completed{};
    bool is_rendered{};
    std::unordered_map<int, std::vector<std::string>> maps{get_map()};
    std::unordered_map<EnvDir, std::unique_ptr<Image>> env{get_env()};
    std::unordered_map<char, PosDir> pos_mapper {
            {'#', PosDir::NON},
            {'@', PosDir::POS},
            {'E', PosDir::NON},
            {'.', PosDir::POS},
            {' ', PosDir::DEATH},
            {'D', PosDir::ACT},
            {'x', PosDir::TARGET},
            {'T', PosDir::NON}
    };
    std::array<std::array<PosDir, WINDOW_WIDTH>, WINDOW_HEIGHT> mask_map{};
    std::array<double, WINDOW_HEIGHT> haze_shifts;
    std::unordered_map<int, Door> doors{};
    std::unordered_map<int, Resource> walls{};
    std::unordered_map<int, Resource> spaces{};
    std::unordered_map<int, Resource> targets{};
    std::unordered_map<int, Resource> torches{};
    std::unordered_map<int, Resource> grass{};
    std::vector<Enemy> enemies{};
public:
    explicit Env(Point pos = {0, 0}) : coords(pos) {
        for (int i = 0; i < haze_shifts.size(); ++i) {
            haze_shifts[i] = std::sin(HAZE_FREQ * (double) i);
        }
    }
    void NextLevel() {
        ResetLevel(true);
        current_level++;
    }
    void ResetLevel(bool reset_add=false) {
        coords.x = 0;
        coords.y = 0;
        level_completed = false;
        is_rendered = false;
        if (reset_add) {
            Add.clear();
        }
        doors.clear();
        walls.clear();
        spaces.clear();
        targets.clear();
        torches.clear();
        grass.clear();
        enemies.clear();
    }
    bool haveAdd() const { return !Add.empty(); }
    bool isRendered() const { return is_rendered; }
    void Rendered() { is_rendered = true; }
    bool LevelCompleted() const { return level_completed; }
    bool GameCompleted() const { return current_level > levelCount; }
    int GetLevel() const { return current_level; }
    void DrawBackground(Image &screen);
    void DrawResources(Image &env_screen, Image &screen);
    void DrawResource(Image &env_screen, Image &screen, std::unordered_map<int, Resource> &objs);
    void GetBloomResource(std::unordered_map<int, Resource> &objs);
    void GetBloomResources(void);
    void ApplyBloom(Image &env_screen, Image &screen);
    Point ProcessResources(void);
    void ProcessEnemies(Image &env_screen, Image &screen, Player &player);
    void DrawTransit(EnvDir n, Image &env_screen, Image &screen);
    void DrawNext(Image &env_screen, Image &screen) { DrawTransit(EnvDir::NEXT, env_screen, screen); }
    void DrawReset(Image &env_screen, Image &screen) { DrawTransit(EnvDir::RESET, env_screen, screen); }
    void DrawGG(Image &env_screen, Image &screen) { DrawTransit(EnvDir::GG, env_screen, screen); }
    void ApplyCoef(Image &screen, uint8_t coef, uint8_t norm=COEF_NORM);
    bool NotValidDoors(void);
    void ApplyHaze(Image &env_screen, Image &screen);
    // void ApplyHazeFull(Image &env_screen, Image &screen);
    void ApplyHazeResource(Image &env_screen, Image &screen, std::unordered_map<int, Resource> &objs);
    void DrawDoors(Image &env_screen, Image &screen);
    CondDir checkPosMove(Player &player, MovementDir k);
    void checkPosDoor(Player &player);
};

#endif //MAIN_ENV_H
