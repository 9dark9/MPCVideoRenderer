// Fix incorrect display YCgCo after incorrect YUV to RGB conversion in DXVA2 Video Processor

sampler s0 : register(s0);

static float4x4 rgb_ycbcr709 = {
     0.2126,  0.7152,  0.0722, 0.0,
    -0.1146, -0.3854,  0.5,    0.0,
     0.5,    -0.4542, -0.0458, 0.0,
     0.0,     0.0,     0.0,    0.0
};

static float4x4 ycgco_rgb = {
    1.0, -1.0,  1.0, 0.0,
    1.0,  1.0,  0.0, 0.0,
    1.0, -1.0, -1.0, 0.0,
    0.0,  0.0,  0.0, 0.0
};

float4 main(float2 tex : TEXCOORD0) : COLOR
{
    float4 color = tex2D(s0, tex); // original pixel

    color = mul(rgb_ycbcr709, color); // convert RGB to YUV and get original YCgCo
    color = mul(ycgco_rgb, color); // convert YCgCo to RGB

    return color;
}
