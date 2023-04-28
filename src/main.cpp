#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "Exception/Exception.h"
#include "RenderEngine/ShaderProgram.h"
#include "RenderEngine/Texture2D.h"
#include "RenderEngine/Sprite.h"
#include "ResourceManager/ResourceManager.h"

#include "Game/Game.h"
#include "Game/Ranges.h"

const glm::ivec2 windowSize(1200, 640);

class App {
public:
    static void init(char** argv) {
        if (! glfwInit()) {
            std::cout << "GLFW not initialized" << std::endl;
            throw -1;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
        _window = glfwCreateWindow(windowSize.x, windowSize.y, "Sweeper", nullptr, nullptr);
        if (! _window) {
            std::cerr << "Window not initialized" << std::endl;
            glfwTerminate();
            throw -2;
        }
        glfwSetKeyCallback(_window, keyCallback);
        glfwSetMouseButtonCallback(_window, mouseCallback);
        glfwMakeContextCurrent(_window);
        if (! gladLoadGL()) {
            std::cerr << "Can't load GLAD" << std::endl;
            glfwTerminate();
            throw -3;
        }
        glClearColor(1, 1, 0 , 1);
        ResourceManager& resourceManager = ResourceManager::instance();
        resourceManager.setExecutablePath(argv[0]);
        resourceManager.loadJSONResources("res/resources.json");
        auto defaultShaderProgram = resourceManager.getShaderProgram("spriteShader");
        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(windowSize.x),
                                                0.f, static_cast<float>(windowSize.y),
                                                -100.f, 100.f);
        defaultShaderProgram->setUniform("projectionMat", projectionMatrix);
        _game = std::make_shared<Game::Game>(30, 16, 99);
    }

    static void start() {
        _game->start();
        while (!glfwWindowShouldClose(_window)) {
            glClear(GL_COLOR_BUFFER_BIT);
            _game->render();
            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
    }

    static void keyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(pWindow, GL_TRUE);
        }
    }

    static void mouseCallback(GLFWwindow *pWindow, int button, int state, int mode) {
        double x;
        double y;
        glfwGetCursorPos(_window, &x, &y);
    #define IMAGE_SIZE 40
        if (button == GLFW_MOUSE_BUTTON_LEFT and state == GLFW_PRESS) {
            _game->pressLeftButton({(int)x / IMAGE_SIZE, 15 - (int)y / IMAGE_SIZE});
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT and state == GLFW_PRESS) {
            _game->pressRightButton({(int)x / IMAGE_SIZE, 15 - (int)y / IMAGE_SIZE});
        } else if (button == GLFW_MOUSE_BUTTON_MIDDLE and state == GLFW_PRESS) {
            _game->start();
        }
    }
private:
    static std::shared_ptr<Game::Game> _game;
    static GLFWwindow* _window;
};

GLFWwindow* App::_window = nullptr;
std::shared_ptr<Game::Game> App::_game = nullptr;

int main(int argc, char** argv) {
    try {
        App::init(argv);
        App::start();
    } catch (Exception::Exception& ex) {
         std::cout << ex.what() << std::endl;
    } catch (int a) {
        std::cout << a << '\n';
    }
    glfwTerminate();
    return 0;
}