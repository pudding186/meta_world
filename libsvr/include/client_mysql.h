#pragma once

#include "mysql/mysql.h"
#include "mysql/errmsg.h"



#ifdef  __cplusplus
extern "C" {
#endif

    typedef struct st_client_mysql*         HCLIENTMYSQL;

    typedef struct st_client_mysql_result
    {
        MYSQL*          cur_mysql;
        MYSQL_RES*      record_set;
        my_ulonglong    affect_row;
        unsigned int    error_code;
    }CLIENTMYSQLRESULT, *HCLIENTMYSQLRESULT;

    typedef struct st_client_mysql_row
    {
        MYSQL_ROW       row_values;
        unsigned long*  row_lengths;
    }CLIENTMYSQLROW;

    typedef struct st_client_mysql_value
    {
        char*           value;
        unsigned long   size;
    }CLIENTMYSQLVALUE;

    extern HCLIENTMYSQL (create_client_mysql)(const char *host, unsigned int port, const char *user,
        const char *passwd, const char* db, const char* charact_set,
        char* err_info, unsigned int err_info_size);

    extern HCLIENTMYSQL (duplicate_client_mysql)(HCLIENTMYSQL client_mysql,
        char* err_info, unsigned int err_info_size);

    extern void (destroy_client_mysql)(HCLIENTMYSQL connection);

    extern CLIENTMYSQLRESULT (client_mysql_query)(HCLIENTMYSQL connection, const char* sql, unsigned long length);

    extern bool (client_mysql_result_success)(HCLIENTMYSQLRESULT result);

    extern HCLIENTMYSQLRESULT (client_mysql_next_result)(HCLIENTMYSQLRESULT last_result);

    extern void (client_mysql_free_result)(HCLIENTMYSQLRESULT result);

    extern bool (client_mysql_result_record)(HCLIENTMYSQLRESULT result);

    extern unsigned long long (client_mysql_result_affected)(HCLIENTMYSQLRESULT result);

    extern CLIENTMYSQLROW (client_mysql_fetch_row)(HCLIENTMYSQLRESULT result);

    extern unsigned long long (client_mysql_rows_num)(HCLIENTMYSQLRESULT result);

    extern unsigned int (client_mysql_fields_num)(HCLIENTMYSQLRESULT result);

    extern CLIENTMYSQLROW (client_mysql_row)(HCLIENTMYSQLRESULT result, unsigned long long row_index);

    extern CLIENTMYSQLVALUE (client_mysql_value)(CLIENTMYSQLROW row, unsigned long field_index);

    extern CLIENTMYSQLVALUE (client_mysql_row_field_value)(HCLIENTMYSQLRESULT result, unsigned long long row_index, unsigned long field_index);

    extern unsigned char (client_mysql_value_uint8)(CLIENTMYSQLVALUE data);

    extern char (client_mysql_value_int8)(CLIENTMYSQLVALUE data);

    extern unsigned short (client_mysql_value_uint16)(CLIENTMYSQLVALUE data);

    extern short (client_mysql_value_int16)(CLIENTMYSQLVALUE data);

    extern unsigned int (client_mysql_value_uint32)(CLIENTMYSQLVALUE data);

    extern int (client_mysql_value_int32)(CLIENTMYSQLVALUE data);

    extern unsigned long long (client_mysql_value_uint64)(CLIENTMYSQLVALUE data);

    extern long long (client_mysql_value_int64)(CLIENTMYSQLVALUE data);

    extern unsigned long (client_mysql_escape_string)(HCLIENTMYSQL connection, const char* src, unsigned long src_size, char* dst, unsigned long dst_size);

    extern const char* (client_mysql_err)(HCLIENTMYSQL connection);

#ifdef  __cplusplus
}
#endif