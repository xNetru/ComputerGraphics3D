#version 330 core

struct Material {
    vec3 specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 2
#define NR_SPOT_LIGHTS 2

uniform bool isDay;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform Material material;
uniform sampler2D texture_diffuse1;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 texCoord);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord);

vec3 calcColorWithLight(vec3 fragPos, vec3 normal, vec2 texCoord, vec3 viewPos)
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0.0, 0.0, 0.0);
    if(isDay)
        result = calcDirLight(dirLight, norm, viewDir, texCoord);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir, texCoord);    
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += calcSpotLight(spotLights[i], norm, fragPos, viewDir, texCoord);
    return result;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 texCoord)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, texCoord));
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, texCoord));
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, texCoord));
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

const float gradient = 1.4;

uniform vec3 skyColor;
uniform float fogDensity;

vec3 addFog(vec3 color, float distanceFromCamera)
{
	float visibility = clamp(exp(-pow((distanceFromCamera * fogDensity), gradient)), 0.0, 1.0);
	return mix(skyColor, color, visibility);
}

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

in vec3 GourandColour;
flat in vec3 FlatColour;

vec3 calcColorWithLight(vec3 fragPos, vec3 normal, vec2 texCoord, vec3 viewPos);
vec3 addFog(vec3 color, float distanceFromCamera);

uniform int shadingMode;

void main()
{
    vec3 result;
    if(shadingMode == 0)
        result = calcColorWithLight(FragPos, Normal, TexCoords, viewPos);
    else if(shadingMode == 1)
        result = GourandColour;
    else
        result = FlatColour;
    result = addFog(result, length(FragPos - viewPos));
    FragColor = vec4(result, 1.0f);
}