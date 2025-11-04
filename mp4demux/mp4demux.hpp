/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef parsemp4_hpp
#define parsemp4_hpp

#include <cstdint>
#include <stddef.h>
#include <vector>
#include <assert.h>
#include <inttypes.h>
#include <cstdio>
#include <cstring> // for memcpy
#include <gst/app/gstappsrc.h>

#define PRINTF(...)
//#define PRINTF printf

#define MultiChar_Constant(Text) \
	( (static_cast<uint32_t>(Text[0]) << 24) | \
		(static_cast<uint32_t>(Text[1]) << 16) | \
		(static_cast<uint32_t>(Text[2]) << 8)  | \
		(static_cast<uint32_t>(Text[3])) )
//#Conversion of text in to decimal value

struct Mp4Sample
{
	const uint8_t *ptr;
	size_t len;
	double pts;
	double dts;
	double duration;
};

class InitializationHeaderInfo
{
public:
	// audio
	uint16_t channel_count;
	uint16_t samplesize;
	uint16_t samplerate;
	uint8_t object_type_id;
	uint8_t stream_type;
	uint8_t upStream;
	uint16_t buffer_size;
	uint32_t maxBitrate;
	uint32_t avgBitrate;
	
	// video
	uint16_t width;
	uint16_t height;
	uint16_t frame_count;
	uint16_t depth;
	uint32_t horizresolution;
	uint32_t vertresolution;
	
	// common
	uint32_t stream_format;
	uint32_t data_reference_index;
	uint32_t codec_type;
	char *compressor_name;
	size_t codec_data_len;
	uint8_t *codec_data;
	
	InitializationHeaderInfo():
	channel_count(), samplesize(), samplerate(),
	width(), height(), frame_count(), depth(), horizresolution(), vertresolution(),
	stream_format(), data_reference_index(), codec_type(), codec_data_len(), codec_data()
	{
	}
	
	~InitializationHeaderInfo()
	{
		if( codec_data )
		{
			free( codec_data );
		}
	}
};

class Mp4Demux
{
public:
	uint32_t timescale;
	
private:
	InitializationHeaderInfo info;
	std::vector<Mp4Sample> samples;
	const uint8_t *moof_ptr; // base address for sample data
	const uint8_t *ptr; // parsing state

