#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 curvature_colors;



out vec3 vNormal;
out vec3 vColor;
out vec3 vPos;
out vec3 vPosView;

uniform mat4 projection, modelview;
uniform mat3 normalMatrix;
uniform float min_gauss;
uniform float max_gauss;
uniform float min_mean;
uniform float max_mean;

uniform bool select_gauss = false;




void main()
{
        //vPos = (modelview * vec4(position, 1.0)).xyz;
        //vNormal = normalMatrix * normal;
        vPosView = (modelview*vec4(position, 1)).xyz;
        vPos = position;
        vNormal = normalize(normalMatrix * normal);

        gl_Position = projection * modelview * vec4(position, 1.0);



        //GAUSSIAN MEAN COLORS:
        float gauss_var = (max_gauss - min_gauss)*0.3;

        //TODO: define the right balance between mean and variance it works ok for the bunny
        float gauss_curv = (curvature_colors.x - min_gauss)/((max_gauss - min_gauss)*0.2);
        float mean_curv = (curvature_colors.y - min_mean)/((max_mean - min_mean)*0.1);

        vec3 col1 = vec3(1,0,0);
        vec3 col2 = vec3(0,0,1);
        vec3 col3 = vec3(0,1,0);

        vec3 gauss_color = mix(col1,col2, gauss_curv);
        vec3 mean_color = mix(col3,col1, mean_curv);
        if(select_gauss)
            vColor = gauss_color;
        else
            vColor = mean_color;
        //vColor = vec3((1.0 - curvature_colors.x/3.0), (1.0 - curvature_colors.x/3.0), (1.0 - curvature_colors.x/3.0) );
        //vColor = vec3((1.0 - curvature_colors.y/3.0), (1.0 - curvature_colors.y/3.0), (1.0 - curvature_colors.y/3.0) );

}

