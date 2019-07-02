cbuffer Projection : register(b0) {
	float4x4 proj;
}

cbuffer View : register(b1) {
	float4x4 view;
}

struct NonSkinVertex {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 Tangent : TANGENT;
	uint color : COLOR;
	float2 TextureCoordinates : TEXTURE;
};

float4 main(NonSkinVertex pos : POSITION) : SV_POSITION
{
	float4 svpos = float4(pos.Position.xyz, 1);

	svpos = mul(svpos, view);
	svpos = mul(svpos, proj);

	return svpos;
}