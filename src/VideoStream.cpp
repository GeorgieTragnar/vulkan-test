

#include "VideoStream.h"

#include "LogMacros.h"
#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_FFMPEG


VideoStream::VideoStream()
{
	initFFmpegEncoder();
}

VideoStream::~VideoStream()
{

}
// Initialize FFmpeg encoder for H.264 encoding
void VideoStream::initFFmpegEncoder() {
	const AVCodec* codec;
    {

        const AVCodec* c = nullptr;
        void* i = nullptr;
        std::stringstream ss;
        std::cout << "\n";
        while ((c = av_codec_iterate(&i))) 
        {
            // std::cout << c->name << "\t";
            if (strcmp(c->name, "h264") == 0)
            {
                
                std::cout << c->name << "\t";
                break;
            }
            // ss << std::string(c->name) << "\t";
            // LOG_WARNING("Codec: {}", c->name);
        }
        codec = c;
    }
    // const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        throw std::runtime_error("H.264 codec not found");
    }
	LOG_CRITICAL("AGSHLKH");
	// List supported pixel formats
	if (codec->pix_fmts) {
		// std::cout << "S" << std::endl;
		LOG_CRITICAL("Supported pixel formats by the encoder:");
		for (const enum AVPixelFormat* p = codec->pix_fmts; *p != AV_PIX_FMT_NONE; ++p) {
			std::cout << " - " << av_get_pix_fmt_name(*p) << std::endl;
		}
	}
	LOG_CRITICAL("AGSHLKH");


    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        throw std::runtime_error("Could not allocate codec context");
    }

    codec_ctx->bit_rate = 400000; // Set bitrate
    codec_ctx->width = 1280;      // Frame width
    codec_ctx->height = 720;      // Frame height
    codec_ctx->time_base = AVRational{1, 30}; // Frame rate (30 FPS)
    codec_ctx->framerate = AVRational{30, 1};
    codec_ctx->gop_size = 10;     // Intra frame interval
    codec_ctx->max_b_frames = 1;  // Max B-frames
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P; // Pixel format

    // Set encoder options
    AVDictionary* codec_options = nullptr;
    av_dict_set(&codec_options, "preset", "ultrafast", 0);
    av_dict_set(&codec_options, "tune", "zerolatency", 0);

    if (avcodec_open2(codec_ctx, codec, &codec_options) < 0) {
        av_dict_free(&codec_options);
        throw std::runtime_error("Could not open codec");
    }
    av_dict_free(&codec_options);

    // Allocate the frame
    frame = av_frame_alloc();
    if (!frame) {
        throw std::runtime_error("Could not allocate video frame");
    }
    frame->format = codec_ctx->pix_fmt;
    frame->width = codec_ctx->width;
    frame->height = codec_ctx->height;

    if (av_frame_get_buffer(frame, 32) < 0) {
        throw std::runtime_error("Could not allocate frame buffer");
    }

    // Allocate the packet
    pkt = av_packet_alloc();
    if (!pkt) {
        throw std::runtime_error("Could not allocate AVPacket");
    }
}

// Function to encode a raw frame into H.264 and store it in out_sample
void VideoStream::encodeFrame(std::vector<char>& out_sample) {
    if (av_frame_make_writable(frame) < 0) {
        throw std::runtime_error("Could not make frame writable");
    }

    // Fill the Y, U, and V data planes
    for (int y = 0; y < codec_ctx->height; y++) {
        for (int x = 0; x < codec_ctx->width; x++) {
            frame->data[0][y * frame->linesize[0] + x] = (x + y + frame_counter * 3) % 256; // Y plane
        }
    }

    for (int y = 0; y < codec_ctx->height / 2; y++) {
        for (int x = 0; x < codec_ctx->width / 2; x++) {
            frame->data[1][y * frame->linesize[1] + x] = (128 + frame_counter * 2) % 256; // U plane
            frame->data[2][y * frame->linesize[2] + x] = (64 + frame_counter * 5) % 256;  // V plane
        }
    }

    frame->pts = frame_counter++; // Set the frame PTS

    // Send the frame to the encoder
	int ret = avcodec_send_frame(codec_ctx, frame);
	if (ret < 0) {
		char errbuf[AV_ERROR_MAX_STRING_SIZE];
		av_strerror(ret, errbuf, sizeof(errbuf));
		throw std::runtime_error(std::string("Error sending frame to encoder: ") + errbuf);
	}

    // Receive the encoded packet
    while (ret >= 0) {
        ret = avcodec_receive_packet(codec_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break; // Need more input or end of stream
        } else if (ret < 0) {
            throw std::runtime_error("Error during encoding");
        }

        // Append packet data to the output sample vector
        out_sample.insert(out_sample.end(), pkt->data, pkt->data + pkt->size);
        av_packet_unref(pkt);
    }
}

