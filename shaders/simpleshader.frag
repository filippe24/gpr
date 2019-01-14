#version 330 core


in vec3 vNormal;
in vec3 vColor;
in vec3 vPos;
in vec3 vPosView;

out vec4 fragColor;


uniform bool bLighting;
uniform bool bReflectionLines = false;

const float PI = 3.141592;




void main()
{
    vec4 final_color = vec4(vColor,1.0);

    if(bLighting)
            final_color = (final_color)*vNormal.z;


    //Reflecting Line
    if(bReflectionLines)
    {
        vec3 R = reflect(vPosView,vNormal);
        vec3 selecter = normalize(vPos + R);
        if( int(selecter.y*5)%2 == 0)
            final_color = vec4(1.0,0.0,0.0,1.0);
        else if(int(selecter.y*5)%2 != 0)
            final_color = vec4(1.0,1.0,0.0,1.0);
        else
            final_color = vec4(0.0,1.0,0.0,1.0);
    }

    fragColor = final_color;


}
