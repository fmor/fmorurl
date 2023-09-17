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


#include "urlparser.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>

int url_parse( struct url* u, const char* str, int len )
{
    enum state
    {
        state_scheme_or_path,
        state_user,
        state_password,
        state_hostname,
        state_ipv6,
        state_port,
        state_path,
        state_query,
        state_fragment,
    };

    int err;
    const char* p;
    char ch;
    enum state st;
    const char* end;

    if( (u == NULL) || (str == NULL) || (len < 0) )
        return url_parse_error_params;

    memset( u, 0, sizeof(struct url) );

    if( len == 0 )
        return url_parse_ok;

    err = url_parse_ok;
    p = str;
    end = str + len;
    st = state_scheme_or_path;

    while( (p < end) && isspace(*p) )
        ++p;

    while( (end > p) && (isspace( *(end-1) )) )
        --end;

    for(;;)
    {
        ch = ( p == end ) ? 0 : *p;

        switch( st )
        {
            case state_scheme_or_path:
                switch( ch )
                {
                    case 0:
                        goto LBL_TERMINATE;

                    case '?':
                        st = state_query;
                        ++p;
                        break;

                    case '#':
                        st = state_fragment;
                        ++p;
                        break;

                    case ':':
                        u->scheme.str = u->path.str;
                        u->scheme.size = u->path.size;

                        u->path.str = NULL;
                        u->path.size = 0;

                        ++p;
                        if( p[0] == '/' )
                        {
                            if( p[1] == '/' )
                            {
                                p += 2;
                                if( u->scheme.size == 4 && (strncmp(u->scheme.str, "file", 4) == 0) )
                                    st = state_path;
                                else
                                {
                                    if( p[0] == '[' )
                                    {
                                        st = state_ipv6;
                                        ++p;
                                    }
                                    else
                                        st = state_user;
                                }
                            }
                            else
                            {
                                ++p;
                                st = state_user;
                            }
                        }
                        else
                            st = state_path;
                        break;

                    default:
                        if( u->path.str == NULL )
                            u->path.str = p;
                        ++u->path.size;
                        ++p;
                }
                break;


            case state_user:
                switch( ch )
                {
                    case 0 :
                        u->hostname.str = u->user.str;
                        u->hostname.size = u->user.size;
                        u->user.str = 0;
                        u->user.size = 0;
                        goto LBL_TERMINATE;

                    case ':':
                        st = state_password;
                        ++p;
                        break;

                    case '/':
                        u->hostname.str = u->user.str;
                        u->hostname.size = u->user.size;
                        u->user.str = 0;
                        u->user.size = 0;
                        st = state_path;
                        break;

                    case '@':
                        ++p;
                        if( *p == '[' )
                        {
                            st = state_ipv6;
                            ++p;
                        }
                        else
                            st = state_hostname;
                        break;

                    default:
                        if( u->user.str == NULL )
                            u->user.str = p;
                        ++u->user.size;
                        ++p;
                }
                break;

            case state_password:
                switch( ch )
                {
                    case 0:
                        u->hostname.str = u->user.str;
                        u->hostname.size = u->user.size;
                        u->user.str = NULL;
                        u->user.size = 0;

                        p = u->password.str;
                        u->password.str = NULL;
                        u->password.size = 0;
                        st = state_port;
                        break;

                    case '/':
                        u->hostname.str = u->user.str;
                        u->hostname.size = u->user.size;
                        u->user.str = NULL;
                        u->user.size = 0;

                        u->port.str = u->password.str;
                        u->port.size = u->password.size;
                        u->password.str = NULL;
                        u->password.size = 0;

                        st = state_path;
                        break;

                    case '@':
                        ++p;
                        if( *p == '[' )
                        {
                            st = state_ipv6;
                            ++p;
                        }
                        else
                            st = state_hostname;
                        break;

                    default:
                        if( u->password.str == NULL )
                            u->password.str = p;
                        ++u->password.size;
                        ++p;
                }
                break;

            case state_hostname:
                switch( ch )
                {
                    case 0:
                        goto LBL_TERMINATE;

                    case '@':
                        if( u->password.str )
                            u->password.size = p - u->password.str;
                        else if( u->user.str )
                            u->user.size = p - u->user.str;

                        u->hostname.str = NULL;
                        u->hostname.size = 0;
                        ++p;
                        if( p[0] == '[' )
                        {
                            st = state_ipv6;
                            ++p;
                        }
                        break;

                    case '/':
                        st = state_path;
                        break;

                    case '?':
                        st = state_query;
                        ++p;
                        break;

                    case '#':
                        st = state_fragment;
                        ++p;
                        break;

                    case ':':
                        if( u->hostname.str )
                            st = state_port;
                        ++p;

                        break;

                    default:
                        if( u->hostname.str == NULL )
                            u->hostname.str = p;
                        ++u->hostname.size;
                        ++p;
                }
                break;

            case state_ipv6:
                switch( ch )
                {
                    case 0:
                        err = url_parse_error_bad_hostname;
                        goto LBL_TERMINATE;

                    case ']':
                        st = state_hostname;
                        ++p;
                        break;

                    default:
                        if( u->hostname.str == NULL )
                            u->hostname.str = p;
                        ++u->hostname.size;
                        ++p;
                }

                break;

            case state_port:
                switch( ch )
                {
                    case 0:
                        goto LBL_TERMINATE;

                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        if( u->port.str == NULL )
                            u->port.str = p;
                        ++u->port.size;
                        ++p;
                        break;

                    case '/':
                        st = state_path;
                        break;

                    case '?':
                        st = state_query;
                        ++p;
                        break;

                    case '#':
                        st = state_fragment;
                        ++p;
                        break;

                    default:
                        err = url_parse_error_bad_port;
                        goto LBL_TERMINATE;
                }

                break;

            case state_path:

                switch( ch )
                {
                    case 0:
                        goto LBL_TERMINATE;

                    case '?':
                        st = state_query;
                        ++p;
                        break;

                    case '#':
                        st = state_fragment;
                        ++p;
                        break;


                    default:
                        if( u->path.str == NULL )
                            u->path.str = p;
                        ++u->path.size;
                        ++p;
                }
                break;

            case state_query:
                switch( ch )
                {
                    case 0:
                        goto LBL_TERMINATE;

                    case '#':
                        st = state_fragment;
                        ++p;
                        break;

                    default:
                        if( u->query.str == NULL )
                            u->query.str = p;
                        ++u->query.size;
                        ++p;
                }
                break;

            case state_fragment:
                if( ch  )
                {
                    u->fragment.str = p;
                    u->fragment.size = end - p;
                }
                goto LBL_TERMINATE;
        }

    }

LBL_TERMINATE:
//    url_dump( u , str);
    return err;
}

