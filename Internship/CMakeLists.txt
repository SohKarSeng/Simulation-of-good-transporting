check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/Internship-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME Internship
    SOURCE_FILES model/DataTransfer.cc
                 model/DataTransferConfigurator.cc
    HEADER_FILES model/DataTransfer.h
                 model/DataTransferConfigurator.h
    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/Internship-test-suite.cc
                 ${examples_as_tests_sources}
)

