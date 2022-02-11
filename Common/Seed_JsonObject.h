#ifndef _SEED_JSON_OBJECT_H_
#define _SEED_JSON_OBJECT_H_

#define NOMINMAX
//#define RAPIDJSON_HAS_STDSTRING

#include <Common/Seed_String.h>
#include <rapidjson/document.h>
#include <vector>
#include <list>
#include <deque>

namespace Seed {

	enum class VALUE_TYPE_E {
		NONE = 0,
		INT32,
		UINT32,
		INT64,
		UINT64,
		STRING,
		OBJECT,
		VECTOR,
		LIST,
		MAX_VALUE_TYPE,
	};
	
	class cJsonObject : public cObject {
	public :
		cJsonObject() {}
		virtual ~cJsonObject() {}

	protected :
		rapidjson::Document _jsonDoc;

	public :
		ERR_TYPE_E loadFromFile(const cString& filename);
		ERR_TYPE_E writeToFile(const cString& filename, bool useEnter = true);
		ERR_TYPE_E parseFromString(const cString& jsonStr);

		virtual ERR_TYPE_E parseMemberValue() = 0;

	public :
		rapidjson::Value makeJsonValue(const bool value);
		rapidjson::Value makeJsonValue(const int32 value);
		rapidjson::Value makeJsonValue(const uint32 value);
		rapidjson::Value makeJsonValue(const int64 value);
		rapidjson::Value makeJsonValue(const uint64 value);
		rapidjson::Value makeJsonValue(const float value);
		rapidjson::Value makeJsonValue(const double value);
		rapidjson::Value makeJsonValue(const cString& value);
		
		template<typename T> rapidjson::Value makeJsonValue(const std::vector<T>& value);

		template<typename T> rapidjson::Value makeJsonValue(const std::list<T>& value);

		template<typename T> rapidjson::Value makeJsonValue(const std::deque<T>& value);
		

		ERR_TYPE_E setMember(const cString& label, const bool value);
		ERR_TYPE_E setMember(const cString& label, const int32 value);
		ERR_TYPE_E setMember(const cString& label, const uint32 value);
		ERR_TYPE_E setMember(const cString& label, const int64 value);
		ERR_TYPE_E setMember(const cString& label, const uint64 value);
		ERR_TYPE_E setMember(const cString& label, const float value);
		ERR_TYPE_E setMember(const cString& label, const double value);
		ERR_TYPE_E setMember(const cString& label, const cString& value);

		ERR_TYPE_E getMember(const cString& label, bool* pValue);
		ERR_TYPE_E getMember(const cString& label, int32* pValue);
		ERR_TYPE_E getMember(const cString& label, uint32* pValue);
		ERR_TYPE_E getMember(const cString& label, int64* pValue);
		ERR_TYPE_E getMember(const cString& label, uint64* pValue);
		ERR_TYPE_E getMember(const cString& label, float* pValue);
		ERR_TYPE_E getMember(const cString& label, double* pValue);
		ERR_TYPE_E getMember(const cString& label, cString* pValue);


		

		rapidjson::Value makeJsonValue2(const std::vector<int>& value);
	}; 
#if 0
	class cJsonObject : public cObject {
	public :
		cJsonObject();
		
		virtual ~cJsonObject();

	protected :
		rapidjson::Document _doc;

	public :
		ERR_TYPE_E loadFromFile(const cString& filename);
		ERR_TYPE_E writeToFile(const cString& filename, bool useEnter = true);
		ERR_TYPE_E parseFromString(const cString& jsonStr);

	public :
		rapidjson::Value makeValueFromDocument();
		rapidjson::Value convertStringToValue(const cString& srcStr);

	public :
		
		rapidjson::Value makeUint32Value(int32 value);


	public :
		virtual ERR_TYPE_E parseMemberValue() = 0;

	private :
		ERR_TYPE_E createCheck(rapidjson::Value& name, rapidjson::Value& value);

	public :
		ERR_TYPE_E createObjectMember(const cString& name, rapidjson::Value& value);
		
		//rapidjson::Value makeInt32Value(int32 value);
		//ERR_TYPE_E createInt32Member(const cString& name, int32 value);
		

	public :
		cString toJsonString(bool useEnter = false);

#define DECLARE_SEED_JSON_TYPE_MEMFUNCS(T, Name)	\
		rapidjson::Value make##Name##Value(T value);\
		ERR_TYPE_E create##Name##Member(const cString& name, T value);

		DECLARE_SEED_JSON_TYPE_MEMFUNCS(int32, Int32)
		DECLARE_SEED_JSON_TYPE_MEMFUNCS(uint32, Uint32)
		DECLARE_SEED_JSON_TYPE_MEMFUNCS(int64, Int64)
		DECLARE_SEED_JSON_TYPE_MEMFUNCS(uint64, Uint64)
		DECLARE_SEED_JSON_TYPE_MEMFUNCS(bool, Bool)
		DECLARE_SEED_JSON_TYPE_MEMFUNCS(float, Float)
		DECLARE_SEED_JSON_TYPE_MEMFUNCS(double, Double)
		
	};
#endif

}

#endif
