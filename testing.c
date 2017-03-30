
#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>

#include "linmath.h"


#define V_SHADER "v.glsl"
#define F_SHADER "f.glsl"


char* load(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);

    long size = ftell(f);
    char* buffer = malloc(size+1);
    if (!buffer) {
        return NULL;
    }

    rewind(f);
    fread(buffer, size, 1, f);
    fclose(f);

    buffer[size] = '\0';
    return buffer;
}


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

    //GLuint attributes, vs, fs, program;
    char *vs = load(V_SHADER);
    printf("%s", vs);

    glfwMakeContextCurrent(window);
    run(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
