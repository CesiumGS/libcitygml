# - Try to find glutess
# Once done this will define
#
#  GLUTESS_FOUND - glutess found
#  GLUTESS_INCLUDE_DIR - the glutess include directory
#  GLUTESS_LIBRARY - Link these to use glutess

FIND_PATH(GLUTESS_INCLUDE_DIR glutess.h
    PATHS ${GLUTESS_ROOT}
    PATH_SUFFIXES glutess include
)

FIND_LIBRARY(GLUTESS_LIBRARY
    NAMES libglutess.a glutess
    HINTS ${GLUTESS_ROOT}
)

INCLUDE(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set GLUTESS_FOUND to TRUE
# if all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLUTESS REQUIRED_VARS GLUTESS_LIBRARY GLUTESS_INCLUDE_DIR)

MARK_AS_ADVANCED(GLUTESS_INCLUDE_DIR GLUTESS_LIBRARY)

SET(GLUTESS_LIBRARIES ${GLUTESS_LIBRARY} )
SET(GLUTESS_INCLUDE_DIRS ${GLUTESS_INCLUDE_DIR} )

