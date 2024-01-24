/*-------------------------------------------------------------------------
 *
 * pg_cooldown.c
 *		  cold-start utilities
 *
 * IDENTIFICATION
 *		  contrib/pg_cooldown/pg_cooldown.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "fmgr.h"
#include "miscadmin.h"
#include "access/relation.h"
#include "storage/bufmgr.h"
#include "utils/acl.h"
#include "utils/lsyscache.h"
#include "utils/rel.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(pg_cooldown);

/*
 * pg_cooldown(regclass)
 *
 * The argument is the relation to be removed from the shared buffer. It can be an actual table or an index structure.
 * The return value is just a zero without any special meaning.
 */
Datum
pg_cooldown(PG_FUNCTION_ARGS)
{
    Oid             relOid;
    SMgrRelation    storageRelation;
    Relation        rel;
    AclResult       aclresult;

    if (PG_ARGISNULL(0))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("relation cannot be null")));
    relOid = PG_GETARG_OID(0);

    rel = relation_open(relOid, AccessShareLock);
    aclresult = pg_class_aclcheck(relOid, GetUserId(), ACL_SELECT);
	if (aclresult != ACLCHECK_OK)
		aclcheck_error(aclresult, get_relkind_objtype(rel->rd_rel->relkind), get_rel_name(relOid));

    storageRelation = RelationGetSmgr(rel);

    DropRelFileNodesAllBuffers(&storageRelation, 1);

    relation_close(rel, AccessShareLock);
    PG_RETURN_INT64(0);
}
