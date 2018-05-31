#version 330 core
in vec3 col;
out vec4 color;
void main()
{
	//colour palette from http://www.color-hex.com/color-palette/1374
	color = vec4(col.r/255, col.g/255, col.b/255, 1.0f);
}