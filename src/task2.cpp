#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cxxabi.h>
#include <sched.h>

#include "glfw_window.hpp"
#include "model/model_manager_task2.hpp"
#include "input/input_manager_task2.hpp"
#include "render/render_manager.hpp"
#include "camera/free_camera.hpp"

int main()
{
    int width = 800, height = 600;
    GLFWwindow *window = CGTask::make_window(width, height);
    try
    {
        CGTask::camera::free_camera camera(glm::vec3(0, 300.0f, 100.0f), glm::vec3(0, -3.0f, -1.0f), width, height);
        CGTask::model::model_manager_task2 model(camera);
        CGTask::render::render_manager render(camera, model);
        CGTask::input::input_manager_task2 input(window, render, camera, model);
        CGTask::set_callback(window, camera, render, input);
        while (!glfwWindowShouldClose(window))
        {
            input.process_input(window);
            model.update();
            render.render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    catch(std::exception const &e)
    {
        std::cout << "Caught exception of type '" << 
            abi::__cxa_demangle(typeid(e).name(), nullptr, nullptr, nullptr) << "' in main.\n" <<
            "what(): `" << e.what() << "`" << std::endl;
    }
    glfwTerminate();
    return 0;
}
