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

/**
 * @file PlayerUtils.h
 * @brief Utility functions and macros for player-related operations such as string handling,
 *        base64 encoding/decoding, thread identification, and URL resolution.
 */

#ifndef __PLAYER_UTILS_H__
#define __PLAYER_UTILS_H__

#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <sys/time.h>
#include <inttypes.h>
#include <iostream>
#include <cstring>

/**
 * @brief Safely deletes a pointer and sets it to NULL.
 */
#define MW_SAFE_DELETE(ptr) { delete(ptr); ptr = NULL; }

/**
 * @brief Safely deletes an array pointer and sets it to NULL.
 */
#define MW_SAFE_DELETE_ARRAY(ptr) { delete [] ptr; ptr = NULL; }

/**
 * @brief Writes a byte as two hexadecimal ASCII characters.
 * @param DST Destination pointer to write the ASCII characters.
 * @param BYTE Byte value to convert.
 */
#define WRITE_HASCII( DST, BYTE ) \
{ \
    *DST++ = "0123456789abcdef"[BYTE>>4]; \
    *DST++ = "0123456789abcdef"[BYTE&0xf]; \
}

/**
 * @brief Checks if a string starts with a given prefix.
 * @param inputStr The input string.
 * @param prefix The prefix to check.
 * @return True if inputStr starts with prefix, false otherwise.
 */
bool player_StartsWith(const char *inputStr, const char *prefix);

/**
 * @brief Encodes binary data into a Base64 URL-safe string.
 * @param src Pointer to the binary data.
 * @param len Length of the binary data.
 * @return Pointer to the encoded Base64 URL-safe string.
 */
char *base64_URL_Encode(const unsigned char *src, size_t len);

/**
 * @brief Decodes a Base64 URL-safe string into binary data.
 * @param src Pointer to the Base64 string.
 * @param len Pointer to store the length of the decoded data.
 * @param srcLen Length of the Base64 string.
 * @return Pointer to the decoded binary data.
 */
unsigned char *base64_URL_Decode(const char *src, size_t *len, size_t srcLen);

/**
 * @brief Retrieves the thread ID from a std::thread object.
 * @param t The std::thread object.
 * @return The thread ID as a size_t.
 */
std::size_t GetThreadID(const std::thread &t);

/**
 * @brief Retrieves the current thread ID.
 * @return The thread ID as a size_t.
 */
std::size_t GetThreadID(void);

/**
 * @brief Retrieves the thread ID from a pthread_t object.
 * @param t The pthread_t object.
 * @return The thread ID as a size_t.
 */
std::size_t GetThreadID(const pthread_t &t);

/**
 * @brief Retrieves a printable thread ID from a pthread_t object.
 * @param t The pthread_t object.
 * @return A printable thread ID as a size_t.
 */
std::size_t GetPrintableThreadID(const pthread_t &t);

/**
 * @brief Retrieves a printable thread ID for the current thread.
 * @return A printable thread ID as a size_t.
 */
std::size_t GetPrintableThreadID();

/**
 * @brief Resolves a relative URI against a base URL.
 * @param dst Output string for the resolved URL.
 * @param base The base URL.
 * @param uri The relative URI.
 * @param bPropagateUriParams Whether to propagate URI parameters.
 */
void ResolveURL(std::string& dst, std::string base, const char *uri, bool bPropagateUriParams);

/**
 * @brief Gets the current system time in milliseconds.
 * @return Current time in milliseconds.
 */
long long GetCurrentTimeMS(void);

/**
 * @brief Resolves a relative URI against a base URL (player-specific version).
 * @param dst Output string for the resolved URL.
 * @param base The base URL.
 * @param uri The relative URI.
 * @param bPropagateUriParams Whether to propagate URI parameters.
 */
void player_ResolveURL(std::string& dst, std::string base, const char *uri, bool bPropagateUriParams);

/**
 * @brief Parses the protocol from a URI string.
 * @param uri The URI string.
 * @return Pointer to the protocol string.
 */
static const char * ParseUriProtocol(const char *uri);

#endif  /* __PLAYER_UTILS_H__ */

