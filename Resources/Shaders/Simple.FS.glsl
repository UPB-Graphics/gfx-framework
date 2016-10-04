#version 410
layout(location = 0) out vec4 fragColor;

uniform vec4 debug_color;

void main() {
	fragColor = debug_color;
}