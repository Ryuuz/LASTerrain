#version 330

in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

void main()
{
   float ambientIntensity = 0.1;
   vec3 ambient = lightColor * ambientIntensity;

   vec3 lightDir = normalize(lightPos - fragPos);
   vec3 aNormal = normalize(norm);
   float diff = max(dot(aNormal, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   float specularIntensity = 0.5;
   vec3 viewDir = normalize(-fragPos);
   vec3 reflectDir = reflect(-lightDir, aNormal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularIntensity * spec * lightColor;

   vec3 result = (ambient + diffuse + specular) * objectColor;
   fragColor = vec4(result, 1.0);
}