int url_parse_query( struct url_query_param* params, int params_max, char separator, const char* str, int str_size )
{
    int i;
    const char* end;
    const char* p;
    int is_name;
    struct url_query_param* param;

    if( params == NULL )
        return url_parse_query_error_bad_parameter;
    if( separator == 0 )
        return  url_parse_query_error_bad_parameter;
    if( params_max <= 0 )
        return 0;
    if( str == NULL )
        return 0;

    i = 0;
    p = str;
    end = str + str_size;
    is_name = 1;
    param = &params[0];

    memset( params, 0, sizeof(struct url_query_param) * params_max );

    for(;;)
    {

        if( *p == 0 )
            break;

        if( p == end )
            break;

        if( *p == separator )
        {
            is_name = 1;

            while( p[1] == separator )
            {
                ++p;
                if( p == end )
                    goto LBL_TERMINATE;
            }

            if( param->name.str || param->value.str)
            {
                ++i;
                if( i == params_max )
                    return url_parse_query_error_params_count;
                param = &params[i];
            }
        }

        else if( *p == '=' && is_name )
            is_name = 0;

        else if( is_name )
        {
            if( param->name.str == NULL )
                param->name.str = p;
            ++param->name.size;
        }
        else
        {
            if( param->value.str == NULL )
                param->value.str = p;
            ++param->value.size;
        }
        ++p;
    }
LBL_TERMINATE:
    if( param->name.str || param->value.str )
        ++i;
    return i;
}

void url_dump( struct url* u, const char* url)
{
    printf( "url : '%s'\n", url );

    printf( "  scheme   : %.*s\n", u->scheme.size, u->scheme.str );
    printf( "  user     : %.*s\n", u->user.size, u->user.str );
    printf( "  password : %.*s\n", u->password.size, u->password.str );
    printf( "  hostname : %.*s\n", u->hostname.size, u->hostname.str );
    printf( "  port     : %.*s\n", u->port.size, u->port.str );
    printf( "  path     : %.*s\n", u->path.size, u->path.str );
    printf( "  query    : %.*s\n", u->query.size, u->query.str );
    printf( "  fragment : %.*s\n", u->fragment.size, u->fragment.str );

    printf( "\n" );

   fflush( stdout );
}

void url_dump_query_params( struct url_query_param* params, int params_count )
{
   struct url_query_param* p;

   for( int i = 0; i < params_count; ++i )
   {
        p = &params[i];
        printf( "[%02d] : '%.*s'='%.*s'\n", i, p->name.size, p->name.str, p->value.size, p->value.str  );
   }
   fflush( stdout );
}


