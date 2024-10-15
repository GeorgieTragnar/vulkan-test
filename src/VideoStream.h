

// #pragma once

// #include "headers.h"

// #include "VulkanApp.h"

// extern "C" {
// 	#include <libavcodec/avcodec.h>
// 	#include <libavformat/avformat.h>
// 	#include <libavutil/avutil.h>
// 	#include <libavutil/imgutils.h>
//     #include <libavutil/opt.h>
// 	#include <libswscale/swscale.h>
// 	#include <libswresample/swresample.h>
// }

// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h>


// #include <rtc/rtc.hpp>
// #include <thread>



// class VideoStream
// {
// public:
// 	static VideoStream& Instance()
// 	{
// 		static VideoStream inst;
// 		return inst;
// 	}

// 	void encode_and_send_video();
// 	void start_ffmpeg_rtp_stream(const std::string& rtp_url);
// 	void handle_signaling(std::shared_ptr<rtc::PeerConnection> pc);
// 	void start_rtc_connection();


// protected:
// 	VideoStream();
// 	~VideoStream();


// 	// TODO: move network code into its own class
// 	int32_t _udp_socket;
// 	struct sockaddr_in _addr;

// 	AVCodecContext* _codec_ctx;
// 	AVFormatContext* _format_ctx;
// 	AVStream* _video_stream;

// 	// AVCodec** codec = nullptr;
// };
