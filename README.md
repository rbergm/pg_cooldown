# pg_cooldown

PostgreSQL extension to remove all pages of specific relations from the shared buffer.

The extension was heavily inspired by _pg_prewarm_ and functions as a complementary tool: Whereas pg_prewarm adds pages from a
relation to the shared buffer, pg_cooldown removes them again.

The main purpose of this tool is to easily simulate cold-start scenarios in research.


## Installation

Install like a normal contrib extension using `make` and `make install`.
Afterwards, the extension can be activated with `CREATE extension pg_cooldown;`.


## Usage

pg_cooldown can be used just like _pg_prewarm_: `SELECT pg_cooldown('my_relation');` removes
all buffered data pages of the relation from the shared buffers.

Likewise, `SELECT pg_cooldown('my_relation_pkey');` can be used to remove the pages of the primary
key index of the relation. This also works for secondary index structures:
`SELECT pg_cooldown('my_index_name');`
