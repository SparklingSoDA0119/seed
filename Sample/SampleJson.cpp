#include <Sample/SampleJson.h>

using namespace Seed;

SampleJson::SampleJson()
	: cJsonObject()
	, _label(L"Label"), _data(1)
{

}

SampleJson::~SampleJson()
{

}

ERR_TYPE_E SampleJson::createJsonDocument()
{
	//ERR_TYPE_E eRet = createMember(_label, &_data, VALUE_TYPE_E::INT32);

	//printf("<%s:%s(%d)> ** JsonStr : %s\n", __FILE__, __FUNCTION__, __LINE__, toJsonString().to_string().c_str());
	

	return ERR_TYPE_E::SUCCESS;
}