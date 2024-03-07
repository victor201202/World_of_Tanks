#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 3) in vec3 vertex_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform int health;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
flat out int frag_health;

void main()
{
    vec3 pos = v_position;
    frag_color = object_color;
    frag_normal = vertex_normal;
    frag_health = health;

    if (frag_health == 2)
        pos.y /= 1.2f;
    if (frag_health == 1)
        pos.y /= 1.6f;
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
