/*************************************************************************
> File Name: ShaderPreset.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: shader presets collection.
> Created Time: 2020/07/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_VOX_SHADER_PRESET_HPP
#define CUBBYFLOW_VOX_SHADER_PRESET_HPP

namespace Vox {
    
    const char* const kPointsShaders[2] = 
    {
        // Vertex shader
        R"glsl(
            #version 330 core
            in vec3 position;
            uniform mat4 ViewProjection;
            #define RADIUS 1.0
            void main() {
                gl_PointSize = RADIUS;
                gl_Position =  ViewProjection * vec4(position, 1.0);
            }
        )glsl", 
    
        // Fragment shader
        R"glsl(
            #version 330 core
            out vec4 fragColor;
            void main() {
                fragColor = vec4(0.87f, 0.54f, 0.22f, 1.0f);
            }
        )glsl"
    };

    const char* const kFluidMeshShaders[2] = 
    {
        // Vertex shader
        R"glsl(
            #version 330 core
            layout(location = 0) in vec3 position;
            layout(location = 1) in vec3 normal;
            uniform mat4 ViewProjection;
            out VSOUT {
                vec3 normal;
                vec3 worldPos;
            } vs_out;
            void main() {
                vs_out.normal = normal;
                vs_out.worldPos = position;
                gl_Position = ViewProjection * vec4(position, 1.0);
            }
        )glsl", 
    
        // Fragment shader
        R"glsl(
            #version 330 core
            #define PI 3.14159265359
            float D_GGX(float NoH, float a) {
                float a2 = a * a;
                float f = (NoH * a2 - NoH) * NoH + 1.0;
                return a2 / (PI * f * f);
            }
            vec3 F_Schlick(float u, vec3 f0) {
                return f0 + (vec3(1.0) - f0) * pow(1.0 - u, 5.0);
            }
            float V_SmithGGXCorrelated(float NoV, float NoL, float a) {
                float a2 = a * a;
                float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
                float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
                return 0.5 / (GGXV + GGXL);
            }
            float Fd_Lambert() {
                return 1.0 / PI;
            }
            in VSOUT {
                vec3 normal;
                vec3 worldPos;
            } fs_in;
            uniform vec3 ViewPos;
            uniform vec3 LightPosition;
            #define PerceptualRoughness 0.0
            out vec4 fragColor;
            void main() {
                vec3 n = normalize(fs_in.normal);
                vec3 v = normalize(ViewPos - fs_in.worldPos);
                vec3 l = normalize(LightPosition - fs_in.worldPos);
                vec3 h = normalize(v + l);

                float NoV = abs(dot(n, v)) + 1e-5;
                float NoL = clamp(dot(n, l), 0.0, 1.0);
                float NoH = clamp(dot(n, h), 0.0, 1.0);
                float LoH = clamp(dot(l, h), 0.0, 1.0);

                float roughness = PerceptualRoughness * PerceptualRoughness;

                float D = D_GGX(NoH, roughness);
                const vec3 f0 = vec3(0.02); // water f0 reflectance.
                vec3 F = F_Schlick(LoH, f0);
                float V = V_SmithGGXCorrelated(NoV, NoL, roughness);
                // specular BRDF
                vec3 Fr = (D * V) * F;
                vec3 Fd = Fd_Lambert() * (vec3(192, 191, 187) / 255);
                
                vec3 color = Fd + Fr * NoL;
                fragColor = vec4(color, 1.0f);
            }
        )glsl"
    };
    
    const char* const kS3TCShaders[2] = 
    {
        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;

            void main() {
                gl_Position = vec4(vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1) * 2 - 1, 0, 1);
            }
        )glsl",

        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;
            const float offset = 128.0 / 255.0;//0.5;
            uniform sampler2D ScreenTexture;
            out uvec4 retVal;

            float saturate(float v) { return clamp(v, -1, 1); }
            vec2 saturate(vec2 v) { return clamp(v, -1, 1); }
            vec3 saturate(vec3 v) { return clamp(v, -1, 1); }
            vec4 saturate(vec4 v) { return clamp(v, -1, 1); }

            vec4 toCoCgAY(vec4 rgba)
            {
                float Y  = (   rgba.r + 2*rgba.g + rgba.b ) * 0.25;
                float Co = ( 2*rgba.r - 2*rgba.b          ) * 0.25 + offset;
                float Cg = (  -rgba.r + 2*rgba.g - rgba.b ) * 0.25 + offset;

                return vec4(Co, Cg, rgba.a, Y);
            }

            void ExtractColorBlockCoCgAY(
                out vec4 col[16] ,
                sampler2D image,
                ivec2 coord,
                int mip)
            {
                for (int i= 0; i<4; i++) {
                    for (int j= 0; j<4; j++) {
                        vec4 color= texelFetch(image, coord + ivec2(j,i), mip);
                        //color = sqrt(color);
                        col[i*4+j] = toCoCgAY(color);
                    }
                }
            }

            void FindMinMaxColorsBox(vec4 block[16] , out vec4 mincol, out vec4 maxcol)
            {
                mincol = block[0];
                maxcol = block[0];

                for (int i= 1; i < 16; i++) {
                    mincol = min(mincol, block[i]);
                    maxcol = max(maxcol, block[i]);
                }
            }

            vec3 RoundAndExpand(vec3 v, out uint w)
            {
                ivec3 c = ivec3(round(v * vec3(31, 63, 31)));
                w = uint((c.r << 11) | (c.g << 5) | c.b);

                c.rb = (c.rb << 3) | (c.rb >> 2);
                c.g = (c.g << 2) | (c.g >> 4);

                return vec3(c * (1.0/255));
            }

            uint EmitEndPointsDXT1(inout vec4 mincol, inout vec4 maxcol)
            {
                vec4 inset = (maxcol - mincol)/16 - (8/255.0)/16;
                mincol = saturate(mincol + inset);
                maxcol = saturate(maxcol - inset);

                uvec2 OUT;
                maxcol.rgb = RoundAndExpand(maxcol.rgb, OUT.x);
                mincol.rgb = RoundAndExpand(mincol.rgb, OUT.y);

                // We have to do this in case we select an alternate diagonal.
                if(OUT.x < OUT.y)
                {
                    vec3 tmp = mincol.rgb;
                    mincol.rgb = maxcol.rgb;
                    maxcol.rgb = tmp;
                    return OUT.y | (OUT.x << 16U);
                }

                return OUT.x | (OUT.y << 16U);
            }

            uint EmitIndicesDXT1_noalpha(vec4 block[16] , vec4 mincol, vec4 maxcol)
            {
                const float RGB_RANGE= 3;  //divisions of min-max range

                vec3 dir= (maxcol.rgb - mincol.rgb);
                vec3 origin= maxcol.rgb + dir.rgb / (2.0 * RGB_RANGE);   //half the division above max
                dir /= dot(dir, dir);

                // Compute indices
                uint indices= 0U;
                for (int i= 0; i < 16; i++)
                {
                    uint index= uint(saturate(dot(origin - block[i].rgb, dir)) * RGB_RANGE);
                    indices |= index << uint(i * 2);
                }

                uint i0= (indices & 0x55555555U);
                uint i1= (indices & 0xAAAAAAAAU) >> 1U;
                indices = ((i0 ^ i1) << 1U) | i1;

                // Output indices
                return indices;
            }

            uint EmitEndPointsAlpha(inout float mincol, inout float maxcol)
            {
                float inset = (maxcol - mincol) / 32.0 - (16.0/255) / 32.0;
                mincol = saturate(mincol + inset);
                maxcol = saturate(maxcol - inset);

                uint c0 = uint(round(mincol * 255));
                uint c1 = uint(round(maxcol * 255));

                return (c0 << 8U) | c1;
            }

            uvec2 EmitIndicesAlpha(vec4 block[16] , float minAlpha, float maxAlpha)
            {
                const int ALPHA_RANGE= 7;

                float bias= maxAlpha + (maxAlpha - minAlpha) / (2.0 * ALPHA_RANGE);
                float scale= 1.0f / (maxAlpha - minAlpha);

                uvec2 indices= uvec2(0U);

                for (int i= 0; i < 6; i++)
                {
                    uint index= uint(saturate((bias - block[i].a) * scale) * ALPHA_RANGE);
                    indices.x |= index << uint(3 * i);
                }

                for (int i= 6; i < 16; i++)
                {
                    uint index= uint(saturate((bias - block[i].a) * scale) * ALPHA_RANGE);
                    indices.y |= index << uint(3 * i - 18);
                }

                uvec2 i0= (indices >> 0U) & 0x09249249U;
                uvec2 i1= (indices >> 1U) & 0x09249249U;
                uvec2 i2= (indices >> 2U) & 0x09249249U;

                i2 ^= i0 & i1;
                i1 ^= i0;
                i0 ^= (i1 | i2);

                indices.x = (i2.x << 2U) | (i1.x << 1U) | i0.x;
                indices.y = (((i2.y << 2U) | (i1.y << 1U) | i0.y) << 2U) | (indices.x >> 16U);
                indices.x <<= 16U;

                return indices;
            }

            void SelectCoCgDiagonal(vec4 block[16] , inout vec2 minColor, inout vec2 maxColor)
            {
                vec2 mid= (maxColor + minColor) * 0.5;

                float cov= 0;
                for (int i= 0; i < 16; i++)
                {
                    vec2 t= block[i].xy - mid;
                    cov += t.x * t.y;
                }
                if (cov < 0) {
                    float tmp= maxColor.y;
                    maxColor.y = minColor.y;
                    minColor.y = tmp;
                }
            }

            void main() {
                ivec2 coord = ivec2(4 * gl_FragCoord.xy) & ivec2(~3);
                vec4 readBlock[16];
                
                ExtractColorBlockCoCgAY(readBlock, ScreenTexture, coord, 0);
                
                vec4 mincol, maxcol;
                FindMinMaxColorsBox(readBlock, mincol, maxcol);

                uvec4 OUT;
                SelectCoCgDiagonal(readBlock, mincol.xy, maxcol.xy);

                // Output CoCg in DXT1 block.
                OUT.z = EmitEndPointsDXT1(mincol, maxcol);
                OUT.w = EmitIndicesDXT1_noalpha(readBlock, mincol, maxcol);

                // Output Y in DXT5 alpha block.
                OUT.x = EmitEndPointsAlpha(mincol.a, maxcol.a);

                uvec2 indices= EmitIndicesAlpha(readBlock, mincol.a, maxcol.a);
                OUT.x |= indices.x;
                OUT.y = indices.y;

                retVal = OUT;
            }
        )glsl"
    };

    const char* const kYCoCgDecodingShaders[2] = 
    {
        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;

            void main() {
                gl_Position = vec4(vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1) * 2 - 1, 0, 1);
            }
        )glsl",

        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;

            uniform sampler2D ScreenTexture;
            out vec4 fragColor;

            void main() {
                const float offset = 128.0 / 255.0;
                vec4 v = texelFetch(ScreenTexture, ivec2(gl_FragCoord.xy), 0);

                float Y = v.a;
                float Co = v.r - offset;
                float Cg = v.g - offset;

                float R = Y + Co - Cg;
                float G = Y + Cg;
                float B = Y - Co - Cg;

                fragColor = vec4(R, G, B, v.b);
            }
        )glsl"
    };

    const char* const kDebugDrawShaders[2] = 
    {
        R"glsl(
            #version 330 core
            in vec3 position;
            uniform mat4 ViewProjection;
            uniform vec3 Color;

            out VSOUT {
                vec3 color;
            } vs_out;
            void main() {
                vs_out.color = Color;
                gl_Position = ViewProjection * vec4(position, 1.0);
            }
        )glsl",

        R"glsl(
            #version 330 core
            in VSOUT {
                vec3 color;
            } fs_in;
            out vec4 fragColor;
            void main() {
                fragColor = vec4(fs_in.color, 1.0);
            };
        )glsl"
    };

    const char* const kScreenShaders[2] = 
    {
        // Vertex shader
        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;
            void main() {
                gl_Position = vec4(vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1) * 2 - 1, 0, 1);
            }
        )glsl",
    
        // Fragment shader
        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;
            uniform sampler2D ScreenTexture;
            out vec4 fragColor;
            void main() {
                fragColor = texelFetch(ScreenTexture, ivec2(gl_FragCoord.xy), 0);
            }
        )glsl"
    };

    const char* const kRayDataShaders[2] = 
    {
        // Vertex shader
        R"glsl(
            #version 330 core
            in vec3 position;
            precision highp float;
            precision highp int;
            uniform mat4 ViewProjection;
            out VSOUT {
                vec3 position;
            } vs_out;
            void main() {
                vs_out.position = (ViewProjection * vec4(position, 1.0)).xyz;
                gl_Position = vec4(vs_out.position, 1.0);
            }
        )glsl",
    
        // Fragment shader
        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;
            in VSOUT {
                vec3 position;
            } fs_in;
            out vec4 fragColor;
            void main() {
                fragColor = vec4(fs_in.position, 1.0);
            }
        )glsl"
    };
    
    const char* const kVolumeRayCastingShaders[2] = 
    {
        // Vertex shader
        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;
            void main() {
                gl_Position = vec4(vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1) * 2 - 1, 0, 1);
            }
        )glsl",
    
        // Fragment shader
        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;
            uniform sampler3D VolumeTexture;
            uniform sampler2D VolumeFrontFace;
            uniform sampler2D VolumeBackFace;
            uniform float StepInc;
            out vec4 fragColor;
            #define maxSamples 10
            void main() {
                vec3 startPos = texture2D(VolumeFrontFace, gl_FragCoord.xy).rgb;
                vec3 endPos = texture2D(VolumeBackFace, gl_FragCoord.xy).rgb;
                vec3 ray = endPos - startPos;
                float rayLength = length(ray);
                vec3 step = StepInc*normalize(ray);
                vec4 dest = vec4(0.0);
                vec3 curSamplePos = startPos;
                for (int i=0; i<maxSamples; i++) {//back to front
                    vec4 src = texture3D(VolumeTexture, endPos - curSamplePos).rgba;
                    dest.rgb = mix(dest.rgb, src.rgb, src.a); //(1- src_alpha)*dest_col + src_alpha*src_col
                    dest.a = mix(dest.a, 1.0, src.a); //(1-src_alpha)*dest_alpha + src_slpha
                    if ((length(curSamplePos)>=rayLength)) //out of volume
                        break;
                    if (dest.a>0.99) {
                        dest.a = 1.0;
                        break;
                    }
                    curSamplePos += step;
                } //end of for
                fragColor = dest; 
            }
        )glsl"
    };
} 

#endif 