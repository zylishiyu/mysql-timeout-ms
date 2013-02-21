#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

void mysql_timeout_ms_test() {
    const char* db_name = "test";
    const char* db_user = "user";
    const char* db_pass = "pass";
    const char* db_host = "hostname";
    const int   db_port = 3600;
    const unsigned int connect_timeout = 10;
    const unsigned int read_timeout    = 100;
    const unsigned int write_timeout   = 50;

    MYSQL* conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &connect_timeout);
    mysql_options(conn, MYSQL_OPT_READ_TIMEOUT, &read_timeout);
    mysql_options(conn, MYSQL_OPT_WRITE_TIMEOUT, &write_timeout);

    if (mysql_real_connect(conn, db_host, db_user, db_pass, db_name, db_port, NULL, 0) == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    const char* sql = "select sleep(2)";
    if (mysql_query(conn, sql)) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    mysql_close(conn);
}

int main() {
    mysql_timeout_ms_test();

    return 0;
}

