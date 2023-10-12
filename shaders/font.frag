#version 330 core

varying vec2 v_tex_coord;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
    gl_FragColor = vec4(texture2D(u_texture, v_tex_coord).a) * u_color;
}