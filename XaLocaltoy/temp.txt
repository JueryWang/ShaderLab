float pi = atan(1.0)*4.0;
float tau = atan(1.0)*8.0;

float scale = 1.0 / 6.0;

float epsilon = 1e-3;
float infinity = 1e6;

#define N_POPS 6
#define TIME_BETWEEN_POPS 1.9
#define TIME_BETWEEN_POPS_RANDOM 0.6
#define N_CONFETTI 100
#define PI 3.1415926535
#define SQUARE_HARDNESS 10.0
#define GLOW_INTENSITY 0.8
#define HUE_VARIANCE 0.2

#define V_INITIAL 600.0
#define V_RANDOM 300.0
#define GRAVITY 17.81
#define TERMINAL_VELOCITY 60.0
#define TERMINAL_VELOCITY_RANDOM 10.0

#define GLOW_WHITENESS 0.30
#define SQUARE_WHITENESS 0.86

#define USE_CHEAP_ARCS


#define BORDER_COLOR vec3(0.05, 0.20, 1.00)

#define BRIGHTNESS 0.004
#define THICKNESS  0.003


bool ApproxEqual(float a, float b)
{
    return abs(a - b) <= epsilon;
}


float dfLine(vec2 start, vec2 end, vec2 uv)
{
	start *= scale;
	end *= scale;
    
	vec2 line = end - start;
	float frac = dot(uv - start,line) / dot(line,line);
	return distance(start + line * clamp(frac, 0.0, 1.0), uv);
}


float dfArc(vec2 origin, float start, float sweep, float radius, vec2 uv)
{
	origin *= scale;
	radius *= scale;  
	uv -= origin;
    
	uv *= mat2(cos(start), sin(start),-sin(start), cos(start));
	
    #ifdef USE_CHEAP_ARCS
        if(ApproxEqual(sweep, pi)) 
        {
            float d = abs(length(uv) - radius) + step(uv.y, 0.0) * infinity;
            d = min(d, min(length(uv - vec2(radius, 0)), length(uv + vec2(radius, 0))));
            return d;
        }
        else if(ApproxEqual(sweep, pi/2.0)) 
        {
            float d = abs(length(uv) - radius) + step(min(uv.x, uv.y), 0.0) * infinity;
            d = min(d, min(length(uv - vec2(0, radius)), length(uv - vec2(radius, 0))));
            return d;
        }
        else //Others
        {
            float offs = (sweep / 2.0 - pi);
            float ang = mod(atan(uv.y, uv.x) - offs, tau) + offs;
            ang = clamp(ang, min(0.0, sweep), max(0.0, sweep));

            return distance(radius * vec2(cos(ang), sin(ang)), uv); 
        }
    #else
        float offs = (sweep / 2.0 - pi);
        float ang = mod(atan(uv.y, uv.x) - offs, tau) + offs;
        ang = clamp(ang, min(0.0, sweep), max(0.0, sweep));

        return distance(radius * vec2(cos(ang), sin(ang)), uv);
	#endif
}

