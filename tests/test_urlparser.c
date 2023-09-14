#include <urlparser.h>

#include <assert.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int url_str_is_equal( const struct url_str* s, const char* str)
{
    if( s->size != strlen(str) )
        return 0;
    if( strncmp(  str, s->str, s->size ) != 0 )
        return 0;
    return 1;
}

static int url_str_is_empty( const struct url_str* s )
{
    if( s->size  )
        return 0;
    if( s->str )
        return 0;
    return 1;
}


static void test_url_parse( )
{
    int r;
    struct url u;
    const char* str;
    int i;


    printf( "Testing url_parse :\n\n");

    i = 1;

    // NULL
    str = NULL;
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_error_null );


    // empty
    str = "";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );


    // relative
    str = "/";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_equal( &u.path, "/" ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );


    str = "file";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r =  url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_equal( &u.path, "file" ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );


    str = "file:///";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "file" ) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_equal( &u.path, "/" ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );



    str = "file://file.html";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "file" ) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_equal( &u.path, "file.html" ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );


    // path
    str = "mailto:user@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "mailto") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port) );
    assert( url_str_is_equal( &u.path, "user@example.com" ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );




    // query
    str = "?";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "??";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_equal( &u.query, "?") );
    assert( url_str_is_empty( &u.fragment) );

    str = "?query=param";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_equal( &u.query, "query=param") );
    assert( url_str_is_empty( &u.fragment) );

    str = "?param0=?value0?";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_equal( &u.query, "param0=?value0?") );
    assert( url_str_is_empty( &u.fragment) );



    // fragment
    str = "#";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "##fragment";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_equal( &u.fragment, "#fragment") );

    str = "#fragment";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_empty( &u.scheme) );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_equal( &u.fragment, "fragment") );


    // simple
    str = "http://example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://example.com/index.html";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port) );
    assert( url_str_is_equal( &u.path, "/index.html") );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://example.com:8080";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_equal( &u.port, "8080") );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://example.com:8080/index.html";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_equal( &u.port, "8080") );
    assert( url_str_is_equal( &u.path, "/index.html") );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://example.com:8080/index.html?param=value";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_equal( &u.port, "8080") );
    assert( url_str_is_equal( &u.path, "/index.html") );
    assert( url_str_is_equal( &u.query, "param=value") );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://example.com:8080/index.html?param=value#fragment";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_equal( &u.port, "8080") );
    assert( url_str_is_equal( &u.path, "/index.html") );
    assert( url_str_is_equal( &u.query, "param=value") );
    assert( url_str_is_equal( &u.fragment, "fragment") );

    str = "http://example.com:8080/index.html#fragment?param=value#fragment";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_equal( &u.port, "8080") );
    assert( url_str_is_equal( &u.path, "/index.html") );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_equal( &u.fragment, "fragment?param=value#fragment") );

    // userinfo
    str = "http://@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://user@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_equal( &u.user, "user" ) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://user:@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_equal( &u.user, "user" ) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://user:pass@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_equal( &u.user, "user" ) );
    assert( url_str_is_equal( &u.password, "pass" ) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://:pass@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_equal( &u.password, "pass" ) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );


    str = "http://:@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "http://user:pass:pas@s2@@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_equal( &u.user, "user" ) );
    assert( url_str_is_equal( &u.password, "pass:pas@s2@" ) );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );


    // urn
    str = "urn:example:animal:ferret:nose";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "urn") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_equal( &u.path, "example:animal:ferret:nose")  );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );



    // ipv6
    str = "ldap://[2001:db8::7]";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "ldap") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "2001:db8::7" ) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "ldap://[2001:db8::7]/c=GB?objectClass?one";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "ldap") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "2001:db8::7" ) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_equal( &u.path, "/c=GB" ) );
    assert( url_str_is_equal( &u.query, "objectClass?one") );
    assert( url_str_is_empty( &u.fragment) );

    str = "ldap://user@[2001:db8::7]";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "ldap") );
    assert( url_str_is_equal( &u.user, "user") );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "2001:db8::7" ) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "ldap://user:@[2001:db8::7]";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "ldap") );
    assert( url_str_is_equal( &u.user, "user") );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_equal( &u.hostname, "2001:db8::7" ) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "ldap://user:pass@[2001:db8::7]";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "ldap") );
    assert( url_str_is_equal( &u.user, "user") );
    assert( url_str_is_equal( &u.password, "pass") );
    assert( url_str_is_equal( &u.hostname, "2001:db8::7" ) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "ldap://[2001:db8::7";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_error_bad_hostname );

    str = "ldap://e[2001:db8::@toto.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "ldap") );
    assert( url_str_is_equal( &u.user, "e[2001") );
    assert( url_str_is_equal( &u.password, "db8::") );
    assert( url_str_is_equal( &u.hostname, "toto.com" ) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_empty( &u.path ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );

    str = "ldap://a:b:c:toto.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_error_bad_port );


    str = "ldap://a[2001:db8:b:c:a:b:d]:toto.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_error_bad_port );


    str = "mailto:john.doe@example.com";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "mailto") );
    assert( url_str_is_empty( &u.user) );
    assert( url_str_is_empty( &u.password) );
    assert( url_str_is_empty( &u.hostname) );
    assert( url_str_is_empty( &u.port ) );
    assert( url_str_is_equal( &u.path, "john.doe@example.com" ) );
    assert( url_str_is_empty( &u.query) );
    assert( url_str_is_empty( &u.fragment) );


    str = "h://joe@toto:pass@c:3333/path/to/file.html?query=param#fragment";
    printf( "[%02d] : Test with '%s'\n", i++, str );fflush(stdout);
    r = url_parse( &u, str );
    assert( r == url_parse_error_bad_port );
    /*
    assert( url_str_is_equal( &u.scheme, "h") );
    assert( url_str_is_equal( &u.user, "joe@toto") );
    assert( url_str_is_equal( &u.password, "pass") );
    assert( url_str_is_equal( &u.hostname, "c") );
    assert( url_str_is_equal( &u.port, "3333") );
    assert( url_str_is_equal( &u.path, "/path/to/file.html") );
    assert( url_str_is_equal( &u.query, "query=param") );
    assert( url_str_is_equal( &u.fragment, "fragment") );
    */


    // README example url
    str = "http://john:doe@example.com:8888/path/to/file.html?param=value#fragment";
    printf( "[%02d] : Test with '%s'\n", i++, str );
    r = url_parse( &u, str );
    assert( r == url_parse_ok );
    assert( url_str_is_equal( &u.scheme, "http") );
    assert( url_str_is_equal( &u.user, "john") );
    assert( url_str_is_equal( &u.password, "doe") );
    assert( url_str_is_equal( &u.hostname, "example.com") );
    assert( url_str_is_equal( &u.port, "8888") );
    assert( url_str_is_equal( &u.path, "/path/to/file.html") );
    assert( url_str_is_equal( &u.query, "param=value") );
    assert( url_str_is_equal( &u.fragment, "fragment") );

    printf( "\n\n");

}

