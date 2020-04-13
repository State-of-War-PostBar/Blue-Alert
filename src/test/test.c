#include <pch.h>

#include "../lib/container/hash_map.h"
#include "../lib/filesystem/filesystem.h"
#include "../lib/profile/profile.h"

#include "test.h"

void prnt(void *e)
{
    sowr_HashMapValue *f = (sowr_HashMapValue *)e;
    printf("%s, has a hashcode of %zu, in slot %zu.\n", f->data, f->value_hash, f->index_hash % 16);
}

void test(void)
{
    printf("sowr.log exists? %d\n", sowr_PathExists("sowr.log"));
    printf("src/lib exists? %d\n", sowr_PathExists("src/lib"));
    printf("lol/ggg.a exists? %d\n", sowr_PathExists("lol/ggg"));
    sowr_HashMap *map = sowr_HashMap_Create();

    const char *index = "sbest";
    const char *value = "taxerap";
    sowr_HashMap_InsertS(map, index, value);
    sowr_HashMapValue *val = sowr_HashMap_TakeS(map, index);
    printf("Got %s from the hashmap.\n", val->data);

    for (int i = 0; i < 100; i++)
    {
        sowr_HashMap_InsertS(map, index, value);
    }

    printf("After 100 insertion of same index, the map has a length of %zu.\n", map->length);

    sowr_HashMap_InsertS(map, value, index);
    printf("Now the map has a length of %zu, got %s from the map.\n", map->length, sowr_HashMap_GetS(map, value)->data);
    printf("And got %s from the map finally.\n", sowr_HashMap_GetS(map, index)->data);

    sowr_HashMap_InsertS(map, "gg", "lol");
    sowr_HashMap_InsertS(map, "keystel", "sowr");
    sowr_HashMap_InsertS(map, "keystel2", "sowd");
    sowr_HashMap_InsertS(map, "player.taxerap", "entity.gled2");
    sowr_HashMap_InsertS(map, "utyo", "recho");
    sowr_HashMap_InsertS(map, "bbbbffff", "sowr.name");

    printf("Printing the hashmap:\n");
    sowr_HashMap_Walk(map, prnt);
    printf("Done.\n");

    printf("keystel2 == %s\n", sowr_HashMap_GetS(map, "keystel2")->data);
    printf("sbest == %s\n", sowr_HashMap_GetS(map, "sbest")->data);
    sowr_HashMap_InsertS(map, "sbest", "gsliu");
    printf("Now sbest == %s\n", sowr_HashMap_GetS(map, "sbest")->data);
    printf("keystel2 == %s\n", sowr_HashMap_GetS(map, "keystel2")->data);

    printf("Hashcodes test:\n");
    printf("sbest: %zu.\n", sowr_GetHashS("sbest"));
    printf("keystel2: %zu.\n", sowr_GetHashS("keystel2"));
    printf("player.taxerap: %zu.\n", sowr_GetHashS("player.taxerap"));
    printf("gg: %zu.\n", sowr_GetHashS("gg"));
    printf("g: %zu.\n", sowr_GetHashS("g"));
    sowr_HashMap_InsertS(map, "sbest", "taxerap");

    printf("sbest hashed: %zu, taxerap hashed: %zu.\n", sowr_GetHashS("sbest"), sowr_GetHashS("taxerap"));
    printf("confirm in hashmap: sbest-%zu, taxerap: %zu\n", sowr_HashMap_GetS(map, "sbest")->index_hash, sowr_HashMap_GetS(map, "sbest")->value_hash);

    sowr_HashMap_Destroy(map);
}
