#!/bin/bash

OPENAPI_GENERATOR_CLI=openapi-generator-cli

GENERATOR="-g cpp-ue4"
TEMPLATE="-t ../template/cpp-ue4"
INPUT="-i https://imx-openapiv3-mr-sandbox.s3.us-east-2.amazonaws.com/openapi.json"
OUTPUT="-o ../.."
ADDITIONAL_PROPERTIES="--additional-properties=modelNamePrefix=API,cppNamespace=ImmutablezkEVMAPI,unrealModuleName=ImmutablezkEVMAPI"

$OPENAPI_GENERATOR_CLI generate "$GENERATOR" "$TEMPLATE" "$INPUT" "$OUTPUT" "$ADDITIONAL_PROPERTIES"