// The function that returns the next H.264 sample
std::vector<char> VideoStream::getNextSample() {
    std::vector<char> sample;
    encodeFrame(sample);
    return sample;
}


// // Function to handle the RTP stream from FFmpeg
// void VideoStream::start_ffmpeg_rtp_stream(const std::string& rtp_url) {
//     AVFormatContext* fmt_ctx = nullptr;
//     AVCodecContext* codec_ctx = nullptr;
//     AVPacket* packet = nullptr;

//     // Initialize FFmpeg
//     // av_register_all();

//     // Open the input video stream (encoded with mpeg1video)
//     const char* input_file = "output.mpg"; // Input video file
//     if (avformat_open_input(&fmt_ctx, input_file, nullptr, nullptr) != 0) {
//         std::cerr << "Could not open input file" << std::endl;
//         return;
//     }

//     // Find the video stream
//     avformat_find_stream_info(fmt_ctx, nullptr);
//     const AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
//     {

//         const AVCodec* c = nullptr;
//         void* i = nullptr;
//         std::stringstream ss;
//         std::cout << "\n";
//         while ((c = av_codec_iterate(&i))) 
//         {
//             // std::cout << c->name << "\t";
//             if (strcmp(c->name, "mpeg1video") == 0)
//             {
                
//                 // std::cout << c->name << "\t";
//                 break;
//             }
//             // ss << std::string(c->name) << "\t";
//             // LOG_WARNING("Codec: {}", c->name);
//         }
//         codec = c;
//     }
//     if (!codec) {
//         std::cerr << "Codec MPEG1 not found" << std::endl;
//         return;
//     }

//     // Set up codec context
//     codec_ctx = avcodec_alloc_context3(codec);
//     if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
//         std::cerr << "Failed to open codec" << std::endl;
//         return;
//     }

//     // Prepare output RTP stream
//     AVFormatContext* output_fmt_ctx = nullptr;
//     avformat_alloc_output_context2(&output_fmt_ctx, nullptr, "rtp", rtp_url.c_str());

//     if (!output_fmt_ctx) {
//         std::cerr << "Could not create output context" << std::endl;
//         return;
//     }

//     // Prepare the video packet
//     packet = av_packet_alloc();

//     // Read frames and send them over RTP
//     while (av_read_frame(fmt_ctx, packet) >= 0) {
//         if (packet->stream_index == 0) {
//             av_interleaved_write_frame(output_fmt_ctx, packet);
//         }
//         av_packet_unref(packet);
//     }

//     avformat_close_input(&fmt_ctx);
//     avformat_free_context(output_fmt_ctx);
//     avcodec_free_context(&codec_ctx);
//     av_packet_free(&packet);
// }

// // Callback function to handle SDP offer/answer exchange
// void VideoStream::handle_signaling(std::shared_ptr<rtc::PeerConnection> pc) {
//     auto dc = pc->createDataChannel("data");
//     dc->onOpen([]() {
//         std::cout << "DataChannel opened!" << std::endl;
//     });

//     pc->onLocalCandidate([pc](const rtc::Candidate& candidate) {
//         std::cout << "New ICE Candidate: " << candidate.candidate() << std::endl;
//         // Send the candidate to the remote peer using your signaling mechanism (e.g., WebSockets)
//     });

//     // Handle SDP offer/answer
//     pc->onLocalDescription([pc](const rtc::Description& description) {
//         std::cout << "Local SDP: " << description.typeString() << std::endl;
//         // Send SDP to the remote peer via your signaling server
//     });

//     // Set the remote description with the SDP received from the other peer
//     // Example: pc->setRemoteDescription(rtc::Description(remote_sdp, "answer"));
// }

// void VideoStream::start_rtc_connection() {
//     // Create configuration and ICE servers
//     rtc::Configuration config;
//     config.iceServers.push_back({"stun:stun.l.google.com:19302"});

//     // Create a peer connection
//     auto pc = std::make_shared<rtc::PeerConnection>(config);

//     // Set up signaling (SDP and ICE candidate exchange)
//     handle_signaling(pc);

