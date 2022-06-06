#include "../include/client_mysql.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __GNUC__
#include <unistd.h>
#endif
#include "../include/memory_pool.h"


////////////////////////////////////////////////////////////////////////

typedef struct st_client_mysql
{
    MYSQL*              real_mysql;
    
    char*               host;
    char*               user;
    char*               passwd;
    char*               db;
    char*               charact_set;
    unsigned int        port;
    unsigned int        err_msg_size;
    char*               err_msg;

}client_mysql;

void _close_client_mysql(HCLIENTMYSQL client_mysql_ptr)
{
    if (client_mysql_ptr->real_mysql)
    {
        mysql_close(client_mysql_ptr->real_mysql);
        client_mysql_ptr->real_mysql = 0;
    }
}

unsigned int _open_client_mysql(HCLIENTMYSQL client_mysql_ptr)
{
    int error_code = 0;

    if (client_mysql_ptr->real_mysql)
    {
        return 0;
    }

    client_mysql_ptr->real_mysql = mysql_init(0);
    if (!client_mysql_ptr->real_mysql)
    {
        if (client_mysql_ptr->err_msg_size)
        {
            const char* err = "call mysql_init(0) fail!";
            size_t err_len = strnlen(err, client_mysql_ptr->err_msg_size - 1);
            memcpy(client_mysql_ptr->err_msg, err, err_len + 1);
            client_mysql_ptr->err_msg[client_mysql_ptr->err_msg_size - 1] = '\0';
        }

        return CR_UNKNOWN_ERROR;
    }

    //mysql_options(client_mysql_ptr->real_mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4");
    mysql_options(client_mysql_ptr->real_mysql, MYSQL_SET_CHARSET_NAME, client_mysql_ptr->charact_set);

    if (mysql_real_connect(
        client_mysql_ptr->real_mysql,
        client_mysql_ptr->host,
        client_mysql_ptr->user,
        client_mysql_ptr->passwd,
        client_mysql_ptr->db,
        client_mysql_ptr->port,
        0,
        CLIENT_MULTI_STATEMENTS
    ))
    {
        if (mysql_autocommit(client_mysql_ptr->real_mysql, 1))
        {
            goto ERROR_DEAL;
        }

        //if (mysql_set_character_set(client_mysql_ptr->real_mysql, "utf8mb4"))
        if (mysql_set_character_set(client_mysql_ptr->real_mysql, client_mysql_ptr->charact_set))
        {
            goto ERROR_DEAL;
        }

        return 0;
    }

ERROR_DEAL:

    if (client_mysql_ptr->err_msg_size)
    {
        const char* err = mysql_error(client_mysql_ptr->real_mysql);

        size_t err_len = strnlen(err, client_mysql_ptr->err_msg_size - 1);
        memcpy(client_mysql_ptr->err_msg, err, err_len + 1);
        client_mysql_ptr->err_msg[client_mysql_ptr->err_msg_size - 1] = '\0';
    }

    error_code = mysql_errno(client_mysql_ptr->real_mysql);
    _close_client_mysql(client_mysql_ptr);

    return error_code;
}

unsigned int _handle_client_mysql_error(HCLIENTMYSQL client_mysql_ptr, unsigned int error_code, unsigned int try_count)
{
    switch (error_code)
    {
    case CR_SERVER_GONE_ERROR:
    case CR_SERVER_LOST:
    case CR_SERVER_LOST_EXTENDED:
    case CR_CONN_HOST_ERROR:
    {
        _close_client_mysql(client_mysql_ptr);

        unsigned int current_error_code = _open_client_mysql(client_mysql_ptr);

        if (!current_error_code)
        {
            return current_error_code;
        }

        if ((--try_count) > 0)
        {
#ifdef _MSC_VER
            Sleep(3000);
#elif __GNUC__
            sleep(3);
#else
#error "unknown compiler"
#endif
            return _handle_client_mysql_error(client_mysql_ptr, current_error_code, try_count);
        }
        else
        {
            return current_error_code;
        }
    }
    break;
    default:
    {
        return error_code;
    }
    }
}

bool client_mysql_result_success(HCLIENTMYSQLRESULT result)
{
    if (result)
    {
        return result->error_code == 0;
    }

    return false;
}

