#include <Common/Seed_Json.h>
#include <Common/Seed_SystemUtil.h>

/* rapidjson header files */
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

/* std */
#include <fstream>

using namespace Seed;

cJson::cJson(JSON_MODE_E eMode)
    : _fileName(L""), _eMode(eMode)
{
    initialize();
}

cJson::~cJson()
{

}

ERR_TYPE_E cJson::initialize()
{
    _doc.SetObject();
    _fileName = L"";

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::loadFromFile(const cString& fileName)
{
    if (_eMode != JSON_MODE_E::FILE) {
        return ERR_TYPE_E::JSON_MODE_MISS_MATCH;
    }

    if (fileName.isEmpty()) {
        return ERR_TYPE_E::FILE_NAME_EMPTY;
    }

    _fileName = fileName;

    ERR_TYPE_E eRet = ERR_TYPE_E::SUCCESS;
    eRet = FileUtil::isExistFile(fileName);
    if (eRet != ERR_TYPE_E::SUCCESS) {
        return eRet;
    }
 
    std::ifstream ifs(_fileName.to_string());
    
    if (!ifs.is_open()) {
        initialize();
        return ERR_TYPE_E::CAN_NOT_FILE_OPEN;
    }

    rapidjson::IStreamWrapper isw(ifs);

    _doc.ParseStream(isw);
    if (_doc.HasParseError()) {
        initialize();
        return ERR_TYPE_E::HAS_PARSE_ERR;   
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::writeToFile(const cString& fileName, bool useEnter)
{
    std::ofstream ofs;
    if (fileName.isEmpty()) {
        if (_fileName.isEmpty()) {
            return ERR_TYPE_E::CAN_NOT_FIND_FILE;
        }
        ofs = std::ofstream(_fileName.to_string());
    }
    else {
        ofs = std::ofstream(fileName.to_string());
    }

    rapidjson::OStreamWrapper osw(ofs);

    if (useEnter) {
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
        _doc.Accept(writer);
    }
    else {
        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
        _doc.Accept(writer);
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::loadFromString(const cString& string)
{
    if (_eMode == JSON_MODE_E::FILE) {
        return ERR_TYPE_E::JSON_MODE_MISS_MATCH;
    }

    if (string.isEmpty()) {
        return ERR_TYPE_E::STRING_IS_EMPTY;
    }

    rapidjson::StringStream s(string.to_string().c_str());
    _doc.ParseStream(s);

    if (_doc.HasParseError()) {
        return ERR_TYPE_E::HAS_PARSE_ERR;
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::makeString(cString* pString, bool useEnter)
{
    if (pString == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    pString->format(L"%s", makeString().to_string().c_str());

    return ERR_TYPE_E::SUCCESS;
}

cString cJson::makeString(bool useEnter)
{
    rapidjson::StringBuffer buffer;

    if (useEnter) {
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        _doc.Accept(writer);
    }
    else {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        _doc.Accept(writer);
    }

    cString result(buffer.GetString());

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

rapidjson::Value cJson::makeJsonValue(const cString& value)
{
    rapidjson::Value jsonValue;
    jsonValue.SetString(value.to_string(), _doc.GetAllocator());

    return jsonValue;
}

rapidjson::Value cJson::makeJsonValue(const bool value)
{
    rapidjson::Value jsonValue(rapidjson::kTrueType);
    jsonValue.SetBool(value);

    return jsonValue;
}

rapidjson::Value cJson::makeJsonValue(const int32 value)
{
    rapidjson::Value jsonValue;
    jsonValue.SetInt(value);

    return jsonValue;
}

rapidjson::Value makeJsonValue(const uint32 value)
{
    rapidjson::Value jsonValue;
    jsonValue.SetUint(value);

    return jsonValue;
}

rapidjson::Value makeJsonValue(const int64 value)
{
    rapidjson::Value jsonValue;
    jsonValue.SetInt64(value);

    return jsonValue;
}

rapidjson::Value makeJsonValue(const uint64 value)
{
    rapidjson::Value jsonValue;
    jsonValue.SetUint64(value);

    return jsonValue;
}

rapidjson::Value makeJsonValue(const float value)
{
    rapidjson::Value jsonValue;
    jsonValue.SetFloat(value);

    return jsonValue;
}

rapidjson::Value makeJsonValue(const double value)
{
    rapidjson::Value jsonValue;
    jsonValue.SetDouble(value);

    return jsonValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ERR_TYPE_E cJson::setMember(const cString& label, const cString& value)
{
    rapidjson::Value jsonValue = makeJsonValue(value);

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsString()) {
            _doc[label.to_string()].Swap(jsonValue);        
        }
        else {
            return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
        }
    }
    else {
        rapidjson::Value jsonLabel = makeJsonValue(label);
        _doc.AddMember(jsonLabel, jsonValue, _doc.GetAllocator());
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::setMember(const cString& label, const bool value)
{
    rapidjson::Value jsonValue = makeJsonValue(value);

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsBool()) {
            _doc[label.to_string()].Swap(jsonValue);        
        }
        else {
            return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
        }
    }
    else {
        rapidjson::Value jsonLabel = makeJsonValue(label);
        _doc.AddMember(jsonLabel, jsonValue, _doc.GetAllocator());
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::setMember(const cString& label, const int32 value)
{
    rapidjson::Value jsonValue = makeJsonValue(value);

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsInt()) {
            _doc[label.to_string()].Swap(jsonValue);        
        }
        else {
            return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
        }
    }
    else {
        rapidjson::Value jsonLabel = makeJsonValue(label);
        _doc.AddMember(jsonLabel, jsonValue, _doc.GetAllocator());
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::setMember(const cString& label, const uint32 value)
{
    rapidjson::Value jsonValue = makeJsonValue(value);

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsUint()) {
            _doc[label.to_string()].Swap(jsonValue);        
        }
        else {
            return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
        }
    }
    else {
        rapidjson::Value jsonLabel = makeJsonValue(label);
        _doc.AddMember(jsonLabel, jsonValue, _doc.GetAllocator());
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::setMember(const cString& label, const int64 value)
{
    rapidjson::Value jsonValue = makeJsonValue(value);

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsInt64()) {
            _doc[label.to_string()].Swap(jsonValue);        
        }
        else {
            return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
        }
    }
    else {
        rapidjson::Value jsonLabel = makeJsonValue(label);
        _doc.AddMember(jsonLabel, jsonValue, _doc.GetAllocator());
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::setMember(const cString& label, const uint64 value)
{
    rapidjson::Value jsonValue = makeJsonValue(value);

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsUint64()) {
            _doc[label.to_string()].Swap(jsonValue);        
        }
        else {
            return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
        }
    }
    else {
        rapidjson::Value jsonLabel = makeJsonValue(label);
        _doc.AddMember(jsonLabel, jsonValue, _doc.GetAllocator());
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::setMember(const cString& label, const float value)
{
    rapidjson::Value jsonValue = makeJsonValue(value);

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsFloat()) {
            _doc[label.to_string()].Swap(jsonValue);        
        }
        else {
            return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
        }
    }
    else {
        rapidjson::Value jsonLabel = makeJsonValue(label);
        _doc.AddMember(jsonLabel, jsonValue, _doc.GetAllocator());
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::setMember(const cString& label, const double value)
{
    rapidjson::Value jsonValue = makeJsonValue(value);

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsDouble()) {
            _doc[label.to_string()].Swap(jsonValue);        
        }
        else {
            return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
        }
    }
    else {
        rapidjson::Value jsonLabel = makeJsonValue(label);
        _doc.AddMember(jsonLabel, jsonValue, _doc.GetAllocator());
    }

    return ERR_TYPE_E::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ERR_TYPE_E cJson::getMember(const cString& label, cString* pValue)
{
    if (pValue == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsString()) {
            pValue->format(L"%s", _doc[label.to_string()].GetString());
        }
    }
    else {
        return ERR_TYPE_E::CAN_NOT_FIND_MEMBER;
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::getMember(const cString& label, bool* pValue)
{
    if (pValue == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsBool()) {
            *pValue = _doc[label.to_string()].GetBool();
        }
    }
    else {
        return ERR_TYPE_E::CAN_NOT_FIND_MEMBER;
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::getMember(const cString& label, int32* pValue)
{
    if (pValue == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsInt()) {
            *pValue = _doc[label.to_string()].GetInt();
        }
    }
    else {
        return ERR_TYPE_E::CAN_NOT_FIND_MEMBER;
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::getMember(const cString& label, uint32* pValue)
{
    if (pValue == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsUint()) {
            *pValue = _doc[label.to_string()].GetUint();
        }
    }
    else {
        return ERR_TYPE_E::CAN_NOT_FIND_MEMBER;
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::getMember(const cString& label, int64* pValue)
{
    if (pValue == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsInt64()) {
            *pValue = _doc[label.to_string()].GetInt64();
        }
    }
    else {
        return ERR_TYPE_E::CAN_NOT_FIND_MEMBER;
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::getMember(const cString& label, uint64* pValue)
{
    if (pValue == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsUint64()) {
            *pValue = _doc[label.to_string()].GetUint64();
        }
    }
    else {
        return ERR_TYPE_E::CAN_NOT_FIND_MEMBER;
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::getMember(const cString& label, float* pValue)
{
    if (pValue == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsFloat()) {
            *pValue = _doc[label.to_string()].GetFloat();
        }
    }
    else {
        return ERR_TYPE_E::CAN_NOT_FIND_MEMBER;
    }

    return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJson::getMember(const cString& label, double* pValue)
{
    if (pValue == nullptr) {
        return ERR_TYPE_E::NULL_PTR;
    }

    if (_doc.HasMember(label.to_string())) {
        if (_doc[label.to_string()].IsDouble()) {
            *pValue = _doc[label.to_string()].GetDouble();
        }
    }
    else {
        return ERR_TYPE_E::CAN_NOT_FIND_MEMBER;
    }

    return ERR_TYPE_E::SUCCESS;
}