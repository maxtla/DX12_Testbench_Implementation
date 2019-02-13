
struct VS_OUT
{
	float4 _pos : SV_POSITION;
	float4 _nor : TEXCOORD0;
	float2 _uv  : TEXCOORD1;
};

cbuffer TintColor : register(b1)
{
	float4 tintColor;
}

SamplerState m_sampler : register(s0);
Texture2D m_srv : register(t0);

float4 PS_main(VS_OUT input) : SV_TARGET
{
	float4 color = float4(1.0, 1.0, 1.0, 1.0);
	if (tintColor.r == 1.0 && tintColor.g == 1.0 && tintColor.b == 1.0)
		color = m_srv.Sample(m_sampler, input._uv);
	return color * float4(tintColor.rgb, 1.0);
}
