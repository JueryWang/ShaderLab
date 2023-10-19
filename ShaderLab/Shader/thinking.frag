#define M_PI 3.1415926

#define MAT_BODY 1.0
#define MAT_FACE 2.0
#define MAT_HAND 3.0
#define MAT_BROW 4.0

#define SUN smoothstep(-0.5, 0.5, sunDir.y)
#define MOON smoothstep(-0.2, -0.5, sunDir.y)

float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec2 x) {
    vec2 i = floor(x), f = fract(x);

	float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(vec2 p) {
	const mat2 m2 = mat2(0.8, -0.6, 0.6, 0.8);
    
    float f = 0.5000 * noise(p); p = m2 * p * 2.02;
    f += 0.2500 * noise(p); p = m2 * p * 2.03;
    f += 0.1250 * noise(p); p = m2 * p * 2.01;
    f += 0.0625 * noise(p);
    return f;
}

float sdSphere(vec3 p, float s)
{
    return length(p) - s;
}

float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
    vec3 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}

vec2 opU(vec2 d1, vec2 d2)
{
	return (d1.x<d2.x) ? d1 : d2;
}

vec2 opS( vec2 d1, vec2 d2 )
{ 
    return (-d1.x>d2.x) ? vec2(-d1.x, d1.y): d2;
}

vec2 opSU( vec2 d1, vec2 d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2.x-d1.x)/k, 0.0, 1.0 );
    return vec2(mix( d2.x, d1.x, h ) - k*h*(1.0-h), d1.y); }

mat2 rot( float th ){ vec2 a = sin(vec2(1.5707963, 0) + th); return mat2(a, -a.y, a.x); }

vec2 thinkingFace(vec3 p)
{
    vec2 face = vec2(sdSphere(p, 1.0), MAT_BODY);
    
    vec3 q = p;
    q.x = abs(q.x);
    q.xz *= rot(-.3);
    q.yz *= rot(-0.25 + 0.05 * step(0.0, p.x));
    q.y *= 0.8;q.z *= 2.0;q.z -= 2.0;
    vec2 eye =  vec2(sdSphere(q, .11) * 0.5, MAT_FACE);
    
    q = p;
    q.x = abs(q.x);
    q.xz *= rot(-.35);
    q.yz *= rot(-0.62 + 0.26 * step(0.0, p.x) + pow(abs(q.x), 1.7) * 0.5);
    q.z -= 1.0;
    vec2 brow = vec2(sdCapsule(q, vec3(0.2, 0.0, 0.0), vec3(-.2, 0.0, 0.0), .05) * 0.5, MAT_BROW);

    q = p;
    q.yz *= rot(0.2 + pow(abs(p.x), 1.8));
    q.xy *= rot(-0.25);
    q.z -= 1.0;
    vec2 mouth = vec2(sdCapsule(q, vec3(0.2, 0.0, 0.0), vec3(-.2, 0.0, 0.0), .045), MAT_BROW);
    
    p -= vec3(-.25, -.73, 1.0);
    p.xy *= rot(0.2);
    q = p;
    q = (q * vec3(1.2, 1.0, 2.0));
    q -= vec3(0.0, 0.01, 0.0);
    vec2 hand = vec2(sdSphere(q, .3) * 0.5, MAT_HAND);
    
    q = p;
    
    vec2 finger1 = vec2(sdCapsule(q - vec3(0.3, 0.2, 0.02), vec3(0.2, 0.0, 0.0), vec3(-.2, 0.0, 0.0), .07), MAT_HAND);
    vec2 finger2 = vec2(sdCapsule(q * vec3(1.2, 1.0, .8) - vec3(0.2, 0.06, 0.02), vec3(0.1, 0.0, 0.0), vec3(-.1, 0.0, 0.0), .08), MAT_HAND);
    vec2 finger3 = vec2(sdCapsule(q * vec3(1.2, 1.0, .8) - vec3(0.15, -0.08, 0.015), vec3(0.1, 0.0, 0.0), vec3(-.1, 0.0, 0.0), .08), MAT_HAND);
    vec2 finger4 = vec2(sdCapsule(q * vec3(1.2, 1.0, .9) - vec3(0.1, -0.2, -0.01), vec3(0.1, 0.0, 0.0), vec3(-.1, 0.0, 0.0), .08), MAT_HAND);
    
    p -= vec3(-0.1, 0.3, 0.0);
    q = p;
    q.x -= q.y * 0.7;

    vec2 finger5 = vec2(sdCapsule(p, vec3(0.0, -0.2, 0.0) - q, vec3(0.0, 0.2, 0.0), .1 - p.y * 0.15), MAT_HAND);
    vec2 finger = opU(finger1, opU(finger5, opSU(finger2, opSU(finger3, finger4, 0.035), 0.035)));
    
    hand = opSU(hand, finger, 0.02);
    
    vec2 d = opU(eye, face);
    d = opU(brow, d);
    d = opS(mouth, d);
    d = opU(hand, d);
    return d;
}

