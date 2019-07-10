#include "Const.hlsli"

struct NonSkinVertex {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 color : COLOR;
	float2 TextureCoordinates : TEXTURE;
};

PS_Input main(NonSkinVertex pos)
{
	float4 svpos = float4(pos.Position.xyz, 1);

	svpos = mul(svpos, worldMat);
	svpos = mul(svpos, viewMat);
	svpos = mul(svpos, projMat);

	float4 tempNormal = float4(pos.Normal.xyz, 0);

	tempNormal = mul(tempNormal, worldMat);

	PS_Input ps;
	ps.Position = svpos;
	ps.Normal = tempNormal;
	ps.texUV = pos.TextureCoordinates;
	return ps;
}