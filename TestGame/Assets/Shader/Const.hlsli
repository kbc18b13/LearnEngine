struct PS_Input {
	float4 Position : SV_POSITION;
	float3 Normal : PS_NORMAL;
	float2 texUV : PS_UV;
};

cbuffer CameraCB : register(b0) {
	float4x4 viewMat;
	float4x4 projMat;
};

cbuffer LightCB : register(b1) {
	float4 directionLightColor;
	float3 directionLightVec;
};

cbuffer ObjectCB : register(b2) {
	float4x4 worldMat;
};