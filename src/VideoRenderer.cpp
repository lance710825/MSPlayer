#include "VideoRenderer.h"
#include "AVOutput_p.h"

namespace Puff {

VideoRenderer::VideoRenderer()
{

}

VideoRenderer::~VideoRenderer()
{

}

void VideoRenderer::receiveFrame(const VideoFrame &frame)
{

}

void VideoRenderer::resizeRenderer(const Size &size) {
    resizeRenderer(size.width(), size.height());
}

void VideoRenderer::resizeRenderer(int width, int height) {
    DPTR_D(VideoRenderer);
    d.renderer_width = width;
    d.renderer_height = height;
    onResizeRenderer(width, height);
}

void VideoRenderer::onResizeRenderer(int width, int height) {
    PU_UNUSED(width)
    PU_UNUSED(height)
}

}