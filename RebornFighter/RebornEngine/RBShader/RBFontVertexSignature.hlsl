struct INPUT_VERTEX
{
	float4 PosL : POSITION;
	float4 ColorFg : COLOR0;
	float4 ColorBg : COLOR1;
	float2 UV : TEXCOORD0;
};

float4 main(INPUT_VERTEX Input) : SV_POSITION
{
	return float4(0, 0, 0, 0);
}