	uint8_t version;
	uint32_t flags;
	uint64_t baseMediaDecodeTime;
	uint32_t fragment_duration;
	uint32_t track_id;
	uint64_t base_data_offset;
	uint32_t default_sample_description_index;
	uint32_t default_sample_duration;
	uint32_t default_sample_size;
	uint32_t default_sample_flags;
	uint64_t creation_time;
	uint64_t modification_time;
	uint32_t duration;
	uint32_t rate;
	uint32_t volume;
	int32_t matrix[9];
	uint16_t layer;
	uint16_t alternate_group;
	uint32_t width;
	uint32_t height;
	uint16_t language;
	/**
	* @brief Reads a specified number of bytes from the current buffer or stream.
	*
	* This function reads the next `n` bytes from the buffer, interprets them
	* as an unsigned integer, and advances the read position accordingly.
	*
	* @param n The number of bytes to read (up to 8).
	* @return uint64_t The value represented by the read bytes.
	*/
	uint64_t ReadBytes( int n )
	{
		uint64_t rc = 0;
		for( int i=0; i<n; i++ )
		{
			rc <<= 8;
			rc |= *ptr++;
		}
		return rc;
	}
	/**
	* @brief Reads an unsigned 16-bit integer from the current buffer or stream.
	*
	* This function interprets the next 2 bytes as a big-endian unsigned 16-bit
	* integer and advances the read position accordingly.
	*
	* @return uint16_t The 16-bit unsigned integer read from the buffer.
	*/
	uint16_t ReadU16()
	{
		return (uint16_t)ReadBytes(2);
	}
	/**
	* @brief Reads an unsigned 32-bit integer from the current buffer or stream.
	*
	* This function interprets the next 4 bytes as a big-endian unsigned 32-bit
	* integer and advances the read position accordingly.
	*
	* @return uint32_t The 32-bit unsigned integer read from the buffer.
	*/	
	uint32_t ReadU32()
	{
		return (uint32_t)ReadBytes(4);
	}
	/**
	* @brief Reads a signed 32-bit integer from the current buffer or stream.
	*
	* This function interprets the next 4 bytes as a big-endian signed 32-bit
	* integer and advances the read position accordingly.
	*
	* @return int32_t The 32-bit signed integer read from the buffer.
	*/
	int32_t ReadI32()
	{
		return (int32_t)ReadBytes(4);
	}
	/**
	* @brief Reads an unsigned 64-bit integer from the current buffer or stream.
	*
	* This function interprets the next 8 bytes as a big-endian unsigned 64-bit
	* integer and advances the read position accordingly.
	*
	* @return uint64_t The 64-bit unsigned integer read from the buffer.
	*/
	uint64_t ReadU64()
	{
		return ReadBytes(8);
	}
	/**
	* @brief Reads and parses the header of the current media or MP4 box.
	*
	* This function extracts essential header information, such as box type,
	* size, and other metadata, preparing the parser for processing the
	* contained data.
	*/
	void ReadHeader( void )
	{
		version = *ptr++;
		flags = (uint32_t)ReadBytes(3);
	}
	/**
	* @brief Skips a specified number of bytes in the current buffer or stream.
	*
	* This function advances the read position by `len` bytes without processing
	* the skipped data. It is typically used to bypass unused or reserved fields
	* in a media stream or file.
	*
	* @param len The number of bytes to skip.
	*/
	void SkipBytes( size_t len )
	{
		PRINTF( "skipping %zu bytes\n", len );
		ptr += len;
	}
	/**
	* @brief Parses a Movie Fragment Header Box ('mfhd') in a fragmented MP4 (fMP4) stream.
	*
	* This function extracts global metadata for a movie fragment, such as the
	* sequence number, which is used to track the order of fragments and ensure
	* correct playback sequencing.
	*/
	void parseMovieFragmentHeaderBox( void )
	{
		ReadHeader();
		uint32_t sequence_number = ReadU32();
		(void)sequence_number;
		PRINTF( "sequence_number=%" PRIu32 "\n", sequence_number );
	}
	/**
	* @brief Parses a Track Fragment Header Box ('tfhd') in a fragmented MP4 (fMP4) stream.
	*
	* This function extracts track fragment-level metadata such as default sample
	* duration, default sample size, default sample flags, and track ID, which
	* are used to properly interpret and decode samples in the fragment.
	*/
	void parseTrackFragmentHeaderBox( void )
	{
		ReadHeader();
		track_id = ReadU32();
		PRINTF( "track_id=%" PRIu32 "\n", track_id );
		if (flags & 0x00001)
		{
			base_data_offset = ReadU64();
			PRINTF( "base_data_offset=%" PRIu64 "\n", base_data_offset );
		}
		if (flags & 0x00002)
		{
			default_sample_description_index = ReadU32();
			PRINTF( "default_sample_description_index=%" PRIu32 "\n", default_sample_description_index );
		}
		if (flags & 0x00008)
		{
			default_sample_duration = ReadU32();
			PRINTF( "default_sample_duration=%" PRIu32 "\n", default_sample_duration );
		}
		if (flags & 0x00010)
		{
			default_sample_size = ReadU32();
			PRINTF( "default_sample_size=%" PRIu32 "\n", default_sample_size );
		}
		if (flags & 0x00020)
		{
			default_sample_flags = ReadU32();
			PRINTF( "default_sample_flags=%" PRIu32 "\n", default_sample_flags );
		}
	}
	/**
	* @brief Parses a Track Fragment Base Media Decode Time Box ('tfdt') in a fragmented MP4 (fMP4) stream.
	*
	* This function extracts the base media decode time for a track fragment,
	* which is used to compute presentation timestamps (PTS) and ensure proper
	* media synchronization during playback.
	*/
	void parseTrackFragmentBaseMediaDecodeTimeBox( void  )
	{
		ReadHeader();
		int sz = (version==1)?8:4;
		baseMediaDecodeTime  = ReadBytes(sz);
		PRINTF( "baseMediaDecodeTime: %" PRIu64 "\n", baseMediaDecodeTime );
	}
	/**
	* @brief Parses a Track Fragment Run Box ('trun') in a fragmented MP4 (fMP4) stream.
	*
	* This function extracts sample information such as sample duration,
	* size, flags, and composition time offsets for a track fragment, which
	* is essential for proper decoding and playback of fragmented media.
	*/	
	void parseTrackFragmentRunBox( void )
	{
		ReadHeader();
		uint32_t sample_count = ReadU32();
		PRINTF( "sample_number=%" PRIu32 "\n", sample_count );
		const unsigned char *data_ptr = moof_ptr;
		//0xE01
		if( flags & 0x0001 )
		{ // offset from start of Moof box field
			int32_t data_offset = ReadI32();
			PRINTF( "data_offset=%" PRIu32 "\n", data_offset );
			data_ptr += data_offset;
		}
		else
		{ // mandatory field? should never reach here
			assert(0);
		}
		uint32_t sample_flags = 0;
		if(flags & 0x0004)
		{
			sample_flags = ReadU32();
			(void)sample_flags;
			PRINTF( "first_sample_flags=0x%" PRIx32 "\n", sample_flags );
		}
		uint64_t dts = baseMediaDecodeTime;
		for( unsigned int i=0; i<sample_count; i++ )
		{
			struct Mp4Sample sample;
			sample.ptr = data_ptr;
			sample.len = default_sample_size;
			sample.pts = 0.0;
			sample.dts = 0.0;
			sample.duration = 0.0;
			PRINTF( "[FRAME] %d\n", i );
			uint32_t sample_duration = default_sample_duration;
			if (flags & 0x0100)
			{
				sample_duration = ReadU32();
				PRINTF( "sample_duration=%" PRIu32 "\n", sample_duration );
				sample.duration = sample_duration / (double)timescale;
			}
			if (flags & 0x0200)
			{
				uint32_t sample_size = ReadU32();
				PRINTF( "sample_size=%" PRIu32 "\n", sample_size );
				sample.len = sample_size;
			}
			data_ptr += sample.len;
			if (flags & 0x0400)
			{ // rarely present?
				sample_flags = ReadU32();
				(void)sample_flags;
				PRINTF( "sample_flags=0x%" PRIx32 "\n", sample_flags );
			}
			int32_t sample_composition_time_offset = 0;
			if (flags & 0x0800)
			{ // for samples where pts and dts differ (overriding 'trex')
				sample_composition_time_offset = ReadI32();
				PRINTF( "sample_composition_time_offset=%" PRIi32 "\n", sample_composition_time_offset );
			}
			sample.dts = dts/(double)timescale;
			sample.pts = (dts+sample_composition_time_offset)/(double)timescale;
			PRINTF( "dts=%f pts=%f\n", sample.dts, sample.pts );
			dts += sample_duration;
			samples.push_back( sample );
		}
	}
	/**
	* @brief Parses the Movie Header Box ('mvhd') in an MP4 stream.
	*
	* This function extracts global movie metadata, including timescale,
	* duration, and playback rate, which are essential for overall
	* media synchronization and timing calculations.
	*/
	void parseMovieHeaderBox( void )
	{
		ReadHeader();
		int sz = (version==1)?8:4;
		creation_time = ReadBytes(sz);
		modification_time = ReadBytes(sz);
		timescale = ReadU32();
		duration = ReadU32();
		rate = ReadU32();
		volume = ReadU32(); // fixed point
		ptr += 8;
		for( int  i=0; i<9; i++ )
		{
			matrix[i] = ReadI32();
		}
	}
	/**
	* @brief Parses the Movie Extends Header Box ('mehd') in an MP4 stream.
	*
	* This function extracts the default duration and other global metadata
	* for a fragmented MP4 (fMP4) movie. The information is used to compute
	* overall movie timing and synchronization.
	*/
	void parseMovieExtendsHeader( void )
	{
		ReadHeader();
		fragment_duration = ReadU32();
	}
	/**
	* @brief Parses a Track Extends Box ('trex') in an MP4 stream.
	*
	* This function extracts default sample description and duration information
	* for a track, which is used in fragmented MP4 (fMP4) streams to initialize
	* track properties before sample data is processed.
	*/
	void parseTrackExtendsBox( void )
	{
		ReadHeader();
		track_id = ReadU32();
		default_sample_description_index = ReadU32();
		default_sample_duration = ReadU32();
		default_sample_size = ReadU32();
		default_sample_flags = ReadU32();
	}
	/**
	* @brief Parses a Track Header Box ('tkhd') in an MP4 stream.
	*
	* This function extracts metadata related to a specific track, such as
	* track ID, duration, layer, alternate group, and other attributes
	* necessary for track identification and playback.
	*/	
	void parseTrackHeader( void )
	{
		ReadHeader();
		int sz = (version==1)?8:4;
		creation_time = ReadBytes(sz);
		modification_time = ReadBytes(sz);
		track_id = ReadU32();
		ptr += 20+sz; // duration, layer, alternate_group, volume
		for( int i=0; i<9; i++ )
		{
			matrix[i] = ReadI32();
		}
		width = ReadU32(); // fixed point
		height = ReadU32(); // fixed point
	}
	/**
	* @brief Parses a Media Header Box ('mdhd') in an MP4 stream.
	*
	* This function extracts timing and media-related metadata from the
	* Media Header Box, including timescale, duration, and other relevant
	* fields used for media synchronization.
	*/
	void parseMediaHeaderBox( void )
	{
		ReadHeader();
		int sz = (version==1)?8:4;
		creation_time = ReadBytes(sz);
		modification_time = ReadBytes(sz);
		timescale = ReadU32();
		duration = ReadU32();
		language = ReadU16();
	}
	/**
	* @brief Parses a Sample Description Box ('stsd') in an MP4 stream.
	*
	* This function interprets the contents of a Sample Description Box, which
	* contains codec-specific information, sample entries, and related metadata.
	* The `indent` parameter can be used for structured logging or debug output.
	*
	* @param next   Pointer to the buffer containing the Sample Description Box data.
	* @param indent Indentation level for debug or logging purposes.
	*/
	void parseSampleDescriptionBox( const uint8_t *next, int indent )
	{ // stsd
		ReadHeader();
		uint32_t count = ReadU32();
		assert( count == 1 );
		DemuxHelper(next, indent+1);
	}
	/**
	* @brief Parses the stream format information for a given codec type.
	*
	* This function interprets the format data of a media stream, such as
	* codec parameters, sample descriptors, or other relevant metadata.
	* The `indent` parameter can be used for structured logging or debug output.
	*
	* @param type   The codec or stream type identifier (e.g., H.264, H.265).
	* @param next   Pointer to the buffer containing the stream format data.
	* @param indent Indentation level for debug or logging purposes.
	*/	
	void parseStreamFormat( uint32_t type, const uint8_t *next, int indent )
	{
		int pad;
		
		info.stream_format = type;
		switch( info.stream_format )
		{
			case MultiChar_Constant("hev1"):
			case MultiChar_Constant("avc1"): 
			case MultiChar_Constant("hvc1"):
				SkipBytes(4); // always zero?
				info.data_reference_index = ReadU32();
				SkipBytes(16); // always zero?
				info.width = ReadU16();
				info.height = ReadU16();
				info.horizresolution = ReadU32();
				info.vertresolution = ReadU32();
				SkipBytes(4);
				info.frame_count = ReadU16();
				SkipBytes(32); // compressor_name
				info.depth = ReadU16();
				pad = ReadU16();
				assert( pad == 0xffff );
				break;
				
			case MultiChar_Constant("mp4a"): 
			case MultiChar_Constant("ec-3"): 
				SkipBytes(4); // zero
				info.data_reference_index = ReadU32();
				SkipBytes(8); // zero
				info.channel_count = ReadU16();
				info.samplesize = ReadU16();
				SkipBytes(4); // zero
				info.samplerate = ReadU16();
				SkipBytes(2); // zero
				break;
				
			default:
				PRINTF( "unk stream_format\n" );
				assert(0);
				break;
		}
		DemuxHelper( next, indent+1 );
	}
	/**
	* @brief Reads the length of the next data element or sample.
	*
	* This function returns the length of the next unit of data to be processed,
	* which may correspond to a sample, box, or other codec-specific structure.
	*
	* @return int The length of the next data element in bytes.
	*/
	int readLen( void )
	{
		int rc = 0;
		for(;;)
		{
			unsigned char octet = *ptr++;
			rc <<= 7;
			rc |= octet&0x7f;
			if( (octet&0x80)==0 ) return rc;
		}
	}
	/**
	* @brief Helper function to parse codec configuration data.
	*
	* This function assists in interpreting codec-specific configuration information
	* from the provided buffer. It is typically called internally by
	* `parseCodecConfiguration()` to extract codec parameters required for decoding.
	*
	* @param next Pointer to the buffer containing codec configuration data.
	*/
	void parseCodecConfigHelper( const uint8_t *next )
	{
		while( ptr < next )
		{
			uint32_t tag = *ptr++;
			uint32_t len = readLen();
			const uint8_t *end = ptr + len;
			switch( tag )
			{
				case 0x03:
					PRINTF( "ES_Descriptor: ");
					SkipBytes(3);
					parseCodecConfigHelper( end );
					break;
					
				case 0x04:
					PRINTF( "DecoderConfigDescriptor:\n");
					info.object_type_id = *ptr++;
					info.stream_type = *ptr++; // >>2
					info.upStream = *ptr++;
					info.buffer_size = ReadU16();
					info.maxBitrate = ReadU32();
					info.avgBitrate = ReadU32();
					PRINTF( "\tmaxBitrate=%" PRIu32 "\n", info.maxBitrate );
					PRINTF( "\tavgBitrate=%" PRIu32 "\n", info.avgBitrate );
					parseCodecConfigHelper( end );
					break;
					
				case 0x05:
					PRINTF( "DecodeSpecificInfo:\n") ;
					info.codec_data_len = len;
					info.codec_data = (uint8_t *)malloc( len );
					if( info.codec_data )
					{
						memcpy( info.codec_data, ptr, len );
						ptr += len;
					}
					break;
					
				case 0x06:
					PRINTF( "SlConfigDescriptor: ");
					SkipBytes( len );
					break;
					
				default:
					assert(0);
					break;
			}
			assert( ptr == end );
			ptr = end;
		}
	}
	
