

#pragma once


extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/avutil.h>
	#include <libavutil/imgutils.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
}


class FFMPEGStream
{
public:
	static FFMPEGStream& Instance()
	{
		static FFMPEGStream inst;
		return inst;
	}

protected:
	FFMPEGStream();
	~FFMPEGStream();


	AVCodecContext* codec_ctx = nullptr;
	AVFormatContext* format_ctx = nullptr;
	// AVCodec** codec = nullptr;
};
