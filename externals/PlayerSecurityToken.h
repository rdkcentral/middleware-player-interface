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
 * @file PlayerSecurityToken.h
 * @brief Security token access for both Thunder 4 (SecurityTokenUtil) and Thunder 5 (ClientSecurityAgent)
 */

#ifndef PLAYER_SECURITY_TOKEN_H
#define PLAYER_SECURITY_TOKEN_H

#if __has_include(<securityagent/SecurityTokenUtil.h>)
#include <securityagent/SecurityTokenUtil.h>

static inline int PlayerGetSecurityToken(unsigned short maxLength, unsigned char buffer[])
{
	return GetSecurityToken(maxLength, buffer);
}
#else
#include <securityagent/securityagent.h>

static inline int PlayerGetSecurityToken(unsigned short maxLength, unsigned char buffer[])
{
	// Same localhost payload RDK's GetSecurityToken() tokenizes
	static const char payload[] = "http://localhost";
	if (maxLength < sizeof(payload))
	{
		return -1;
	}
	for (unsigned short i = 0; i < sizeof(payload); i++)
	{
		buffer[i] = static_cast<unsigned char>(payload[i]);
	}
	return GetToken(maxLength, static_cast<unsigned short>(sizeof(payload) - 1), buffer);
}
#endif

#endif /* PLAYER_SECURITY_TOKEN_H */
