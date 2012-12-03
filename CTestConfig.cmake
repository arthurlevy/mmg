SET(CTEST_PROJECT_NAME "Mmg3d5")
SET(UPDATE_COMMAND "${SVNCOMMAND}")

SET(CTEST_CONTINUOUS_DURATION 600)
SET(CTEST_START_WITH_EMPTY_BINARY_DIRECTORY_ONCE TRUE)
SET(CTEST_CONTINUOUS_MINIMUM_INTERVAL 30)
SET(CTEST_NIGHTLY_START_TIME "01:00AM")

SET(CTEST_DROP_METHOD "http")
SET(CTEST_DROP_SITE "localhost:2000")
#SET(CTEST_DROP_SITE "vulcain.bordeaux.inria.fr")
SET(CTEST_DROP_LOCATION "/CDash/submit.php?project=Mmg3d5")
SET(CTEST_DROP_SITE_CDASH TRUE)