	/**
	* @brief Parses codec-specific configuration data.
	*
	* This function interprets the codec configuration information pointed to
	* by `next` for a given codec type. It is typically used during MP4 or
	* media initialization to extract codec parameters needed for decoding.
	*
	* @param type The codec type identifier (e.g., H.264, H.265, etc.).
	* @param next Pointer to the buffer containing codec configuration data.
	*/
	void parseCodecConfiguration( uint32_t type, const uint8_t *next )
	{
		info.codec_type = type;
		if( type == MultiChar_Constant("esds") )
		{
			SkipBytes(4);
			parseCodecConfigHelper( next );
		}
		else
		{
			info.codec_data_len = next - ptr;
			info.codec_data = (uint8_t *)malloc( info.codec_data_len );
			if( info.codec_data )
			{
				memcpy( info.codec_data, ptr, info.codec_data_len );
			}
		}
	}
	
	void DemuxHelper( const uint8_t *fin, int indent )
	{
		while( ptr < fin )
		{
			uint32_t size = ReadU32();
			PRINTF( "size=%" PRIu32 "\n", size );
			const uint8_t *next = ptr+size-4;
			uint32_t type = ReadU32();
			for( int i=0; i<indent; i++ )
			{
				PRINTF( "\t" );
			}
			PRINTF( "'%c%c%c%c'\n",
				   (type>>24)&0xff, (type>>16)&0xff, (type>>8)&0xff, type&0xff );
			switch( type )
			{
				case MultiChar_Constant("hev1"):
				case MultiChar_Constant("hvc1"): 
				case MultiChar_Constant("avc1"):
				case MultiChar_Constant("mp4a"): 
				case MultiChar_Constant("ec-3"): 
					parseStreamFormat( type, next, indent );
					break;
					
				case MultiChar_Constant("hvcC"): 
				case MultiChar_Constant("dec3"): 
				case MultiChar_Constant("avcC"): 
				case MultiChar_Constant("esds"): // Elementary Stream Descriptot Box
					parseCodecConfiguration( type, next );
					break;
					
				case MultiChar_Constant("ftyp"): //  FileType Box
					/*
					 major_brand // 4 chars
					 minor_version // 4 bytes
					 compatible_brands // 16 bytes, uint32be
					 */
					break;
					
				case MultiChar_Constant("mfhd"): 
					parseMovieFragmentHeaderBox();
					break;
					
				case MultiChar_Constant("tfhd"): 
					parseTrackFragmentHeaderBox();
					break;
					
				case MultiChar_Constant("trun"): 
					parseTrackFragmentRunBox();
					break;
					
				case MultiChar_Constant("tfdt"): 
					parseTrackFragmentBaseMediaDecodeTimeBox();
					break;
					
				case MultiChar_Constant("mvhd"):
					parseMovieHeaderBox();
					break;
					
				case MultiChar_Constant("mehd"): 
					parseMovieExtendsHeader();
					break;
					
				case MultiChar_Constant("trex"): 
					parseTrackExtendsBox();
					break;
					
				case MultiChar_Constant("tkhd"): 
					parseTrackHeader();
					break;
					
				case MultiChar_Constant("mdhd"): 
					parseMediaHeaderBox();
					break;
					
				case MultiChar_Constant("hdlr"): // Handler Reference Box
					/*
					 handler	vide
					 name	Bento4 Video Handler
					 */
					break;
					
				case MultiChar_Constant("vmhd"): // Video Media Header
					/*
					 graphicsmode	0
					 opcolor	0,0,0
					 */
					break;
					
				case MultiChar_Constant("smhd"): // Sound Media Header
					/*
					 balance	0
					 */
					break;
					
				case MultiChar_Constant("dref"): // Data Reference Box
					/*
					 url
					 */
					break;
					
				case MultiChar_Constant("stsd"): // Sample Description Box
					parseSampleDescriptionBox(next,indent);
					break;
					
				case MultiChar_Constant("stts"): // DecodingTimeToSample
					break;
				case MultiChar_Constant("stsc"): //  SampleToChunkBox
					break;
				case MultiChar_Constant("stsz"): //  SampleSizeBoxes
					break;
				case MultiChar_Constant("stco"): //  ChunkOffsets
					break;
				case MultiChar_Constant("stss"): //  Sync Sample
					break;
				case MultiChar_Constant("prft"): //  ProducerReferenceTime
					break;
				case MultiChar_Constant("edts"): //  Edit Box
					break;
				case MultiChar_Constant("fiel"): 
					break;
				case MultiChar_Constant("colr"): // Color Pattern Atom
					break;
				case MultiChar_Constant("pasp"): // Pixel Aspect Ratio
					/*
					00 00 04 f0 // hSpacing
					00 00 04 ef // vSpacing
					*/
					break;
				case MultiChar_Constant("btrt"): // Buffer Time to Render Time
					/*
					00 02 49 f0 // bufferSizeDB
					00 16 db 90 // maxBitrate
					00 15 5c c0 // avgBitrate
					*/
					break;
					
				case MultiChar_Constant("styp"): // Segment Type Box
				case MultiChar_Constant("sidx"): // Segment Index Box
				case MultiChar_Constant("udta"): // User Data Box
				case MultiChar_Constant("mdat"): // Movie Data Box
					break;

				case MultiChar_Constant("moof"):  // Movie Fragment Box
					moof_ptr = ptr-8;
					DemuxHelper(next, indent+1 ); // walk children
					break;
					
				case MultiChar_Constant("traf"): //  Track Fragment Boxes
				case MultiChar_Constant("moov"): //  Movie Boxes
				case MultiChar_Constant("trak"): //  Track Box
				case MultiChar_Constant("minf"): //  Media Information Container
				case MultiChar_Constant("dinf"): //  Data Information Box
				case MultiChar_Constant("mvex"): //  Movie Extends Box
				case MultiChar_Constant("mdia"): //  Media Box
				case MultiChar_Constant("stbl"): //  Sample Table Box
					DemuxHelper(next, indent+1 ); // walk children
					break;
										
				default:
					PRINTF( "unknown box type!\n" );
					break;
			}
			ptr = next;
		}
	}

public:
	Mp4Demux( const void *ptr, size_t len, uint32_t timescale=0 )
	{
		this->ptr = (const uint8_t *)ptr;
		this->moof_ptr = NULL;
		this->timescale = timescale;
		DemuxHelper( &this->ptr[len], 0 );
	}
	
