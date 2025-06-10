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
 * @file PlayerUtils.cpp
 * @brief Common utility functions
 */
#include "PlayerUtils.h"
#include "_base64.h"

/**
 * @brief Checks whether a given string starts with a specified prefix.
 *
 * This function compares the beginning of the input string with the prefix
 * and returns true if the input string starts with the prefix.
 *
 * @param inputStr The input string to be checked.
 * @param prefix The prefix to compare against the start of inputStr.
 * @return true if inputStr starts with prefix, false otherwise.
 */
bool player_StartsWith( const char *inputStr, const char *prefix )
{
	bool rc = true;
	while( *prefix )
	{
		if( *inputStr++ != *prefix++ )
		{
			rc = false;
			break;
		}
	}
	return rc;
}

/**
 * @brief convert blob of binary data to ascii base64-URL-encoded equivalent
 * @retval pointer to malloc'd cstring containing base64 URL encoded version of string
 * @retval NULL if insufficient memory to allocate base64-URL-encoded copy
 * @note caller responsible for freeing returned cstring
 */
char *base64_URL_Encode(const unsigned char *src, size_t len)
{
	char *rc = base64_Encode(src,len);
	if( rc )
	{
		char *dst = rc;
		while( *dst )
		{
			switch( *dst )
			{
				case '+':
					*dst = '-';
					break;
				case '/':
					*dst = '_';
					break;
				case '=':
					*dst = '\0';
					break;
				default:
					break;
			}
			dst++;
		}
	}
	return rc;
}

/**
 * @brief decode base64 URL encoded data to binary equivalent
 * @retval pointer to malloc'd memory containing decoded binary data
 * @retval NULL if insufficient memory to allocate decoded data
 * @note caller responsible for freeing returned data
 */
unsigned char *base64_URL_Decode(const char *src, size_t *len, size_t srcLen)
{
	unsigned char * rc = NULL;
	char *temp = (char *)malloc(srcLen+3);
	if( temp )
	{
		temp[srcLen+2] = '\0';
		temp[srcLen+1] = '=';
		temp[srcLen+0] = '=';
		for( int iter = 0; iter < srcLen; iter++ )
		{
			char c = src[iter];
			switch( c )
			{
				case '_':
					c = '/';
					break;
				case '-':
					c = '+';
					break;
				default:
					break;
			}
			temp[iter] = c;
		}
		rc = base64_Decode(temp, len );
		free(temp);
	}
	else
	{
		*len = 0;
	}
	return rc;
}

/// @brief Hash function object used to generate unique thread IDs from std::thread::id.
static std::hash<std::thread::id> std_thread_hasher;


/**
 * @brief Retrieves a unique identifier for the current thread.
 *
 * Uses a hash function to convert the current thread's ID into a size_t value.
 *
 * @return A hashed thread ID as a size_t.
 */
std::size_t GetThreadID( const std::thread &t )
{
	return std_thread_hasher( t.get_id() );
}


/**
 * @brief Retrieves a unique identifier for a given std::thread object.
 *
 * Uses a hash function to convert the thread's ID into a size_t value.
 *
 * @param t The std::thread object whose ID is to be retrieved.
 * @return A hashed thread ID as a size_t.
 */
std::size_t GetThreadID( void )
{
	return std_thread_hasher( std::this_thread::get_id() );
}


/**
 * @brief Retrieves a unique identifier for the current thread.
 *
 * Uses a hash function to convert the current thread's ID into a size_t value.
 *
 * @return A hashed thread ID as a size_t.
 */

std::size_t GetThreadID( const pthread_t &t )
{
	static std::hash<pthread_t> pthread_hasher;
	return pthread_hasher( t );
}

/**
 * @brief Get current time from epoch is milliseconds
 * @retval - current time in milliseconds
 */
long long GetCurrentTimeMS(void)
{
        struct timeval t;
        gettimeofday(&t, NULL);
        return (long long)(t.tv_sec*1e3 + t.tv_usec*1e-3);
}

/**
 * @brief parse leading protocol from uri if present
 * @param[in] uri manifest/ fragment uri
 * @retval return pointer just past protocol (i.e. http://) if present (or) return NULL uri doesn't start with protcol
 */
static const char * ParseUriProtocol(const char *uri)
{
	if(NULL == uri)
	{
		//MW_LOG_ERR("Empty URI");
		return NULL;
	}
	for(;;)
	{
		char ch = *uri++;
		if( ch ==':' )
		{
			if (uri[0] == '/' && uri[1] == '/')
			{
				return uri + 2;
			}
			break;
		}
		else if (isalnum (ch) || ch == '.' || ch == '-' || ch == '+') // other valid (if unlikely) characters for protocol
		{ // legal characters for uri protocol - continue
			continue;
		}
		else
		{
			break;
		}
	}
	return NULL;
}

/**
 * @brief Resolves a relative URI against a base URL.
 *
 * If the URI is absolute (i.e., contains a protocol), it is returned as-is.
 * Otherwise, it is resolved relative to the provided base URL. Optionally,
 * query parameters from the base URL can be propagated to the resolved URI
 * if the input URI does not already contain them.
 *
 * @param dst Output string where the resolved URL will be stored.
 * @param base The base URL to resolve against if the URI is relative.
 * @param uri The URI to resolve. Can be relative or absolute.
 * @param bPropagateUriParams If true, query parameters from the base URL
 *        will be appended to the resolved URI if the URI does not already contain them.
 */
void ResolveURL(std::string& dst, std::string base, const char *uri , bool bPropagateUriParams)
{
	if( ParseUriProtocol(uri) )
	{
		dst = uri;
	}
	else
	{
		if(base.empty())
		{
			//MW_LOG_WARN("Empty base");
			return;
		}
		const char *baseStart = base.c_str();
		const char *basePtr = ParseUriProtocol(baseStart);
		const char *baseEnd;
		for(;;)
		{
			char c = *basePtr;
			if( c==0 || c=='/' || c=='?' )
			{
				baseEnd = basePtr;
				break;
			}
			basePtr++;
		}

		if( uri[0]!='/' && uri[0]!='\0' )
		{
			for(;;)
			{
				char c = *basePtr;
				if( c=='/' )
				{
					baseEnd = basePtr;
				}
				else if( c=='?' || c==0 )
				{
					break;
				}
				basePtr++;
			}
		}
		dst = base.substr(0,baseEnd-baseStart);
		if( uri[0]!='/' )
		{
			dst += "/";
		}
		dst += uri;
		if( bPropagateUriParams )
		{
			if (strchr(uri,'?') == 0)
			{ // uri doesn't have url parameters; copy from parents if present
				const char *baseParams = strchr(basePtr,'?');
				if( baseParams )
				{
					std::string params = base.substr(baseParams-baseStart);
					dst.append(params);
				}
			}
		}
	}
}
