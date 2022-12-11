#version 330

float sm(float t)
{
    return (3.0 * t * t) - (2.0 * t * t * t);
}

float clamp(float v, float mn, float mx)
{
    return max(mn, min(mx, v));
}

float sm_w(float start, float end, float value)
{
    return clamp(((value-start) / (end-start)), 0.0, 1.0);
}

float smoothstep(float start, float end, float value)
{
    return start + (end - start) * sm_w(start, end, value);
}

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

float dist(vec2 a, vec2 b)
{
    return length(diff(a, b));
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

float round_corner(vec2 frag, vec2 texture_size, float corner_dist)
{
    float r = corner_dist;

    float dist_to_topleft = length(frag);
    if (dist_to_topleft < r)
    {
        float dist_to_inner = dist(frag, vec2(r, r));
        return 1.0 - float(dist_to_inner > r);
    }

    float dist_to_topright = dist(frag, vec2(texture_size.x, 0.0));
    if (dist_to_topright < r)
    {
        float dist_to_inner = dist(frag, vec2(texture_size.x - r, r));
        return 1.0 - float(dist_to_inner > r);
    }

    float dist_to_botleft = dist(frag, vec2(0.0, texture_size.y));
    if (dist_to_botleft < r)
    {
        float dist_to_inner = dist(frag, vec2(r, texture_size.y - r));
        return 1.0 - float(dist_to_inner > r);
    }

    float dist_to_botright = dist(frag, texture_size);
    if (dist_to_botright < r)
    {
        float dist_to_inner = dist(frag, vec2(texture_size.x - r, texture_size.y - r));
        return 1.0 - float(dist_to_inner > r);
    }

    return 1.0;
}


// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 color_hint;
uniform vec2 body_size;

// uniform vec2 gi_direction;

// Output fragment color
out vec4 finalColor;

float sdf_circle(vec2 uv, vec2 position, float radius)
{
    float d = 1e9;
    d = dist(uv, position) - radius;
    return d;
}

float sdf_box(vec2 uv, vec2 position, vec2 size)
{
    float d = 1e9;
    d = max(abs(uv.x - position.x) - size.x, abs(uv.y - position.y) - size.y);
    return d;
}

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
    vec2 uv = fragTexCoord;
    vec2 frag_coord = vec2(uv.x * body_size.x, uv.y * body_size.y);
    
    color = vec4(vec3(0.0), 1.0);
    float d = 1e9;

    d = min(d, sdf_box(frag_coord, vec2(400.0, 400.0), vec2(40.0, 80.0)));
    d = min(d, sdf_circle(frag_coord, vec2(400.0, 400.0), 60.0));


    color.r = 1.0 - max(0.0, d);;
    finalColor = color;
}

