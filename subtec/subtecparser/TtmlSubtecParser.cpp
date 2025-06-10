/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2025 RDK Management
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

#include "TtmlSubtecParser.hpp"
#include <regex>

// #define TTML_DEBUG

/**
 * @brief Constructs a TtmlSubtecParser instance for TTML subtitle parsing.
 *
 * Initializes the parser with the specified subtitle type and screen dimensions.
 * It sets up the communication channel with the Subtec subtitle engine, sends
 * initialization packets, and optionally resumes track downloads if a callback is set.
 *
 * @param type The MIME type of the subtitle (e.g., TTML).
 * @param width The width of the video screen. Defaults to 1920 if 0.
 * @param height The height of the video screen. Defaults to 1080 if 0.
 *
 * @throws std::runtime_error if the subtitle communication channel fails to initialize.
 */
TtmlSubtecParser::TtmlSubtecParser(SubtitleMimeType type, int width, int height) : SubtitleParser(type, width, height), m_channel(nullptr)
{
#ifdef TTML_DEBUG
	printf( "TtmlSubtecParser::TtmlSubtecParser\n" );
#endif
	m_channel = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
	if (!m_channel->InitComms())
	{
		MW_LOG_INFO("Init failed - subtitle parsing disabled");
		throw std::runtime_error("PacketSender init failed");
	}
	m_channel->SendResetAllPacket();

	int screenWidth = 1920, screenHeight = 1080;
	if(width != 0 && height != 0)
	{
		screenWidth = width;
		screenHeight = height;
	}
	m_channel->SendSelectionPacket(screenWidth, screenHeight);
	m_channel->SendMutePacket();
 	if(playerResumeTrackDownloads_CB)
 	{
		playerResumeTrackDownloads_CB();
 	}
}


/**
 * @brief Initializes the TTML subtitle parser with a starting position and base PTS.
 *
 * Sends a timestamp packet to the subtitle channel based on the provided start position
 * in seconds. Also resets internal state flags and optionally resumes track downloads
 * if a callback is registered.
 *
 * @param startPosSeconds The starting playback position in seconds.
 * @param basePTS The base presentation timestamp (PTS) in microseconds.
 * @return true Always returns true upon successful initialization.
 */
bool TtmlSubtecParser::init(double startPosSeconds, unsigned long long basePTS)
{
#ifdef TTML_DEBUG
	printf( "TtmlSubtecParser::init(startPosSeconds=%.3fs,basePTS=%llu\n", startPosSeconds, basePTS );
#endif
	MW_LOG_INFO("startPosSeconds %.3fs basePTS=%llu", startPosSeconds, basePTS);
	m_channel->SendTimestampPacket(static_cast<uint64_t>(startPosSeconds * 1000.0));
 	if(playerResumeTrackDownloads_CB)
 	{
		playerResumeTrackDownloads_CB();
 	}

	m_parsedFirstPacket = false;
	m_sentOffset = false;
	m_firstBeginOffset = 0.0;

	return true;
}

/**
 * @brief Updates the subtitle parser with the current playback timestamp.
 *
 * Sends a timestamp packet to the subtitle channel to synchronize subtitle rendering
 * with the current playback position.
 *
 * @param positionMs The current playback position in milliseconds.
 */
void TtmlSubtecParser::updateTimestamp(unsigned long long positionMs)
{
#ifdef TTML_DEBUG
	printf( "TtmlSubtecParser::updateTimestamp(positionMs=%llu\n", positionMs );
#endif
	m_channel->SendTimestampPacket(positionMs);
}

/**
 * @brief Resets the subtitle parser's internal channel state.
 *
 * Sends a reset packet to the subtitle channel to clear any existing subtitle data
 * and prepare for fresh input. Typically used when restarting or reinitializing playback.
 */
void TtmlSubtecParser::reset()
{
#ifdef TTML_DEBUG
	printf( "TtmlSubtecParser::reset\n" );
#endif
	m_channel->SendResetChannelPacket();
}


/**
 * @brief Parses the first "begin" timestamp from a TTML-formatted input stream.
 *
 * This function scans through the input stream line by line, searching for a `begin="HH:MM:SS.mmm"` pattern
 * using a regular expression. If a match is found, it extracts the hours, minutes, seconds, and optional
 * milliseconds, and converts the timestamp into a total number of milliseconds.
 *
 * @param ss A reference to a std::stringstream containing TTML subtitle data.
 * @return The first "begin" timestamp found, in milliseconds. If no match is found, returns the maximum value of int64_t.
 */
static std::int64_t parseFirstBegin(std::stringstream &ss)
{
	std::int64_t firstBegin = std::numeric_limits<std::int64_t>::max();
	std::string line;
	static const std::regex beginRegex(R"(begin="([0-9]+):([0-9][0-9]?):([0-9][0-9]?)\.?([0-9]+)?")");
	
	while(std::getline(ss, line))
	{
		try {
			bool matched = false;
			//Regex still works with no hours and/or no ms. Mins and secs are required
			std::smatch match;
			matched = std::regex_search(line, match, beginRegex);

			if (matched) {
				std::int64_t hours = 0, minutes = 0, seconds = 0, milliseconds = 0;

				if (!match.str(1).empty()) MW_LOG_WARN("h:%s", match.str(1).c_str()); hours = std::stol(match.str(1));
				if (!match.str(2).empty()) MW_LOG_WARN("m:%s", match.str(2).c_str()); minutes = std::stol(match.str(2));
				if (!match.str(3).empty()) MW_LOG_WARN("s:%s", match.str(3).c_str()); seconds = std::stol(match.str(3));
				if (!match.str(4).empty()) MW_LOG_WARN("ms:%s", match.str(4).c_str()); milliseconds = std::stol(match.str(4));

				firstBegin = milliseconds + (1000 * (seconds + (60 * (minutes + (60 * hours)))));
				break;
			}
		}
		catch (const std::regex_error& e) {
			MW_LOG_WARN("Regex error %s from line %s", std::to_string(e.code()).c_str(), line.c_str());
		}
	}
	
	return firstBegin;
}

