# Copyright (C) 2006 MySQL AB
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
INCLUDE("${PROJECT_SOURCE_DIR}/win/mysql_manifest.cmake")

SET(CMAKE_CXX_FLAGS_DEBUG 
    "${CMAKE_CXX_FLAGS_DEBUG} -DSAFEMALLOC -DSAFE_MUTEX -DUSE_SYMDIR /Zi")
SET(CMAKE_C_FLAGS_DEBUG 
    "${CMAKE_C_FLAGS_DEBUG} -DSAFEMALLOC -DSAFE_MUTEX -DUSE_SYMDIR /Zi")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /MAP /MAPINFO:EXPORTS") 

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include 
                    ${CMAKE_SOURCE_DIR}/extra/yassl/include 
                    ${CMAKE_SOURCE_DIR}/sql 
                    ${CMAKE_SOURCE_DIR}/regex 
                    ${CMAKE_SOURCE_DIR}/zlib
)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_SOURCE_DIR}/sql/message.rc 
                            ${CMAKE_SOURCE_DIR}/sql/message.h 
                            ${CMAKE_SOURCE_DIR}/sql/sql_yacc.h 
                            ${CMAKE_SOURCE_DIR}/sql/sql_yacc.cc
                            ${CMAKE_SOURCE_DIR}/include/mysql_version.h
                            ${CMAKE_SOURCE_DIR}/sql/sql_builtin.cc
                            ${CMAKE_SOURCE_DIR}/sql/lex_hash.h 
                            ${PROJECT_SOURCE_DIR}/include/mysqld_error.h
                            ${PROJECT_SOURCE_DIR}/include/mysqld_ername.h
                            ${PROJECT_SOURCE_DIR}/include/sql_state.h 
                              PROPERTIES GENERATED 1)

ADD_DEFINITIONS(-DMYSQL_SERVER -D_CONSOLE -DHAVE_DLOPEN -DHAVE_EVENT_SCHEDULER)

ADD_EXECUTABLE(mysqld
               ../sql-common/client.c derror.cc des_key_file.cc
               discover.cc ../libmysql/errmsg.c field.cc  field_conv.cc 
               filesort.cc gstream.cc
               ha_partition.cc
               handler.cc hash_filo.cc hash_filo.h 
               hostname.cc init.cc item.cc item_buff.cc item_cmpfunc.cc 
               item_create.cc item_func.cc item_geofunc.cc item_row.cc 
               item_strfunc.cc item_subselect.cc item_sum.cc item_timefunc.cc 
               key.cc log.cc lock.cc message.rc 
               log_event.cc rpl_record.cc rpl_reporting.cc
               log_event_old.cc rpl_record_old.cc
               message.h mf_iocache.cc my_decimal.cc ../sql-common/my_time.c
               mysqld.cc net_serv.cc 
               nt_servc.cc nt_servc.h opt_range.cc opt_range.h opt_sum.cc 
               ../sql-common/pack.c parse_file.cc password.c procedure.cc 
               protocol.cc records.cc repl_failsafe.cc rpl_filter.cc set_var.cc 
               slave.cc sp.cc sp_cache.cc sp_head.cc sp_pcontext.cc 
               sp_rcontext.cc spatial.cc sql_acl.cc sql_analyse.cc sql_base.cc 
               sql_cache.cc sql_class.cc sql_client.cc sql_crypt.cc sql_crypt.h 
               sql_cursor.cc sql_db.cc sql_delete.cc sql_derived.cc sql_do.cc 
               sql_error.cc sql_handler.cc sql_help.cc sql_insert.cc sql_lex.cc 
               sql_list.cc sql_load.cc sql_manager.cc sql_map.cc sql_parse.cc 
               sql_partition.cc sql_plugin.cc sql_prepare.cc sql_rename.cc 
               sql_repl.cc sql_select.cc sql_show.cc sql_state.c sql_string.cc 
               sql_table.cc sql_test.cc sql_trigger.cc sql_udf.cc sql_union.cc
               sql_update.cc sql_view.cc strfunc.cc table.cc thr_malloc.cc 
               time.cc tztime.cc uniques.cc unireg.cc item_xmlfunc.cc 
               rpl_tblmap.cc sql_binlog.cc event_scheduler.cc event_data_objects.cc
               event_queue.cc event_db_repository.cc 
               sql_tablespace.cc events.cc ../sql-common/my_user.c 
               partition_info.cc rpl_utility.cc rpl_injector.cc sql_locale.cc
               rpl_rli.cc rpl_mi.cc sql_servers.cc
               sql_connect.cc scheduler.cc 
               sql_profile.cc event_parse_data.cc
               ${PROJECT_SOURCE_DIR}/sql/sql_yacc.cc
               ${PROJECT_SOURCE_DIR}/sql/sql_yacc.h
               ${PROJECT_SOURCE_DIR}/include/mysqld_error.h
               ${PROJECT_SOURCE_DIR}/include/mysqld_ername.h 
               ${PROJECT_SOURCE_DIR}/include/sql_state.h
               ${PROJECT_SOURCE_DIR}/include/mysql_version.h 
               ${PROJECT_SOURCE_DIR}/sql/sql_builtin.cc
               ${PROJECT_SOURCE_DIR}/sql/lex_hash.h)