//     // Create video track and send over RTP
//     // auto video_track = pc->addTrack(rtc::Description::Direction::SendRecv, "video");
//     rtc::Description::Media videoDesc("video", 96, rtc::Description::Direction::SendRecv);
//     auto video_track = pc->addTrack(videoDesc);

    
//     // Start FFmpeg in a separate thread to stream RTP
//     std::thread ffmpeg_thread([&]() {
//         start_ffmpeg_rtp_stream("rtp://127.0.0.1:5004");
//     });

//     ffmpeg_thread.join();
// }


// VideoStream::VideoStream()
// {
//     start_rtc_connection();

//     return;
// 	const char* codec_name = "vp8";
// 	const char* ip_address = "127.0.0.1"; // Destination IP for streaming
// 	int32_t port = 12345;                     // Destination port for streaming

// 	LOG_CRITICAL("ffmpeg version: {}", av_version_info());
// 	const AVCodec* c = nullptr;
// 	void* i = nullptr;
// 	std::stringstream ss;
// 	std::cout << "\n";
// 	while ((c = av_codec_iterate(&i))) 
// 	{
// 		// std::cout << c->name << "\t";
// 		if (strcmp(c->name, "mpeg1video") == 0)
// 		{
			
// 			// std::cout << c->name << "\t";
// 			break;
// 		}
// 		// ss << std::string(c->name) << "\t";
// 		// LOG_WARNING("Codec: {}", c->name);
// 	}
// 	// LOG_WARNING("codecs: {}", ss.str());

// 	// Initialize FFmpeg
// 	avformat_network_init();

// 	// Find the VP9 encoder
// 	// const AVCodec* codec = avcodec_find_encoder_by_name(codec_name);
// 	const AVCodec* codec = c;
// 	if (codec == nullptr) 
// 	{
// 		throw std::runtime_error("codec not found");
// 	}

// 	// Allocate codec context
// 	_codec_ctx = avcodec_alloc_context3(codec);
// 	if (!_codec_ctx) 
// 	{
// 		throw std::runtime_error("Could not allocate video codec context");
// 	}

// 	int32_t width, height;
// 	VulkanApp::Instance().getResolution(width, height);

// 	LOG_CRITICAL("width: {}, height: {}", width, height);

// 	// Set up codec parameters
// 	_codec_ctx->bit_rate = 100000;         // Bitrate: 500kbps
// 	_codec_ctx->width = width / 4;               // Video width
// 	_codec_ctx->height = height / 4;              // Video height
// 	_codec_ctx->time_base = (AVRational){1, 30}; // Frame rate: 30 fps
// 	_codec_ctx->framerate = (AVRational){30, 1};
// 	_codec_ctx->gop_size = 10;             // GOP size
// 	_codec_ctx->max_b_frames = 1;          // Maximum B-frames
// 	// _codec_ctx->pix_fmt = AV_PIX_FMT_GRAY8; // Pixel format
// 	_codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P; // Pixel format

// 	// Open the codec
// 	if (avcodec_open2(_codec_ctx, codec, nullptr) < 0) 
// 	{
// 		avcodec_free_context(&_codec_ctx);
// 		_codec_ctx = nullptr;
// 		throw std::runtime_error("Could not open codec");
// 	}

// 	// Create UDP socket for sending the stream	
// 	_udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
// 	if (_udp_socket < 0) 
// 	{
// 		avcodec_free_context(&_codec_ctx);
// 		_codec_ctx = nullptr;
// 		throw std::runtime_error("Socket creation failed");
// 	}

// 	// struct sockaddr_in _addr;
// 	memset(&_addr, 0, sizeof(_addr));
// 	_addr.sin_family = AF_INET;
// 	_addr.sin_port = htons(port);
// 	if (inet_pton(AF_INET, ip_address, &_addr.sin_addr) <= 0) 
// 	{
// 		avcodec_free_context(&_codec_ctx);
// 		_codec_ctx = nullptr;
// 		throw std::runtime_error("Invalid address");
// 	}

// 	if (_udp_socket < 0) 
// 	{
// 		avcodec_free_context(&_codec_ctx);
// 		_codec_ctx = nullptr;
// 		throw std::runtime_error("Failed to create UDP socket");
// 	}



// // // Set up the format context for UDP streaming
// // avformat_alloc_output_context2(&_format_ctx, nullptr, "mpegts", "udp://127.0.0.1:12345");

