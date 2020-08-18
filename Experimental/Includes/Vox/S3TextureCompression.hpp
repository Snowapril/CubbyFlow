/*************************************************************************
> File Name: S3TextureCompression.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: https://github.com/OpenGLInsights/OpenGLInsightsCode
> Purpose: Video capture class using Texture Compression technique
> Created Time: 2020/08/01
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_S3_TEXTURE_COMPRESSION
#define CUBBYFLOW_VOX_S3_TEXTURE_COMPRESSION

#include <Vox/GLTypes.hpp>
#include <memory>

namespace Vox {
    class FrameBuffer;
    class FrameContext;

    /**
     * OpenGL Context wrapper which will be used for rendering one frame.
     */
    class S3TextureCompression 
    {
    public:
        //! Default Constructor
        S3TextureCompression(GLsizei width, GLsizei height);
        //! Default Destructor
        ~S3TextureCompression();

        //! Initialize buffers, textures and framebuffers.
        //! After initializing resources, add them to frame context.
        //! initialized textures : DX5Texture, EncodingTexture, CompressedTexture
        //! initialzied frame buffer : S3TCPass, YCoCgDecodingPass
        void Initialize(const std::shared_ptr<FrameContext>& ctx);

        //! Compress given texture argument with S3TC_DXT5.
        void DXT5Compress(const std::shared_ptr<FrameContext>& ctx, const std::string& textureName);
        
    protected:
    private:
        GLuint _vao;
        GLuint _vboDXT;
        GLsizei _width, _height;
    };

};

#endif