#include "Const.hlsli"

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_Input input) : SV_TARGET
{
	return tex.Sample(samp, input.texUV);
}