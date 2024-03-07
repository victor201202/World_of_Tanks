#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 frag_normal;
flat in int frag_health;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    vec3 color = frag_color;
    if (frag_health == 2)
    {
        color.x = frag_color.x - 90.f / 255.f;
        color.y = frag_color.y - 90.f / 255.f;
        color.z = frag_color.z - 90.f / 255.f;
    }
    else if(frag_health == 1)
    {
        color.x = frag_color.x - 160.f / 255.f;
        color.y = frag_color.y - 160.f / 255.f;
        color.z = frag_color.z - 160.f / 255.f;
    }
    if (frag_color.x < 0)
        color.x = 0;
    if (frag_color.y < 0)
        color.y = 0;
    if (frag_color.z < 0)
        color.y = 0;

    out_color = vec4(color, 1);
}
