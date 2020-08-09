/*************************************************************************
> File Name: SimulationBuffer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_ROUND_ROBIN_ASYNC_BUFFER_HPP
#define CUBBYFLOW_VOX_ROUND_ROBIN_ASYNC_BUFFER_HPP

#include <Vox/GLTypes.hpp>
#include <memory>
#include <vector>

typedef struct __GLsync *GLsync;

namespace Vox {
    class VoxScene;
    class FrameContext;
    /**
     * Buffer for simulating fluid particles which need huge effort for optimizing data transfer performance.
     * Implemented with multiple buffer technique (round-robin)
     **/
    class RoundRobinAsyncBuffer
    {
    public:
        //! Default Constructor.
        RoundRobinAsyncBuffer();
        //! Constructor with number of the buffers.
        RoundRobinAsyncBuffer(const size_t numBuffer);
        //! Default destructor.
        ~RoundRobinAsyncBuffer();
    
        //! Draw one frame of the particles data.
        bool CheckFence(GLuint64 timeout);
        //! Asynchronously transfer scene data to vertex buffer.
        void AsyncBufferTransfer(const std::shared_ptr<VoxScene>& scn);
        //! Draw the frmae with the transferred vertex buffer.
        void DrawFrame(const std::shared_ptr<FrameContext>& ctx);
        //! Advance the frame index.
        void AdvanceFrame();

        static const size_t kMaxBufferSize = 0x1000000; //! 2097 kB - This is GPU memory
        static const size_t kDefaultNumBuffer = 3;
    protected:
    private:
        std::vector<GLuint> _vaos;
        std::vector<GLuint> _buffers;
        std::vector<GLsync> _fences; 
        size_t _frameIndex { 0 } ;
        size_t _numVertices;
    };
};

#endif