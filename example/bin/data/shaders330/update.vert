#version 330

in vec4  position;
in vec2  texcoord;

out vec2 texCoordVarying;

void main()
{
    texCoordVarying = texcoord;
	gl_Position = position;
}
