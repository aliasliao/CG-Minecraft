#version 400 core

in vec3 FragPos;
in vec3 TextureVec;
in vec3 Normal;

out vec4 FragColor;  // default output as gl_FragColor

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform sampler2DArray texes;

vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
	// texture color
	vec4 objRGBA = texture(texes, TextureVec);
	if (objRGBA.a == 0.0f)  // handle transparent
		discard;
	vec3 objectColor = vec3(objRGBA);

	//if (TextureVec.z == 0.0f)  // external model
		//objectColor = vec3(1.0f, 1.0f, 1.0f);

	// ambient
    //float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}