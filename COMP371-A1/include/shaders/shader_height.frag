#version 330 core
in vec3 pos;
out vec4 color;
vec3 col;
void main()
{
	//colour palette from http://www.color-hex.com/color-palette/1374
	if (pos.y < 10)
	{
		col = vec3(89.0,106.0,72.0);
	}
	else if (pos.y < 30)
	{
		col = vec3(33.0,37.0,40.0);
	}
	else if (pos.y <100)
	{
		col = vec3(153.0,151.0,136.0);
	}
	else if (pos.y <200)
	{
		col = vec3(201.0,193.0,182.0);
	}
	else if (pos.y < 256)
	{
		col = vec3(64.0,59.0,53.0);
	}
	color = vec4(col.r/255, col.g/255, col.b/255, 1.0f);
}

