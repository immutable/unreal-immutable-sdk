# Usage

TO generate OpenAPI locally:

    ```
    openapi-generator-cli generate -i openapi.json -g cpp-ue4 -o . --additional-properties=modelNamePrefix="API",cppNamespace=ImmutableOrderbook,unrealModuleName=ImmutableOrderbook
    
    ```