HCLIENTMYSQL create_client_mysql(const char *host, unsigned int port,
    const char *user, const char *passwd, const char* db, const char* charact_set,
    char* err_info, unsigned int err_info_size)
{
    if (!charact_set)
    {
        charact_set = "utf8mb4";
    }

    size_t host_length = strlen(host);
    size_t user_length = strlen(user);
    size_t passwd_length = strlen(passwd);
    size_t db_length = strlen(db);
    size_t charact_set_length = strlen(charact_set);
    size_t port_length = sizeof(port);

    char* ptr = (char*)malloc(
        sizeof(client_mysql)
        + host_length + 1
        + user_length + 1
        + passwd_length + 1
        + db_length + 1
        + charact_set_length + 1
        + port_length);

    client_mysql* client_mysql_ptr = (client_mysql*)ptr;
    
    client_mysql_ptr->real_mysql = 0;
    ptr += sizeof(client_mysql);
    
    client_mysql_ptr->host = ptr;
    memcpy(ptr, host, host_length);
    ptr += host_length;
    *ptr = '\0';
    ++ptr;

    client_mysql_ptr->user = ptr;
    memcpy(ptr, user, user_length);
    ptr += user_length;
    *ptr = '\0';
    ++ptr;

    client_mysql_ptr->passwd = ptr;
    memcpy(ptr, passwd, passwd_length);
    ptr += passwd_length;
    *ptr = '\0';
    ++ptr;

    client_mysql_ptr->db = ptr;
    memcpy(ptr, db, db_length);
    ptr += db_length;
    *ptr = '\0';
    ++ptr;

    client_mysql_ptr->charact_set = ptr;
    memcpy(ptr, charact_set, charact_set_length);
    ptr += charact_set_length;
    *ptr = '\0';
    ++ptr;

    client_mysql_ptr->port = port;

    client_mysql_ptr->err_msg_size = err_info_size;
    client_mysql_ptr->err_msg = err_info;

    unsigned int error_code = _open_client_mysql(client_mysql_ptr);

    if (error_code)
    {
        free(client_mysql_ptr);
        client_mysql_ptr = 0;
    }
    else
    {
        client_mysql_ptr->err_msg_size = 0;
    }

    return client_mysql_ptr;
}

HCLIENTMYSQL duplicate_client_mysql(HCLIENTMYSQL client_mysql,
    char* err_info, unsigned int err_info_size)
{
    return create_client_mysql(
        client_mysql->host, 
        client_mysql->port, 
        client_mysql->user,
        client_mysql->passwd,
        client_mysql->db,
        client_mysql->charact_set,
        err_info, err_info_size);
}

void destroy_client_mysql(HCLIENTMYSQL client_mysql_ptr)
{
    _close_client_mysql(client_mysql_ptr);

    free(client_mysql_ptr);
}

CLIENTMYSQLRESULT client_mysql_query(HCLIENTMYSQL client_mysql_ptr, const char* sql, unsigned long length)
{
    CLIENTMYSQLRESULT result;

    result.cur_mysql = 0;
    result.record_set = 0;
    result.affect_row = 0;
    result.error_code = 0;

    if (client_mysql_ptr->real_mysql)
    {
        if (mysql_real_query(client_mysql_ptr->real_mysql, sql, length))
        {
            unsigned int error_code = _handle_client_mysql_error(client_mysql_ptr, mysql_errno(client_mysql_ptr->real_mysql), 5);
            if (!error_code)
            {
                return client_mysql_query(client_mysql_ptr, sql, length);
            }

            result.error_code = error_code;
            return result;
        }
    }
    else
    {
        result.error_code = CR_UNKNOWN_ERROR;
        return result;
    }

    result.cur_mysql = client_mysql_ptr->real_mysql;

    result.record_set = mysql_store_result(result.cur_mysql);

    if (!result.record_set)
    {
        if (mysql_field_count(result.cur_mysql) == 0)
        {
            result.affect_row = mysql_affected_rows(result.cur_mysql);
        }
        else
        {
            result.cur_mysql = 0;
        }
    }

    return result;
}

HCLIENTMYSQLRESULT client_mysql_next_result(HCLIENTMYSQLRESULT last_result)
{
    MYSQL* real_mysql = 0;

    if (!last_result)
    {
        return 0;
    }

    real_mysql = last_result->cur_mysql;

    if (last_result->record_set)
    {
        mysql_free_result(last_result->record_set);
        last_result->record_set = 0;
    }

    last_result->affect_row = 0;

    switch (mysql_next_result(real_mysql))
    {
    case 0:
        break;
    case -1:
    {
        last_result->error_code = 0;
        return 0;
    }
    break;
    default:
    {
        last_result->error_code = mysql_errno(real_mysql);
        return last_result;
    }
    }

    last_result->record_set = mysql_store_result(real_mysql);

    if (last_result->record_set)
    {
        last_result->affect_row = 0;
    }
    else
    {
        if (mysql_field_count(real_mysql) == 0)
        {
            last_result->affect_row = mysql_affected_rows(real_mysql);
        }
        else
        {
            last_result->error_code = CR_NO_RESULT_SET;
            return last_result;
        }
    }

    last_result->error_code = 0;
    return last_result;
}