// Starfield01 by xaot88
// https://www.shadertoy.com/view/Md2SR3
float Noise2d( in vec2 x )
{
    float xhash = cos( x.x * 37.0 );
    float yhash = cos( x.y * 57.0 );
    return fract( 415.92653 * ( xhash + yhash ) );
}

// Convert Noise2d() into a "star field" by stomping everthing below fThreshhold to zero.
float NoisyStarField( in vec2 vSamplePos, float fThreshhold )
{
    float StarVal = Noise2d( vSamplePos );
    if ( StarVal >= fThreshhold )
        StarVal = pow( (StarVal - fThreshhold)/(1.0 - fThreshhold), 6.0 );
    else
        StarVal = 0.0;
    return StarVal;
}

// Stabilize NoisyStarField() by only sampling at integer values.
float StableStarField( in vec2 vSamplePos, float fThreshhold )
{
    // Linear interpolation between four samples.
    // Note: This approach has some visual artifacts.
    // There must be a better way to "anti alias" the star field.
    vSamplePos *= rot(iTime*.5);
    float fractX = fract( vSamplePos.x );
    float fractY = fract( vSamplePos.y );
    vec2 floorSample = floor( vSamplePos );    
    float v1 = NoisyStarField( floorSample, fThreshhold );
    float v2 = NoisyStarField( floorSample + vec2( 0.0, 1.0 ), fThreshhold );
    float v3 = NoisyStarField( floorSample + vec2( 1.0, 0.0 ), fThreshhold );
    float v4 = NoisyStarField( floorSample + vec2( 1.0, 1.0 ), fThreshhold );

    float StarVal =   v1 * ( 1.0 - fractX ) * ( 1.0 - fractY )
        			+ v2 * ( 1.0 - fractX ) * fractY
        			+ v3 * fractX * ( 1.0 - fractY )
        			+ v4 * fractX * fractY;
	return StarVal;
}

vec2 map(vec3 p)
{
    vec2 think = thinkingFace(p - vec3(-0., 3.0, 0.));
    return think;
}

float hash( float n )
{
    return fract(sin(n)*43758.5453);
}

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0 + 113.0*p.z;

    float res = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                        mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
                    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                        mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
    return res;
}

mat3 m = mat3( 0.00,  0.80,  0.60,
              -0.80,  0.36, -0.48,
              -0.60, -0.48,  0.64 );


float fbm( vec3 pos )
{
    float o = 0.;
    for ( int i=0; i < 5; i++ )
    {
    	o = o*2. + abs(noise(pos)*2.-1.);
        pos = m * pos * 2.;
    }
    return o / 40.0;
}

vec3 normal( in vec3 pos, float eps )
{
    vec2 e = vec2(1.0,-1.0)*0.5773*eps;
    float intensity = 0.02;
    float n1 = fbm(pos * 1.5 + e.xyy) * intensity;
    float n2 = fbm(pos * 1.5 + e.yyx) * intensity;
    float n3 = fbm(pos * 1.5 + e.yxy) * intensity;
    float n4 = fbm(pos * 1.5 + e.xxx) * intensity;
    return normalize( e.xyy*(map( pos + e.xyy ).x + n1) +
					  e.yyx*(map( pos + e.yyx ).x + n2) +
					  e.yxy*(map( pos + e.yxy ).x + n3) +
					  e.xxx*(map( pos + e.xxx ).x + n4) );
}

float shadow(in vec3 p, in vec3 l)
{
    float t = 0.15;
    float t_max = 20.0;
    
    float res = 1.0;
    for (int i = 0; i < 16; ++i)
    {
        if (t > t_max) break;
        
        float d = map(p + t*l).x;
        if (d < 0.01)
        {
            return 0.0;
        }
        t += d;
        res = min(res, 1.0 * d / t);
    }
    
    return res;
}

