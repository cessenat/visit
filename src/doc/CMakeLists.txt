# Copyright (c) Lawrence Livermore National Security, LLC and other VisIt
# Project developers.  See the top-level LICENSE file for dates and other
# details.  No copyright assignment is required to contribute to VisIt.

#****************************************************************************
# Modifications:
#   Kathleen Biagas, Thu March 7, 2019
#   Added sphinx_launcher (python executable) for Windows.
#
#   Mark C. Miller, Mon Jun  8 16:45:05 PDT 2020
#   Added linkcheck builder
#
#   Kathleen Biagas, Wed Oct 21, 2020
#   Replaced old logic with python-3 creation of manuals, which was moved
#   from root CMakeLists.txt.
#
#   Kathleen Biagas, Thu Jan 7, 2021
#   Merge logic for creating the build and install commands, they are the
#   same for all platforms, except the sphinx build command which is
#   different on Windows.  Added logic to ensure sphinx build command exists.
#
#   Eric Brugger, Wed Feb 10 15:07:41 PST 2021
#   Update for switch to Python 3.
#
#   Kathleen Biagas, Thu Apr 22, 2021
#   Don't build manuals for VISIT_STATIC, as sphinx won't be built.
#
#   Kathleen Biagas, Thu July 1, 2021
#   Change FATAL_ERROR to WARNING with early return.
#
#****************************************************************************

if(VISIT_PYTHON_DIR AND VISIT_ENABLE_MANUALS AND NOT VISIT_STATIC)
    message(STATUS "Configure manuals targets")
    set(errmsgtail "To remove this warning, either install sphinx or set VISIT_ENABLE_MANUALS to false.")
    if(WIN32)
        # Need a different sphinx build command for windows
        if(NOT EXISTS ${VISIT_PYTHON_DIR}/Scripts/sphinx-build-script.py)
            message(WARNING "Manuals are enabled but"
                   " ${VISIT_PYTHON_DIR}/Scripts/sphinx-build-script.py"
                   " does not exist so manuals will not be built. ${errmsgtail}")
            return()
        endif()
        set(sphinx_build_cmd "${VISIT_PYTHON_DIR}/python.exe \ "
              "${VISIT_PYTHON_DIR}/Scripts/sphinx-build-script.py")
    else()
        if(NOT EXISTS ${VISIT_PYTHON_DIR}/bin/sphinx-build)
            message(WARNING "Manuals are enabled but"
                    " ${VISIT_PYTHON_DIR}/bin/sphinx-build"
                    " does not exist so manuals will not be built. ${errmsgtail}")
            return()
        endif()
        set(sphinx_build_cmd ${VISIT_PYTHON_DIR}/bin/sphinx-build)
    endif()

    # Add custom target to build the manuals
    add_custom_target(manuals COMMAND ${sphinx_build_cmd}
        -b html ${VISIT_SOURCE_DIR}/doc
        ${VISIT_BINARY_DIR}/resources/help/en_US/manual -a)

    # Add command to ensure the manuals are installed to the correct location
    install(DIRECTORY ${VISIT_BINARY_DIR}/resources/help/en_US/manual
            DESTINATION ${VISIT_INSTALLED_VERSION_RESOURCES}/help/en_US/
            FILE_PERMISSIONS OWNER_READ OWNER_WRITE
                             GROUP_READ GROUP_WRITE
                             WORLD_READ
            DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                                  GROUP_READ GROUP_WRITE GROUP_EXECUTE
                                  WORLD_READ             WORLD_EXECUTE
            CONFIGURATIONS Debug Release RelWithDebInfo MinSizeRel)
endif()
