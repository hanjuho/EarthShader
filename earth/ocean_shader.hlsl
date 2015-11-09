//
// Globals
//

sampler2D BaseTex : register(s0);
sampler2D SpecTex : register(s1);
sampler2D BumpTex : register(s2);

vector	Attraction;
float	Gravity;

//
// Input and Output structures.
//

struct PS_INPUT
{
	float3 normal   : COLOR0;
	float2 tex		: TEXCOORD0;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR0;
};

//
// Main
//

PS_OUTPUT PSMain(PS_INPUT input)
{
	// zero out all members of the output instance.
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 base = tex2D(BaseTex, input.tex);
	float4 spec = tex2D(SpecTex, input.tex);
	float4 bump = tex2D(BumpTex, input.tex);

	float baseLen = length(base);
	
	// �ٴ� �κ�0
	if (spec.x)
	{
		// ������ �߷¿��� �η��� ���ϸ� ���� �ٴٿ� �������� �η��� ũ�Ⱑ ��Ÿ����.
		output.diffuse.x = base.x * (((input.normal.x * Gravity) - Attraction.x) / Gravity);
		output.diffuse.y = base.y * (((input.normal.y * Gravity) - Attraction.y) / Gravity);
		output.diffuse.z = base.z * (((input.normal.z * Gravity) - Attraction.z) / Gravity);
	}
	// ���� �κ�
	else
	{
		float bumpLen = length(bump);

		// ���� ��⿡�� �븻���� ���� ����
		output.diffuse = base * (bumpLen / baseLen);
	}
	output.diffuse.w = base.w;

	return output;
}