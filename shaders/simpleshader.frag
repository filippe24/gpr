#version 330 core


in vec3 vNormal;
in vec3 vColor;
in vec3 vPos;
in vec3 vPosView;

out vec4 fragColor;


uniform bool bLighting;
uniform bool bReflectionLines = false;
uniform bool param = false;

const float PI = 3.141592;

smooth in vec2 vTexCoord;


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
    else if(param)
    {
        int row = int(floor(vTexCoord.s * 5));
        int column = int(floor(vTexCoord.t * 5));
        float selected_color = float(((row + column +1)%2)*0.7);
        final_color = vec4(selected_color, 0.8, 0.2,1.0);




    }

    fragColor = final_color;


}
