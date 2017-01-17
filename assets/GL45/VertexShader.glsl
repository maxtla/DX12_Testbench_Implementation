
// inputs
#ifdef NORMAL
	layout(binding=NORMAL) buffer nor { vec4 normal_in[]; };
	layout(location=NORMAL) out vec4 normal_out;
#endif

#ifdef TEXTCOORD
	layout(binding=TEXTCOORD) buffer text { vec2 uv_in[]; };
	layout(location=TEXTCOORD) out vec2 uv_out;
#endif

// always position in this shader
layout(binding=POSITION) buffer pos { vec4 position_in[]; };


void main() {

	#ifdef NORMAL
		normal_out = normal_in[gl_VertexID];
	#endif

	#ifdef TEXTCOORD
		uv_out = uv_in[gl_VertexID];
	#endif

	gl_Position = position_in[gl_VertexID];
};
