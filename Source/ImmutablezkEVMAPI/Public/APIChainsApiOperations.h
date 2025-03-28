/**
 * Immutable zkEVM API
 * Immutable Multi Rollup API
 *
 * OpenAPI spec version: 1.0.0
 * Contact: support@immutable.com
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * https://github.com/OpenAPITools/openapi-generator
 * Do not edit the class manually.
 */

#pragma once

#include "APIBaseModel.h"
#include "APIChainsApi.h"

#include "APIAPIError400.h"
#include "APIAPIError404.h"
#include "APIAPIError500.h"
#include "APIListChainsResult.h"

namespace ImmutablezkEVMAPI
{

/* List supported chains
 *
 * List supported chains
*/
class IMMUTABLEZKEVMAPI_API APIChainsApi::ListChainsRequest : public Request
{
public:
    virtual ~ListChainsRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* Encoded page cursor to retrieve previous or next page. Use the value returned in the response. */
	TOptional<FString> PageCursor;
	/* Maximum number of items to return */
	TOptional<int32> PageSize;
};

class IMMUTABLEZKEVMAPI_API APIChainsApi::ListChainsResponse : public Response
{
public:
    virtual ~ListChainsResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    APIListChainsResult Content;
};

}
