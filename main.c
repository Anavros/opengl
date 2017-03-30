
#include <stdlib.h>
#include <stdio.h>

#include "GL/glew.h"
#include "GL/glut.h"
#include "GL/glu.h"


void update_display(void) {
    puts("Update.");
    glutSwapBuffers();
}


void idle_display(void) {
}


void key_press(unsigned char key, int x, int y) {
    puts("Pressed!");
    glutLeaveMainLoop();
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(300, 200);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("Hello There");
    glutDisplayFunc(&update_display);
    glutIdleFunc(&idle_display);
    glutKeyboardFunc(&key_press);

    glewInit();
    if(!GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 is not available.\n");
        return 1;
    }

    printf("OpenGL v%s\n", glGetString(GL_VERSION));

    glutMainLoop();
    return 0;
}
