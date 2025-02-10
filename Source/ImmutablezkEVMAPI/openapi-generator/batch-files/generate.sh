#!/bin/bash

OPENAPI_GENERATOR_CLI=openapi-generator-cli

GENERATOR="-g cpp-ue4"
TEMPLATE="-t ../template/cpp-ue4"
CONFIG="-c ../config/cpp-ue4.json"
INPUT="-i https://imx-openapiv3-mr-sandbox.s3.us-east-2.amazonaws.com/openapi.json"
OUTPUT="-o ../.."
ADDITIONAL_PROPERTIES="--additional-properties="

$OPENAPI_GENERATOR_CLI generate "$GENERATOR" "$TEMPLATE" "$CONFIG" "$INPUT" "$OUTPUT" "$ADDITIONAL_PROPERTIES"