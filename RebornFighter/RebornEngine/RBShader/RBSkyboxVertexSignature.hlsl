struct INPUT_VERTEX
{
	float3 PosL		: POSITION;
};

float4 main(INPUT_VERTEX Input) : SV_POSITION
{
	return float4(0, 0, 0, 0);
}