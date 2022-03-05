#ifndef _SEED_JSON_H_
#define _SEED_JSON_H_

#define RAPIDJSON_HAS_STDSTRING 1

#include <Common/Seed_Object.h>
#include <Common/Seed_String.h>
#include <rapidjson/document.h>

namespace Seed {

    enum class JSON_MODE_E {
        NONE = 0,
        FILE,
        OBJECT,
        MAX,
    };

    class cJson : public cObject {
    public :
        cJson(JSON_MODE_E eMode = JSON_MODE_E::NONE);
        virtual ~cJson();

    private :
        rapidjson::Document _doc;
        cString _fileName;
        JSON_MODE_E _eMode;

    public :
        const cString& fileName() const { return _fileName; }
        JSON_MODE_E mode() const { return _eMode; }

    public :
        ERR_TYPE_E initialize();

    public :
        ERR_TYPE_E loadFromFile(const cString& filename);
        ERR_TYPE_E writeToFile(const cString& fileName = L"", bool useEnter = true);

    public :
        ERR_TYPE_E loadFromString(const cString& string);
        ERR_TYPE_E makeString(cString* pString, bool useEnter = false);
        cString makeString(bool useEnter = false);

    private :
        rapidjson::Value makeJsonValue(const cString& value);
        rapidjson::Value makeJsonValue(const bool value);
        rapidjson::Value makeJsonValue(const int32 value);
        rapidjson::Value makeJsonValue(const uint32 value);
        rapidjson::Value makeJsonValue(const int64 value);
        rapidjson::Value makeJsonValue(const uint64 value);
        rapidjson::Value makeJsonValue(const float value);
        rapidjson::Value makeJsonValue(const double value);

    public :
        ERR_TYPE_E setMember(const cString& label, const cString& value);
        ERR_TYPE_E setMember(const cString& label, const bool value);
        ERR_TYPE_E setMember(const cString& label, const int32 value);
        ERR_TYPE_E setMember(const cString& label, const uint32 value);
        ERR_TYPE_E setMember(const cString& label, const int64 value);
        ERR_TYPE_E setMember(const cString& label, const uint64 value);
        ERR_TYPE_E setMember(const cString& label, const float value);
        ERR_TYPE_E setMember(const cString& label, const double value);

    public :
        ERR_TYPE_E getMember(const cString& label, cString* pValue);
        ERR_TYPE_E getMember(const cString& label, bool* pValue);
        ERR_TYPE_E getMember(const cString& label, int32* pValue);
        ERR_TYPE_E getMember(const cString& label, uint32* pValue);
        ERR_TYPE_E getMember(const cString& label, int64* pValue);
        ERR_TYPE_E getMember(const cString& label, uint64* pValue);
        ERR_TYPE_E getMember(const cString& label, float* pValue);
        ERR_TYPE_E getMember(const cString& label, double* pValue);
    };
}

#endif