TARGET_LINK_LIBRARIES(mysqld
                      heap myisam myisammrg mysys yassl zlib debug dbug yassl 
                      taocrypt strings vio regex wsock32 ws2_32)

SET_TARGET_PROPERTIES(mysqld PROPERTIES OUTPUT_NAME mysqld${MYSQLD_EXE_SUFFIX})

IF(cmake_version EQUAL 20406)
# Work around for 2.4.6 bug, OUTPUT_NAME will not set the right .PDB
# file name. Note that COMPILE_FLAGS set some temporary pdb during build,
# LINK_FLAGS sets the real one.
SET_TARGET_PROPERTIES(mysqld PROPERTIES
                      COMPILE_FLAGS "/Fd${CMAKE_CFG_INTDIR}/mysqld${MYSQLD_EXE_SUFFIX}.pdb"
                      LINK_FLAGS  "/PDB:${CMAKE_CFG_INTDIR}/mysqld${MYSQLD_EXE_SUFFIX}.pdb")
ENDIF(cmake_version EQUAL 20406)

IF(EMBED_MANIFESTS)
  MYSQL_EMBED_MANIFEST("mysqld" "asInvoker")
ENDIF(EMBED_MANIFESTS)
IF(WITH_ARCHIVE_STORAGE_ENGINE)
  TARGET_LINK_LIBRARIES(mysqld archive)
ENDIF(WITH_ARCHIVE_STORAGE_ENGINE)
IF(WITH_BLACKHOLE_STORAGE_ENGINE)
  TARGET_LINK_LIBRARIES(mysqld blackhole)
ENDIF(WITH_BLACKHOLE_STORAGE_ENGINE)
IF(WITH_CSV_STORAGE_ENGINE)
  TARGET_LINK_LIBRARIES(mysqld csv)
ENDIF(WITH_CSV_STORAGE_ENGINE)
IF(WITH_EXAMPLE_STORAGE_ENGINE)
  TARGET_LINK_LIBRARIES(mysqld example)
ENDIF(WITH_EXAMPLE_STORAGE_ENGINE)
IF(WITH_FEDERATED_STORAGE_ENGINE)
  TARGET_LINK_LIBRARIES(mysqld federated)
ENDIF(WITH_FEDERATED_STORAGE_ENGINE)
IF(WITH_INNOBASE_STORAGE_ENGINE)
  TARGET_LINK_LIBRARIES(mysqld innobase)
ENDIF(WITH_INNOBASE_STORAGE_ENGINE)

ADD_DEPENDENCIES(mysqld GenError)

# NOTE CMake 2.4.6 creates strange dependencies between files in OUTPUT,
# so for now we only list one if more than one

# Sql Parser custom command
ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/sql/sql_yacc.h
#              ${PROJECT_SOURCE_DIR}/sql/sql_yacc.cc
        COMMAND bison.exe ARGS -y -p MYSQL --defines=sql_yacc.h
                               --output=sql_yacc.cc sql_yacc.yy
        DEPENDS ${PROJECT_SOURCE_DIR}/sql/sql_yacc.yy)

IF(WIN32)
  # Windows message file
  ADD_CUSTOM_COMMAND(
          OUTPUT ${PROJECT_SOURCE_DIR}/sql/message.h
#                ${PROJECT_SOURCE_DIR}/sql/message.rc
          COMMAND mc ARGS message.mc
          DEPENDS ${PROJECT_SOURCE_DIR}/sql/message.mc)
ENDIF(WIN32)

# Gen_lex_hash
ADD_EXECUTABLE(gen_lex_hash gen_lex_hash.cc)
TARGET_LINK_LIBRARIES(gen_lex_hash debug dbug mysqlclient wsock32)
GET_TARGET_PROPERTY(GEN_LEX_HASH_EXE gen_lex_hash LOCATION)
ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/sql/lex_hash.h
        COMMAND ${GEN_LEX_HASH_EXE} ARGS > lex_hash.h
        DEPENDS ${GEN_LEX_HASH_EXE})

ADD_CUSTOM_TARGET(
        GenServerSource ALL
        DEPENDS ${PROJECT_SOURCE_DIR}/sql/sql_yacc.h
#               ${PROJECT_SOURCE_DIR}/sql/sql_yacc.cc
                ${PROJECT_SOURCE_DIR}/sql/message.h
#               ${PROJECT_SOURCE_DIR}/sql/message.rc
                ${PROJECT_SOURCE_DIR}/sql/lex_hash.h)

ADD_DEPENDENCIES(mysqld GenServerSource)

# Remove the auto-generated files as part of 'Clean Solution'
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES 
                         "lex_hash.h;message.rc;message.h;sql_yacc.h;sql_yacc.cc")

ADD_LIBRARY(udf_example MODULE udf_example.c udf_example.def)
ADD_DEPENDENCIES(udf_example strings GenError)
TARGET_LINK_LIBRARIES(udf_example strings wsock32)
