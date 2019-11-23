#include "include/curl.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <hermes/AST.h>
#include <hermes/dynamic_list.h>
#include <hermes/hermes_runtime.h>


AST_T* hcurl_get_option(AST_T* self, dynamic_list_T* args)
{
    runtime_expect_args(args, 1, (int[]){ AST_STRING });
    AST_T* ast_string = (AST_T*) args->items[0];

    if (strcmp(ast_string->string_value, "CURLOPT_URL") == 0)
    {
        AST_T* ast_integer = init_ast(AST_INTEGER);
        ast_integer->int_value = CURLOPT_URL;

        return ast_integer; 
    }
}

AST_T* hcurl_easy_init(AST_T* self, dynamic_list_T* args)
{
    AST_T* curl_object = init_ast(AST_OBJECT);
    curl_object->object_value = (void*) curl_easy_init();

    return curl_object;
}

AST_T* hcurl_easy_setopt(AST_T* self, dynamic_list_T* args)
{
    runtime_expect_args(args, 3, (int[]){ AST_OBJECT, AST_INTEGER, AST_STRING });

    AST_T* ast_object = (AST_T*) args->items[0];
    AST_T* ast_integer = (AST_T*) args->items[1];
    AST_T* ast_string = (AST_T*) args->items[2];

    curl_easy_setopt((CURL*)ast_object->object_value, ast_integer->int_value, ast_string->string_value);

    return init_ast(AST_NULL);
}

AST_T* hcurl_easy_perform(AST_T* self, dynamic_list_T* args)
{
    runtime_expect_args(args, 1, (int[]){ AST_OBJECT });

    AST_T* ast_object = (AST_T*) args->items[0];

    curl_easy_perform((CURL*)ast_object->object_value);

    return init_ast(AST_NULL);
}

AST_T* hcurl_easy_cleanup(AST_T* self, dynamic_list_T* args)
{
    runtime_expect_args(args, 1, (int[]){ AST_OBJECT });

    AST_T* ast_object = (AST_T*) args->items[0];

    curl_easy_cleanup((CURL*)ast_object->object_value);

    return init_ast(AST_NULL);
}
