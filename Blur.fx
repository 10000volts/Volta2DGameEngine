/*
Gaussian Blur
Implement by 666Volts
*/

// precision mediump float;

const static float w[5][5] = { { 0.0317564, 0.0375157, 0.0336973, 0.0396589, 0.0317564 },
{ 0.0375157, 0.0443196, 0.0468515, 0.0443196, 0.0375157 },
{ 0.0396589, 0.0468515, 0.049528, 0.0468515, 0.0396589 },
{ 0.0317564, 0.0375157, 0.0336973, 0.0396589, 0.0317564 },
{ 0.0375157, 0.0443196, 0.0468515, 0.0443196, 0.0375157 } };

cbuffer cbChangesPerObject : register(b0)
{
	float2 size_;
	matrix mvp_;
	float progress_;
};

cbuffer cbChangesPerFrame : register(b1){
	// global time.
	float g_time;
}

cbuffer cbChangesLoose : register(b2){
	
}

Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

struct VS_Input
{
	float4 pos  : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos  : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};


PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, mvp_);
	vsOut.tex0 = vertex.tex0;

	return vsOut;
}

float4 blur(float2 xy){
	float4 color;

	for (int i = -2; i < 3; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			float2 t = {xy.x + float(i) * 4 / size_.x, xy.y + float(j) / size_.y};
			color += colorMap_.Sample( colorSampler_, t ) * w[i + 2][j + 2];
		}
	}
	return color;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{ 
	return blur(frag.tex0);
}
