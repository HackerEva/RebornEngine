struct INPUT_VERTEX
{
	float3 PosL		: POSITION;
	float2 UV0		: TEXCOORD0;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float2 UV1		: TEXCOORD1;
};

float4 main(INPUT_VERTEX Input) : SV_POSITION
{
	return float4(0, 0, 0, 0);
}