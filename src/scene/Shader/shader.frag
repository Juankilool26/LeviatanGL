#version 420 core

out vec4 FragColor;

in vec2 txCoords;
in vec3 Normal;
in vec3 FragPos;

#define NR_DIRECTIONAL_LIGHTS 1
#define NR_POINT_LIGHTS 8
#define NR_SPOT_LIGHTS 1

struct Material
{
   vec3 ambient;
   sampler2D diffuse;
   vec3 specular;
   float shininess;
};

uniform int actual_dLights;

struct DirectionalLight
{
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   bool isOn;
};

uniform int actual_pLights;

struct PointLight
{
   vec3 position;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   bool isOn;
};

uniform int actual_sLights;

struct SpotLight
{
   vec3 position;
   vec3 direction;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float cutOff;
   float outerCutOff;

   bool isOn;
};

uniform Material material;
uniform DirectionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform vec3 camPos;

vec3 CalcDirectionLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDir);


void main()
{
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(camPos - FragPos);
   vec3 final = vec3(0.0);

   for(int i = 0;i < actual_dLights; i++)
   {
      if(directionalLights[i].isOn == true)
         final += CalcDirectionLight(directionalLights[i], norm, viewDir);
   }

   for(int j = 0; j < actual_pLights; j++)
   {
      if(pointLights[j].isOn == true)
         final += CalcPointLight(pointLights[j], norm, FragPos, viewDir);
   }

   for(int k = 0; k < actual_sLights; k++)
   {
      if(spotLights[k].isOn == true)
         final += CalcSpotLight(spotLights[k], norm, FragPos, viewDir);
   }

   FragColor = vec4(final, 1.0);
}

vec3 CalcDirectionLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
   vec3 lightDir = normalize(-light.direction);
   //diffuse component
   float diffus = max(dot(normal, lightDir), 0.0);
   //specular component
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   //final result
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, txCoords));
   vec3 diffuse = light.diffuse * diffus * vec3(texture(material.diffuse, txCoords));
   vec3 specular = light.specular * (spec * material.specular);

   return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   vec3 lightDir = normalize(light.position - fragPos);

   //diffuse component
   float diff = max(dot(normal, lightDir), 0.0);

   //specular component
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

   //attenuation component
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance +
                              light.quadratic * (distance * distance));
   //final result
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, txCoords));
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, txCoords));
   vec3 specular = light.specular * (spec * material.specular);

   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;

   return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPosition);

    //diffuse component
    float diff = max(dot(normal, lightDir), 0.0);

    //specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //attenuation component
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                              light.quadratic * (distance * distance));

    //spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //final result
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, txCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, txCoords));
    vec3 specular = light.specular * (spec * material.specular);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}