float dfLogo(vec2 uv)
{
	float dist = infinity;
    //Name row
    const float right_off_x = 0.5; 
    dist = min(dist, dfLine(vec2(4.9+right_off_x,3.500), vec2(4.9+right_off_x,2), uv));
    dist = min(dist, dfLine(vec2(4.9+right_off_x,2), vec2(5.9+right_off_x,2),uv));
    dist = min(dist, dfLine(vec2(6.5+right_off_x,2.4), vec2(7.322+right_off_x,2.4), uv));
    dist = min(dist, dfArc(vec2(6.9+right_off_x, 2.4),0, 1.7*PI, .422, uv));
    dist = min(dist, dfArc(vec2(8.4+right_off_x,2.4),0,PI,.422,uv));
    dist = min(dist, dfArc(vec2(8.4+right_off_x,2.4),0,-PI,.422,uv));
    //Up row
	dist = min(dist, dfLine(vec2(0.0,0.000), vec2(0.0,1.40), uv));
    dist = min(dist, dfLine(vec2(0.70,0.000), vec2(0.70,1.40), uv));
	dist = min(dist, dfLine(vec2(0.0,0.700), vec2(0.7,0.700), uv));
    dist = min(dist, dfLine(vec2(1.00,0.000), vec2(1.5,1.40), uv));
    dist = min(dist, dfLine(vec2(2.,0.000), vec2(1.5,1.40), uv));
    //dist = min(dist, dfLine(vec2(0.0,0.000), vec2(0.0,1.40), uv));
    dist = min(dist, dfLine(vec2(2.3,0.000), vec2(2.3,1.40), uv));
    dist = min(dist, dfArc(vec2(2.4,1.0),3.142, -4.142, .567, uv));
    dist = min(dist, dfArc(vec2(3.4,1.0),3.142, -4.142, .567, uv));
    dist = min(dist, dfLine(vec2(3.3,0.000), vec2(3.3,1.40), uv));
    dist = min(dist, dfLine(vec2(4.6,0.000), vec2(4.6,0.7), uv));
    dist = min(dist, dfLine(vec2(4.2,1.400), vec2(4.6,0.7), uv));
    dist = min(dist, dfLine(vec2(5.0,1.400), vec2(4.6,0.7), uv));
    //dist = min(dist, dfLine(vec2(0.0,0.000), vec2(0.0,1.40), uv));
    dist = min(dist, dfLine(vec2(1.25,0.750), vec2(1.75,0.75), uv));
    //Low row
    dist = min(dist, dfLine(vec2(1.5,-0.200), vec2(1.5,-2.0), uv));
    dist = min(dist, dfArc(vec2(1.6,-0.6),5.142, 3.142, .567, uv));
    dist = min(dist, dfArc(vec2(1.6,-1.6),5.142, 2.642, .567, uv));
    //dist = min(dist, dfArc(vec2(1.6,-1.6),5.142, 2.642, .567, uv));
    dist = min(dist, dfLine(vec2(1.25,0.750), vec2(1.75,0.75), uv));
    dist = min(dist, dfLine(vec2(2.7,-0.2), vec2(2.7,-2.0), uv));
    dist = min(dist, dfLine(vec2(3.2,-0.200), vec2(3.2,-2.00), uv));
    dist = min(dist, dfArc(vec2(3.2,-.6),3.142, -4.142, .567, uv));
    dist = min(dist, dfArc(vec2(3.2,-1.8),6.12, 1.942, .567, uv));
    dist = min(dist, dfLine(vec2(4.0,-0.200), vec2(5.,-0.20), uv));
    dist = min(dist, dfLine(vec2(4.5,-0.200), vec2(4.5,-2.00), uv));
    dist = min(dist, dfLine(vec2(5.3,-0.200), vec2(5.3,-2.0), uv));
    dist = min(dist, dfArc(vec2(5.3,-1.),4.62, 3.442, .967, uv));
    dist = min(dist, dfLine(vec2(6.30,-2.00), vec2(6.8,-0.20), uv));
    dist = min(dist, dfLine(vec2(7.3,-2.00), vec2(6.8,-0.20), uv));
    dist = min(dist, dfLine(vec2(6.55,-1.00), vec2(7.075,-1.0), uv));
    
    dist = min(dist, dfLine(vec2(7.6,-0.200), vec2(8.0,-1.0), uv));
    dist = min(dist, dfLine(vec2(8.0,-1.00), vec2(8.6,-0.2),uv));
    dist = min(dist, dfLine(vec2(8.0,-1.00), vec2(8.0,-2.0), uv));
    dist = min(dist, dfLine(vec2(9.2,-0.20), vec2(8.9,-1.6), uv));
    dist = min(dist, dfLine(vec2(8.9,-2.0), vec2(8.9,-2.0), uv));
	return dist;
}

float dfBorder(vec2 uv)
{
    float dist = infinity;
    
	
    
    return dist;
}


float trapezium(float x)
{
    
	return min(1.0, max(0.0, 1.0 - abs(-mod(x, 1.0) * 3.0 + 1.0)) * 2.0);
}

