#include <memory.h>
#include <stdio.h>

#include <hawkey/package.h>
#include <hawkey/packagelist.h>
#include <hawkey/query.h>
#include <hawkey/sack.h>

#include "rpm_checker.h"

static volatile bool sackCreated = false;
static volatile HySack sack;

static void createSack() {
  sack = hy_sack_create(NULL, NULL, NULL, NULL, HY_MAKE_CACHE_DIR);
  hy_sack_load_system_repo(sack, NULL, HY_BUILD_CACHE | HY_LOAD_FILELISTS);
  sackCreated = true;
  printf("loaded %d packages\n", hy_sack_count(sack));
}

char audit(char const * libraryFile) {
  if (!sackCreated) createSack();

  HyQuery query = hy_query_create(sack);

  char queryString[1024] = "*";
  strncat(queryString, libraryFile, 1024-strlen(queryString)-1);
  hy_query_filter(query, HY_PKG_FILE, HY_GLOB, queryString);
  printf("running query for %s\n", queryString);
  HyPackageList packageList = hy_query_run(query);

  printf("found %d packages\n", hy_packagelist_count(packageList));
  if (0 == hy_packagelist_count(packageList)) {
    printf("no package contained file\n");
    return 0;
  }
  HyPackage package = hy_packagelist_get(packageList, 0);

  printf("found package: %s\n", hy_package_get_name(package));
  printf("license is %s\n", hy_package_get_license(package));
  hy_packagelist_free(packageList);
  hy_query_free(query);

  return 1;
}
