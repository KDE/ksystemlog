# - Try to find Audit library.
# Once done this will define
#
#  AUDIT_FOUND - system has Audit
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
find_package(PkgConfig)
pkg_check_modules(PC_AUDIT QUIET IMPORTED_TARGET audit)

set(AUDIT_FOUND ${PC_AUDIT_FOUND})

set_package_properties(Audit PROPERTIES
  DESCRIPTION "Dynamic library for security auditing")
