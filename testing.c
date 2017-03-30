
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        puts("Pressed!");
        glfwSetWindowShouldClose(window, 1);
    }
}


void run(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}


int main(void) {
    glfwSetErrorCallback(error_callback);

    int init_success = glfwInit();
    if (!init_success) { return 1; }

    GLFWwindow* window = glfwCreateWindow(300, 200, "GLFW3!", NULL, NULL);
    if (!window) { return 1; }
    glfwMakeContextCurrent(window);
    run(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
