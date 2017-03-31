#version 120

attribute vec2 pos;

void main(void) {
    gl_Position = vec4(pos, 0.0, 1.0);
    //gl_PointSize = 100.0;
}
