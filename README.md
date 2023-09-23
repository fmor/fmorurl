# fmorurl

C89 Url parser that does no malloc neither it modify the supplied string. No validation is performed.


This library does __NOT__ support UTF strings.


## Parsing url

call __url_parse__ function to fill a an __url__ struct which is defined as follow :

```C

struct url_str
{
    const char* str;
    int         size;
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
```



```C
#include <string.h>
#include <url_parser.h>

int main()
{
    int r;
    struct url u;

    const char* url =  "http://john:doe@example.com:8888/path/to/file?param=value#fragment";

    r = url_parse( &u, url, strlen(url) );
    if( r != url_parse_ok )
        return -1;

    url_dump( &u, url );

    return 0;
}
```


## Parsing url query

Use url_parse_query, it fill an array of __url_query_param__ structs and returns the number of key value pairs ( even if one of the two is null ) or < 0 in case of error.

```C
struct url_query_param
{
    struct url_str name;
    struct url_str value;
};
```

```C
#include <stdio.h>
#include <string.h>

#include <urlparser.h>

#define QUERY_PARAMS_MAX    3

int main()
{
    int r;
    struct url u;
    struct url_query_param params[QUERY_PARAMS_MAX];

    const char* url =  "http://john:doe@example.com:8888/path/to/file?param0=value0&param1=value1#fragment";

    r = url_parse( &u, url, strlen(url) );
    if( r != url_parse_ok )
        return -1;

    url_dump( &u, url );


    // query
    printf( "url query params : '%.*s'\n", u.query.size, u.query.str );
    r = url_parse_query( params, QUERY_PARAMS_MAX, '&', u.query.str, u.query.size );
    if( r < 0 )
        return -1;

    url_dump_query_params( params, r );

    // query parse : supplied array too small
    printf( "url query params : '%.*s'\n", u.query.size, u.query.str );
    r = url_parse_query( params, 1 , '&', u.query.str, u.query.size );
    if( r == url_parse_query_error_params_count )
        printf( "params is too small\n");
    else
        return -1;

    return 0;
}
```




## Include in your project

Just drop src/url_parser.h and src/url_parser.c in your project sources directory, or [wrap](https://mesonbuild.com/Wrap-dependency-system-manual.html) it with meson.

## Meson options

|  Name                | Description            | Type     | Default |
|----------------------|------------------------|----------|---------|
| fmorurl_build_tests  | build test executable  | boolean  | true    |



