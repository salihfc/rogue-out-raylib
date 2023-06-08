#version 330

float dot(vec2 a, vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

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

vec2 sum(vec2 a, vec2 b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

vec2 normalized(vec2 a)
{
    float l = length(a);
    a.x /= l;
    a.y /= l;
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
// const vec4 fortify_color = {0.4, .03, 0.9, 1.0};
const bool fortify_enabled = false;

in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 color_hint;
uniform vec2 body_size;
uniform vec2 object_top_left;

uniform float border_thickness;
uniform vec4 border_color;
uniform vec2 gi_direction;

// gameplay value
uniform int left_fortify;
uniform int right_fortify;
uniform int top_fortify;
uniform int bottom_fortify;

// Light uniforms
uniform int light_count;
uniform vec2 light_pos[16];
uniform float light_intensity[16];

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
    color.rgb = mix(color.rgb, color_hint.rgb, 1.0 - color_hint.a);

    vec2 uv = fragTexCoord;
    vec2 fragCoord = vec2(uv.x * body_size.x, uv.y * body_size.y);
    float fragDist = dist_to_edges(fragCoord, body_size);
    // color.rgb = (fragDist < border_thickness) ? border_color.rgb : color.rgb;
    float w = sm_w(border_thickness + 5.0, border_thickness, fragDist);
    color.rgb = mix(color.rgb, border_color.rgb, w);
    color.a = round_corner(fragCoord, body_size, border_thickness * 2.);

    float total_cont = 0.0;
    for (int i = 0; i < light_count; i++)
    {
        float distance_to_light = dist(sum(fragCoord, object_top_left), light_pos[i]);
        float cont = light_intensity[i] / (distance_to_light * distance_to_light);
        total_cont += cont;
    }

    color.rgb += min(0.2, total_cont / 1.0);

    // if left fortify > 0
    // override colors with fortification

    // color.b += float(left_fortify);
    // if (left_fortify > 0)
    // {
    //     color.b += float(left_fortify);
        // color.rgb += smoothstep(5.0*uv.x, 5.0*uv.x - 0.1, (1.0 - abs(y - 0.5) * 2.0));
    // }
    if(fortify_enabled)
    {
        vec3 fortify_color = vec3(0.4, 0.3, 0.9);
        if (left_fortify > 0)
        {
            float k = - 5.0 * uv.x + (1.0 - abs(uv.y - 0.5) * 2.0);
            if (k > 0)
                color.rgb = fortify_color; 
        }

        if (right_fortify > 0)
        {
            float k = - 5.0 * (1.0 - uv.x) + (1.0 - abs(uv.y - 0.5) * 2.0);
            if (k > 0)
                color.rgb = fortify_color; 
        }

        if (top_fortify > 0)
        {
            float k = - 3.0 * uv.y + (1.0 - abs(uv.x - 0.5) * 2.0);
            if (k > 0)
                color.rgb = fortify_color; 
        }

        if (bottom_fortify > 0)
        {
            float k = - 3.0 * (1.0 - uv.y) + (1.0 - abs(uv.x - 0.5) * 2.0);
            if (k > 0)
                color.rgb = fortify_color; 
        }
    }


    // y=0 -> 0.0> uv.x
    // y=0.5 -> 0.2> uv.x
    // y=1.0 -> 0.0> uv.x

    // 1 - abs(y - 0.5) * 2 > uv.x * 5

    // color.r = pow(color.r, 1.0 - total_cont);
    // color.g = pow(color.g, 1.0 - total_cont);
    // color.b = pow(color.b, 1.0 - total_cont);
    // color.r = pow(color.r, total_cont);
    // vec2 dir_from_center = normalized(diff(fragCoord, body_size / 2.0));
    // float k = (1.0 - dot(dir_from_center, gi_direction));
    // color.a *= k;
    finalColor = color;
}

