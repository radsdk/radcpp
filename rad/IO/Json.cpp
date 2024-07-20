#include <rad/IO/Json.h>
#include <rad/IO/File.h>

namespace rad
{

static void SetJsonParseOptions(boost::json::parse_options& options)
{
    options.allow_comments = true;
    options.allow_trailing_commas = true;
    options.allow_infinity_and_nan = true;
}

JsonValue ParseJson(std::string_view str)
{
    boost::json::parse_options options = {};
    SetJsonParseOptions(options);
    return boost::json::parse(str, {}, options);
}

JsonValue ParseJsonFromFile(std::string_view fileName)
{
    boost::json::parse_options options = {};
    SetJsonParseOptions(options);
    return boost::json::parse(File::ReadAll(fileName), {}, options);
}

JsonValue* FindMemberCaseInsensitive(JsonValue& value, std::string_view key)
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
