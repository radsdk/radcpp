#include <rad/Core/String.h>
#include <cstdarg>

#if defined(RAD_OS_WINDOWS)
#include <Windows.h>
#endif

namespace rad
{

std::vector<std::string> StrSplit(
    std::string_view str, std::string_view delimiters, bool skipEmptySubStr)
{
    std::vector<std::string> substrs;

    std::string::size_type pos = 0;
    std::string::size_type offset = 0;

    while (offset < str.length() + 1)
    {
        pos = str.find_first_of(delimiters, offset);
        if (pos == std::string::npos)
        {
            pos = str.length();
        }
        if (pos != offset || !skipEmptySubStr)
        {
            substrs.push_back(std::string(str.data() + offset, pos - offset));
        }
        offset = pos + 1;
    }

    return substrs;
}

std::string StrPrint(const char* format, ...)
{
    std::string str;
    va_list args;
    va_start(args, format);
    StrPrintInPlaceArgList(str, format, args);
    va_end(args);
    return str;
}

int StrPrintInPlace(std::string& buffer, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int charsPrinted = vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);

    if ((charsPrinted > 0) && (size_t(charsPrinted) + 1 > buffer.size()))
    {
        buffer.resize(size_t(charsPrinted) + 1);
        va_start(args, format);
        charsPrinted = vsnprintf(buffer.data(), buffer.size(), format, args);
        va_end(args);
    }
    buffer.pop_back();

    return charsPrinted;
}

int StrPrintInPlaceArgList(std::string& buffer, const char* format, va_list args)
{
    va_list args1;
    va_copy(args1, args);
    int charsPrinted = vsnprintf(buffer.data(), buffer.size(), format, args1);
    va_end(args1);

    if ((charsPrinted > 0) && (size_t(charsPrinted) + 1 > buffer.size()))
    {
        buffer.resize(size_t(charsPrinted) + 1);
        va_copy(args1, args);
        charsPrinted = vsnprintf(buffer.data(), buffer.size(), format, args1);
        va_end(args1);
    }
    buffer.pop_back();
    return charsPrinted;
}

bool StrEqual(std::string_view str1, std::string_view str2)
{
    return (str1 == str2);
}

bool StrCaseEqual(std::string_view str1, std::string_view str2)
{
#if defined(RAD_COMPILER_MSVC)
    return (_stricmp(str1.data(), str2.data()) == 0);
#else
    return (strcasecmp(str1.data(), str2.data()) == 0);
#endif
}

int StrCompare(std::string_view left, std::string_view right)
{
    return strcmp(left.data(), right.data());
}

int StrCaseCompare(std::string_view left, std::string_view right)
{
    return strcasecmp(left.data(), right.data());
}

std::string StrWideToU8(std::wstring_view wstr)
{
#if defined(RAD_OS_WINDOWS)
    std::string str;
    int charCount = ::WideCharToMultiByte(CP_UTF8,
        0, wstr.data(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
    if (charCount > 0)
    {
        str.resize(charCount, 0);
        ::WideCharToMultiByte(CP_UTF8,
            0, wstr.data(), static_cast<int>(wstr.length()), &str[0], charCount, NULL, NULL);
    }
    return str;
#else
    return utf_to_utf<char>(wstr.data());
#endif
}

std::wstring StrU8ToWide(std::string_view str)
{
#if defined(RAD_OS_WINDOWS)
    std::wstring wstr;
    int charCount = ::MultiByteToWideChar(CP_UTF8,
        0, str.data(), static_cast<int>(str.length()), NULL, 0);
    if (charCount > 0)
    {
        wstr.resize(charCount, 0);
        ::MultiByteToWideChar(CP_UTF8,
            0, str.data(), static_cast<int>(str.length()), &wstr[0], charCount);
    }
    return wstr;
#else
    return utf_to_utf<wchar_t>(str.data());
#endif
}

std::string StrUpper(std::string_view s)
{
    std::string buffer(s);
    StrUpperInplace(buffer);
    return buffer;
}

std::string StrLower(std::string_view s)
{
    std::string buffer(s);
    StrLowerInplace(buffer);
    return buffer;
}

void StrUpperInplace(std::string& s)
{
    for (size_t i = 0; i < s.size(); ++i)
    {
        s[i] = std::toupper(s[i]);
    }
}

void StrLowerInplace(std::string& s)
{
    for (size_t i = 0; i < s.size(); ++i)
    {
        s[i] = std::tolower(s[i]);
    }
}

bool StrIsDecInteger(std::string_view str)
{
    if (str.empty())
    {
        return false;
    }
    size_t i = 0;
    if (str.size() >= 2)
    {
        if (str[0] == '+' || str[0] == '-')
        {
            i++;
        }
    }
    for (; i < str.size(); ++i)
    {
        if (!IsDigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool StrIsHexNumber(std::string_view str)
{
    if (str.starts_with("0x") || str.starts_with("0X"))
    {
        for (size_t i = 2; i < str.size(); ++i)
        {
            if (!IsHexDigit(str[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool StrIsBinNumber(std::string_view str)
{
    if (str.starts_with("0b") || str.starts_with("0B"))
    {
        for (size_t i = 2; i < str.size(); ++i)
        {
            if ((str[i] != '0') && (str[i] != '1'))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool StrIsNumeric(std::string_view str)
{
    const char* p = str.data();
    if ((*p == '-') || (*p == '+'))
    {
        ++p;
    }

    bool hasDot = false;
    while (*p != '\0')
    {
        if (*p == '.')
        {
            if (hasDot)
            {
                return false;
            }
            hasDot = true;
        }
        else if (!IsDigit(*p))
        {
            return false;
        }

        ++p;
    }

    return true;
}

bool IsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

bool IsHexDigit(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

std::string StrTrim(std::string_view str, std::string_view charlist)
{
    size_t beg = str.find_first_not_of(charlist);
    size_t end = str.find_last_not_of(charlist) + 1;
    return std::string(str.substr(beg, end - beg));
}

void StrTrimInPlace(std::string& str, std::string_view charlist)
{
    str.erase(0, str.find_first_not_of(charlist));
    str.erase(str.find_last_not_of(charlist) + 1);
}

std::string StrReplace(std::string_view str, std::string_view subOld, std::string_view subNew)
{
    std::string newStr;
    newStr.reserve(str.size());
    std::string::size_type offset = 0u;
    std::string::size_type pos = 0u;
    while ((pos = str.find(subOld, offset)) != std::string::npos)
    {
        newStr.append(str, offset, pos - offset);
        newStr.append(subNew);
        offset = pos + subOld.size();
    }
    if (offset < str.size())
    {
        pos = str.size();
        newStr.append(str, offset, pos - offset);
    }
    return newStr;
}

void StrReplaceInPlace(std::string& str, std::string_view subOld, std::string_view subNew)
{
    std::string::size_type pos = 0u;
    while ((pos = str.find(subOld, pos)) != std::string::npos)
    {
        str.replace(pos, subOld.length(), subNew);
        pos += subNew.length();
    }
}

} // namespace rad
