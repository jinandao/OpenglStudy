#version 330

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 TexCoord;

//uniform mat4 gWVP;

out vec2 TexCoord0;

void main()
{
    //gl_Position = gWVP * vec4(Position, 1.0);
	//gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, 0.5*Position.z, 1.0);
	gl_Position = vec4(Position,1.0,1.0);
    TexCoord0 = TexCoord;
}
