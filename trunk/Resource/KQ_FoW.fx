sampler g_samScreen;
sampler g_samVisible =
sampler_state
{
    texture1 = <g_Visible>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//float2 Pos;
float PosX;
float PosY;


float4 main( float2 Tex : TEXCOORD ) : COLOR
{
	float4 Color;
	float4 Pos;

	//Pos.x = PosX;
	//Pos.y = PosY;

	//Grab the color from the texture based on coordinates
	Color = tex2D(g_samScreen, Tex.xy);

	//Test
	//Color = tex2D(g_samScreen, Tex.xy) * (3 * Pos.xy);

	//Fog of War
	Color = tex2D(g_samScreen, Tex.xy) * (Tex.y - Tex.x + .5);
	//Color = Color * (Pos);
	
	//Pos.x = tex2D(g_samScreen, (Tex.x * Tex.y)) + PosX;
	//Pos.y = tex2D(g_samVisible, (Tex.x * Tex.y)) + PosY;

	//return tex2D(g_samScreen, Tex) * tex2D(g_samVisible, Tex);
	return Color;
	//return Pos;
}

technique PostProcess
{
	pass p0
	{
		VertexShader = null;
		PixelShader = compile ps_2_0 main();
	}
}