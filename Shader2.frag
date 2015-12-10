//[FRAGMENT SHADER]
#version 330
 
smooth in vec3 LightVector0;
smooth in vec3 EyeNormal;
out vec4 FragColor;
 
void main()
{
  vec3 eyeNormal;
  vec3 lightVector;
  float dotProduct;

  eyeNormal = normalize(EyeNormal);
  lightVector = normalize(LightVector0);

  dotProduct = dot(eyeNormal, lightVector);

  FragColor = vec4(dotProduct);
}
