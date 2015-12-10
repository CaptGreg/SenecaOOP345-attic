//[VERTEX SHADER]
#version 330
 
in vec3 InVertex;
in vec4 InColor;
smooth out vec4 Color;
uniform mat4 ProjectionModelviewMatrix;
 
void main()
{
  gl_Position = ProjectionModelviewMatrix * vec4(InVertex, 1.0);
  Color = InColor;
}
