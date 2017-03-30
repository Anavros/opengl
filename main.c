
#include <stdlib.h>
#include <stdio.h>

// This is the magic right here.
#define GL_GLEXT_PROTOTYPES
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
    (void)error;
    fprintf(stderr, "Error: %s\n", description);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        puts("Pressed!");
        glfwSetWindowShouldClose(window, 1);
    }
}


void run(GLFWwindow* window, GLuint program, GLuint verts) {
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        float ratio = w / (float) h;
        (void)ratio;
        (void)verts;
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

GLfloat vertices[] =
    { -0.5f, -0.5f
    ,  0.5f, -0.5f
    ,  0.5f,  0.5f
    };


GLuint gen_buffer(GLfloat *verts) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    return buffer;
}


GLuint gen_shader(const char *text, GLuint shader) {
    glShaderSource(shader, 1, &text, NULL);
    // TODO: include error checking.
    glCompileShader(shader);

    // Error checking.
    GLint err = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
    if (err) {
        //GLsizei log_size = 0;
        GLchar log[512];
        puts("Shader compilation error.");
        /*
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
        printf("LOG SIZE: %d\n", log_size);
        log = malloc(sizeof(GLchar) * log_size);
        if (!log) {
            puts("Malloc failed! Panic!");
            return 0;
        }
        glGetShaderInfoLog(shader, log_size, NULL, log);
        */
        glGetShaderInfoLog(shader, 512, NULL, log);
        fprintf(stderr, "%s\n", log);
        glDeleteShader(shader);
        return 0;
    }

    // Remember that GL objects are just uint identifiers.
    // Everything stays on the graphics card.
    // Returning the 'shader' is just returning its handle.
    return shader;
}


GLuint gen_v_shader(const char *text) {
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    return gen_shader(text, shader);
}


GLuint gen_f_shader(const char *text) {
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    return gen_shader(text, shader);
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
    GLuint vs = gen_v_shader(load(V_SHADER));
    GLuint fs = gen_f_shader(load(F_SHADER));
    GLuint program = gen_program(vs, fs);
    GLuint verts = gen_buffer(vertices); // global vertices


    run(window, program, verts);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
