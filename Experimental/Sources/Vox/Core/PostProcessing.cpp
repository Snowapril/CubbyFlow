/*************************************************************************
> File Name: PostProcessing.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL PostProcessing Class
> Created Time: 2020/08/09
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/PostProcessing.hpp>
#include <Vox/Core/Renderer.hpp>
#include <Vox/Core/Texture.hpp>
#include <Vox/Core/ShaderPreset.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Program.hpp>
#include <glad/glad.h>

namespace Vox {

    PostProcessing::PostProcessing()
    {
        //! Do nothing
    }

    PostProcessing::~PostProcessing()
    {
        if (_screenQuad) glDeleteVertexArrays(1, &_screenQuad);
    }

    void PostProcessing::Initialize(GLuint programID)
    {
        _postProcessingProgram = std::make_shared<Program>(programID);

        auto& params = _postProcessingProgram->GetParameters();
        params.SetParameter("ScreenTexture", 0);
        
        if (_screenQuad) glDeleteVertexArrays(1, &_screenQuad);
        glGenVertexArrays(1, &_screenQuad);
    }

    void PostProcessing::DrawFrame(const std::shared_ptr<FrameContext>& ctx, const std::shared_ptr<Texture>& screenTexture) const
    {
        _postProcessingProgram->BindProgram(ctx);
        glBindVertexArray(_screenQuad);
        screenTexture->BindTexture(0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }


};