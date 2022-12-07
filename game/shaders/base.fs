#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 color_hint;
uniform vec2 body_size;

uniform float border_thickness;
uniform vec4 border_color;
// uniform vec2 gi_direction;

// Output fragment color
out vec4 finalColor;

float mix(float a, float b, float w)
{
    return a + (b - a) * w;
}

vec3 mix(vec3 a, vec3 b, float w)
{
    a.x = mix(a.x, b.x, w);
    a.y = mix(a.y, b.y, w);
    a.z = mix(a.z, b.z, w);
    return a;
}

vec2 diff(vec2 a, vec2 b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

float abs(float x)
{
    return (x > 0) ? x : -x;
}

float dist_to_edges(vec2 frag, vec2 text_size)
{
    return min(
        min(frag.x, text_size.x - frag.x),
        min(frag.y, text_size.y - frag.y)
    );
}

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
    color.rgb = mix(color.rgb, color_hint.rgb, 1.0 - color_hint.a);

    vec2 uv = fragTexCoord;
    vec2 fragCoord = vec2(uv.x * body_size.x, uv.y * body_size.y);
    float fragDist = dist_to_edges(fragCoord, body_size);
    color.rgb = (fragDist < border_thickness) ? border_color.rgb : color.rgb;
    finalColor = color;
}

