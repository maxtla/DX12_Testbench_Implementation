
struct VS_OUT
{
	float4 _pos : SV_POSITION;
	float4 _nor : TEXCOORD0;
	float2 _uv  : TEXCOORD1;
};

SamplerState m_sampler : register(s0);
Texture2D m_srv : register(t0);

float4 PS_main(VS_OUT input) : SV_TARGET
{
	return m_srv.Sample(m_sampler, input._uv);
}
