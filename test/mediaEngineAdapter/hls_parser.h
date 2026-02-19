#ifndef HLS_PARSER_H
#define HLS_PARSER_H

#include <string>
#include <vector>
#include <map>

/**
 * @brief Represents a single media segment in HLS playlist
 */
struct HLSSegment
{
    std::string url;              // Absolute or relative URL
    double duration;              // Duration in seconds
    int sequenceNumber;           // Media sequence number
    bool isDiscontinuity;         // True if discontinuity tag present
    std::string title;            // EXTINF title/metadata
    
    HLSSegment() 
        : url(""), duration(0.0), sequenceNumber(0), 
          isDiscontinuity(false), title("") {}
};

/**
 * @brief Represents parsed HLS playlist
 */
struct HLSPlaylist
{
    std::string version;          // HLS version
    int targetDuration;           // Target duration in seconds
    int mediaSequence;            // First media sequence number
    bool isLive;                  // True if EVENT or no ENDLIST
    bool hasEndList;              // True if #EXT-X-ENDLIST present
    std::string playlistType;     // VOD, EVENT, or empty
    std::vector<HLSSegment> segments;
    std::string baseUrl;          // Base URL for resolving relative URLs
    
    HLSPlaylist()
        : version(""), targetDuration(0), mediaSequence(0),
          isLive(true), hasEndList(false), playlistType(""), baseUrl("") {}
};

/**
 * @brief Simple HLS M3U8 parser
 */
class HLSParser
{
public:
    /**
     * @brief Parse HLS master playlist
     * @param content M3U8 content
     * @param baseUrl Base URL for resolving relative URLs
     * @return Map of bandwidth to variant playlist URL
     */
    static std::map<int, std::string> ParseMasterPlaylist(
        const std::string& content,
        const std::string& baseUrl
    );
    
    /**
     * @brief Parse HLS media playlist
     * @param content M3U8 content
     * @param baseUrl Base URL for resolving relative URLs
     * @return Parsed playlist structure
     */
    static HLSPlaylist ParseMediaPlaylist(
        const std::string& content,
        const std::string& baseUrl
    );
    
    /**
     * @brief Check if playlist is master playlist
     * @param content M3U8 content
     * @return True if master playlist
     */
    static bool IsMasterPlaylist(const std::string& content);
    
    /**
     * @brief Resolve relative URL to absolute URL
     * @param baseUrl Base URL
     * @param relativeUrl Relative URL
     * @return Absolute URL
     */
    static std::string ResolveUrl(const std::string& baseUrl, const std::string& relativeUrl);
    
private:
    static std::string Trim(const std::string& str);
    static std::vector<std::string> SplitLines(const std::string& content);
    static std::string GetAttributeValue(const std::string& line, const std::string& attribute);
};

#endif // HLS_PARSER_H
