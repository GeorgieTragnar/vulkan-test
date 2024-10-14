

#include "FFMPEGStream.h"

#include "LogMacros.h"
#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_FFMPEG


FFMPEGStream::FFMPEGStream()
{
	const char* codec_name = "vp8";
	const char* ip_address = "127.0.0.1"; // Destination IP for streaming
	int32_t port = 12345;                     // Destination port for streaming

	LOG_CRITICAL("ffmpeg version: {}", av_version_info());
	const AVCodec* c = nullptr;
	void* i = nullptr;
	std::stringstream ss;
	std::cout << "\n";
	while ((c = av_codec_iterate(&i))) 
	{
		std::cout << c->name << "\t";
		if (strcmp(c->name, "vp9"))
			break;
		// ss << std::string(c->name) << "\t";
		// LOG_WARNING("Codec: {}", c->name);
	}
	// LOG_WARNING("codecs: {}", ss.str());

	// Initialize FFmpeg
	avformat_network_init();

	// Find the VP9 encoder
	// const AVCodec* codec = avcodec_find_encoder_by_name(codec_name);
	const AVCodec* codec = c;
	if (codec == nullptr) 
	{
		throw std::runtime_error("codec not found");
	}

	// Allocate codec context
	_codec_ctx = avcodec_alloc_context3(codec);
	if (!_codec_ctx) 
	{
		throw std::runtime_error("Could not allocate video codec context");
	}

	int32_t width, height;
	VulkanApp::Instance().getResolution(width, height);

	// Set up codec parameters
	_codec_ctx->bit_rate = 500000;         // Bitrate: 500kbps
	_codec_ctx->width = width;               // Video width
	_codec_ctx->height = height;              // Video height
	_codec_ctx->time_base = (AVRational){1, 30}; // Frame rate: 30 fps
	_codec_ctx->framerate = (AVRational){30, 1};
	_codec_ctx->gop_size = 10;             // GOP size
	_codec_ctx->max_b_frames = 1;          // Maximum B-frames
	_codec_ctx->pix_fmt = AV_PIX_FMT_GRAY8; // Pixel format

	// Open the codec
	if (avcodec_open2(_codec_ctx, codec, nullptr) < 0) 
	{
		avcodec_free_context(&_codec_ctx);
		_codec_ctx = nullptr;
		throw std::runtime_error("Could not open codec");
	}

	// Create UDP socket for sending the stream	
	_udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (_udp_socket < 0) 
	{
		avcodec_free_context(&_codec_ctx);
		_codec_ctx = nullptr;
		throw std::runtime_error("Socket creation failed");
	}

	// struct sockaddr_in _addr;
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip_address, &_addr.sin_addr) <= 0) 
	{
		avcodec_free_context(&_codec_ctx);
		_codec_ctx = nullptr;
		throw std::runtime_error("Invalid address");
	}

	if (_udp_socket < 0) 
	{
		avcodec_free_context(&_codec_ctx);
		_codec_ctx = nullptr;
		throw std::runtime_error("Failed to create UDP socket");
	}
}

FFMPEGStream::~FFMPEGStream()
{
	// Clean up
	if (_udp_socket >= 0)
		close(_udp_socket);
	if (_codec_ctx != nullptr)
		avcodec_free_context(&_codec_ctx);
	avformat_network_deinit();
}

// extern "C" {
//     #include <libavcodec/avcodec.h>
//     #include <libavformat/avformat.h>
//     #include <libavutil/imgutils.h>
//     #include <libavutil/opt.h>
//     #include <libswscale/swscale.h>
// }

// #include <iostream>
// #include <cstring>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// // Function to create a UDP socket and set up the destination address
// int create_udp_socket(const char* ip, int port, struct sockaddr_in* addr) {
//     int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//     if (sockfd < 0) {
//         perror("Socket creation failed");
//         return -1;
//     }

//     memset(addr, 0, sizeof(*addr));
//     addr->sin_family = AF_INET;
//     addr->sin_port = htons(port);
//     if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
//         perror("Invalid address");
//         return -1;
//     }

//     return sockfd;
// }

// // Function to encode and send frames using VP9
// void encode_and_send_video(AVCodecContext* codec_ctx, int udp_socket, struct sockaddr_in* addr) {
//     AVFrame* frame = av_frame_alloc();
//     AVPacket* pkt = av_packet_alloc();
    
