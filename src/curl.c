#include "include/curl.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <hermes/AST.h>
#include <hermes/dynamic_list.h>
#include <hermes/hermes_runtime.h>

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);

    if (s->ptr == NULL)
    {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }

    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);

    if (s->ptr == NULL)
    {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}


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
    else
    {
        printf("Unsupported option `%s`", ast_string->string_value);
        exit(1);
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
    runtime_expect_args(args, 3, (int[]){ AST_OBJECT, AST_INTEGER, AST_ANY });

    int last_arg_type = AST_ANY;

    switch (((AST_T*)args->items[1])->int_value)
    {
        case CURLOPT_URL: last_arg_type = AST_STRING; break;
        default: last_arg_type = AST_ANY; break;
    }

    runtime_expect_args(args, 3, (int[]){ AST_OBJECT, AST_INTEGER, last_arg_type });

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
    CURL* curl = (CURL*)ast_object->object_value;
    
    CURLcode res;

    struct string s;
    init_string(&s);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

    res = curl_easy_perform(curl);

    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = s.ptr;

    return ast_string;
}

AST_T* hcurl_easy_cleanup(AST_T* self, dynamic_list_T* args)
{
    runtime_expect_args(args, 1, (int[]){ AST_OBJECT });

    AST_T* ast_object = (AST_T*) args->items[0];

    curl_easy_cleanup((CURL*)ast_object->object_value);

    return init_ast(AST_NULL);
}
