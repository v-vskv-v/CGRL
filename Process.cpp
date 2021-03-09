#include "Process.h"

std::unordered_map<EnvDir, std::unique_ptr<Image>> get_env(void) {
    std::unordered_map<EnvDir, std::unique_ptr<Image>> env;
    env.insert(std::make_pair(EnvDir::WALL_0, std::make_unique<Image>(Image("../resources/tiles/wall0.png"))));
    env.insert(std::make_pair(EnvDir::WALL_1, std::make_unique<Image>(Image("../resources/tiles/wall1.png"))));
    env.insert(std::make_pair(EnvDir::WALL_2, std::make_unique<Image>(Image("../resources/tiles/wall2.png"))));
    env.insert(std::make_pair(EnvDir::WALL_3, std::make_unique<Image>(Image("../resources/tiles/wall3.png"))));
    env.insert(std::make_pair(EnvDir::FLOOR, std::make_unique<Image>(Image("../resources/tiles/floor.png"))));
    env.insert(std::make_pair(EnvDir::TORCH_0, std::make_unique<Image>(Image("../resources/tiles/torch0.png"))));
    env.insert(std::make_pair(EnvDir::TORCH_1, std::make_unique<Image>(Image("../resources/tiles/torch1.png"))));
    env.insert(std::make_pair(EnvDir::TORCH_2, std::make_unique<Image>(Image("../resources/tiles/torch2.png"))));
    env.insert(std::make_pair(EnvDir::SPACE_0, std::make_unique<Image>(Image("../resources/tiles/space0.png"))));
    env.insert(std::make_pair(EnvDir::SPACE_1, std::make_unique<Image>(Image("../resources/tiles/space1.png"))));
    env.insert(std::make_pair(EnvDir::SPACE_2, std::make_unique<Image>(Image("../resources/tiles/space2.png"))));
    env.insert(std::make_pair(EnvDir::SPACE_3, std::make_unique<Image>(Image("../resources/tiles/space3.png"))));
    env.insert(std::make_pair(EnvDir::DOOR_0, std::make_unique<Image>(Image("../resources/tiles/door_closed.png"))));
    env.insert(std::make_pair(EnvDir::DOOR_1, std::make_unique<Image>(Image("../resources/tiles/door_step_1.png"))));
    env.insert(std::make_pair(EnvDir::DOOR_2, std::make_unique<Image>(Image("../resources/tiles/door_step_2.png"))));
    env.insert(std::make_pair(EnvDir::DOOR_3, std::make_unique<Image>(Image("../resources/tiles/door_opened.png"))));
    env.insert(std::make_pair(EnvDir::TARGET_0, std::make_unique<Image>(Image("../resources/tiles/target0.png"))));
    env.insert(std::make_pair(EnvDir::TARGET_1, std::make_unique<Image>(Image("../resources/tiles/target1.png"))));
    env.insert(std::make_pair(EnvDir::TARGET_2, std::make_unique<Image>(Image("../resources/tiles/target2.png"))));
    env.insert(std::make_pair(EnvDir::TARGET_3, std::make_unique<Image>(Image("../resources/tiles/target3.png"))));
    env.insert(std::make_pair(EnvDir::RESET, std::make_unique<Image>(Image("../resources/tiles/retry.png"))));
    env.insert(std::make_pair(EnvDir::NEXT, std::make_unique<Image>(Image("../resources/tiles/next.png"))));
    env.insert(std::make_pair(EnvDir::GG, std::make_unique<Image>(Image("../resources/tiles/end.png"))));
    return env;
}

