#include "common.h"
#include "Image.h"
#include "Player.h"
#include "Env.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

struct InputState
{
    bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
    GLfloat lastX = 400, lastY = 300; //исходное положение мыши
    bool firstMouse = true;
    bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
    bool capturedMouseJustNow = false;
} static Input;

static int prevSpaceState = GLFW_RELEASE;
int time_wait = 2;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_E:
            if (action == GLFW_RELEASE and prevSpaceState == GLFW_PRESS) {
                Input.keys[key] = true;
            } else {
                Input.keys[key] = false;
            }
            prevSpaceState = action;
            break;
        case GLFW_KEY_1:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case GLFW_KEY_2:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            if (action == GLFW_PRESS)
                Input.keys[key] = true;
            else if (action == GLFW_RELEASE)
                Input.keys[key] = false;
    }
}

MovementDir processPlayerMovement(Player &player, Env &env)
{
    if (Input.keys[GLFW_KEY_E]) {
        Input.keys[GLFW_KEY_E] = false;
        env.checkPosDoor(player);
        return MovementDir::IDLE;
    } else {
        MovementDir move;
        if (Input.keys[GLFW_KEY_W]) {
            move = MovementDir::UP;
        } else if (Input.keys[GLFW_KEY_S]) {
            move = MovementDir::DOWN;
        } else if (Input.keys[GLFW_KEY_A]) {
            move = MovementDir::LEFT;
        } else if (Input.keys[GLFW_KEY_D]) {
            move = MovementDir::RIGHT;
        } else {
            return MovementDir::IDLE;
        }
        auto check = env.checkPosMove(player, move);
        if (check == CondDir::WELL_TRUE) {
            player.ProcessInput(move);
        }
        return move;

    }
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        Input.captureMouse = !Input.captureMouse;

    if (Input.captureMouse)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input.capturedMouseJustNow = true;
    }
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    if (Input.firstMouse)
    {
        Input.lastX = float(xpos);
        Input.lastY = float(ypos);
        Input.firstMouse = false;
    }

    GLfloat xoffset = float(xpos) - Input.lastX;
    GLfloat yoffset = Input.lastY - float(ypos);

    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    // ...
}


int initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "Controls: "<< std::endl;
    std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
    std::cout << "W, A, S, D - movement  "<< std::endl;
    std::cout << "E - interact  "<< std::endl;
    std::cout << "press ESC to exit" << std::endl;

    return 0;
}

int main(int argc, char** argv)
{
    if(!glfwInit())
        return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CGRL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback        (window, OnKeyboardPressed);
    glfwSetCursorPosCallback  (window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetScrollCallback     (window, OnMouseScroll);

    if(initGL() != 0)
        return -1;

    //Reset any OpenGL errors which could be present for some reason
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR)
        gl_error = glGetError();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

    Point starting_env{0, 0};
    Env env{starting_env};

    //game loop
    while (!glfwWindowShouldClose(window) and !env.GameCompleted()) {
        Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

        env.DrawBackground(screenBuffer);
        auto starting_pos = env.ProcessResources();
        if (!env.haveAdd()) {
            env.GetBloomResources();
        }

        Image screenBufferStatic(screenBuffer);
        Player player{starting_pos};
        while (!glfwWindowShouldClose(window)) {

            GLfloat currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            glfwPollEvents();

            env.DrawResources(screenBuffer, screenBufferStatic);
            env.ApplyBloom(screenBuffer, screenBufferStatic);
            Image screenBufferHero(screenBufferStatic);
            auto move = processPlayerMovement(player, env);
            if (env.NotValidDoors()) {
                move = MovementDir::IDLE;
                player.Stop();
            }
            player.Draw(screenBufferStatic, screenBufferHero, move);
            env.ProcessEnemies(screenBufferStatic, screenBufferHero, player);
            Image screenBufferAll(screenBufferHero);
            env.DrawDoors(screenBufferHero, screenBufferAll);
            if (player.isAlive()) {
                if (env.isRendered()) {
                    env.ApplyHaze(screenBufferHero, screenBufferAll);
                    // if (env.GetLevel() == levelCount) {
                    //    env.ApplyHazeFull(screenBufferHero, screenBufferAll);
                    // } else {
                    //     env.ApplyHaze(screenBufferHero, screenBufferAll);
                    // }
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
                    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBufferAll.Data()); GL_CHECK_ERRORS;
                    glfwSwapBuffers(window);
                } else {
                    for (uint8_t coef = 0; coef <= COEF_NORM and !glfwWindowShouldClose(window); coef++) {
                        Image screenBufferTrans(screenBufferAll);
                        env.ApplyCoef(screenBufferTrans, coef);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
                        glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBufferTrans.Data()); GL_CHECK_ERRORS;
                        glfwSwapBuffers(window);
                    }
                    env.Rendered();
                }
            } else {
                Image screenBufferTransTmp(screenBufferAll);
                env.ResetLevel();
                env.DrawReset(screenBufferAll, screenBufferTransTmp);
                for (uint8_t coef = COEF_NORM; coef > 0 and !glfwWindowShouldClose(window); coef--) {
                    Image screenBufferTrans(screenBufferTransTmp);
                    env.ApplyCoef(screenBufferTrans, coef);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
                    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBufferTrans.Data()); GL_CHECK_ERRORS;
                    glfwSwapBuffers(window);
                }
                break;
            }
            if (env.LevelCompleted()) {
                env.NextLevel();
                if (env.GameCompleted()) {
                    Image screenBufferTransTmp(screenBufferAll);
                    env.DrawGG(screenBufferAll, screenBufferTransTmp);
                    for (uint8_t coef = 3*COEF_NORM; coef > 0 and !glfwWindowShouldClose(window); coef--) {
                        Image screenBufferTrans(screenBufferTransTmp);
                        env.ApplyCoef(screenBufferTrans, coef, 3*COEF_NORM);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
                        glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBufferTrans.Data()); GL_CHECK_ERRORS;
                        glfwSwapBuffers(window);
                    }
                    break;
                }
                Image screenBufferTransTmp(screenBufferAll);
                env.DrawNext(screenBufferAll, screenBufferTransTmp);
                for (uint8_t coef = COEF_NORM; coef > 0 and !glfwWindowShouldClose(window); coef--) {
                    Image screenBufferTrans(screenBufferTransTmp);
                    env.ApplyCoef(screenBufferTrans, coef);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
                    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBufferTrans.Data()); GL_CHECK_ERRORS;
                    glfwSwapBuffers(window);
                }
                break;
            }
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