vec3 sunDir;
vec3 fakeSky(vec3 rd)
{   
    vec3 moning = mix(vec3(0.85,0.5,0.2)*1.1, vec3(0.2,0.5,0.85)*1.1, rd.y);
    vec3 noon = mix(vec3(0.2,0.5,0.85)*1.3, vec3(0.05,0.2,0.7), rd.y);
    vec3 night = mix(vec3(.5,0.2,0.05) * 2.0, vec3(0.3,0.1,0.5), clamp(pow(rd.y, 0.5), 0.0, 1.0));
    night = mix(night, vec3(0.0, 0.0, 0.0), MOON);
    
    // sky		
    vec3 col = noon;
    col = mix(moning, noon, SUN);
    col = mix(col, night, MOON);
    
    // stars
    col += vec3(StableStarField(rd.xy * 1100.0, 0.99)) * 0.3;

    // Clouds
    vec3 cloud = mix(mix(vec3(1.0, 0.95, 1.0), vec3(1.0, 0.2, 0.1), 1.0 - SUN), vec3(0.02), MOON);
    col = mix(col, cloud, 0.75 * step(0.0, rd.y) * smoothstep(0.4, 0.9, fbm(vec2(iTime, 0.0) + (rd.xz / rd.y) * 3.0)));
    
    return col * 3.0;
}

float ndfGGX(float NdotH, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (M_PI * denom * denom);
}

float gaSchlickG1(float theta, float k)
{
	return theta / (theta * (1.0 - k) + k);
}

float gaSchlickGGX(float NdotL, float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return gaSchlickG1(NdotL, k) * gaSchlickG1(NdotV, k);
}

vec3 fresnelSchlick_roughness(vec3 F0, float cosTheta, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 shade(vec3 pos, vec3 albedo, float metalness, float roughness, vec3 N, vec3 V, vec3 L, vec3 Lradiance)
{
	vec3 H = normalize(L + V);
	float NdotV = max(0.0, dot(N, V));
	float NdotL = max(0.0, dot(N, L));
	float NdotH = max(0.0, dot(N, H));
		
	vec3 F0 = mix(vec3(0.04), albedo, metalness);

	vec3 F  = fresnelSchlick_roughness(F0, max(0.0, dot(H, L)), roughness);
	float D = ndfGGX(NdotH, roughness);
	float G = gaSchlickGGX(NdotL, NdotV, roughness);

	vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

    float shadow = shadow(pos, L);
	vec3 diffuseBRDF = kd * albedo / M_PI;
	vec3 specularBRDF = (F * D * G) / max(0.0001, 4.0 * NdotL * NdotV);

	return (diffuseBRDF + specularBRDF) * Lradiance * NdotL * shadow;
}

// Unreal Engine Ambient BRDF Approx
// https://www.unrealengine.com/en-US/blog/physically-based-shading-on-mobile?lang=en-US
vec3 EnvBRDFApprox( vec3 SpecularColor, float Roughness, float NoV )
{
	const vec4 c0 = vec4( -1, -0.0275, -0.572, 0.022 );
	const vec4 c1 = vec4( 1, 0.0425, 1.04, -0.04 );
	vec4 r = Roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
	vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;
	return SpecularColor * AB.x + AB.y;
}

// Specular Occlusion
// https://media.contentapi.ea.com/content/dam/eacom/frostbite/files/course-notes-moving-frostbite-to-pbr-v2.pdf
float so(float NoV, float ao, float roughness) {
    return clamp(pow(NoV + ao, exp2(-16.0 * roughness - 1.0)) - 1.0 + ao, 0.0, 1.0);
}

vec3 calcAmbient(vec3 pos, vec3 albedo, float metalness, float roughness, vec3 N, vec3 V, float t)
{
	vec3 F0 = mix(vec3(0.04), albedo, metalness);
    vec3 F  = fresnelSchlick_roughness(F0, max(0.0, dot(N, V)), roughness);
    vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);
    
	float aoRange = t/40.0;
	float occlusion = max( 0.0, 1.0 - map( pos + N*aoRange ).x/aoRange );
	occlusion = min(exp2( -.8 * pow(occlusion, 2.0) ), 1.0);
    
    vec3 ambientColor = mix(vec3(0.2,0.5,0.85)*0.5, vec3(0.2,0.25,0.8) * 0.75, 0.5+0.5*N.y);
    ambientColor = mix(vec3(0.3, 0.15, 0.05), ambientColor, 1.0 - smoothstep(0.2, -0.5, sunDir.y));
    ambientColor = mix(vec3(0.03), ambientColor,  1.0 - smoothstep(-0.2, -0.5, sunDir.y));
    
    vec3 diffuseAmbient = kd * albedo * ambientColor * min(1.0, 0.75+0.5*N.y) * 3.0;
    vec3 R = reflect(-V, N);
    
    vec3 col = mix(fakeSky(R) * pow( 1.0-max(-R.y,0.0), 4.0), ambientColor, pow(roughness, 0.5));
    vec3 ref = EnvBRDFApprox(F0, roughness, max(dot(N, V), 0.0));
    vec3 specularAmbient = col * ref;

    diffuseAmbient *= occlusion;
    specularAmbient *= so(max(0.0, dot(N, V)), occlusion, roughness);

    return vec3(diffuseAmbient + specularAmbient);
}

