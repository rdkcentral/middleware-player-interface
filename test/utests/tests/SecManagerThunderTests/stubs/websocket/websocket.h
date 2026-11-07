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
 * @file websocket/websocket.h
 * @brief Minimal unit-test stub of the WPEFramework websocket header.
 *
 * ThunderAccessPlayer.h pulls in <websocket/websocket.h> for
 * JSONRPC::LinkType and the JsonObject / VariantContainer types used by
 * SecManagerThunder.cpp.  The real WPEFramework libraries are not available
 * to the L1 harness, so this stub provides just the API surface the code
 * under test consumes:
 *
 *   WPEFramework::Core::JSON::VariantContainer  (a lightweight JSON object)
 *   WPEFramework::JsonObject                    (typedef of the above)
 *   WPEFramework::Core::JSON::IElement
 *   WPEFramework::JSONRPC::LinkType<T>
 *   _T(x) macro
 */

#ifndef TEST_STUB_WEBSOCKET_WEBSOCKET_H
#define TEST_STUB_WEBSOCKET_WEBSOCKET_H

#include <cstdint>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#ifndef _T
#define _T(x) x
#endif

namespace WPEFramework {
namespace Core {
namespace JSON {

class IElement
{
};

/**
 * @brief Lightweight JSON object variant compatible with the subset of the
 *        WPEFramework Core::JSON::VariantContainer API used by the code
 *        under test: operator[], HasLabel, ToString, and per-key values
 *        exposing Boolean()/Number()/String()/Object().
 */
class VariantContainer
{
public:
	class Variant
	{
	public:
		enum class Type
		{
			None,
			Boolean,
			Number,
			String,
			Object
		};

		Variant() = default;

		Variant &operator=(const char *value)
		{
			set(value ? std::string(value) : std::string());
			return *this;
		}
		Variant &operator=(const std::string &value)
		{
			set(value);
			return *this;
		}
		Variant &operator=(bool value)
		{
			mType = Type::Boolean;
			mBool = value;
			return *this;
		}
		Variant &operator=(int value)
		{
			set(static_cast<int64_t>(value));
			return *this;
		}
		Variant &operator=(unsigned int value)
		{
			set(static_cast<int64_t>(value));
			return *this;
		}
		Variant &operator=(int64_t value)
		{
			set(value);
			return *this;
		}
		Variant &operator=(uint64_t value)
		{
			set(static_cast<int64_t>(value));
			return *this;
		}
		Variant &operator=(float value)
		{
			set(static_cast<int64_t>(value));
			return *this;
		}
		Variant &operator=(double value)
		{
			set(static_cast<int64_t>(value));
			return *this;
		}
		Variant &operator=(const VariantContainer &value)
		{
			mType = Type::Object;
			mObject = std::make_shared<VariantContainer>(value);
			return *this;
		}

		bool Boolean() const { return mType == Type::Boolean ? mBool : false; }
		int64_t Number() const { return mType == Type::Number ? mNumber : 0; }
		std::string String() const { return mType == Type::String ? mString : std::string(); }
		VariantContainer Object() const
		{
			return (mType == Type::Object && mObject) ? *mObject : VariantContainer();
		}
		Type GetType() const { return mType; }

	private:
		void set(const std::string &value)
		{
			mType = Type::String;
			mString = value;
		}
		void set(int64_t value)
		{
			mType = Type::Number;
			mNumber = value;
		}

		Type mType = Type::None;
		bool mBool = false;
		int64_t mNumber = 0;
		std::string mString;
		std::shared_ptr<VariantContainer> mObject;
	};

	Variant &operator[](const std::string &key)
	{
		return mValues[key];
	}

	const Variant &operator[](const std::string &key) const
	{
		static const Variant empty;
		auto it = mValues.find(key);
		return it != mValues.end() ? it->second : empty;
	}

	bool HasLabel(const std::string &key) const
	{
		return mValues.find(key) != mValues.end();
	}

	void ToString(std::string &out) const
	{
		out = serialize();
	}

	std::string serialize() const
	{
		std::ostringstream ss;
		ss << "{";
		bool first = true;
		for (const auto &kv : mValues)
		{
			if (!first)
			{
				ss << ",";
			}
			first = false;
			ss << "\"" << kv.first << "\":";
			switch (kv.second.GetType())
			{
				case Variant::Type::Boolean:
					ss << (kv.second.Boolean() ? "true" : "false");
					break;
				case Variant::Type::Number:
					ss << kv.second.Number();
					break;
				case Variant::Type::String:
					ss << "\"" << kv.second.String() << "\"";
					break;
				case Variant::Type::Object:
					ss << kv.second.Object().serialize();
					break;
				default:
					ss << "null";
					break;
			}
		}
		ss << "}";
		return ss.str();
	}

private:
	std::map<std::string, Variant> mValues;
};

} // namespace JSON
} // namespace Core

/** Alias matching WPEFramework's JsonObject used throughout the sources. */
using JsonObject = Core::JSON::VariantContainer;

namespace JSONRPC {

/**
 * @brief Stub of JSONRPC::LinkType - only its type identity is needed since
 *        the fake ThunderAccessPlayer never dereferences it.
 */
template <typename INTERFACE>
class LinkType
{
};

} // namespace JSONRPC
} // namespace WPEFramework

#endif /* TEST_STUB_WEBSOCKET_WEBSOCKET_H */
