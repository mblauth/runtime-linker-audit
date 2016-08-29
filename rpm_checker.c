#include "rpm_checker.h"

#include <rpm/rpmlib.h>
#include <rpm/rpmts.h>
#include <rpm/rpmdb.h>

void readConfig() {
  if (rpmReadConfigFiles(NULL, NULL) == -1) {
     printf("failed\n");
     exit(1);
  }
}

// libhawkeye

void indexRPMDB() {
  printf("creating rpm transaction\n");
  rpmts transaction = rpmtsCreate();
  printf("reading config files\n");
  readConfig();
  printf("creating iterator\n");
  rpmdbMatchIterator iterator = rpmtsInitIterator(transaction, RPMDBI_PACKAGES, NULL, 0);
  Header h;
  printf("running transaction\n");
  while (NULL != (h = rpmdbNextIterator(iterator))) {
    h = headerLink(h);
    printf("package: %s\n", headerGetString(h, RPMTAG_NAME));
    printf("license: %s\n", headerGetString(h, RPMTAG_LICENSE));
    headerFree(h);
  }
  rpmdbFreeIterator(iterator);
  rpmtsFree(transaction);
}
