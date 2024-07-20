#pragma once

#include <rad/Core/Platform.h>
#include <boost/json.hpp>
// array: sequence container of JSON values supporing dynamic size and fast, random access.
// object: associative container of key-value pairs with unique keys (string-value pairs).
// string: a contiguous range of characters (UTF-8).
// value: a special variant holds one of the six standard JSON data types.
// kind: array, object, string, int64, uint64, double, bool, null.

namespace rad
{

using JsonArray = boost::json::array;
using JsonObject = boost::json::object;
using JsonString = boost::json::string;
using JsonValue = boost::json::value;

JsonValue ParseJson(std::string_view str);
JsonValue ParseJsonFromFile(std::string_view fileName);
JsonValue* FindMemberCaseInsensitive(JsonValue& value, std::string_view key);

} // namespace rad
