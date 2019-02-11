struct VS_IN
{
	float4 pos : POSITION;
	float4 nor : NORMAL;
	float2 uv  : TEXCOORD;
};

struct VS_OUT
{
	float4 _pos : SV_POSITION;
	float4 _nor : TEXCOORD0;
	float2 _uv  : TEXCOORD1;
};

cbuffer Translation : register(b0)
{
	float4 translation;
}

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;

	output._pos = input.pos + translation;
	output._nor = input.nor;
	output._uv = input.uv;

	return output;
};
