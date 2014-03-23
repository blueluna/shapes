uniform mediump vec4 color;

void main(void)
{
	if (gl_FragCoord.z > 0) {
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else {
		gl_FragColor = color;
	}
}
