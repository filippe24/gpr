#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 curvature_colors;



out vec3 fragNormal;
out vec3 vertColor;

uniform mat4 projection, modelview;
uniform mat3 normalMatrix;


void main()
{
	gl_Position = projection * modelview * vec4(position, 1.0);
	fragNormal = normalMatrix * normal;
        vertColor = vec3(curvature_colors.x/3.0, curvature_colors.y/2.0, (curvature_colors.x/3.0 + curvature_colors.y/2.0)/2.0 );
//        vertColor = vec3((1.0 - curvature_colors.x/3.0), (1.0 - curvature_colors.x/3.0), (1.0 - curvature_colors.x/3.0) );
//        vertColor = vec3((1.0 - curvature_colors.y/3.0), (1.0 - curvature_colors.y/3.0), (1.0 - curvature_colors.y/3.0) );



}

