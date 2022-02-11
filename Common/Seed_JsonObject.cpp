#include <Common/Seed_JsonObject.h>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#include <fstream>

using namespace Seed;

ERR_TYPE_E cJsonObject::loadFromFile(const cString& filename)
{
	if (filename.isEmpty()) {
		return ERR_TYPE_E::FILENAME_IS_EMPTY;
	}

	std::ifstream ifs(filename.to_string());
	if (!ifs.is_open()) {
		return ERR_TYPE_E::CAN_NOT_OPEN_FILE;
	}

	rapidjson::IStreamWrapper isw(ifs);
	_jsonDoc.SetObject();
	_jsonDoc.ParseStream(isw);

	if (_jsonDoc.HasParseError()) {
		_jsonDoc.SetObject();
		return ERR_TYPE_E::JSON_PARSE_ERROR;
	}

	ERR_TYPE_E eRet = parseMemberValue();
	if (eRet != ERR_TYPE_E::SUCCESS) {
		return eRet;
	}

	return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJsonObject::writeToFile(const cString& filename, bool useEnter)
{
	if (filename.isEmpty()) {
		return ERR_TYPE_E::FILENAME_IS_EMPTY;
	}

	std::ofstream ofs(filename.to_string());

	if (!ofs.is_open()) {
		return ERR_TYPE_E::CAN_NOT_OPEN_FILE;
	}

	rapidjson::OStreamWrapper osw(ofs);

	if (useEnter) {
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
		_jsonDoc.Accept(writer);
	}
	else {
		rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
		_jsonDoc.Accept(writer);
	}

	return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJsonObject::parseFromString(const cString& jsonStr)
{
	if (jsonStr.isEmpty()) {
		return ERR_TYPE_E::FILENAME_IS_EMPTY;
	}

	_jsonDoc.SetObject();
	_jsonDoc.Parse(jsonStr.to_string());
	if (_jsonDoc.HasParseError()) {
		_jsonDoc.SetObject();
		return ERR_TYPE_E::JSON_PARSE_ERROR;
	}

	ERR_TYPE_E eRet = parseMemberValue();
	if (eRet != ERR_TYPE_E::SUCCESS) {
		return eRet;
	}

	return ERR_TYPE_E::SUCCESS;
}

#define SEED_MAKE_VALUE_FUNC_DEFINE(ValueType, func)	\
{	\
	rapidjson::Value jsonValue(##ValueType##);	\
	jsonValue.##func##(value);	\
	\
	return jsonValue;	\
}

rapidjson::Value cJsonObject::makeJsonValue(const bool value)
SEED_MAKE_VALUE_FUNC_DEFINE(rapidjson::kTrueType, SetBool)

rapidjson::Value cJsonObject::makeJsonValue(const int32 value)
SEED_MAKE_VALUE_FUNC_DEFINE(rapidjson::kNumberType, SetInt)

rapidjson::Value cJsonObject::makeJsonValue(const uint32 value)
SEED_MAKE_VALUE_FUNC_DEFINE(rapidjson::kNumberType, SetUint)

rapidjson::Value cJsonObject::makeJsonValue(const int64 value)
SEED_MAKE_VALUE_FUNC_DEFINE(rapidjson::kNumberType, SetInt64)

rapidjson::Value cJsonObject::makeJsonValue(const uint64 value)
SEED_MAKE_VALUE_FUNC_DEFINE(rapidjson::kNumberType, SetUint64)

rapidjson::Value cJsonObject::makeJsonValue(const float value)
SEED_MAKE_VALUE_FUNC_DEFINE(rapidjson::kNumberType, SetFloat)

rapidjson::Value cJsonObject::makeJsonValue(const double value)
SEED_MAKE_VALUE_FUNC_DEFINE(rapidjson::kNumberType, SetDouble)

rapidjson::Value cJsonObject::makeJsonValue(const cString& value)
{
	rapidjson::Value jsonValue(rapidjson::kStringType);
	jsonValue.SetString(value.to_string(), _jsonDoc.GetAllocator());

	return jsonValue;
}

template <typename T>
rapidjson::Value cJsonObject::makeJsonValue(const std::vector<T>& value)
{
	rapidjson::Value jsonValue(rapidjson::kArrayType);
	
	if (value.size != 0) {
		jsonValue.Reserve(static_cast<rapidjson::SizeType>(value.size()), _jsonDoc.GetAllocator());

		std::vector<T>::iterator itr = value.begin();
		for (itr; itr != value.end(); ++itr) {
			jsonValue.PushBack(makeJsonValue(*itr), _jsonDoc.GetAllocator());
		}
	}
	
	return jsonValue;
}

#define SEED_SET_MEMBER_FUNC_DEFINE											\
{																			\
	rapidjson::Value jsonLabel = makeJsonValue(label);						\
	rapidjson::Value jsonValue = makeJsonValue(value);						\
																			\
	if (_jsonDoc.HasMember(label.to_string())) {							\
		_jsonDoc[label.to_string()].Swap(jsonValue);						\
	}																		\
	else {																	\
		_jsonDoc.AddMember(jsonLabel, jsonValue, _jsonDoc.GetAllocator());	\
																			\
		if (!_jsonDoc.HasMember(label.to_string())) {						\
			return ERR_TYPE_E::JSON_CAN_NOT_ADD_MEMBER;						\
		}																	\
	}																		\
																			\
	return ERR_TYPE_E::SUCCESS;												\
}

ERR_TYPE_E cJsonObject::setMember(const cString& label, const bool value)
{
	rapidjson::Value jsonLabel = makeJsonValue(label);
	rapidjson::Value jsonValue = makeJsonValue(value);

	if (_jsonDoc.HasMember(label.to_string())) {
		_jsonDoc[label.to_string()].Swap(jsonValue);
	}
	else {
		_jsonDoc.AddMember(jsonLabel, jsonValue, _jsonDoc.GetAllocator());

		if (!_jsonDoc.HasMember(label.to_string())) {
			return ERR_TYPE_E::JSON_CAN_NOT_ADD_MEMBER;
		}
	}

	return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJsonObject::setMember(const cString& label, const int32 value)
SEED_SET_MEMBER_FUNC_DEFINE

ERR_TYPE_E cJsonObject::setMember(const cString& label, const uint32 value)
SEED_SET_MEMBER_FUNC_DEFINE

ERR_TYPE_E cJsonObject::setMember(const cString& label, const int64 value)
SEED_SET_MEMBER_FUNC_DEFINE

ERR_TYPE_E cJsonObject::setMember(const cString& label, const uint64 value)
SEED_SET_MEMBER_FUNC_DEFINE

ERR_TYPE_E cJsonObject::setMember(const cString& label, const float value)
SEED_SET_MEMBER_FUNC_DEFINE

ERR_TYPE_E cJsonObject::setMember(const cString& label, const double value)
SEED_SET_MEMBER_FUNC_DEFINE

ERR_TYPE_E cJsonObject::setMember(const cString& label, const cString& value)
SEED_SET_MEMBER_FUNC_DEFINE


#define SEED_GET_MEMBER_FUNC_DEFINE(jsonType, func)						\
{																		\
	if (_jsonDoc.HasMember(label.to_string())) {						\
		if (_jsonDoc[label.to_string()].GetType() == ##jsonType##) {	\
			*pValue = _jsonDoc[label.to_string()].##func##();			\
			return ERR_TYPE_E::SUCCESS;									\
		}																\
		else {															\
			pValue = nullptr;											\
			return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;				\
		}																\
	}																	\
	else {																\
		pValue = nullptr;												\
		return ERR_TYPE_E::JSON_CAN_NOT_FIND_MEMBER;					\
	}																	\
}

ERR_TYPE_E cJsonObject::getMember(const cString& label, bool* pValue)
{
	if (_jsonDoc.HasMember(label.to_string())) {
		if (_jsonDoc[label.to_string()].GetType() == rapidjson::kTrueType ||
			_jsonDoc[label.to_string()].GetType() == rapidjson::kFalseType) {
			*pValue = _jsonDoc[label.to_string()].GetBool();
			return ERR_TYPE_E::SUCCESS;
		}
		else {
			pValue = nullptr;
			return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
		}
	}
	else {
		pValue = nullptr;
		return ERR_TYPE_E::JSON_CAN_NOT_FIND_MEMBER;
	}
}

ERR_TYPE_E cJsonObject::getMember(const cString& label, int32* pValue)
SEED_GET_MEMBER_FUNC_DEFINE(rapidjson::kNumberType, GetInt)

ERR_TYPE_E cJsonObject::getMember(const cString& label, uint32* pValue)
SEED_GET_MEMBER_FUNC_DEFINE(rapidjson::kNumberType, GetUint)

ERR_TYPE_E cJsonObject::getMember(const cString& label, int64* pValue)
SEED_GET_MEMBER_FUNC_DEFINE(rapidjson::kNumberType, GetInt64)

ERR_TYPE_E cJsonObject::getMember(const cString& label, uint64* pValue)
SEED_GET_MEMBER_FUNC_DEFINE(rapidjson::kNumberType, GetUint64)

ERR_TYPE_E cJsonObject::getMember(const cString& label, float* pValue)
SEED_GET_MEMBER_FUNC_DEFINE(rapidjson::kNumberType, GetFloat)

ERR_TYPE_E cJsonObject::getMember(const cString& label, double* pValue)
SEED_GET_MEMBER_FUNC_DEFINE(rapidjson::kNumberType, GetDouble)

ERR_TYPE_E cJsonObject::getMember(const cString& label, cString* pValue)
{
	if (_jsonDoc.HasMember(label.to_string())) {
		if (_jsonDoc[label.to_string()].GetType() == rapidjson::kStringType) {
			*pValue = cString(_jsonDoc[label.to_string()].GetString());
			return ERR_TYPE_E::SUCCESS;
		}
		else {
			pValue = nullptr;
			return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
		}
	}
	else {
		pValue = nullptr;
		return ERR_TYPE_E::JSON_CAN_NOT_FIND_MEMBER;
	}
}

#if 0
cJsonObject::cJsonObject()
{
	_doc.SetObject();
}

cJsonObject::~cJsonObject()
{

}

ERR_TYPE_E cJsonObject::loadFromFile(const cString& filename)
{
	if (filename.isEmpty()) {
		return ERR_TYPE_E::FILENAME_IS_EMPTY;
	}

	std::ifstream ifs(filename.to_string());
	if (!ifs.is_open()) {
		return ERR_TYPE_E::CAN_NOT_OPEN_FILE;
	}

	rapidjson::IStreamWrapper isw(ifs);
	_doc.SetObject();
	_doc.ParseStream(isw);

	if (_doc.HasParseError()) {
		_doc.SetObject();
		return ERR_TYPE_E::JSON_PARSE_ERROR;
	}

	ERR_TYPE_E eRet = parseMemberValue();
	if (eRet != ERR_TYPE_E::SUCCESS) {
		return eRet;
	}

	return ERR_TYPE_E::SUCCESS;
}

ERR_TYPE_E cJsonObject::writeToFile(const cString& filename, bool useEnter)
{
	if (filename.isEmpty()) {
		return ERR_TYPE_E::FILENAME_IS_EMPTY;
	}

	std::ofstream ofs(filename.to_string());

	if (!ofs.is_open()) {
		return ERR_TYPE_E::CAN_NOT_OPEN_FILE;
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

ERR_TYPE_E cJsonObject::parseFromString(const cString& jsonStr)
{
	if (jsonStr.isEmpty()) {
		return ERR_TYPE_E::FILENAME_IS_EMPTY;
	}

	_doc.SetObject();
	_doc.Parse(jsonStr.to_string());
	if (_doc.HasParseError()) {
		_doc.SetObject();
		return ERR_TYPE_E::JSON_PARSE_ERROR;
	}

	ERR_TYPE_E eRet = parseMemberValue();
	if (eRet != ERR_TYPE_E::SUCCESS) {
		return eRet;
	}

	return ERR_TYPE_E::SUCCESS;
}

cString cJsonObject::toJsonString(bool useEnter)
{
	if (!_doc.IsObject()) {
		return L"";
	}

	rapidjson::StringBuffer buffer;

	if (useEnter) {
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		_doc.Accept(writer);
	}
	else {
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		_doc.Accept(writer);
	}

	return cString(buffer.GetString());
}

rapidjson::Value cJsonObject::makeValueFromDocument()
{
	rapidjson::Value docValue;
	docValue.SetObject();

	rapidjson::Value::ConstMemberIterator itr;
	for (itr = _doc.MemberBegin(); itr != _doc.MemberEnd(); ++itr) {
		cString nameStr(itr->name.GetString());
		rapidjson::Value jsonName = convertStringToValue(nameStr);
		rapidjson::Value jsonValue(itr->value, _doc.GetAllocator());
		docValue.AddMember(jsonName, jsonValue, _doc.GetAllocator());
	}

	return docValue;
}

rapidjson::Value cJsonObject::convertStringToValue(const cString& srcStr)
{
	rapidjson::Value dstValue(rapidjson::kStringType);
	dstValue.SetString(srcStr.to_string(), _doc.GetAllocator());

	return dstValue;
}

rapidjson::Value cJsonObject::makeUint32Value(int32 value)
{
	rapidjson::Value jsonValue(rapidjson::kNumberType);
	jsonValue.SetUint(value);

	return jsonValue;
}


ERR_TYPE_E cJsonObject::createCheck(rapidjson::Value& name, rapidjson::Value& value)
{
	cString jsonName(name.GetString());
	rapidjson::Type eType = value.GetType();
	_doc.AddMember(name, value, _doc.GetAllocator());

	if (_doc.HasMember(jsonName.to_string())) {
		if (_doc[jsonName.to_string()].GetType() == eType) {
			return ERR_TYPE_E::SUCCESS;
		}
		else {
			return ERR_TYPE_E::JSON_VALUE_TYPE_MISS_MATCH;
		}
	}
	else {
		return ERR_TYPE_E::JSON_CAN_NOT_FIND_MEMBER;
	}
}



ERR_TYPE_E cJsonObject::createObjectMember(const cString& name, rapidjson::Value& value)
{
	rapidjson::Value jsonName = convertStringToValue(name);
	
	ERR_TYPE_E eErr = createCheck(jsonName, value);
	if (eErr != ERR_TYPE_E::SUCCESS) {
		return eErr;
	}
	
	return ERR_TYPE_E::SUCCESS;
}

#define MACRO_SEED_JSON_TYPE_MEMFUCS_DEFINE(T, Name, TT, SetFunc)	\
	rapidjson::Value cJsonObject::make##Name##Value(T value)	\
	{															\
		rapidjson::Value jValue(##TT##);						\
		jValue.##SetFunc##;									\
		return jValue;											\
	}															\
	ERR_TYPE_E cJsonObject::create##Name##Member(const cString& name, T value)	\
	{																			\
		rapidjson::Value jsonName = convertStringToValue(name);					\
		rapidjson::Value jValue(TT);											\
		jValue.##SetFunc##;												\
		ERR_TYPE_E eErr = createCheck(jsonName, jValue);						\
		if (eErr != ERR_TYPE_E::SUCCESS) {										\
			return eErr;														\
		}																		\
		return ERR_TYPE_E::SUCCESS;												\
	}

MACRO_SEED_JSON_TYPE_MEMFUCS_DEFINE(int32, Int32, rapidjson::kNumberType, SetInt(value))
MACRO_SEED_JSON_TYPE_MEMFUCS_DEFINE(uint32, Uint32, rapidjson::kNumberType, SetUint(value))
MACRO_SEED_JSON_TYPE_MEMFUCS_DEFINE(int64, Int64, rapidjson::kNumberType, SetInt64(value))
MACRO_SEED_JSON_TYPE_MEMFUCS_DEFINE(uint64, Uint64, rapidjson::kNumberType, SetUint64(value))
MACRO_SEED_JSON_TYPE_MEMFUCS_DEFINE(bool, Bool, rapidjson::kTrueType, SetBool(value))
MACRO_SEED_JSON_TYPE_MEMFUCS_DEFINE(float, Float, rapidjson::kNumberType, SetFloat(value))
MACRO_SEED_JSON_TYPE_MEMFUCS_DEFINE(double, Double, rapidjson::kNumberType, SetDouble(value))

#endif