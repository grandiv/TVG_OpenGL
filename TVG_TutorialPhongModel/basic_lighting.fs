#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos; // titik P yang berada di surface
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    vec3 ambient = vec3(0.3, 0.0, 0.0);

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(0.0, dot(norm, lightDir));  
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 32);
    vec3 specular = specularStrength * spec * lightColor;
        
    // result
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
} 