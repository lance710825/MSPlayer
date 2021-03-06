#ifndef PUFF_COMMPEG_H
#define PUFF_COMMPEG_H

#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/avstring.h>
#include <libavutil/dict.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
#include <libavutil/mathematics.h>
#include <libavutil/cpu.h>
#include <libavutil/error.h>
#include <libavutil/opt.h>
#include <libavutil/parseutils.h>
#include <libavutil/pixdesc.h>
#include <libavutil/avstring.h>
#include <libavutil/fifo.h>
#include <libavutil/display.h>
#include <libavfilter/version.h>
#include <libswresample/swresample.h>
#ifdef __cplusplus
}
#endif

#define PUFF_USE_FFMPEG(MODULE) (MODULE##_VERSION_MICRO >= 100)
#define PUFF_USE_LIBAV(MODULE)  !PUFF_USE_FFMPEG(MODULE)
#define FFMPEG_MODULE_CHECK(MODULE, MAJOR, MINOR, MICRO) \
    (PUFF_USE_FFMPEG(MODULE) && MODULE##_VERSION_INT >= AV_VERSION_INT(MAJOR, MINOR, MICRO))
#define LIBAV_MODULE_CHECK(MODULE, MAJOR, MINOR, MICRO) \
    (PUFF_USE_LIBAV(MODULE) && MODULE##_VERSION_INT >= AV_VERSION_INT(MAJOR, MINOR, MICRO))
#define AV_MODULE_CHECK(MODULE, MAJOR, MINOR, MICRO, MINOR2, MICRO2) \
    (LIBAV_MODULE_CHECK(MODULE, MAJOR, MINOR, MICRO) || FFMPEG_MODULE_CHECK(MODULE, MAJOR, MINOR2, MICRO2))
/// example: AV_ENSURE(avcodec_close(avctx), false) will print error and return false if failed. AV_WARN just prints error.
#define AV_ENSURE_OK(FUNC, ...) AV_RUN_CHECK(FUNC, return, __VA_ARGS__)
#define AV_ENSURE(FUNC, ...) AV_RUN_CHECK(FUNC, return, __VA_ARGS__)
#define AV_WARN(FUNC) AV_RUN_CHECK(FUNC, void)

#if AV_MODULE_CHECK(LIBAVUTIL, 55, 0, 0, 0, 100)
#define DESC_VAL(X) (X)
#else
#define DESC_VAL(X) (X##_minus1 + 1)
#endif

#define AVCODEC_STATIC_REGISTER FFMPEG_MODULE_CHECK(LIBAVCODEC, 58, 10, 100)
#define AVFORMAT_STATIC_REGISTER FFMPEG_MODULE_CHECK(LIBAVFORMAT, 58, 9, 100)

av_always_inline char* averror2str(int errnum)
{
    static char str[AV_ERROR_MAX_STRING_SIZE];
    memset(str, 0, sizeof(str));
    return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}

#define AV_RUN_CHECK(FUNC, RETURN, ...) do { \
    int ret = FUNC; \
    if (ret < 0) { \
        char str[AV_ERROR_MAX_STRING_SIZE]; \
        memset(str, 0, sizeof(str)); \
        av_strerror(ret, str, sizeof(str)); \
        av_log(NULL, AV_LOG_WARNING, "Error " #FUNC " @%d " __FILE__ ": (%#x) %s\n", __LINE__, ret, str); \
        RETURN __VA_ARGS__; \
     } } while(0)

static const char *get_codec_long_name(enum AVCodecID id)
{
    if (id == AV_CODEC_ID_NONE)
        return "none";
    const AVCodecDescriptor *cd = avcodec_descriptor_get(id);
    if (cd)
        return cd->long_name;
    av_log(NULL, AV_LOG_WARNING, "Codec 0x%x is not in the full list.\n", id);
    AVCodec *codec = avcodec_find_decoder(id);
    if (codec)
        return codec->long_name;
    codec = avcodec_find_encoder(id);
    if (codec)
        return codec->long_name;
    return "unknown_codec";
}

#endif //PUFF_COMMPEG_H
