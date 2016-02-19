#ifndef DEFAULT_SHADERS_H
#define DEFAULT_SHADERS_H

const char* vertex_shader =
"#version 400\n"
"in vec3 vp;"
"void main () {"
"  gl_Position = vec4 (vp, 1.0);"
"}";

const char* fragment_shader =
"#version 400\n"
"out vec4 frag_colour;"
"void main () {"
"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
"}";

const char* vertex_shader_mvp =
"#version 400\n"
"layout(location = 0) in vec4 vp;"
"layout(location = 1) in vec4 vc;"
"out vec4 vert_colour;"
"uniform mat4 MVP;"
"void main () {"
"  gl_Position = MVP * vp;"
"  vert_colour = vc;"
"}";

const char* vertex_shader_normalvis =
"#version 400\n"
"layout(location = 0) in vec4 vp;"
"layout(location = 1) in vec4 vn;"
"out vec4 vert_colour;"
"uniform mat4 MVP;"
"void main () {"
"  gl_Position = MVP * vp;"
"  vert_colour = (1.0+vn) * 0.5;"
"}";


const char* fragment_shader_color =
"#version 400\n"
"out vec4 frag_colour;"
"in vec4 vert_colour;"
"void main () {"
"  frag_colour = vert_colour;"
"}";

#endif /* DEFAULT_SHADERS_H */
