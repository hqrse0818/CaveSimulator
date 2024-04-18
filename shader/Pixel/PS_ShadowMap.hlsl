struct PS_IN
{
    float4 pos : SV_Position;
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(pin.pos.z, pin.pos.z, pin.pos.z, 1.0f);
    
	return color;
}