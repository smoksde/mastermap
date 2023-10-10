#version 330 core

varying vec2 v_tex_coord;

uniform sampler2D u_texture;

void main()
{
    gl_FragColor = vec4(texture2D(u_texture, v_tex_coord).a) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}