void client_mysql_free_result(HCLIENTMYSQLRESULT result)
{
    if (result)
    {
        if (result->record_set)
        {
            mysql_free_result(result->record_set);
            result->record_set = 0;
        }

        result->affect_row = 0;
    }
}

bool client_mysql_result_record(HCLIENTMYSQLRESULT result)
{
    if (result->record_set)
    {
        return true;
    }

    return false;
}

unsigned long long client_mysql_result_affected(HCLIENTMYSQLRESULT result)
{
    return result->affect_row;
}



CLIENTMYSQLROW client_mysql_fetch_row(HCLIENTMYSQLRESULT result)
{
    CLIENTMYSQLROW row;

    row.row_values = mysql_fetch_row(result->record_set);
    if (row.row_values)
    {
        row.row_lengths = mysql_fetch_lengths(result->record_set);
    }
    else
    {
        row.row_lengths = 0;
    }

    return row;
}

CLIENTMYSQLROW client_mysql_row(HCLIENTMYSQLRESULT result, unsigned long long row_index)
{
    mysql_data_seek(result->record_set, row_index);

    return client_mysql_fetch_row(result);
}

CLIENTMYSQLVALUE client_mysql_value(CLIENTMYSQLROW row, unsigned long field_index)
{
    CLIENTMYSQLVALUE data;

    data.value = row.row_values[field_index];
    data.size = row.row_lengths[field_index];

    return data;
}

CLIENTMYSQLVALUE client_mysql_row_field_value(HCLIENTMYSQLRESULT result, unsigned long long row_index, unsigned long field_index)
{
    CLIENTMYSQLVALUE data;

    MYSQL_ROW row_values = 0;
    unsigned long* row_lengths = 0;

    mysql_data_seek(result->record_set, row_index);

    row_values = mysql_fetch_row(result->record_set);

    if (row_values)
    {
        row_lengths = mysql_fetch_lengths(result->record_set);

        data.value = row_values[field_index];
        data.size = row_lengths[field_index];
    }
    else
    {
        data.value = 0;
        data.size = 0;
    }

    return data;
}

unsigned long client_mysql_escape_string(HCLIENTMYSQL connection, const char* src, unsigned long src_size, char* dst, unsigned long dst_size)
{
    if (dst_size < src_size * 2 + 1)
    {
        return 0;
    }

    if (connection->real_mysql)
    {
        return mysql_real_escape_string(connection->real_mysql, dst, src, src_size);
    }
    
    return 0;
}

unsigned char client_mysql_value_uint8(CLIENTMYSQLVALUE data)
{
    return (unsigned char)atoi(data.value);
}

char client_mysql_value_int8(CLIENTMYSQLVALUE data)
{
    return (char)atoi(data.value);
}

unsigned short client_mysql_value_uint16(CLIENTMYSQLVALUE data)
{
    return (unsigned short)atoi(data.value);
}

short client_mysql_value_int16(CLIENTMYSQLVALUE data)
{
    return (short)atoi(data.value);
}

unsigned int client_mysql_value_uint32(CLIENTMYSQLVALUE data)
{
    return (unsigned int)strtoull(data.value, 0, 10);
}

int client_mysql_value_int32(CLIENTMYSQLVALUE data)
{
    return atoi(data.value);
}

unsigned long long client_mysql_value_uint64(CLIENTMYSQLVALUE data)
{
    return strtoull(data.value, 0, 10);
}

long long client_mysql_value_int64(CLIENTMYSQLVALUE data)
{
    return strtoll(data.value, 0, 10);
}

const char* client_mysql_err(HCLIENTMYSQL connection)
{
    if (connection->real_mysql)
    {
        return mysql_error(connection->real_mysql);
    }

    return "mysql is nullptr";
}

unsigned long long client_mysql_rows_num(HCLIENTMYSQLRESULT result)
{
    if (result->record_set)
    {
        return mysql_num_rows(result->record_set);
    }
    
    return 0;
}

unsigned int client_mysql_fields_num(HCLIENTMYSQLRESULT result)
{
    if (result->record_set)
    {
        return mysql_num_fields(result->record_set);
    }
    
    return 0;
}
