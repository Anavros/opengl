
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

typedef struct Attributes Attributes;
struct Attributes {
    float x, y;
};

const Attributes vertices[3] =
    { {-1.0f, -1.0f }
    , { 1.0f, -1.0f }
    , { 1.0f,  1.0f }
};


void run(GLFWwindow* window, GLuint program) {
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        //float ratio = w / (float) h;
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


GLuint gen_buffer(Attributes* attrs) {
    GLuint buffer;
    glGenBuffer(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(attrs), attrs, GL_STATIC_DRAW);
    return buffer;
}


GLuint gen_shader(const char *text, int type) {
    // 'type' should be either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &text, NULL);
    // TODO: include error checking.
    glCompileShader(shader);
    // Remember that GL objects are just uint identifiers.
    // Everything stays on the graphics card.
    // Returning the 'shader' is just returning its handle.
    return shader;
}


GLuint gen_program(GLuint vs, GLuint fs) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    return program;
}


int main(void) {
    glfwSetErrorCallback(error_callback);

    int init_success = glfwInit();
    if (!init_success) { return 1; }

    GLFWwindow* window = glfwCreateWindow(300, 200, "GLFW3!", NULL, NULL);
    if (!window) { return 1; }

    glfwMakeContextCurrent(window);

    //GLuint attributes, vs, fs, program;
    GLuint vs = gen_shader(load(V_SHADER), GL_VERTEX_SHADER);
    GLuint fs = gen_shader(load(F_SHADER), GL_FRAGMENT_SHADER);
    GLuint program = gen_program(vs, fs);


    run(window, program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