vec3 colFromHue(float hue)
{
	return vec3(trapezium(hue - 1.0/3.0), trapezium(hue), trapezium(hue + 1.0/3.0));
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float getPopRandom(float time, float random) {
    time = floor(time / TIME_BETWEEN_POPS);
    return rand(vec2(time, random));
}

float getPopTime(float time) {
    float pop_start_time = floor(time / TIME_BETWEEN_POPS) * TIME_BETWEEN_POPS
        + (getPopRandom(time, 2.2)) * TIME_BETWEEN_POPS_RANDOM;
    return iTime - pop_start_time;
}

float xposition(float time, float angle, float v_initial, float terminal_v) {
    float g = GRAVITY;
    float v = v_initial;
    float t = time;
    float sin_amp = 20.0 * (1.0 - exp(-pow(time / 7.0, 2.0)));
    float x_t = sin(time / 5.0) * sin_amp + time * 3.0;
    return v * terminal_v / g * (1.0 - exp(-g*t/terminal_v)) * cos(angle) + x_t;
}

float yposition(float time, float angle, float v_initial, float terminal_v) {
    float g = GRAVITY;
    float v = v_initial;
    float t = time;
    return v * terminal_v / g * (1.0 - exp(-g*t/terminal_v)) * sin(angle) - terminal_v * t;
}

float isInExtendedTriangle(vec2 b, vec2 a, vec2 c, vec2 p)
{
           
    vec2 v0 = c - a;
    vec2 v1 = b - a;
    vec2 v2 = p - a;

   
    float dot00 = dot(v0, v0);
    float dot01 = dot(v0, v1);
    float dot02 = dot(v0, v2);
    float dot11 = dot(v1, v1);
    float dot12 = dot(v1, v2);

    
    float denom = (dot00 * dot11 - dot01 * dot01);
    if (denom < 0.001) {
     	return 0.0;   
    }
    float invDenom = 1.0 / denom;
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    
    return clamp(u * SQUARE_HARDNESS, 0.0, 1.0) *
           clamp(v * SQUARE_HARDNESS, 0.0, 1.0);  
}

float isInQuad(vec2 a, vec2 b, vec2 c, vec2 d, vec2 p)
{
    return isInExtendedTriangle(a, b, c, p) * isInExtendedTriangle(c, d, a, p);
}

vec4 rotate(float phi, float theta, float psi) {
    float cosPhi = cos(phi), sinPhi = sin(phi);
    float cosTheta = cos(theta), sinTheta = sin(theta);
    float cosPsi = cos(psi), sinPsi = sin(psi);
    mat3 matrix;
    
    vec3 row0 = vec3(cosTheta * cosPsi, -cosTheta * sinPsi, sinTheta);
    
    vec3 row1 = vec3(cosPhi * sinPsi + sinPhi * sinTheta * cosPsi, 
                     cosPhi * cosPsi - sinPhi * sinTheta * sinPsi, 
                     -sinPhi * cosTheta);
    
    vec3 a = row0 - row1;
    
    vec3 b = row0 + row1;
    
    return vec4(a, b);
}

float isInRotatedQuad(vec4 offsets, vec2 center, vec2 p)
{
    return isInQuad(center + offsets.xy, 
                    center + offsets.zw, 
                    center - offsets.xy,
                    center - offsets.zw, p);
}


void main() 
{	
    float tf_text = max(epsilon, iTime - 1.7);
    
    
    vec2 scaledFragCoord = fragCoord / iResolution.xy * vec2(1000.0, 600.0);
	const float size = 6.0;
    const float max_square_dist = size * size * 128.0;
    float max_dist = sqrt(max_square_dist);
    const float confettiRotateTimeScale = 2.0;
    
    float t = iTime * confettiRotateTimeScale / 5.0 + 1.3;
    vec4 matrix1 = rotate(t * 8.0, sin(t) * 0.5, t / 4.0) * size;
    t = iTime * confettiRotateTimeScale / 5.1 + 92.2;
    vec4 matrix2 = rotate(t * 8.0, sin(t) * 0.5, t / 4.0) * size;
    t = iTime * confettiRotateTimeScale / 5.5 + 7.1;
    vec4 matrix3 = rotate(t * 8.0, sin(t) * 0.5, t / 4.0) * size;
    t = iTime * confettiRotateTimeScale / 4.3 + 1.0;
    vec4 matrix4 = rotate(t * 8.0, sin(t) * 0.5, t / 4.0) * size;
    
    vec4 col = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i=0; i<N_POPS; i++) {
        float sampleTime = iTime - float(i) * TIME_BETWEEN_POPS;
        float popTime = getPopTime(sampleTime);
        vec2 point = vec2(getPopRandom(sampleTime, 3.1) * 800.0, 0);
        float baseHue = getPopRandom(sampleTime, 3.5);
        
        if (abs(scaledFragCoord.x - point.x) > 300.0) {
            continue;
        }
        
        
        for (int j=0; j<N_CONFETTI; j++) {
            float angle = PI / 2.0 + PI / 4.0 * (getPopRandom(sampleTime, float(j)) - 0.5);
            float v_initial =  V_INITIAL + V_RANDOM * getPopRandom(sampleTime, float(j - 5));
            float terminal_v = TERMINAL_VELOCITY + TERMINAL_VELOCITY_RANDOM * (getPopRandom(sampleTime, float(j - 10)) - 0.5);
                
            float alterTime = popTime * 10.0;
            float x = xposition(alterTime, angle, v_initial, terminal_v) / 5.0;
            float y = yposition(alterTime, angle, v_initial, terminal_v) / 5.0;
            
            vec2 confettiLocation = vec2(point.x + x, y);
            vec2 delta = confettiLocation - scaledFragCoord;
            float dist = dot(delta, delta);
            if (dist > max_square_dist) {
             	continue;   
            }
            //dist = sqrt(dist);
            float glowIntensity = (clamp(1.0 - pow(dist / max_square_dist, 0.05), 0.0, 1.0) / 1.0) * GLOW_INTENSITY;
            float f = getPopRandom(sampleTime, float(j - 2));
            vec4 matrix;
            float matrixIndex = mod(f * 4.0, 4.0);
            if (matrixIndex < 1.0) {
                matrix = matrix1;
            } else if (matrixIndex < 2.0) {
                matrix = matrix2;
            } else if (matrixIndex < 3.0) {
                matrix = matrix3;
            } else {
                matrix = matrix4;
            }
            float squareIntensity = isInRotatedQuad(matrix,
                                  			        confettiLocation, 
                                                    scaledFragCoord);
            vec3 pastelColour = colFromHue(baseHue + getPopRandom(sampleTime, float(j - 15)) * HUE_VARIANCE);
            vec3 pastelGlowColour = glowIntensity * (pastelColour * (1.0 - GLOW_WHITENESS) + vec3(GLOW_WHITENESS, GLOW_WHITENESS, GLOW_WHITENESS));
            vec3 pastelSqColour = squareIntensity * (pastelColour * (1.0 - SQUARE_WHITENESS) + vec3(SQUARE_WHITENESS, SQUARE_WHITENESS, SQUARE_WHITENESS));
            col += vec4(pastelGlowColour + pastelSqColour, 0.0);
        }
    }
    float bright_confetti = BRIGHTNESS * min(1.0, 1.0 - sin(tf_text	* pi * 50.0) / (tf_text * pi * 1.3));
    col.a = 1.0;
    col.z*= bright_confetti;
    
    vec3 TEXT_COLOR;
    TEXT_COLOR = vec3(abs(sin(iTime)), 0.4, abs(cos(iTime)));
	vec2 aspect = iResolution.xy / iResolution.y;
	vec2 uv = fragCoord.xy / iResolution.y - aspect/2.0;
	
    vec2 offs = vec2(5.0, 1.5) * scale/2.0;
    
    float dist = 0.0;
    float shade = 0.0;
    vec3 color = vec3(0);
    
    float bright_text = BRIGHTNESS * min(1.0, 1.0 - sin(tf_text * pi * 50.0) / (tf_text * pi * 1.3));
    
    float tf_bord = max(epsilon, iTime - 0.5);
    float bright_bord = BRIGHTNESS * min(1.0, 1.0 - sin(tf_bord * pi * 50.0) / (tf_bord * pi * 1.3));
    
    dist = dfLogo(uv + offs);
	
	shade = bright_text / max(epsilon, dist - THICKNESS);
	
	color += TEXT_COLOR * shade;
    
    dist = dfBorder(uv + offs);
	
	shade = bright_bord / max(epsilon, dist - THICKNESS);
	
	color += BORDER_COLOR * shade;
    color+= col.xyz;
	fragColor = vec4(color, 1.0)  ;
}