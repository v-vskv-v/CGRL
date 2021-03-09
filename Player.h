#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Process.h"
#include <unordered_map>
#include <memory>

struct Player {
    explicit Player(Point pos = {10, 10}) :
            coords(pos), old_coords(coords) {}
    bool Moved() const;
    int GetSpeed() const { return move_speed; }
    Point GetCoords() const { return coords; }
    Point *GetShapes() { return shapes; }
    bool isAlive() const { return alive; }
    void Dead() { alive = false; }
    void ProcessInput(MovementDir dir);
    void Stop() { coords = old_coords; }
    void Draw(Image &env, Image &screen, MovementDir move);
private:
    Point coords;
    Point shapes[2] = {{12, 0}, {20, 8}};
    const StepDir steps[5]{StepDir::HERO_IDLE, StepDir::HERO_UP, StepDir::HERO_RIGHT, StepDir::HERO_DOWN, StepDir::HERO_LEFT};
    std::unordered_map<int, std::unique_ptr<Image>> hero{get_hero()};
    Point old_coords;
    int move_speed{MOVE_SPEED};
    int cond{};
    int step{};
    bool alive{true};
};

#endif //MAIN_PLAYER_H
