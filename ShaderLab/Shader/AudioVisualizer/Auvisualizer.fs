/**
 * Based on 
 * "Into The Dream" by Vanilla
 * https://vanillabeats.bandcamp.com/album/into-the-dream-3
 */
#version 450 core

out vec4 fragColor;
in vec2 fragCoord;

layout (binding = 0) uniform sampler2D iChannel0;

struct runtime_param
{
    float iTime;
};
uniform runtime_param runtime_data;
uniform ivec2 iResolution;

#define iTime runtime_data.iTime
#define PI 3.1415926
#define RADIUS 0.791
#define RINGS 20.0
#define SECTIONS 20.0
#define AA 4

struct specturmInfo_t
{
    float preval;
    float curval;
};

layout (std140) uniform droplets
{
    specturmInfo_t info[int(RINGS)];    
};
const int[] SECTION_IDS = int[](
     0,  1,  1,  1,  1,  0,  0,  0,
     0,  1,  3,  3,  3,  2,  0,  0,
     0,  1,  3,  5,  5,  5,  4,  0,
     0,  1,  3,  5,  7,  7,  7,  6,
     8,  1,  3,  5,  7,  9,  9,  9,
    11, 10,  3,  5,  7,  9, 11, 11,
    13, 13, 12,  5,  7,  9, 11, 13,
    15, 15, 15, 14, 16, 17, 18, 19,
    15, 15, 15, 14, 16, 17, 18, 19,
    15, 15, 15, 14, 16, 17, 18, 19
);


//https://iquilezles.org/articles/distfunctions2d
float sdpeakBar(in vec2 p,in vec2 a,in vec2 b,float th)
{
    float l = length(b-a);
    vec2  d = (b-a)/l;
    vec2  q = (p-(a+b)*0.5);
          q = mat2(d.x,-d.y,d.y,d.x)*q;
          q = abs(q)-vec2(l,th)*0.5;
    return length(max(q,0.0)) + min(max(q.x,q.y),0.0);
}

// https://www.shadertoy.com/view/MsS3Wc by iq
vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

vec3 color_for_section(int s) {
    float hue = 0.25 + 0.25 * sin(iTime*2.25) + (float(s) / 45.0);
    return hsb2rgb(vec3(hue, 0.65, 0.71));
}

vec3 color_for_panel(int s) {
    float hue = 0.25 + 0.25 * sin(iTime*0.25) + (float(s) / 45.0);
    return hsb2rgb(vec3(hue, 0.65, 0.71));
}

// https://www.shadertoy.com/view/XtfyRS by knarkowicz
float glyph_sdf(vec2 p, float pos) {
	p = abs(p.x - .5) > .5 || abs(p.y - .5) > .5 ? vec2(0.) : p;
	return 2. * (texture(iChannel0, p / 4. + fract(vec2(pos, 3. - floor(pos / 4.)) / 4.)).w);
}

// https://www.shadertoy.com/view/XtfyRS by knarkowicz
void background(inout vec3 color, vec2 p) {
	float glyphRatio = 2.0;
	vec2 glyphScale = 6. * vec2(1., glyphRatio);
    
	vec2 t = floor(p / glyphScale + 1e-6);

	uint v = 0u;
	v = t.y == 1. ? ( t.x < 4. ? 1229865302u : ( t.x < 8. ? 4279372u : 0u ) ) : v;
    v = t.x >= 0. && t.x < 16. ? v : 0u;
	float pos = float((v >> uint(8. * t.x)) & 255u);

	vec2 posInCell = (p - t * glyphScale) / glyphScale;
	posInCell.x = (posInCell.x - .5) / glyphRatio + .5;

	float sdf = glyph_sdf(posInCell, pos);
	if (pos != 0.) {
		color = mix(vec3(.2), color, smoothstep(-.04, +.04, sdf));
	}
    
    p.x += 42.0;
    
    t = floor(p / glyphScale + 1e-6);

	v = 0u;
    v = t.y == 0. ? ( t.x < 4. ? 1330925129u : ( t.x < 8. ? 1162368032u : ( t.x < 12. ? 1163019296u : 19777u ) ) ) : v;
    v = t.x >= 0. && t.x < 16. ? v : 0u;
	pos = float((v >> uint(8. * t.x)) & 255u);

	posInCell = (p - t * glyphScale) / glyphScale;
	posInCell.x = (posInCell.x - .5) / glyphRatio + .5;

	sdf = glyph_sdf(posInCell, pos);
	if (pos != 0.) {
        float m = clamp((p.x - 25.0) * 0.2, 0.0, 1.0);
        vec3 text_color = mix(color_for_panel(14), color_for_panel(19), m);
		color = mix(text_color, color, smoothstep(-.04, +.04, sdf));
	}
}

vec3 image(vec2 uv) {
    float a = atan(uv.y, uv.x);
    float r = length(uv);
    
    vec2 st = vec2(a, r);
    st.x = st.x * 0.5 / PI + 0.5;
    st.y = st.y * 1.0 / RADIUS*2.0;

    float s_id = SECTIONS-1.0 - floor(st.x * SECTIONS);
    float r_id = RINGS-1.0 - floor(st.y * RINGS);

    int id = SECTION_IDS[int(r_id*SECTIONS+s_id)];
    
    return id == 0 || st.y > 1.0 ? vec3(0.85) : color_for_section(id);
}

mat2 rot2d(float a) {
    return mat2(cos(a),-sin(a),sin(a),cos(a));
}

void main()
{
    vec3 color = vec3(0.0);  
    
    for (int m = 0; m < AA; ++m) {
    for (int n = 0; n < AA; ++n) {
        vec2 o = vec2(float(m), float(n)) / float(AA) - 0.5;
        vec2 uv = ((fragCoord + o) - 0.5 * iResolution.xy) / iResolution.y;
        vec3 col = image(uv);
        uv += vec2(0.5, 0.473);
        uv *= 320;
        background(col, uv);
        color += col;    
    }
    }
    color /= float(AA*AA);
    color.rgb = pow(color.rgb, vec3(1.0/2.2));
    fragColor = vec4(color, 1.0);
}