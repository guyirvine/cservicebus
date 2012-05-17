#include "libpowder_pg.h"

#define ADDRESS_BY_PARTY_LINE1_COL 1

#define ADDRESS_PARTY_ID_COL 1

int create( PGconn *conn, char *party_id, char *addresstype_id, char *line1, char *line2, char *line3, char *city, char *country_iso3, char *postcode ) {
    PGresult *res = NULL;
    char *param_values[9];

    param_values[0] = party_id;
    param_values[1] = addresstype_id;
    param_values[2] = line1;
    param_values[3] = line2;
    param_values[4] = line3;
    param_values[5] = city;
    param_values[6] = country_iso3;
    param_values[7] = postcode;

    POWDER_PG_EXEC_PREPARED( "addressservice_create", 8 );

    return -1;
}

int update( PGconn *conn, char *id, char *party_id, char *addresstype_id, char *line1, char *line2, char *line3, char *city, char *country_iso3, char *postcode ) {
    PGresult *res = NULL;
    char *param_values[9];

    param_values[0] = party_id;
    param_values[1] = addresstype_id;
    param_values[2] = line1;
    param_values[3] = line2;
    param_values[4] = line3;
    param_values[5] = city;
    param_values[6] = country_iso3;
    param_values[7] = postcode;
    param_values[8] = id;

    POWDER_PG_EXEC_PREPARED( "addressservice_update", 9 );

    return -1;
}

int get( apr_pool_t *pool, PGconn *conn, char *id, char ****array, int *row_count ) {
    PGresult *res = NULL;

    char *param_values[1];
    param_values[0] = id;
    POWDER_PG_EXEC_PREPARED_SELECT( "addressservice_get", 1 );
    POWDER_PG_RST_TO_ARRAY( res, array );
    *row_count = PQntuples( res );
    PQclear(res);

    return -1;
}

int get_by_party( apr_pool_t *pool, PGconn *conn, char *party_id, char ****array, int *row_count ) {
    PGresult *res = NULL;

    char *param_values[1];
    param_values[0] = party_id;
    POWDER_PG_EXEC_PREPARED_SELECT( "addressservice_get_by_party", 1 );
    POWDER_PG_RST_TO_ARRAY( res, array );
    *row_count = PQntuples( res );
    PQclear(res);

    return -1;
}

int delete( PGconn *conn, char *id ) {
    PGresult *res = NULL;
    char *param_values[1];

    param_values[0] = id;

    POWDER_PG_EXEC_PREPARED( "addressservice_delete", 1 );

    return -1;
}

int init_dao( PGconn *conn ) {
    int result = -1;
    PGresult *res;

    POWDER_PG_PREPARE( conn, "addressservice_create", "INSERT INTO address_tbl ( id, party_id, addresstype_id, line1, line2, line3, city, country_iso3, postcode ) VALUES ( NEXTVAL( 'address_seq' ), $1, $2, $3, $4, $5, $6, $7, $8 )", 8 );
    POWDER_PG_PREPARE( conn, "addressservice_update", "UPDATE address_tbl SET party_id=$1, addresstype_id=$2, line1=$3, line2=$4, line3=$5, city=$6, country_iso3=$7, postcode=$8 WHERE id = $9", 9 );
    POWDER_PG_PREPARE( conn, "addressservice_get", "SELECT id, party_id, addresstype_id, line1, line2, line3, city, country_iso3, postcode FROM address_tbl WHERE id = $1", 1 );
    POWDER_PG_PREPARE( conn, "addressservice_get_by_party", "SELECT id, line1 FROM address_tbl WHERE party_id = $1", 1 );
    POWDER_PG_PREPARE( conn, "addressservice_delete", "DELETE FROM address_tbl WHERE id = $1", 1 );

    return result;
}
