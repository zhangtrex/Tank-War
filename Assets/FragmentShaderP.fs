#version 330

struct LightSource {
    vec3 position;
    vec3 rgbIntensity;
};

in VsOutFsIn {
	vec3 position_ES; // Eye-space position
	vec3 normal_ES;   // Eye-space normal
	LightSource light;
} fs_in;

in vec2 TexCoord;

out vec4 fragColour;

struct Material {
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform Material material;

// Ambient light intensity for each RGB component.
uniform vec3 ambientIntensity;

// opacity
uniform float opacity;

// if use texture mapping
uniform bool texture_mapping;

// the texture
uniform sampler2D myTexture;

// if draw ambient light only
uniform bool ambient_only;

uniform bool skybox;

vec3 phongModel(vec3 fragPosition, vec3 fragNormal, bool texMap, vec4 texColour) {
	LightSource light = fs_in.light;

    // Direction from fragment to light source.
    vec3 l = normalize(light.position - fragPosition);

    // Direction from fragment to viewer (origin - fragPosition).
    vec3 v = normalize(-fragPosition.xyz);

    float n_dot_l = max(dot(fragNormal, l), 0.0);

	vec3 diffuse;
    if (texMap) {
        diffuse = vec3(texColour) * n_dot_l+vec3(texColour)* ambientIntensity;
    }
    else {
	    diffuse = material.kd * n_dot_l;
    }

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
		// Halfway vector.
		vec3 h = normalize(v + l);
        float n_dot_h = max(dot(fragNormal, h), 0.0);

        specular = material.ks * pow(n_dot_h, material.shininess);
    }

    vec3 lightInt = light.rgbIntensity * (diffuse + specular);
    lightInt[0] = max(lightInt[0], 0.0f);
    lightInt[1] = max(lightInt[1], 0.0f);
    lightInt[2] = max(lightInt[2], 0.0f);

    return lightInt;
}

void main() {
    if (skybox) {
        fragColour = texture(myTexture, TexCoord);
    }
    else if (ambient_only) {
        if (true) {
            vec4 texColor = texture(myTexture, TexCoord);
            fragColour = texColor * vec4(ambientIntensity, opacity);
        }
        else { 
            fragColour = vec4(material.kd * ambientIntensity, opacity);
        }
    }
    else if (texture_mapping) {
        vec4 texColor = texture(myTexture, TexCoord);
        fragColour = vec4(phongModel(fs_in.position_ES, fs_in.normal_ES, true, texColor), opacity) +
        vec4(0.02,0.02,0.02,0.0);
    }
    else {
	    fragColour = vec4(phongModel(fs_in.position_ES, fs_in.normal_ES, false, vec4(0,0,0,0)), opacity) + vec4(0.04,0.04,0.04,0.0);
    }
}