vec3 materialize(vec3 p, vec3 ray, float depth, vec2 mat)
{
    vec3 col = vec3(0.0);
    vec3 nor = normal(p, 0.001);
    vec3 sky = fakeSky(ray);
    if (depth > 2000.0) {
        col = sky;
    } else {
        float checker = mod(floor(p.x) + floor(p.z), 2.0);
        float roughness, metalness;
    	if(mat.y == MAT_BODY) {
            col = pow(vec3(1.0, 204.0 / 255.0, 77.0 / 255.0), vec3(2.2)) * 0.6;
            roughness = 0.5;
            metalness = 0.0;
        } else if(mat.y == MAT_FACE) {
            col = pow(vec3(102.0 / 255.0, 69.0 / 255.0, 0.0), vec3(2.2)) * 0.6;
            roughness = 0.1;
            metalness = 0.0;
    	} else if (mat.y == MAT_HAND) {
        	col = pow(vec3(244.0 / 255.0, 144.0 / 255.0, 12.0 / 255.0), vec3(2.2)) * 0.7;
            roughness = 0.8;
            metalness = 0.0;
        } else if(mat.y == MAT_BROW) {
            col = pow(vec3(102.0 / 255.0, 69.0 / 255.0, 0.0), vec3(2.2)) * 0.8;
            roughness = 0.6;
            metalness = 0.0;
        }
        vec3 result = vec3(0.);
        result += shade(p, col, metalness, roughness, nor, -ray, normalize(sunDir), vec3(1.0, 0.98, 0.95) * 100.) * SUN;
        result += shade(p, col, metalness, roughness, nor, -ray, normalize(-sunDir), vec3(1.0, 0.98, 0.95) * 2.) * MOON;
        result += calcAmbient(p, col, metalness, roughness, nor, -ray, depth);
        col = result;
    }
    float fo = 1.0-exp(-0.0015*depth);
    col = mix( col, sky, fo );
    return col;
}

vec3 trace(vec3 p, vec3 ray)
{
    float t = 0.0;
    vec3 pos;
    vec2 mat;
    for (int i = 0; i < 100; i++) {
        pos = p + ray * t;
        mat = map(pos);
        if (mat.x < 0.00001) {
        	break;
        }
        t += mat.x;
    }
    return materialize(pos, ray, t, mat);
}

mat3 camera(vec3 ro, vec3 ta, float cr )
{
	vec3 cw = normalize(ta - ro);
	vec3 cp = vec3(sin(cr), cos(cr),0.);
	vec3 cu = normalize( cross(cw,cp) );
	vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

float luminance(vec3 col)
{
    return dot(vec3(0.298912, 0.586611, 0.114478), col);
}

vec3 reinhard(vec3 col, float exposure, float white) {
    col *= exposure;
    white *= exposure;
    float lum = luminance(col);
    return (col * (lum / (white * white) + 1.0) / (lum + 1.0));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 p = (fragCoord.xy * 2.0 - iResolution.xy) / min(iResolution.x, iResolution.y);
    float t = iTime * M_PI * 2.0 / 6.0 - M_PI * 0.5;
    sunDir = normalize(vec3(.5, sin(t), cos(t)));
    vec3 ro = vec3(-0.43 * 5.0, 1.3, 0.9 * 5.0);
    vec3 ta = vec3(2.4, 4.6, -0.3);
    mat3 c = camera(ro, ta, 0.0);
    vec3 ray = c * normalize(vec3(p, 3.5));
    vec3 col = trace(ro, ray);
    col = reinhard(col, .6, 30.0);
    col = pow(col, vec3(1.0/2.2));
    fragColor = vec4(col,1.0);
}