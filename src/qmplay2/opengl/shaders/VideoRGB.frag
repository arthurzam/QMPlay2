#define texture texture2D

varying vec2 vTexCoord;
uniform vec3 uVideoAdj;
uniform vec2 uTextureSize;
uniform sampler2D uRGB;
uniform int uTrc;
uniform mat3 uColorPrimariesMatrix;
uniform float uMaxLuminance;
uniform int uNegative;

#ifdef GL3
vec4 getRGBAtOffset(float x, float y)
{
    return texture(uRGB, vTexCoord + vec2(x, y));
}
#endif

void main()
{
    float brightness = uVideoAdj[0];
    float contrast = uVideoAdj[1];
    vec4 RGB = texture(uRGB, vTexCoord);
#ifdef GL3
    float sharpness = uVideoAdj[2];
    if (sharpness != 0.0)
    {
        // Kernel 3x3
        // 1 2 1
        // 2 4 2
        // 1 2 1
        vec2 single = 1.0 / uTextureSize;
        vec4 blur = (
            getRGBAtOffset(-single.x, -single.y) / 16.0 + getRGBAtOffset(0.0, -single.y) / 8.0 + getRGBAtOffset(single.x, -single.y) / 16.0 +
            getRGBAtOffset(-single.x,  0.0     ) /  8.0 + RGB                            / 4.0 + getRGBAtOffset(single.x,  0.0     ) /  8.0 +
            getRGBAtOffset(-single.x,  single.y) / 16.0 + getRGBAtOffset(0.0,  single.y) / 8.0 + getRGBAtOffset(single.x,  single.y) / 16.0
        );
        // Subtract blur from original image, multiply and then add it to the original image
        RGB = clamp(RGB + (RGB - blur) * sharpness, 0.0, 1.0);
    }
    if (uTrc == AVCOL_TRC_BT709)
    {
        colorspace_trc_bt709(RGB.rgb, uColorPrimariesMatrix);
    }
    else if (uTrc == AVCOL_TRC_SMPTE2084)
    {
        colorspace_trc_smpte2084(RGB.rgb, uColorPrimariesMatrix, uMaxLuminance);
    }
    if (uNegative != 0)
    {
        RGB = 1.0 - RGB;
    }
#endif
    gl_FragColor = clamp((RGB - 0.5) * contrast + 0.5, 0.0, 1.0) + brightness;
}
