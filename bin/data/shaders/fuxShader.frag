#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect eyeTexDepth;
uniform sampler2DRect eyeTexColor;

void main (void)
{
	vec4 color = texture2DRect(eyeTexColor, gl_TexCoord[0].st);
	vec4 depth = texture2DRect(eyeTexDepth, gl_TexCoord[0].st);	
	
	if (depth.r > 0.0)
	{
		gl_FragColor = color;
	}else{
		gl_FragColor = vec4(0,0,0,0);
	}
}


