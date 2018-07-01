#version 430 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main() {
	color = texture(ourTexture1, TexCoord);
    //color = texture(ourTexture1, vec2(TexCoord.x + offsetUV.x, TexCoord.y + offsetUV.y); // * vec4(ourColor, 1.0);
}