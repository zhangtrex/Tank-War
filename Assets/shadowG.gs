#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform vec3 lightPos;

uniform mat4 Perspective;

float EPSILON = 0.00;

bool faceLight( vec3 a, vec3 b, vec3 c, float delta)
{
    vec3 n = normalize(cross( b - a, c - a ));
    vec3 la = normalize(lightPos - a);
    vec3 lb = normalize(lightPos - b);
    vec3 lc = normalize(lightPos - c);
    return dot(n, la) > delta ;
    //|| dot(n, lb) >  -0.2 || dot(n, lc) >  0.2
}

void drawVolume(vec3 a, vec3 b)
{

	vec3 deriation = normalize(a-b);
    vec3 lightDir = normalize(a - lightPos);
	gl_Position = Perspective * vec4((a + lightDir * EPSILON), 1.0);
	EmitVertex();
	gl_Position = Perspective * vec4(a+lightDir*5, 0.0);
    EmitVertex();

	lightDir = normalize(b - lightPos);
    gl_Position = Perspective * vec4((b + lightDir * EPSILON), 1.0);
    EmitVertex();
	gl_Position = Perspective * vec4(b+lightDir*5, 0.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
	vec3 lightDir;
	vec3 vPos[3];
	for(int i=0; i < 3; i++) {
		vPos[i] =  gl_in[i].gl_Position.xyz;
	}

	if (!faceLight(vPos[0], vPos[2], vPos[1], -0.1)) return;

	for (int i = 0; i < 3; i++) {

		int start = i;
		int end = (i+1) % 3; 

		drawVolume(vPos[start], vPos[end]);
	} 

	if (faceLight(vPos[0], vPos[2], vPos[1],0.4)) {
	    for (int i = 0; i < 3; i++) {
			lightDir = normalize(vPos[i] - lightPos);
			gl_Position = Perspective * vec4((vPos[i] + lightDir * EPSILON), 1.0);
			EmitVertex();
		}
		EndPrimitive();

		for (int i = 2; i >= 0; i--) {
			lightDir = normalize(vPos[i] - lightPos);
			gl_Position = Perspective * vec4((vPos[i] + lightDir * 5), 1.0);
			EmitVertex();
		}
		EndPrimitive();
	}
	
	
	

} 

void mian() {
	vec3 lightDir;
	vec3 vPos[3];
	for(int i=0; i < 3; i++) {
		vPos[i] =  gl_in[i].gl_Position.xyz;
	}

	if (!faceLight(vPos[0], vPos[1], vPos[2],0.04)) return;
	
	for (int i = 0; i < 3; i++) {

		int start = i;
		int end = (i+1) % 3; 

		drawVolume(vPos[start], vPos[end]);
	} 

	
	

}
