#version 330

in vec2 v_texcoord;

out vec4 colorOut;

uniform sampler2D texMap;

in vec4 colorBORRAR;

void main()
{
    //colorOut = vec4(1.0, 0.0, 0.0, 1.0) ;
    if(v_texcoord.x <0.5){
        //colorOut  = vec4(texture(texMap, v_texcoord).rgb, 1.0);
        colorOut = vec4(1.0,0,0,1);
    }else{
        //colorOut  = vec4(texture(texMap, v_texcoord).rgb, 1.0);
        colorOut = vec4(0,1.0,0,1);

    }
    colorOut  = vec4(texture(texMap, v_texcoord).rgb, 1.0);
    //colorOut = vec4(v_texcoord.x,v_texcoord.y,0.0,1.0);

}
