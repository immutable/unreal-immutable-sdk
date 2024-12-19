@echo off

set OPENAPI_GENERATOR_CLI=openapi-generator-cli

set GENERATOR=-g cpp-ue4
set TEMPLATE=-t ../template/cpp-ue4
set INPUT=-i https://imx-openapiv3-mr-sandbox.s3.us-east-2.amazonaws.com/openapi.json
set OUTPUT=-o ../..
set ADDITIONAL_PROPERTIES=--additional-properties=modelNamePrefix="API",cppNamespace=ImmutablezkEVMAPI,unrealModuleName=ImmutablezkEVMAPI

%OPENAPI_GENERATOR_CLI% generate %GENERATOR% %TEMPLATE% %INPUT% %OUTPUT% %ADDITIONAL_PROPERTIES%

pause