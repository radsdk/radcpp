#pragma once

#include "rad/Core/Platform.h"
#include <boost/json.hpp>
// array: sequence container of JSON values supporing dynamic size and fast, random access.
// object: associative container of key-value pairs with unique keys (string-value pairs).
// string: a contiguous range of characters (UTF-8).
// value: a special variant holds one of the six standard JSON data types.
// kind: array, object, string, int64, uint64, double, bool, null.

namespace rad
{

boost::json::value ParseJson(std::string_view str);
boost::json::value ParseJsonFromFile(std::string_view fileName);

boost::json::value* FindMemberCaseInsensitive(boost::json::value& value, std::string_view key);

} // namespace rad
