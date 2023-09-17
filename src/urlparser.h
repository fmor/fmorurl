/*
The MIT License (MIT)

Copyright 2023 Frédéric Morvan

Permission is hereby granted, free of charge,
to any person obtaining a copy of this software
and associated documentation files (the “Software”),
to deal in the Software without restriction,
including without limitation the rights to use,copy,
modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF
ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#ifndef FMORURL_HEADER
#define FMORURL_HEADER

#ifdef __cplusplus
extern "C" {
#endif

enum url_parse_error
{
    url_parse_ok = 0,
    url_parse_error_params,
    url_parse_error_bad_hostname,
    url_parse_error_bad_port,
};

enum url_parse_query_error
{
    url_parse_query_error_params_count = -2,
    url_parse_query_error_bad_parameter = -1,
};

struct url_str
{
    const char* str;
    int         size;
};

struct url_query_param
{
    struct url_str name;
    struct url_str value;
};

struct url
{
    struct url_str scheme;
    struct url_str user;
    struct url_str password;
    struct url_str hostname;
    struct url_str port;
    struct url_str path;
    struct url_str query;
    struct url_str fragment;
};

int url_parse( struct url* u, const char* str, int len );
int url_parse_query( struct url_query_param* params, int params_max, char separator, const char* str, int str_size );

void url_dump( struct url* u, const char* url );
void url_dump_query_params( struct url_query_param* params, int params_count );

#ifdef __cplusplus
}
#endif

#endif
