/*
 * Wazuh Module to analyze system vulnerabilities
 * Copyright (C) 2015-2019, Wazuh Inc.
 * January 4, 2018.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef WM_VUNALIZER_DB
#define WM_VUNALIZER_DB

#define CVE_DBS_PATH            "queue/vulnerabilities/"
#define CVE_DB CVE_DBS_PATH     "cve.db"

#define AGENTS_TABLE            "AGENTS"
#define CVE_TABLE               "VULNERABILITIES"
#define CVE_INFO_TABLE          "VULNERABILITIES_INFO"
#define INFO_STATE_TABLE        "INFO_STATE"
#define METADATA_TABLE          "METADATA"
#define METADATADB_TABLE        "DB_METADATA"
#define MAX_QUERY_SIZE          OS_SIZE_1024
#define MAX_SQL_ATTEMPTS        10
#define VU_MAX_PACK_REQ         40

typedef enum vu_query {
    SELECT_QUERY,
    DELETE_QUERY,
    TIMESTAMP_QUERY,
    VU_INSERT_QUERY,
    VU_INSERT_CVE,
    VU_INSERT_CVE_INFO,
    VU_INSERT_METADATA,
    VU_INSERT_METADATA_DB,
    VU_INSERT_AGENTS,
    VU_UPDATE_DOUBLE_CVE,
    VU_UPDATE_CVE,
    VU_UPDATE_CVE_SEC,
    VU_UPDATE_CVE_VAL,
    VU_UPDATE_CVE_VAL_SEC,
    VU_JOIN_QUERY,
    VU_JOIN_RH_QUERY,
    VU_REMOVE_OS,
    VU_REMOVE_AGENTS_TABLE,
    VU_REMOVE_PATCH,
    VU_SOFTWARE_REQUEST,
    VU_SOFTWARE_FULL_REQ,
    VU_SYSC_SCAN_REQUEST,
    VU_SYSC_UPDATE_SCAN,
    VU_GLOBALDB_REQUEST,
    BEGIN_T,
    END_T
} vu_query;

static const char *vu_queries[] = {
    "SELECT %s FROM %s WHERE %s;",
    "DELETE FROM %s WHERE %s;",
    "SELECT TIMESTAMP FROM " METADATA_TABLE " WHERE OS = ?;",
    "INSERT INTO ",
    "INSERT INTO " CVE_TABLE " VALUES(?,?,?,?,?,?,?,?);",
    "INSERT INTO " CVE_INFO_TABLE " VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?);",
    "INSERT INTO " METADATA_TABLE " VALUES(?,?,?,?,?);",
    "INSERT INTO " METADATADB_TABLE " VALUES(?);",
    "INSERT INTO " AGENTS_TABLE " VALUES(?,?,?,?);",
    "UPDATE " CVE_TABLE " SET OPERATION = ?, SECOND_OPERATION = ? WHERE OPERATION = ? AND SECOND_OPERATION IS NULL;",
    "UPDATE " CVE_TABLE " SET OPERATION = ? WHERE OPERATION = ?;",
    "UPDATE " CVE_TABLE " SET SECOND_OPERATION = ? WHERE SECOND_OPERATION = ?;",
    "UPDATE " CVE_TABLE " SET OPERATION = ?, OPERATION_VALUE = ? WHERE OPERATION = ?;",
    "UPDATE " CVE_TABLE " SET SECOND_OPERATION = ?, SECOND_OPERATION_VALUE = ? WHERE SECOND_OPERATION = ?;",
    "SELECT ID, PACKAGE_NAME, TITLE, SEVERITY, PUBLISHED, UPDATED, REFERENCE, RATIONALE, VERSION, OPERATION, OPERATION_VALUE, SECOND_OPERATION, SECOND_OPERATION_VALUE, PENDING, CVSS, CVSS3 FROM " CVE_INFO_TABLE " INNER JOIN " CVE_TABLE " ON ID = CVEID AND " CVE_INFO_TABLE ".OS = " CVE_TABLE ".OS INNER JOIN " AGENTS_TABLE " ON PACKAGE_NAME = PACKAGE WHERE VULNERABILITIES_INFO.OS = ? AND AGENT_ID = ? ORDER BY ID;",
    "SELECT ID, PACKAGE_NAME, TITLE, SEVERITY, PUBLISHED, UPDATED, REFERENCE, RATIONALE, VERSION, OPERATION, OPERATION_VALUE, SECOND_OPERATION, SECOND_OPERATION_VALUE, PENDING, CVSS, CVSS3, CVSS_VECTOR, BUGZILLA_REFERENCE, CWE, ADVISORIES FROM VULNERABILITIES_INFO INNER JOIN VULNERABILITIES ON ID = CVEID AND VULNERABILITIES_INFO.OS = 'REDHAT' AND VULNERABILITIES.OS = ? INNER JOIN AGENTS ON AGENT_ID = ? AND PACKAGE_NAME = PACKAGE ORDER BY ID, PACKAGE_NAME;",
    "DELETE FROM %s WHERE OS = ?;",
    "DELETE FROM " AGENTS_TABLE ";",
    "DELETE FROM " CVE_TABLE " WHERE CVEID = ?;",
    "agent %s sql SELECT DISTINCT NAME, VERSION, ARCHITECTURE FROM SYS_PROGRAMS WHERE TRIAGED = 0 AND SCAN_ID = '%s' LIMIT %i OFFSET %i;",
    "agent %s sql SELECT DISTINCT NAME, VERSION, ARCHITECTURE FROM SYS_PROGRAMS WHERE SCAN_ID = '%s' LIMIT %i OFFSET %i;",
    "agent %s sql SELECT SCAN_ID FROM SYS_PROGRAMS WHERE SCAN_TIME = (SELECT SCAN_TIME FROM SYS_PROGRAMS S1 WHERE NOT EXISTS (SELECT SCAN_TIME FROM SYS_PROGRAMS S2 WHERE S2.SCAN_TIME > S1.SCAN_TIME)) LIMIT 1;",
    "agent %s sql UPDATE SYS_PROGRAMS SET TRIAGED = 1 WHERE SCAN_ID = '%s';",
    "SELECT OS_NAME, OS_MAJOR, NAME, ID, IP, OS_ARCH FROM AGENT WHERE (STRFTIME('%s', 'NOW', 'LOCALTIME') - STRFTIME('%s', LAST_KEEPALIVE)) < ?;",
    "BEGIN TRANSACTION;",
    "END TRANSACTION;"
};

extern char *schema_vuln_detector_sql;

#endif
