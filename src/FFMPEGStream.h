

#pragma once

#include "headers.h"

#include "VulkanApp.h"

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/avutil.h>
	#include <libavutil/imgutils.h>
    #include <libavutil/opt.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
}

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


class FFMPEGStream
{
public:
	static FFMPEGStream& Instance()
	{
		static FFMPEGStream inst;
		return inst;
	}

	void encode_and_send_video() {};


protected:
	FFMPEGStream();
	~FFMPEGStream();


	// TODO: move network code into its own class
	int32_t _udp_socket;
	struct sockaddr_in _addr;

	AVCodecContext* _codec_ctx;
	AVFormatContext* _format_ctx;

	// AVCodec** codec = nullptr;
};
