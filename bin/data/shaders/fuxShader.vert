uniform float eyeMultiply;
uniform sampler2DRect eyeTexDepth;
uniform sampler2DRect eyeTexColor;

void main()
{
	vec4 depth = texture2DRect(eyeTexDepth, gl_MultiTexCoord0.st);
	vec4 color = texture2DRect(eyeTexColor, gl_MultiTexCoord0.st);
	
	vec4 eyeDepthVertex = vec4(gl_Vertex);
	
	vec4 eyeColorVertex = vec4(gl_Vertex);
	
	eyeColorVertex.z = color.r * eyeMultiply;
	eyeDepthVertex.z = depth.r * eyeMultiply;
	
	gl_TexCoord[0]= gl_MultiTexCoord0;
	
	//if (eyeDepthVertex.z > 0.1)
	//{
		gl_Position =  gl_ModelViewProjectionMatrix  * eyeDepthVertex;
	//}else{
	//	gl_Position =  gl_ModelViewProjectionMatrix  * eyeColorVertex;
	//}
	
	
	
}

