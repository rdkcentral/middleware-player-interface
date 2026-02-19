#include "hls_parser.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstring>

/**
 * @brief Trim whitespace from string
 */
std::string HLSParser::Trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Split content into lines
 */
std::vector<std::string> HLSParser::SplitLines(const std::string& content)
{
    std::vector<std::string> lines;
    std::stringstream ss(content);
    std::string line;
    
    while (std::getline(ss, line)) {
        line = Trim(line);
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    return lines;
}

/**
 * @brief Get attribute value from tag line
 */
std::string HLSParser::GetAttributeValue(const std::string& line, const std::string& attribute)
{
    size_t pos = line.find(attribute + "=");
    if (pos == std::string::npos) return "";
    
    pos += attribute.length() + 1;
    
    // Handle quoted values
    if (pos < line.length() && line[pos] == '"') {
        pos++;
        size_t endPos = line.find('"', pos);
        if (endPos != std::string::npos) {
            return line.substr(pos, endPos - pos);
        }
    }
    
    // Handle unquoted values
    size_t endPos = line.find_first_of(",\r\n", pos);
    if (endPos == std::string::npos) {
        return line.substr(pos);
    }
    return line.substr(pos, endPos - pos);
}

/**
 * @brief Resolve relative URL to absolute
 */
std::string HLSParser::ResolveUrl(const std::string& baseUrl, const std::string& relativeUrl)
{
    // Already absolute
    if (relativeUrl.find("http://") == 0 || relativeUrl.find("https://") == 0) {
        return relativeUrl;
    }
    
    // Find base path
    std::string base = baseUrl;
    size_t lastSlash = base.find_last_of('/');
    if (lastSlash != std::string::npos) {
        base = base.substr(0, lastSlash + 1);
    }
    
    return base + relativeUrl;
}

/**
 * @brief Check if playlist is master playlist
 */
bool HLSParser::IsMasterPlaylist(const std::string& content)
{
    return content.find("#EXT-X-STREAM-INF") != std::string::npos;
}

/**
 * @brief Parse master playlist to get variant streams
 */
std::map<int, std::string> HLSParser::ParseMasterPlaylist(
    const std::string& content,
    const std::string& baseUrl)
{
    std::map<int, std::string> variants;
    std::vector<std::string> lines = SplitLines(content);
    
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i].find("#EXT-X-STREAM-INF") == 0) {
            // Get bandwidth
            std::string bandwidthStr = GetAttributeValue(lines[i], "BANDWIDTH");
            int bandwidth = bandwidthStr.empty() ? 0 : std::stoi(bandwidthStr);
            
            // Next line is the URL
            if (i + 1 < lines.size() && lines[i + 1][0] != '#') {
                std::string url = ResolveUrl(baseUrl, lines[i + 1]);
                variants[bandwidth] = url;
                std::cout << "[HLS] Found variant: bandwidth=" << bandwidth << " url=" << url << std::endl;
            }
        }
    }
    
    return variants;
}

/**
 * @brief Parse media playlist to get segments
 */
HLSPlaylist HLSParser::ParseMediaPlaylist(
    const std::string& content,
    const std::string& baseUrl)
{
    HLSPlaylist playlist;
    playlist.baseUrl = baseUrl;
    
    std::vector<std::string> lines = SplitLines(content);
    
    double currentDuration = 0.0;
    std::string currentTitle = "";
    bool nextDiscontinuity = false;
    int currentSeq = 0;
    
    for (size_t i = 0; i < lines.size(); i++) {
        const std::string& line = lines[i];
        
        // Version
        if (line.find("#EXT-X-VERSION:") == 0) {
            playlist.version = line.substr(15);
        }
        // Target duration
        else if (line.find("#EXT-X-TARGETDURATION:") == 0) {
            playlist.targetDuration = std::stoi(line.substr(22));
        }
        // Media sequence
        else if (line.find("#EXT-X-MEDIA-SEQUENCE:") == 0) {
            playlist.mediaSequence = std::stoi(line.substr(22));
            currentSeq = playlist.mediaSequence;
        }
        // Playlist type
        else if (line.find("#EXT-X-PLAYLIST-TYPE:") == 0) {
            playlist.playlistType = line.substr(21);
            if (playlist.playlistType == "VOD") {
                playlist.isLive = false;
            }
        }
        // End list
        else if (line.find("#EXT-X-ENDLIST") == 0) {
            playlist.hasEndList = true;
            playlist.isLive = false;
        }
        // Discontinuity
        else if (line.find("#EXT-X-DISCONTINUITY") == 0) {
            nextDiscontinuity = true;
        }
        // Segment info
        else if (line.find("#EXTINF:") == 0) {
            // Parse duration
            size_t commaPos = line.find(',');
            std::string durationStr = line.substr(8, commaPos - 8);
            currentDuration = std::stod(durationStr);
            
            if (commaPos != std::string::npos) {
                currentTitle = line.substr(commaPos + 1);
            }
        }
        // Segment URL
        else if (!line.empty() && line[0] != '#') {
            HLSSegment segment;
            segment.url = ResolveUrl(baseUrl, line);
            segment.duration = currentDuration;
            segment.sequenceNumber = currentSeq++;
            segment.isDiscontinuity = nextDiscontinuity;
            segment.title = currentTitle;
            
            playlist.segments.push_back(segment);
            
            // Reset
            currentDuration = 0.0;
            currentTitle = "";
            nextDiscontinuity = false;
        }
    }
    
    std::cout << "[HLS] Parsed playlist: " << playlist.segments.size() << " segments, "
              << "type=" << (playlist.playlistType.empty() ? "LIVE" : playlist.playlistType)
              << ", mediaSeq=" << playlist.mediaSequence << std::endl;
    
    return playlist;
}
