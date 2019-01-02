#version 330 core


in vec3 fragNormal;
in vec3 vertColor;
in vec3 fragPos;


out vec4 fragColor;


uniform bool bLighting;
uniform bool bReflectionLines = false;
uniform bool bCurvature = false;



uniform vec4 color;
const float PI = 3.141592;

uniform mat4 projection, modelview;
uniform mat3 normalMatrix;



void main()
{
    vec4 final_color = color;

    if(bCurvature)
    {
        final_color = vec4(vertColor,1.0);

    }

    if(bLighting)
            fragColor = final_color * normalize(normalMatrix * fragNormal).z;
    else
            fragColor = final_color;

    //Reflecting Line
    //object space
    if(bReflectionLines)
    {
        vec3 V = -normalize(fragPos);
        vec3 N = normalize(fragNormal);

        vec3 R = reflect(V, N);

        vec3 B = vec3 (1.0,0.0,0.0);

        vec2 R2d = R.xy;
        R2d = normalize(R2d);

        float angle = acos(dot(R2d,B.xy));

        float unit = PI/100.0;
        int section = int(angle/unit);
        if( fract(section/2.0) == 0)
            fragColor = vec4(1.0,0.0,0.0,1.0);
        else if(fract(section/2.0) != 0)
            fragColor = vec4(1.0,1.0,0.0,1.0);
        else
            fragColor = vec4(0.0,1.0,0.0,1.0);
    }


}
