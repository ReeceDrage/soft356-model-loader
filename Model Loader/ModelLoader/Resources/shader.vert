
#version 400 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec4 vColour;
out vec4 vColourFragment;

void
main()
{
    gl_Position = vPosition;
	vColourFragment = vColour;
}
