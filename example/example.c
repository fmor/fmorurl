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
