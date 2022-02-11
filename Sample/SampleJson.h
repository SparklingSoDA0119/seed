#ifndef _SAMPLE_JSON_H_
#define _SAMPLE_JSON_H_



#include <Common/Seed_JsonObject.h>


//#if !defined(NOMINMAX)

//#endif

//#if !defined(RAPIDJSON_HAS_STDSTRING)

//#endif

namespace Seed {

	class SampleJson : public cJsonObject {
	public :
		SampleJson();
		virtual ~SampleJson();

	private :
		cString _label;	int32 _data;

	public :
		virtual ERR_TYPE_E parseMemberValue() { return ERR_TYPE_E::SUCCESS; }

	public :
		ERR_TYPE_E createJsonDocument();
	};
}

#endif
