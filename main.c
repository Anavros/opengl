
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

GLfloat vertices[] =
    { -0.5f, -0.5f
    ,  0.5f, -0.5f
    ,  0.5f,  0.5f
    };


GLuint gen_buffer(void) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}


GLuint gen_vertex_array(GLuint buffer, GLfloat *verts) {
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);

    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return vertex_array;
}


GLuint gen_shader(const char *text, GLuint shader, const char *label) {
    glShaderSource(shader, 1, &text, NULL);
    glCompileShader(shader);

    // Error checking.
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        puts("Shader compilation error.");
        GLchar log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        fprintf(stderr, "%s: %s\n", label, log);
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
    return gen_shader(text, shader, "Vertex Shader");
}


GLuint gen_f_shader(const char *text) {
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    return gen_shader(text, shader, "Fragment Shader");
}


GLuint gen_program(GLuint vs, GLuint fs) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        fprintf(stderr, "%s\n", log);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}


// missing args
void run(GLFWwindow* window, GLuint program, GLuint vao) {
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        float ratio = w / (float) h;
        (void)ratio;
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
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

    // Create the rendering program.
    GLuint vs = gen_v_shader(load(V_SHADER));
    GLuint fs = gen_f_shader(load(F_SHADER));
    GLuint program = gen_program(vs, fs);

    // Delete shaders now that they've been linked.
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint buffer = gen_buffer();
    GLuint vao = gen_vertex_array(buffer, vertices); // global vertices


    run(window, program, vao);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