// // // Create the video stream
// // _video_stream = avformat_new_stream(_format_ctx, nullptr);
// // avcodec_parameters_from_context(video_stream->codecpar, codec_ctx);

// // // Open the UDP output
// // avio_open(&format_ctx->pb, "udp://127.0.0.1:12345", AVIO_FLAG_WRITE);
// // avformat_write_header(format_ctx, nullptr);

// // // Encode and send frames (assuming you have raw RGB frames)
// // AVPacket packet = {0};
// // av_init_packet(&packet);

// // // Fill packet with encoded frame and send
// // avcodec_send_frame(codec_ctx, frame);  // Assuming 'frame' is the raw RGB frame
// // avcodec_receive_packet(codec_ctx, &packet);
// // av_interleaved_write_frame(format_ctx, &packet);
// // av_packet_unref(&packet);

// // // Write trailer and close
// // av_write_trailer(format_ctx);



// 	avformat_alloc_output_context2(&_format_ctx, nullptr, "rtp", nullptr);

// 	if (_format_ctx == nullptr)
// 	{
// 		throw std::runtime_error("couldnt find format ctx");
// 	}

// 	_video_stream = avformat_new_stream(_format_ctx, nullptr);
// 	if (_video_stream == nullptr) 
// 	{
// 		throw std::runtime_error("Failed to create new stream");
// 	}


// 	// Copy codec parameters to the stream
// 	if (avcodec_parameters_from_context(_video_stream->codecpar, _codec_ctx) < 0) 
// 	{
// 		throw std::runtime_error("Could not copy codec parameters to stream");
// 	}

// 	// Opening udp connection
// 	avio_open(&_format_ctx->pb, "rtp://127.0.0.1:12345", AVIO_FLAG_WRITE);
// 	avformat_write_header(_format_ctx, nullptr);

// 	encode_and_send_video();

// 	// TODO: called once the streams ends
// 	av_write_trailer(_format_ctx);
// }

// VideoStream::~VideoStream()
// {
// 	// Clean up
// 	if (_udp_socket >= 0)
// 		close(_udp_socket);
// 	if (_codec_ctx != nullptr)
// 		avcodec_free_context(&_codec_ctx);
// 	avformat_network_deinit();
// }


// void VideoStream::encode_and_send_video() 
// {
// 	AVCodecContext* codec_ctx = _codec_ctx;
// 	AVFormatContext* format_ctx = _format_ctx;
// 	int udp_socket = _udp_socket;
// 	struct sockaddr_in* addr = &_addr;
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
//                 // frame->data[0][y * frame->linesize[0] + x] = x + y + frame_count * 3;  // Y
//                 // if (x < codec_ctx->width / 2 && y < codec_ctx->height / 2) {
//                 //     frame->data[1][y * frame->linesize[1] + x] = 128 + frame_count * 2;  // U
//                 //     frame->data[2][y * frame->linesize[2] + x] = 64 + frame_count * 5;   // V
//                 // }
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

// 		LOG_TRACE("sent frame");
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

// // extern "C" {
// //     #include <libavcodec/avcodec.h>
// //     #include <libavformat/avformat.h>
// //     #include <libavutil/imgutils.h>
// //     #include <libavutil/opt.h>
// //     #include <libswscale/swscale.h>
// // }

// // #include <iostream>
// // #include <cstring>
// // #include <sys/socket.h>
// // #include <arpa/inet.h>
// // #include <unistd.h>

// // // Function to create a UDP socket and set up the destination address
// // int create_udp_socket(const char* ip, int port, struct sockaddr_in* addr) {
// //     int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
// //     if (sockfd < 0) {
// //         perror("Socket creation failed");
// //         return -1;
// //     }

// //     memset(addr, 0, sizeof(*addr));
// //     addr->sin_family = AF_INET;
// //     addr->sin_port = htons(port);
// //     if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
// //         perror("Invalid address");
// //         return -1;
// //     }

// //     return sockfd;
// // }

// // // Function to encode and send frames using VP9
// // void encode_and_send_video(AVCodecContext* codec_ctx, int udp_socket, struct sockaddr_in* addr) {
// //     AVFrame* frame = av_frame_alloc();
// //     AVPacket* pkt = av_packet_alloc();
    
// //     if (!frame || !pkt) {
// //         std::cerr << "Could not allocate frame or packet" << std::endl;
// //         return;
// //     }

