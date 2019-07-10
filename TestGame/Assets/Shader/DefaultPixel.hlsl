#include "Const.hlsli"

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_Input input) : SV_TARGET
{

	//ディレクションライト
	float hansyaPower = -dot(directionLightVec, input.Normal);
    hansyaPower = (hansyaPower + 1) * 0.5f;

    return tex.Sample(samp, input.texUV) * hansyaPower;
}