	int count( void )
	{
		return (int)samples.size();
	}
	
	const uint8_t * getPtr( int part )
	{
		return samples[part].ptr;
	}
	/**
	* @brief Retrieves the length of the specified sample part.
	*
	* @param part Index of the sample to query.
	* @return size_t Length of the sample in bytes.
	*/
	size_t getLen( int part )
	{
		return samples[part].len;
	}
	
	/**
	* @brief Retrieves the presentation timestamp (PTS) of the specified sample part.
	*
	* @param part Index of the sample to query.
	* @return double The PTS of the sample.
	*/
	double getPts( int part )
	{
		return samples[part].pts;
	}
	/**
	* @brief Retrieves the decoding timestamp (DTS) of the specified sample part.
	*
	* @param part Index of the sample to query.
	* @return double The DTS of the sample.
	*/
	double getDts( int part )
	{
		return samples[part].dts;
	}
	/**
	* @brief Retrieves the duration of the specified sample part.
	*
	* @param part Index of the sample to query.
	* @return double Duration of the sample in the same time units as PTS/DTS.
	*/
	double getDuration( int part )
	{
		return samples[part].duration;
	}

	~Mp4Demux()
	{
	}
	
	/**
	* @brief Copy constructor for the Mp4Demux class (stub).
	*
	* This is a stub copy constructor and is not intended to be used.
	* Calling this constructor will trigger an assertion failure.
	*
	* @param other The #Mp4Demux instance to copy from.
	*/
	Mp4Demux(const Mp4Demux & other)
	{ // stub copy constructor
		assert(0);
	}
	
