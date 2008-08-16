sampler g_samScreen : register(s0);

struct VertexShaderOutput
{
	float4 color : COLOR0;
    float2 texCoord : TEXCOORD0;
};

float4 main(VertexShaderOutput input) : COLOR0
{	
	float4 col = tex2D(g_samScreen,input.texCoord);
	
	if(input.texCoord.x < .5)
		col.a =  input.texCoord.x * 1;
	else
		col.a =  1-input.texCoord.x;
		
	if(input.texCoord.y < 1)
		col.a +=  input.texCoord.y * 1;
	else
		col.a +=  1-input.texCoord.y;
	
	return col;
}

technique PostProcess
{
    pass Pass0
    {
		PixelShader = compile ps_2_0 main();		
    }
}