#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/String.h"
#include "rad/IO/File.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

NAMESPACE_RAD_BEGIN

using JsonValue = rapidjson::Value;

class JsonDoc
{
public:
    JsonDoc();
    ~JsonDoc();

    bool ParseFile(const IO::Path& filePath);
    bool Parse(std::string_view str);
    bool HasParseError() { return m_doc.HasParseError(); }
    const char* GetParseError();
    size_t GetParseErrorOffset();

    rapidjson::Document& GetDoc() { return m_doc; }
    JsonValue& GetRoot() { return m_doc.GetObject(); }
    JsonValue& operator[](std::string_view name) { return m_doc[name.data()]; }

    bool IsObject() { return m_doc.IsObject(); }
    bool HasMember(std::string_view name) { return m_doc.HasMember(name.data()); }
    auto MemberBegin() { return m_doc.MemberBegin(); }
    auto MemberEnd() { return m_doc.MemberEnd(); }

    void SetNull() { m_doc.SetNull(); }

    std::string Stringify();
    std::string StringifyPretty();

private:
    rapidjson::Document m_doc;

}; // class JsonDoc

NAMESPACE_RAD_END
