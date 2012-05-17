#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <libpq-fe.h>

#include "libpowder_test.h"
#include "libmhaddressservice.c"

#define FAIL_FAST if ( result == 0 ) { printf( "Fail fast\n" ); PQfinish(conn); exit(1); }

PGconn *open_db( const char *conninfo ) {
    int result = 0;
    PGconn     *conn= PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) { printf( "Connection to database failed: %s", PQerrorMessage(conn)); FAIL_FAST }

    return conn;
}

int close_db(PGconn *conn) {
    if ( conn != NULL ) {
        PQfinish(conn);
    }
}

void clean_records( PGconn *conn ) {
    PQexec( conn, "DELETE FROM loading_tbl WHERE guid IN ( 'ABC', 'ABC1', 'ABC2' )" );
    PQexec( conn, "DELETE FROM address_tbl WHERE party_id = 0" );
}

void fill_set_address_buffer( char *buffer, char *guid, char *id, char *line1 ) {
    strcpy( buffer, "" );
    strcat( buffer, "<SetAddressMessage>" );
      strcat( buffer, "<guid>" );
        strcat( buffer, guid );
      strcat( buffer, "</guid>" );
      strcat( buffer, "<loggedInUserId>0</loggedInUserId>" );
      strcat( buffer, "<id>" );
          strcat( buffer, id );
      strcat( buffer, "</id>" );
      strcat( buffer, "<partyId>0</partyId>" );
      strcat( buffer, "<addressTypeId>1</addressTypeId>" );
      strcat( buffer, "<line1>" );
          strcat( buffer, line1 );
      strcat( buffer, "</line1>" );
      strcat( buffer, "<line2>line2</line2>" );
      strcat( buffer, "<line3>line3</line3>" );
      strcat( buffer, "<city>Hamilton</city>" );
      strcat( buffer, "<country>NZ</country>" );
      strcat( buffer, "<postcode>3280</postcode>" );
    strcat( buffer, "</SetAddressMessage>" );
}

void test_set( PGconn *conn ) {
    char buffer[2048] = "";
    int result = 0;
    char *id=NULL;
    apr_pool_t *pool = NULL;

    apr_initialize();
    apr_pool_create( &pool, NULL );

    clean_records( conn );

    //Run tests across alert data
    {
        char ***array = NULL;
        int row_count;
        result = get_by_party( pool, conn, "0", &array, &row_count );
        powder_assert_equal_int( 0, row_count, "test_set. 1" );
    }

    {
        char ***array = NULL;
        int row_count;
        struct _powder_xml2_node *node = NULL;
        char guid[10] = "ABC1";

        fill_set_address_buffer( buffer, guid, "", "line1.1" );

        result = powder_xml2_parse( pool, buffer, &node );FAIL_FAST;
        result = process_setaddress( pool, conn, node );FAIL_FAST;
        {
            result = get_by_party( pool, conn, "0", &array, &row_count );
            powder_assert_equal_int( 1, row_count, "test_set. 2" );
            powder_assert_equal_string( "line1.1", array[0][1], "test_update. 4" );
            id = array[0][0];
        }
        {
            char ***array = NULL;
            int row_count;

            result = _loading_get_by_guid( pool, conn, guid, &array, &row_count );FAIL_FAST
            powder_assert_equal_int( 1, row_count, "test_update. 5" );
            powder_assert_equal_string( guid, array[0][0], "test_update. 6" );
        }
    }


    {
        char ***array = NULL;
        int row_count;
        struct _powder_xml2_node *node = NULL;
        char guid[10] = "ABC2";

        fill_set_address_buffer( buffer, guid, id, "line1.2" );
        result = powder_xml2_parse( pool, buffer, &node );
        result = process_setaddress( pool, conn, node );
        {
            result = get_by_party( pool, conn, "0", &array, &row_count );
            powder_assert_equal_int( 1, row_count, "test_set. 7" );
            powder_assert_equal_string( "line1.2", array[0][1], "test_update. 8" );
            id = array[0][0];
        }
        {
            char ***array = NULL;
            int row_count;

            result = _loading_get_by_guid( pool, conn, guid, &array, &row_count );FAIL_FAST
            powder_assert_equal_int( 1, row_count, "test_update. 9" );
            powder_assert_equal_string( guid, array[0][0], "test_update. 10" );
        }
    }


    printf( "Passed: test_update\n" );
}

void test_delete( PGconn *conn ) {
    char buffer[2048] = "";
    int result = 0;
    char *id=NULL;
    apr_pool_t *pool = NULL;

    apr_initialize();
    apr_pool_create( &pool, NULL );

    clean_records( conn );

    //Run tests across alert data
    {
        char ***array = NULL;
        int row_count;
        result = get_by_party( pool, conn, "0", &array, &row_count );
        powder_assert_equal_int( 0, row_count, "test_delete. 1" );
    }

    {
        char ***array = NULL;
        int row_count;
        struct _powder_xml2_node *node = NULL;
        char guid[10] = "ABC1";

        fill_set_address_buffer( buffer, guid, "", "line1.1" );

        result = powder_xml2_parse( pool, buffer, &node );FAIL_FAST;
        result = process_setaddress( pool, conn, node );FAIL_FAST;
        {
            result = get_by_party( pool, conn, "0", &array, &row_count );
            powder_assert_equal_int( 1, row_count, "test_delete. 2" );
            powder_assert_equal_string( "line1.1", array[0][1], "test_delete. 3" );
            id = array[0][0];
        }
        {
            char ***array = NULL;
            int row_count;

            result = _loading_get_by_guid( pool, conn, guid, &array, &row_count );FAIL_FAST
            powder_assert_equal_int( 1, row_count, "test_delete. 4" );
            powder_assert_equal_string( guid, array[0][0], "test_delete. 5" );
        }
    }


    {
        char ***array = NULL;
        int row_count;
        struct _powder_xml2_node *node = NULL;
        char guid[10] = "ABC2";
        strcpy( buffer, "" );
        strcat( buffer, "<DeleteAddressMessage>" );
          strcat( buffer, "<guid>ABC2</guid>" );
          strcat( buffer, "<loggedInUserId>0</loggedInUserId>" );
          strcat( buffer, "<id>" );
              strcat( buffer, id );
          strcat( buffer, "</id>" );
        strcat( buffer, "</DeleteAddressMessage>" );

        result = powder_xml2_parse( pool, buffer, &node );FAIL_FAST;
        result = process_deleteaddress( pool, conn, node );
        {
            result = get_by_party( pool, conn, "0", &array, &row_count );
            powder_assert_equal_int( 0, row_count, "test_delete. 6" );
        }
        {
            char ***array = NULL;
            int row_count;

            result = _loading_get_by_guid( pool, conn, guid, &array, &row_count );FAIL_FAST
            powder_assert_equal_int( 1, row_count, "test_delete. 7" );
            powder_assert_equal_string( guid, array[0][0], "test_delete. 8" );
        }
    }


    printf( "Passed: test_delete\n" );
}


int main(int argc, char **argv) {

  PGconn *conn = NULL;
  conn = open_db( "dbname = powder" );

  init_dao( conn );

  test_set( conn );
  test_delete( conn );

  close_db( conn );
  
  return 0;
}