#define URL_QUERY_MAX_PARAMS    10
static void test_url_parse_query()
{
    const char* str;
    int i;
    int r;
    struct url_query_param params[URL_QUERY_MAX_PARAMS];
    char sep;

    printf( "Testing url_parse_query :\n\n");

    i = 0;
    sep = '&';

    // params
    str = "query";
    r = url_parse_query( NULL, URL_QUERY_MAX_PARAMS, sep, str, strlen(str) );
    printf( "[%02d] : Test params == NUL\n", i++ ); fflush(stdout);
    assert( r == -1 );

    str = "query";
    r = url_parse_query( params, 0, sep, str, strlen(str) );
    printf( "[%02d] : Test params_count == 0\n", i++ ); fflush(stdout);
    assert( r == 0 );


    str = "query";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, 0, str, strlen(str) );
    printf( "[%02d] : Test sep == 0\n", i++ );fflush(stdout);
    assert( r == -1 );

    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, NULL, strlen(str) );
    printf( "[%02d] : Test str NULL\n", i++ );fflush(stdout);
    assert( r == 0 );

    str = NULL;
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, 0 );
    printf( "[%02d] : Test strlen(str) == 0\n", i++ );fflush(stdout);
    assert( r == 0 );



    str = "param";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 1 );
    assert( url_str_is_equal( &params[0].name, "param") );
    assert( url_str_is_empty( &params[0].value) );

    str = "param";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str) - 1  );
    printf( "[%02d] : Test str '%s' but size < strlen\n", i++, str );fflush(stdout);
    assert( r == 1 );
    assert( url_str_is_equal( &params[0].name, "para") );
    assert( url_str_is_empty( &params[0].value) );

    str = "param";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str) + 1  );
    printf( "[%02d] : Test str '%s' but size > strlen\n", i++, str );fflush(stdout);
    assert( r == 1 );
    assert( url_str_is_equal( &params[0].name, "param") );
    assert( url_str_is_empty( &params[0].value) );

    str = "param=";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 1 );
    assert( url_str_is_equal( &params[0].name, "param") );
    assert( url_str_is_empty( &params[0].value) );

    str = "=";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str = '%s'\n", i++, str );fflush(stdout);
    assert( r == 0 );
    assert( url_str_is_empty( &params[0].name) );
    assert( url_str_is_empty( &params[0].value) );

    str = "=value";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 1 );
    assert( url_str_is_empty( &params[0].name) );
    assert( url_str_is_equal( &params[0].value, "value") );

    str = "=value=";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 1 );
    assert( url_str_is_empty( &params[0].name) );
    assert( url_str_is_equal( &params[0].value, "value=") );



    str = "param=value";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 1 );
    assert( url_str_is_equal( &params[0].name, "param") );
    assert( url_str_is_equal( &params[0].value,"value") );

    str = "param=value&";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 1 );
    assert( url_str_is_equal( &params[0].name, "param") );
    assert( url_str_is_equal( &params[0].value,"value") );

    str = "param=value&param1";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 2 );
    assert( url_str_is_equal( &params[0].name, "param") );
    assert( url_str_is_equal( &params[0].value,"value") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_empty( &params[1].value) );

    str = "param=value&param1=";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 2 );
    assert( url_str_is_equal( &params[0].name, "param") );
    assert( url_str_is_equal( &params[0].value,"value") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_empty( &params[1].value) );

    str = "param=value&param1=value1";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == 2 );
    assert( url_str_is_equal( &params[0].name, "param") );
    assert( url_str_is_equal( &params[0].value,"value") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_equal( &params[1].value, "value1") );


    str = "param=value&param1=value1&param1=value1&param2=value2&param3=value3&param4=value4&param5=value5&param6=value6&param7=value1&param7=value7&param8=value8&param9=value9&param10=value10&param11=value11";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str );fflush(stdout);
    assert( r == url_parse_query_error_params_count );



    str = "&";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );fflush(stdout);
    assert( r == 0 );

    str = "&=";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );fflush(stdout);
    assert( r == 0 );

    str = "&=&";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );fflush(stdout);
    assert( r == 0 );

    str = "=&";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );fflush(stdout);
    assert( r == 0 );

    str = "param0=value0&param1=value1&param2=value2&param3=value3&param4=value4&param5=value5&param6=value6&param7=value7&param8=value8&param9=value9";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );fflush(stdout);
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );
    assert( r == 10 );
    assert( url_str_is_equal( &params[0].name, "param0") );
    assert( url_str_is_equal( &params[0].value,"value0") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_equal( &params[1].value, "value1") );
    assert( url_str_is_equal( &params[2].name, "param2") );
    assert( url_str_is_equal( &params[2].value,"value2") );
    assert( url_str_is_equal( &params[3].name, "param3") );
    assert( url_str_is_equal( &params[3].value, "value3") );
    assert( url_str_is_equal( &params[4].name, "param4") );
    assert( url_str_is_equal( &params[4].value,"value4") );
    assert( url_str_is_equal( &params[5].name, "param5") );
    assert( url_str_is_equal( &params[5].value, "value5") );
    assert( url_str_is_equal( &params[6].name, "param6") );
    assert( url_str_is_equal( &params[6].value,"value6") );
    assert( url_str_is_equal( &params[7].name, "param7") );
    assert( url_str_is_equal( &params[7].value, "value7") );
    assert( url_str_is_equal( &params[8].name, "param8") );
    assert( url_str_is_equal( &params[8].value,"value8") );
    assert( url_str_is_equal( &params[9].name, "param9") );
    assert( url_str_is_equal( &params[9].value,"value9") );



    str = "param0=value0&&&&&&&param1=value1&param2=value2&param3=value3&param4=value4&param5=value5&param6=value6&param7=value7&param8=value8&param9=value9";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );
    assert( r == 10 );
    assert( url_str_is_equal( &params[0].name, "param0") );
    assert( url_str_is_equal( &params[0].value,"value0") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_equal( &params[1].value, "value1") );
    assert( url_str_is_equal( &params[2].name, "param2") );
    assert( url_str_is_equal( &params[2].value,"value2") );
    assert( url_str_is_equal( &params[3].name, "param3") );
    assert( url_str_is_equal( &params[3].value, "value3") );
    assert( url_str_is_equal( &params[4].name, "param4") );
    assert( url_str_is_equal( &params[4].value,"value4") );
    assert( url_str_is_equal( &params[5].name, "param5") );
    assert( url_str_is_equal( &params[5].value, "value5") );
    assert( url_str_is_equal( &params[6].name, "param6") );
    assert( url_str_is_equal( &params[6].value,"value6") );
    assert( url_str_is_equal( &params[7].name, "param7") );
    assert( url_str_is_equal( &params[7].value, "value7") );
    assert( url_str_is_equal( &params[8].name, "param8") );
    assert( url_str_is_equal( &params[8].value,"value8") );
    assert( url_str_is_equal( &params[9].name, "param9") );
    assert( url_str_is_equal( &params[9].value,"value9") );



    str = "param0=value0&param1=value1&param2=value2&param3=value3&param4=value4&param5=value5&param6=value6&param7=value7&&&&&&&&&&&";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );
    assert( r == 8 );
    assert( url_str_is_equal( &params[0].name, "param0") );
    assert( url_str_is_equal( &params[0].value,"value0") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_equal( &params[1].value, "value1") );
    assert( url_str_is_equal( &params[2].name, "param2") );
    assert( url_str_is_equal( &params[2].value,"value2") );
    assert( url_str_is_equal( &params[3].name, "param3") );
    assert( url_str_is_equal( &params[3].value, "value3") );
    assert( url_str_is_equal( &params[4].name, "param4") );
    assert( url_str_is_equal( &params[4].value,"value4") );
    assert( url_str_is_equal( &params[5].name, "param5") );
    assert( url_str_is_equal( &params[5].value, "value5") );
    assert( url_str_is_equal( &params[6].name, "param6") );
    assert( url_str_is_equal( &params[6].value,"value6") );
    assert( url_str_is_equal( &params[7].name, "param7") );
    assert( url_str_is_equal( &params[7].value, "value7") );

    str = "param0=value0&&=&param1=value1&param2=value2&param3=value3&param4=value4&param5=value5&param6=value6&param7=value7";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );
    assert( r == 8 );
    assert( url_str_is_equal( &params[0].name, "param0") );
    assert( url_str_is_equal( &params[0].value,"value0") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_equal( &params[1].value, "value1") );
    assert( url_str_is_equal( &params[2].name, "param2") );
    assert( url_str_is_equal( &params[2].value,"value2") );
    assert( url_str_is_equal( &params[3].name, "param3") );
    assert( url_str_is_equal( &params[3].value, "value3") );
    assert( url_str_is_equal( &params[4].name, "param4") );
    assert( url_str_is_equal( &params[4].value,"value4") );
    assert( url_str_is_equal( &params[5].name, "param5") );
    assert( url_str_is_equal( &params[5].value, "value5") );
    assert( url_str_is_equal( &params[6].name, "param6") );
    assert( url_str_is_equal( &params[6].value,"value6") );
    assert( url_str_is_equal( &params[7].name, "param7") );
    assert( url_str_is_equal( &params[7].value, "value7") );

    str = "param0=value0&=&param1=value1&param2=value2&param3=value3&param4=value4&param5=value5&param6=value6&param7=value7";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );
    assert( r == 8 );
    assert( url_str_is_equal( &params[0].name, "param0") );
    assert( url_str_is_equal( &params[0].value,"value0") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_equal( &params[1].value, "value1") );
    assert( url_str_is_equal( &params[2].name, "param2") );
    assert( url_str_is_equal( &params[2].value,"value2") );
    assert( url_str_is_equal( &params[3].name, "param3") );
    assert( url_str_is_equal( &params[3].value, "value3") );
    assert( url_str_is_equal( &params[4].name, "param4") );
    assert( url_str_is_equal( &params[4].value,"value4") );
    assert( url_str_is_equal( &params[5].name, "param5") );
    assert( url_str_is_equal( &params[5].value, "value5") );
    assert( url_str_is_equal( &params[6].name, "param6") );
    assert( url_str_is_equal( &params[6].value,"value6") );
    assert( url_str_is_equal( &params[7].name, "param7") );
    assert( url_str_is_equal( &params[7].value, "value7") );


    str = "param0=value0&&param1=value1&param2=value2&param3=value3&param4=value4&param5=value5&param6=value6&param7=value7";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );
    assert( r == 8 );
    assert( url_str_is_equal( &params[0].name, "param0") );
    assert( url_str_is_equal( &params[0].value,"value0") );
    assert( url_str_is_equal( &params[1].name, "param1") );
    assert( url_str_is_equal( &params[1].value, "value1") );
    assert( url_str_is_equal( &params[2].name, "param2") );
    assert( url_str_is_equal( &params[2].value,"value2") );
    assert( url_str_is_equal( &params[3].name, "param3") );
    assert( url_str_is_equal( &params[3].value, "value3") );
    assert( url_str_is_equal( &params[4].name, "param4") );
    assert( url_str_is_equal( &params[4].value,"value4") );
    assert( url_str_is_equal( &params[5].name, "param5") );
    assert( url_str_is_equal( &params[5].value, "value5") );
    assert( url_str_is_equal( &params[6].name, "param6") );
    assert( url_str_is_equal( &params[6].value,"value6") );
    assert( url_str_is_equal( &params[7].name, "param7") );
    assert( url_str_is_equal( &params[7].value, "value7") );


    str = "param0=value0&HELLO&param1=value1&param2=value2&param3=value3&param4=value4&param5=value5&param6=value6&param7=value7";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, strlen(str)  );
    printf( "[%02d] : Test str '%s'\n", i++, str ); fflush( stdout );
    assert( r == 9 );
    assert( url_str_is_equal( &params[0].name, "param0") );
    assert( url_str_is_equal( &params[0].value,"value0") );
    assert( url_str_is_equal( &params[1].name, "HELLO") );
    assert( url_str_is_empty( &params[1].value) );
    assert( url_str_is_equal( &params[2].name, "param1") );
    assert( url_str_is_equal( &params[2].value, "value1") );
    assert( url_str_is_equal( &params[3].name, "param2") );
    assert( url_str_is_equal( &params[3].value,"value2") );
    assert( url_str_is_equal( &params[4].name, "param3") );
    assert( url_str_is_equal( &params[4].value, "value3") );
    assert( url_str_is_equal( &params[5].name, "param4") );
    assert( url_str_is_equal( &params[5].value,"value4") );
    assert( url_str_is_equal( &params[6].name, "param5") );
    assert( url_str_is_equal( &params[6].value, "value5") );
    assert( url_str_is_equal( &params[7].name, "param6") );
    assert( url_str_is_equal( &params[7].value,"value6") );
    assert( url_str_is_equal( &params[8].name, "param7") );
    assert( url_str_is_equal( &params[8].value, "value7") );



    str = "param0=value0&&&param2=value2";
    r = url_parse_query( params, URL_QUERY_MAX_PARAMS, sep, str, 15 );
    printf( "[%02d] : Test str '%s' and size < strlen with multiple separator\n", i++, str ); fflush( stdout );
    assert( r == 1 );
    assert( url_str_is_equal( &params[0].name, "param0") );
    assert( url_str_is_equal( &params[0].value, "value0") );

    printf( "\n\n" );
}
int main()
{
    test_url_parse();
    test_url_parse_query();
    return 0;
}
