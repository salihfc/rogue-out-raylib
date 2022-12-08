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

uniform float border_thickness;
uniform vec4 border_color;
// uniform vec2 gi_direction;

// Output fragment color
out vec4 finalColor;
// Light uniforms
uniform int light_count;
uniform vec2 light_pos[16];
uniform float light_intensity[16];

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
		color = color_hint;
		vec2 uv = fragTexCoord;
    vec2 fragCoord = vec2(uv.x * body_size.x, uv.y * body_size.y);

		float total_cont = 0.0;
		float intensity_factor = 100.1;
    for (int i = 0; i < light_count; i++)
    {
        float distance_to_light = dist(fragCoord, light_pos[i]);
				float light_denom = pow(distance_to_light, 4); 
				// pow(distance_to_light, 1.75);
        float cont = light_intensity[i] / (light_denom);
        total_cont += cont;
    }

		total_cont *= intensity_factor;
		color.rgb += min(total_cont, 0.1);

    finalColor = color;
}

