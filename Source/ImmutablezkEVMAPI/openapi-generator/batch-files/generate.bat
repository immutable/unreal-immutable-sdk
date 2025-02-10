@echo off

set OPENAPI_GENERATOR_CLI=openapi-generator-cli

set GENERATOR=-g cpp-ue4
set TEMPLATE=-t ../template/cpp-ue4
set CONFIG=-c ../config/cpp-ue4.json
set INPUT=-i https://imx-openapiv3-mr-sandbox.s3.us-east-2.amazonaws.com/openapi.json
set OUTPUT=-o ../..
set ADDITIONAL_PROPERTIES=--additional-properties=

%OPENAPI_GENERATOR_CLI% generate %GENERATOR% %TEMPLATE% %CONFIG% %INPUT% %OUTPUT% %ADDITIONAL_PROPERTIES%

pause