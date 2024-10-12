
#include "FFMPEGStream.h"

FFMPEGStream::FFMPEGStream()
{

	
	// Initialize codec for hardware-accelerated VP9 encoding (e.g., Intel Quick Sync)
	const AVCodec* codec = avcodec_find_encoder_by_name("vp9_qsv");
	codec_ctx = avcodec_alloc_context3(codec);

	// Set codec parameters and initialize hardware frames context
	// Use av_hwdevice_ctx_create() to set up VAAPI or QSV hardware acceleration

}

FFMPEGStream::~FFMPEGStream()
{

}
