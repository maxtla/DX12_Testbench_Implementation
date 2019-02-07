struct VS_OUT
{
	float4 pos : SV_POSITION;
};

float4 PS_main(VS_OUT input) : COLOR
{
	return float4(1.0,0.0,0.0,1.0);
}
