cbuffer Projection : register(b0) {
	float4x4 proj;
}

cbuffer View : register(b1) {
	float4x4 view;
}

float4 main(float3 pos : POSITION) : SV_POSITION
{
	float4 svpos = float4(pos.xyz, 1);

	svpos = mul(svpos, view);
	svpos = mul(svpos, proj);

	return svpos;
}