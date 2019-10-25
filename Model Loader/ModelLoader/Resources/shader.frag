#version 450 core

out vec4 fColor;
in vec4 vColourFragment;

void main()
{	
    fColor = vColourFragment;
}
