#version 330


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

layout (location = 0) in vec2 aPos;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 controlPoints;
uniform float step;

float bernstein(int n, int k, float t);
float getZ(vec2 pos);
vec3 getNormal(vec2 pos);
vec3 getTangentX(vec2 pos);
vec3 getTangentY(vec2 pos);

vec3 calcColorWithLight(vec3 fragPos, vec3 normal, vec2 texCoord, vec3 viewPos);
vec3 addFog(vec3 color, float distanceFromCamera);

out vec3 GourandColour;
flat out vec3 FlatColour;

void main()
{
    TexCoords = aPos;    
    Normal = mat3(transpose(inverse(view * model))) * getNormal(aPos);
    FragPos = vec3(view * model * vec4(aPos, getZ(aPos), 1.0));
    gl_Position = projection * vec4(FragPos, 1.0);
    GourandColour = calcColorWithLight(FragPos, Normal, TexCoords, viewPos);
    FlatColour = GourandColour;
}

float bernstein(int n, int k, float t)
{
    float result = 0.0;
    if(n == 3)
    {
        if(k == 0 || k == 3) result = 1.0;
        else if(k == 1 || k == 2) result = 3.0;
    }
    else if(n == 2)
    {
        if(k == 0 || k == 2) result = 1.0;
        else if(k == 1) result = 2.0;
    }
    for(int i = 0; i <= n; i++)
    {
        if(i < k || k == n) result *= t;
        else result *= (1.0 - t);
    }
    return result;
}

float getZ(vec2 pos)
{
    float z = 0.0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            z += controlPoints[i][j] * bernstein(3, i, pos.x) * bernstein(3, j, pos.y);
        }
    }
    return z;
}

vec3 getNormal(vec2 pos)
{
    return cross(getTangentX(pos), getTangentY(pos));
}

vec3 getTangentX(vec2 pos)
{
    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j <= 3; j++)
        {
            float coeff = bernstein(2, i, pos.x) * bernstein(3, j, pos.y);
            result.y += coeff;
            result.z += coeff * (controlPoints[i + 1][j] - controlPoints[i][j]);
        }
    }
    result.y *= step;
    return 3.0 * result;
}

vec3 getTangentY(vec2 pos)
{
    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i <= 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            float coeff = bernstein(3, i, pos.x) * bernstein(2, j, pos.y);
            result.x += coeff;
            result.z += coeff * (controlPoints[i][j + 1] - controlPoints[i][j]);
        }
    }
    result.x *= step;
    return 3.0 * result;
}