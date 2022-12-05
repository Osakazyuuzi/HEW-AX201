struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1,1,1,1);

	float2 mGrid = floor(abs(pin.uv) * 2.0f);
	float2 sGrid = floor(abs(pin.uv) * 8.0f);

	float mid = fmod(mGrid.x + mGrid.y, 2.0f);
	float small = fmod(sGrid.x + sGrid.y, 2.0f);

	color.rgb = ((mid * 0.1f) * small + 0.45f) + (1 - small) * 0.05f;
	return color;
}