//     if (!frame || !pkt) {
//         std::cerr << "Could not allocate frame or packet" << std::endl;
//         return;
//     }

//     frame->format = codec_ctx->pix_fmt;
//     frame->width = codec_ctx->width;
//     frame->height = codec_ctx->height;
//     av_frame_get_buffer(frame, 32); // Align frame buffer

//     int frame_count = 0;

//     while (frame_count < 100) { // Encode 100 frames for the example
//         // Simulate a video source by filling the frame with dummy data
//         av_frame_make_writable(frame);
//         for (int y = 0; y < codec_ctx->height; y++) {
//             for (int x = 0; x < codec_ctx->width; x++) {
//                 frame->data[0][y * frame->linesize[0] + x] = x + y + frame_count * 3;  // Y
//                 if (x < codec_ctx->width / 2 && y < codec_ctx->height / 2) {
//                     frame->data[1][y * frame->linesize[1] + x] = 128 + frame_count * 2;  // U
//                     frame->data[2][y * frame->linesize[2] + x] = 64 + frame_count * 5;   // V
//                 }
//             }
//         }

//         frame->pts = frame_count;

//         // Send the frame for encoding
//         int ret = avcodec_send_frame(codec_ctx, frame);
//         if (ret < 0) {
//             std::cerr << "Error sending a frame for encoding" << std::endl;
//             break;
//         }

//         // Receive the encoded packet and send it over the network
//         while (ret >= 0) {
//             ret = avcodec_receive_packet(codec_ctx, pkt);
//             if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
//                 break;
//             } else if (ret < 0) {
//                 std::cerr << "Error during encoding" << std::endl;
//                 return;
//             }

//             // Send packet over the network (UDP)
//             if (sendto(udp_socket, pkt->data, pkt->size, 0, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
//                 perror("Failed to send packet");
//             }

//             av_packet_unref(pkt);
//         }

//         frame_count++;
//     }

//     // Flush the encoder
//     avcodec_send_frame(codec_ctx, nullptr);
//     while (avcodec_receive_packet(codec_ctx, pkt) == 0) {
//         if (sendto(udp_socket, pkt->data, pkt->size, 0, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
//             perror("Failed to send packet");
//         }
//         av_packet_unref(pkt);
//     }

//     av_frame_free(&frame);
//     av_packet_free(&pkt);
// }

// int main() {
//     const char* codec_name = "libvpx-vp9";
//     const char* ip_address = "127.0.0.1"; // Destination IP for streaming
//     int port = 12345;                     // Destination port for streaming

//     // Initialize FFmpeg
//     avcodec_register_all();
//     avformat_network_init();

//     // Find the VP9 encoder
//     const AVCodec* codec = avcodec_find_encoder_by_name(codec_name);
//     if (!codec) {
//         std::cerr << "Codec not found: " << codec_name << std::endl;
//         return -1;
//     }

//     // Allocate codec context
//     AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
//     if (!codec_ctx) {
//         std::cerr << "Could not allocate video codec context" << std::endl;
//         return -1;
//     }

//     // Set up codec parameters
//     codec_ctx->bit_rate = 500000;         // Bitrate: 500kbps
//     codec_ctx->width = 640;               // Video width
//     codec_ctx->height = 360;              // Video height
//     codec_ctx->time_base = (AVRational){1, 30}; // Frame rate: 30 fps
//     codec_ctx->framerate = (AVRational){30, 1};
//     codec_ctx->gop_size = 10;             // GOP size
//     codec_ctx->max_b_frames = 1;          // Maximum B-frames
//     codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P; // Pixel format

//     // Open the codec
//     if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
//         std::cerr << "Could not open codec" << std::endl;
//         avcodec_free_context(&codec_ctx);
//         return -1;
//     }

//     // Create UDP socket for sending the stream
//     struct sockaddr_in addr;
//     int udp_socket = create_udp_socket(ip_address, port, &addr);
//     if (udp_socket < 0) {
//         std::cerr << "Failed to create UDP socket" << std::endl;
//         avcodec_free_context(&codec_ctx);
//         return -1;
//     }

//     // Start encoding and sending video
//     encode_and_send_video(codec_ctx, udp_socket, &addr);

//     // Clean up
//     close(udp_socket);
//     avcodec_free_context(&codec_ctx);
//     avformat_network_deinit();

//     return 0;
// }

