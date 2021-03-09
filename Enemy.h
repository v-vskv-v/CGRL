#ifndef MAIN_ENEMY_H
#define MAIN_ENEMY_H

#include "Image.h"
#include "Process.h"
#include <unordered_map>
#include <memory>
#include <list>

struct Enemy {
    explicit Enemy(Point pos = {10, 10}) :
            coords(pos), old_coords(coords) {}
    int GetSpeed() const { return move_speed; }
    Point GetCoords() const { return coords; }
    void MovePos(bool move) { can_move = move; }
    bool isAlive() const { return alive; }
    void Dead() { alive = false; }
    void ProcessInput(MovementDir dir);
    void Stop() { coords = old_coords; }
    void Draw(Image &env_screen, Image &screen);
    void Update();
    MovementDir GetNextCond() const { return static_cast<MovementDir>(cond); }

private:
    Point coords;
    static std::array<MovementDir, ENEMY_PATH_LEN> path;
    static std::unordered_map<int, std::unique_ptr<Image>> enemy;
    static std::unordered_map<MovementDir, StepDir> MoveToStep;
    Point old_coords;
    int move_speed{MOVE_SPEED};
    int cond{(int) MovementDir::IDLE};
    int step{-1};
    int cond_step{};
    bool alive{true};
    bool can_move{};
};

#endif //MAIN_ENEMY_H