// //     frame->format = codec_ctx->pix_fmt;
// //     frame->width = codec_ctx->width;
// //     frame->height = codec_ctx->height;
// //     av_frame_get_buffer(frame, 32); // Align frame buffer

// //     int frame_count = 0;

// //     while (frame_count < 100) { // Encode 100 frames for the example
// //         // Simulate a video source by filling the frame with dummy data
// //         av_frame_make_writable(frame);
// //         for (int y = 0; y < codec_ctx->height; y++) {
// //             for (int x = 0; x < codec_ctx->width; x++) {
// //                 frame->data[0][y * frame->linesize[0] + x] = x + y + frame_count * 3;  // Y
// //                 if (x < codec_ctx->width / 2 && y < codec_ctx->height / 2) {
// //                     frame->data[1][y * frame->linesize[1] + x] = 128 + frame_count * 2;  // U
// //                     frame->data[2][y * frame->linesize[2] + x] = 64 + frame_count * 5;   // V
// //                 }
// //             }
// //         }

// //         frame->pts = frame_count;

// //         // Send the frame for encoding
// //         int ret = avcodec_send_frame(codec_ctx, frame);
// //         if (ret < 0) {
// //             std::cerr << "Error sending a frame for encoding" << std::endl;
// //             break;
// //         }

// //         // Receive the encoded packet and send it over the network
// //         while (ret >= 0) {
// //             ret = avcodec_receive_packet(codec_ctx, pkt);
// //             if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
// //                 break;
// //             } else if (ret < 0) {
// //                 std::cerr << "Error during encoding" << std::endl;
// //                 return;
// //             }

// //             // Send packet over the network (UDP)
// //             if (sendto(udp_socket, pkt->data, pkt->size, 0, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
// //                 perror("Failed to send packet");
// //             }

// //             av_packet_unref(pkt);
// //         }

// //         frame_count++;
// //     }

// //     // Flush the encoder
// //     avcodec_send_frame(codec_ctx, nullptr);
// //     while (avcodec_receive_packet(codec_ctx, pkt) == 0) {
// //         if (sendto(udp_socket, pkt->data, pkt->size, 0, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
// //             perror("Failed to send packet");
// //         }
// //         av_packet_unref(pkt);
// //     }

// //     av_frame_free(&frame);
// //     av_packet_free(&pkt);
// // }

// // int main() {
// //     const char* codec_name = "libvpx-vp9";
// //     const char* ip_address = "127.0.0.1"; // Destination IP for streaming
// //     int port = 12345;                     // Destination port for streaming

// //     // Initialize FFmpeg
// //     avcodec_register_all();
// //     avformat_network_init();

// //     // Find the VP9 encoder
// //     const AVCodec* codec = avcodec_find_encoder_by_name(codec_name);
// //     if (!codec) {
// //         std::cerr << "Codec not found: " << codec_name << std::endl;
// //         return -1;
// //     }

// //     // Allocate codec context
// //     AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
// //     if (!codec_ctx) {
// //         std::cerr << "Could not allocate video codec context" << std::endl;
// //         return -1;
// //     }

// //     // Set up codec parameters
// //     codec_ctx->bit_rate = 500000;         // Bitrate: 500kbps
// //     codec_ctx->width = 640;               // Video width
// //     codec_ctx->height = 360;              // Video height
// //     codec_ctx->time_base = (AVRational){1, 30}; // Frame rate: 30 fps
// //     codec_ctx->framerate = (AVRational){30, 1};
// //     codec_ctx->gop_size = 10;             // GOP size
// //     codec_ctx->max_b_frames = 1;          // Maximum B-frames
// //     codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P; // Pixel format

// //     // Open the codec
// //     if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
// //         std::cerr << "Could not open codec" << std::endl;
// //         avcodec_free_context(&codec_ctx);
// //         return -1;
// //     }

// //     // Create UDP socket for sending the stream
// //     struct sockaddr_in addr;
// //     int udp_socket = create_udp_socket(ip_address, port, &addr);
// //     if (udp_socket < 0) {
// //         std::cerr << "Failed to create UDP socket" << std::endl;
// //         avcodec_free_context(&codec_ctx);
// //         return -1;
// //     }

// //     // Start encoding and sending video
// //     encode_and_send_video(codec_ctx, udp_socket, &addr);

// //     // Clean up
// //     close(udp_socket);
// //     avcodec_free_context(&codec_ctx);
// //     avformat_network_deinit();

// //     return 0;
// // }

