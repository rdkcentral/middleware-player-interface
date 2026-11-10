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
 * @file InfraStubs.cpp
 * @brief Link support for the InfraTests suite.
 *
 * ProcessHandler.cpp calls trim() from PlayerUtils.  Compiling all of
 * PlayerUtils.cpp would drag in generated version.h and base64 sources that
 * are unrelated to this suite, so the single required helper is provided
 * here - copied verbatim from PlayerUtils.cpp so behaviour is identical.
 */

#include "PlayerUtils.h"

void trim(std::string& src)
{
	size_t first = src.find_first_not_of(" \n\r\t\f\v");
	if (first != std::string::npos)
	{
		size_t last = src.find_last_not_of(" \n\r\t\f\v");
		std::string dst = src.substr(first, (last - first + 1));
		src = std::move(dst);
	}
}
