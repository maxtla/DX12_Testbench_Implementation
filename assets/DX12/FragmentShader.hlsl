struct VS_OUT
{
	float4 _pos : SV_POSITION;
};

float4 PS_main(VS_OUT input) : SV_TARGET
{
	return float4(1.0,0.0,0.0,1.0);
}
