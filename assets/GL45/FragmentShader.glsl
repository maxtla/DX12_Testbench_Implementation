// inputs
#ifdef NORMAL
	layout( location = NORMAL ) in vec4 normal_in;
#endif

#ifdef TEXTCOORD
	layout (location = TEXTCOORD ) in vec2 uv_in;
#endif

out vec4 fragment_color;

void main () {
	#ifdef NORMAL
		fragment_color = vec4(normal_in.xyz, 1.0);
	#else
		fragment_color = vec4(0.0,1.0,0.0,1.0);
	#endif
}
