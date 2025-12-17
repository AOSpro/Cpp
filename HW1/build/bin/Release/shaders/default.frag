#version 330 core
out vec4 FragColor;
in vec3 color;
in vec2 tex;
uniform sampler2D tex0;
uniform int useTexture; // 0 = use vertex color, 1 = sample texture
void main(){
    if(useTexture==1){
        FragColor = texture(tex0, tex);
    } else {
        FragColor = vec4(color, 1.0);
    }
}
