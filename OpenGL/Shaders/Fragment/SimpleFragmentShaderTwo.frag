#version 330 core
in vec2 Tex;

out vec4 fragColor;

uniform sampler2D ourTexture;

void main()
{
	//fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	fragColor = texture(ourTexture, Tex);
}