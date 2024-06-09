#include "Json.h"
#include "File.h"

namespace rad
{

static void SetJsonParseOptions(boost::json::parse_options& options)
{
    options.allow_comments = true;
    options.allow_trailing_commas = true;
    options.allow_infinity_and_nan = true;
}

boost::json::value ParseJson(std::string_view str)
{
    boost::json::parse_options options = {};
    SetJsonParseOptions(options);
    return boost::json::parse(str, {}, options);
}

boost::json::value ParseJsonFromFile(std::string_view fileName)
{
    boost::json::parse_options options = {};
    SetJsonParseOptions(options);
    return boost::json::parse(File::ReadAll(fileName), {}, options);
}

boost::json::value* FindMemberCaseInsensitive(boost::json::value& value, std::string_view key)
{
    if (value.is_object())
    {
        for (auto& iter : value.get_object())
        {
            if (rad::StrCaseEqual(iter.key(), key))
            {
                return &iter.value();
            }
        }
    }
    return nullptr;
}

} // namespace rad
