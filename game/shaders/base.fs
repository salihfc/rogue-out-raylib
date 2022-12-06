#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 color_hint;
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

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
    color.rgb = mix(color.rgb, color_hint.rgb, 1.0 - color_hint.a);

    vec2 uv = fragTexCoord;
    float dist = max(abs(uv.x - 0.5), abs(uv.y - 0.5));
    color.rgb *= (dist > 0.3) ? 0.3 : 1.0;
    // color.rg = fragTexCoord.xy;
    // color.b = 0.0;
    finalColor = color;
}