/**
 * @brief Processes subtitle data from a TTML stream and sends it to the subtitle rendering channel.
 *
 * This function parses the input buffer as an ISO BMFF (MP4) stream. If the buffer is not an
 * initialization segment, it extracts the `mdat` box, parses the TTML data, and sends it to
 * the subtitle rendering channel. For linear content, it also calculates and sends a time offset
 * based on the first `begin` timestamp found in the TTML data.
 *
 * @param buffer Pointer to the input data buffer.
 * @param bufferLen Length of the input buffer in bytes.
 * @param position Current playback position in seconds.
 * @param duration Duration of the current segment in seconds.
 * @return true Always returns true after processing.
 */
bool TtmlSubtecParser::processData(const char* buffer, size_t bufferLen, double position, double duration)
{
#ifdef TTML_DEBUG
	printf( "TtmlSubtecParser::processData(bufferLen=%zu,position=%f,duration=%f)\n", bufferLen, position, duration );
#endif

	PlayerIsoBmffBuffer isobuf;

	isobuf.setBuffer( (uint8_t *)buffer, bufferLen);
	isobuf.parseBuffer();

	if (!isobuf.isInitSegment())
	{
		uint8_t *mdat;
		size_t mdatLen;

		//isobuf.printBoxes();
		isobuf.getMdatBoxSize(mdatLen);

		mdat = (uint8_t *)malloc(mdatLen);
		isobuf.parseMdatBox(mdat, mdatLen);

		std::vector<uint8_t> data(mdatLen);
		data.assign(mdat, mdat+mdatLen);
		
		//necessary because the offset into the TTML
		//is not available in the linear manifest
		//Take the first instance of the "begin" tag as the time offset for subtec
		if (!m_parsedFirstPacket && m_isLinear)
		{
			m_firstBeginOffset = position;
			m_parsedFirstPacket = true;
		}

		if (!m_sentOffset && m_parsedFirstPacket && m_isLinear)
		{
			MW_LOG_TRACE("Linear content - parsing first begin as offset - pos %.3f dur %.3f m_firstBeginOffset %.3f", 
				 position, duration, m_firstBeginOffset);
			std::stringstream ss(std::string(data.begin(), data.end()));
			std::int64_t offset = parseFirstBegin(ss);
			
			if (offset != std::numeric_limits<std::int64_t>::max())
			{
				auto positionDeltaSecs = (position - m_firstBeginOffset);
				long long getPositionMS = 0;
				double seekPositionSeconds = 0.0;
				if(playerGetPositions_CB)
 				{
					playerGetPositions_CB(getPositionMS, seekPositionSeconds);
 				}

				auto timeFromStartMs = getPositionMS - (seekPositionSeconds * 1000.0);
				std::int64_t totalOffset = offset - (positionDeltaSecs * 1000.0) + timeFromStartMs;

				std::stringstream output;
				output << "setting totalOffset " << totalOffset << " positionDeltaSecs " << positionDeltaSecs <<
					" timeFromStartMs " << timeFromStartMs;
				MW_LOG_TRACE("%s",  output.str().c_str());
				m_sentOffset = true;
				m_channel->SendTimestampPacket(totalOffset);
			}
		}

		m_channel->SendDataPacket(std::move(data), 0);

		free(mdat);
		MW_LOG_TRACE("Sent buffer with size %zu position %.3f", bufferLen, position);
	}
	else
	{
		MW_LOG_INFO("Init Segment");
	}
	return true;
}


/**
 * @brief Mutes or unmutes subtitle rendering.
 *
 * Sends a mute or unmute packet to the subtitle rendering channel based on the input flag.
 * This is typically used to control subtitle visibility during playback.
 *
 * @param mute If true, subtitles are muted (hidden); if false, subtitles are unmuted (shown).
 */
void TtmlSubtecParser::mute(bool mute)
{
#ifdef TTML_DEBUG
	printf( "TtmlSubtecParser::mute(mute=%d)\n", mute );
#endif
	if (mute)
	{
		m_channel->SendMutePacket();
	}
	else
	{
		m_channel->SendUnmutePacket();
	}
}

/**
 * @brief Pauses or resumes subtitle rendering.
 *
 * Sends a pause or resume packet to the subtitle rendering channel based on the input flag.
 * This is typically used to synchronize subtitle behavior with media playback state.
 *
 * @param pause If true, subtitles are paused; if false, subtitles are resumed.
 */
void TtmlSubtecParser::pause(bool pause)
{
#ifdef TTML_DEBUG
	printf( "TtmlSubtecParser::pause(pause=%d)\n", pause );
#endif
	if (pause)
	{
		m_channel->SendPausePacket();
	}
	else
	{
		m_channel->SendResumePacket();
	}
}