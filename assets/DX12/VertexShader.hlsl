struct VS_IN
{
	float4 pos : POSITION;
};

struct VS_OUT
{
	float4 _pos : SV_POSITION;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;

	output._pos = input.pos;

	return output;
};
