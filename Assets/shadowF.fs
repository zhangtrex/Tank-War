#version 330

out vec4 fragColor;

uniform sampler2D myTexture;

void main()
{
	vec3 color = vec3(0.0f, 1.0f, 0.0f);
    fragColor = vec4(color, 1.0f);
} 
