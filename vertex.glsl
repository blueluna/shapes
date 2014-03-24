attribute highp vec2 vertex;
uniform highp mat4 matrix;

void main(void)
{
	vec4 pos = vec4(vertex, 0.0, 1.0);
	gl_Position = matrix * pos;
}