std::unordered_map<int, std::unique_ptr<Image>> get_enemy(void) {
    std::unordered_map<int, std::unique_ptr<Image>> enemy;
    std::string base("../resources/tiles/blackdragon/");
    for (int i = 0, j = (int) MovementDir::IDLE * 10; i < (int) StepDir::ENEMY_IDLE; ++i) {
        std::stringstream ss;
        ss << base << "blackdragon_idle" << i << ".png";
        enemy.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::UP * 10; i < (int) StepDir::ENEMY_UP; ++i) {
        std::stringstream ss;
        ss << base << "blackdragon_up" << i << ".png";
        enemy.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::DOWN * 10; i < (int) StepDir::ENEMY_DOWN; ++i) {
        std::stringstream ss;
        ss << base << "blackdragon_down" << i << ".png";
        enemy.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::RIGHT * 10; i < (int) StepDir::ENEMY_RIGHT; ++i) {
        std::stringstream ss;
        ss << base << "blackdragon_right" << i << ".png";
        enemy.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::LEFT * 10; i < (int) StepDir::ENEMY_LEFT; ++i) {
        std::stringstream ss;
        ss << base << "blackdragon_left" << i << ".png";
        enemy.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::ENEMY_DEAD * 10; i < (int) StepDir::ENEMY_DEAD; ++i) {
        std::stringstream ss;
        ss << base << "blackdragon_dead" << i << ".png";
        enemy.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    return enemy;
}

std::unordered_map<int, std::unique_ptr<Image>> get_hero(void) {
    std::unordered_map<int, std::unique_ptr<Image>> hero;
    std::string base("../resources/tiles/hero/");
    for (int i = 0, j = (int) MovementDir::IDLE * 10; i < (int) StepDir::HERO_IDLE; ++i) {
        std::stringstream ss;
        ss << base << "char_idle_" << i << ".png";
        hero.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::UP * 10; i < (int) StepDir::HERO_UP; ++i) {
        std::stringstream ss;
        ss << base << "char_run_up_" << i << ".png";
        hero.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::DOWN * 10; i < (int) StepDir::HERO_DOWN; ++i) {
        std::stringstream ss;
        ss << base << "char_run_down_" << i << ".png";
        hero.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::RIGHT * 10; i < (int) StepDir::HERO_RIGHT; ++i) {
        std::stringstream ss;
        ss << base << "char_run_right_" << i << ".png";
        hero.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    for (int i = 0, j = (int) MovementDir::LEFT * 10; i < (int) StepDir::HERO_LEFT; ++i) {
        std::stringstream ss;
        ss << base << "char_run_left_" << i << ".png";
        hero.insert(std::make_pair(j + i, std::make_unique<Image>(Image(ss.str()))));
    }
    return hero;
}

std::unordered_map<int, std::vector<std::string>> get_map(int n) {
    std::unordered_map<int, std::vector<std::string>> maps;
    for (int i = 0; i <= n; i++) {
        maps.insert(std::make_pair(i, std::vector<std::string>{}));
        std::stringstream ss;
        ss << "../resources/levels/level" << i << ".txt";
        std::ifstream file(ss.str());
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                maps[i].push_back(line);
            }
            file.close();
            std::reverse(maps[i].begin(), maps[i].end());
        }
    }
    return maps;
}

std::array<std::array<uint8_t , 2*RADIUS_BLOOM-1>, 2*RADIUS_BLOOM-1> make_bloom_matrix(void) {
    std::array<std::array<uint8_t, 2*RADIUS_BLOOM-1>, 2*RADIUS_BLOOM-1> M{};
    M[RADIUS_BLOOM-1][RADIUS_BLOOM-1] = CENTER_BLOOM;
    uint8_t value;
    for (uint8_t z = 1; z < RADIUS_BLOOM; z++) {
        if (z > CENTER_STRENGTH) {
            value = CENTER_BLOOM;
        } else {
            value = (int) CENTER_BLOOM - z > 0 ? CENTER_BLOOM - z : 0;
        }

        for (uint8_t i = RADIUS_BLOOM - z - 1; i <= RADIUS_BLOOM - 1 + z; i++) {
            M[RADIUS_BLOOM - z - 1][i] += value;
            M[RADIUS_BLOOM + z - 1][i] += value;
            M[i][RADIUS_BLOOM - z - 1] += value;
            M[i][RADIUS_BLOOM + z - 1] += value;
        }
        M[RADIUS_BLOOM - z - 1][RADIUS_BLOOM - z - 1] >>= 1;
        M[RADIUS_BLOOM + z - 1][RADIUS_BLOOM - z - 1] >>= 1;
        M[RADIUS_BLOOM - z - 1][RADIUS_BLOOM + z - 1] >>= 1;
        M[RADIUS_BLOOM + z - 1][RADIUS_BLOOM + z - 1] >>= 1;
    }
    return M;
}