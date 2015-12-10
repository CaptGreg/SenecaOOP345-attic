//[VERTEX SHADER]
#version 330
 
in vec3 InVertex;
in vec3 InNormal;
smooth out vec3 LightVector0;
smooth out vec3 EyeNormal;
uniform mat4 ProjectionModelviewMatrix;
 
void main()
{
  gl_Position = ProjectionModelviewMatrix * vec4(InVertex, 1.0);

  LightVector0 = vec3(1.0, 1.0, 1.0);
  EyeNormal = InNormal;
}