	/**
	* @brief Copy assignment operator for the Mp4Demux class.
	*
	* This operator copies the state and internal data from another
	* #Mp4Demux instance into this instance. It ensures that all
	* relevant members are properly duplicated.
	*
	* @param other The other #Mp4Demux instance to copy from.
	* @return Mp4Demux& Reference to this instance after assignment.
	*/
	Mp4Demux& operator=(const Mp4Demux & other)
	{ // stub move constructor
		assert(0);
	}
	
	/**
	* @brief Sets the capabilities (caps) on a GStreamer AppSrc element.
	*
	* This function configures the given #GstAppSrc element with the appropriate
	* media caps, such as format, resolution, framerate, and codec information,
	* required for downstream elements to process the stream correctly.
	*
	* @param appsrc Pointer to the #GstAppSrc element to configure.
	*/
	void setCaps( GstAppSrc *appsrc ) const
	{
		GstCaps * caps = NULL;
		GstBuffer *buf = gst_buffer_new_and_alloc(info.codec_data_len);
		gst_buffer_fill(buf, 0, info.codec_data, info.codec_data_len);
		switch( info.codec_type )
		{
			case 0x68766343: // 'hvcC'
				caps = gst_caps_new_simple(
										   "video/x-h265",
										   "stream-format", G_TYPE_STRING, "hvc1",
										   "alignment", G_TYPE_STRING, "au",
										   "codec_data", GST_TYPE_BUFFER, buf,
										   "width", G_TYPE_INT, info.width,
										   "height", G_TYPE_INT, info.height,
										   "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
										   NULL );
				break;
				
			case 0x61766343: // 'avcC'
				caps = gst_caps_new_simple(
										   "video/x-h264",
										   "stream-format", G_TYPE_STRING, "avc",
										   "alignment", G_TYPE_STRING, "au",
										   "codec_data", GST_TYPE_BUFFER, buf,
										   "width", G_TYPE_INT, info.width,
										   "height", G_TYPE_INT, info.height,
										   "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
										   NULL );
				break;
				
			case 0x65736473: // 'esds'
				caps = gst_caps_new_simple(
										   "audio/mpeg",
										   "mpegversion",G_TYPE_INT,4,
										   "framed", G_TYPE_BOOLEAN, TRUE,
										   "stream-format",G_TYPE_STRING,"raw", // FIXME
										   "codec_data", GST_TYPE_BUFFER, buf,
										   NULL );
				break;
				
			case 0x64656333: // 'dec3'
				caps = gst_caps_new_simple(
										   "audio/x-eac3",
										   "framed", G_TYPE_BOOLEAN, TRUE,
										   "rate", G_TYPE_INT, info.samplerate,
										   "channels", G_TYPE_INT, info.channel_count,
										   NULL );
				break;
				
			default:
				g_print( "unk codec_type: %" PRIu32 "\n", info.codec_type );
				return;
		}
		gst_app_src_set_caps(appsrc, caps);
		gst_caps_unref(caps);
		gst_buffer_unref (buf);
	}
};

/**
 * @brief Adjusts the media decode timestamps in an MP4 stream.
 *
 * This function processes the provided MP4 data buffer and applies a
 * timestamp adjustment to the media decode time (DTS) fields by the
 * specified delta. This is useful for resynchronizing or offsetting
 * media playback.
 *
 * @param ptr               Pointer to the MP4 data buffer to process.
 * @param len               Length of the MP4 data buffer in bytes.
 * @param pts_restamp_delta The delta (in time units) to apply to the decode timestamps.
 *
 * @return uint64_t The number of bytes processed in the buffer.
 */
uint64_t mp4_AdjustMediaDecodeTime( uint8_t *ptr, size_t len, int64_t pts_restamp_delta );

#endif /* parsemp4_hpp */
