#ifdef TEXTURE_RECTANGLE
    #define sampler sampler2DRect
    #define texCoordY  (vTexCoord * uTextureSize)
    #define texCoordUV (vTexCoord * uTextureSize / 2.0)
    #define texCoordYWithOffset(offset) ((vTexCoord + offset) * uTextureSize)
    #define texture texture2DRect
#else
    #define sampler sampler2D
    #define texCoordY  (vTexCoord)
    #define texCoordUV (vTexCoord)
    #define texCoordYWithOffset(offset) (vTexCoord + offset)
    #define texture texture2D
#endif

varying vec2 vTexCoord;
uniform mat3 uYUVtRGB;
uniform vec2 uRangeMultiplier;
uniform float uBL;
uniform vec4 uVideoEq;
uniform float uSharpness;
uniform vec2 uTextureSize;
uniform float uBitsMultiplier;
uniform int uTrc;
uniform mat3 uColorPrimariesMatrix;
uniform float uMaxLuminance;
uniform int uNegative;
uniform sampler uY;
#ifdef NV12
    uniform sampler uCbCr;
#else
    uniform sampler uCb, uCr;
#endif

#ifdef GL3
float getLumaAtOffset(float x, float y)
{
    return texture(uY, texCoordYWithOffset(vec2(x, y)))[0] - uBL;
}
#endif

void main()
{
    float brightness = uVideoEq[0];
    vec3 contrastSaturation = vec3(
        uVideoEq[1],
        uVideoEq[1] * uVideoEq[2],
        uVideoEq[1] * uVideoEq[2]
    );

#ifdef NV12
    vec3 YCbCr = vec3(
        texture(uY   , texCoordY )[0],
        texture(uCbCr, texCoordUV).xy
    );
#else
    vec3 YCbCr = vec3(
        texture(uY , texCoordY )[0],
        texture(uCb, texCoordUV)[0],
        texture(uCr, texCoordUV)[0]
    );
#endif
    YCbCr *= uBitsMultiplier;
    YCbCr -= vec3(uBL, vec2(128.0 / 255.0));

#ifdef GL3
    if (uSharpness != 0.0)
    {
        // Kernel 3x3
        // 1 2 1
        // 2 4 2
        // 1 2 1
        vec2 single = 1.0 / uTextureSize;
        float lumaBlur = (
            getLumaAtOffset(-single.x, -single.y) / 16.0 + getLumaAtOffset(0.0, -single.y) / 8.0 + getLumaAtOffset(single.x, -single.y) / 16.0 +
            getLumaAtOffset(-single.x,  0.0     ) /  8.0 + YCbCr[0]                        / 4.0 + getLumaAtOffset(single.x,  0.0     ) /  8.0 +
            getLumaAtOffset(-single.x,  single.y) / 16.0 + getLumaAtOffset(0.0,  single.y) / 8.0 + getLumaAtOffset(single.x,  single.y) / 16.0
        );
        // Subtract blur from original image, multiply and then add it to the original image
        YCbCr[0] = clamp(YCbCr[0] + (YCbCr[0] - lumaBlur) * uSharpness, 0.0, 1.0);
    }

    float hueAdj = uVideoEq[3];
    if (hueAdj != 0.0)
    {
        float hue = atan(YCbCr[2], YCbCr[1]) + hueAdj;
        float chroma = sqrt(YCbCr[1] * YCbCr[1] + YCbCr[2] * YCbCr[2]);
        YCbCr[1] = chroma * cos(hue);
        YCbCr[2] = chroma * sin(hue);
    }
#endif

    vec3 rgb = clamp(uYUVtRGB * ((YCbCr * uRangeMultiplier.xyy - vec3(0.5, 0.0, 0.0)) * contrastSaturation + vec3(0.5, 0.0, 0.0)), 0.0, 1.0);

#ifdef GL3
    if (uTrc == AVCOL_TRC_BT709)
    {
        colorspace_trc_bt709(rgb, uColorPrimariesMatrix);
    }
    else if (uTrc == AVCOL_TRC_SMPTE2084)
    {
        colorspace_trc_smpte2084(rgb, uColorPrimariesMatrix, uMaxLuminance);
    }
    if (uNegative != 0)
    {
        rgb = 1.0 - rgb;
    }
#endif

    gl_FragColor = vec4(rgb + brightness, 1.0);
}
