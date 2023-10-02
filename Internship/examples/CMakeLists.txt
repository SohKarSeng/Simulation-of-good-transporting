build_lib_example(
    NAME test2
    SOURCE_FILES test2.cc
    LIBRARIES_TO_LINK 
        ${libcore}
        ${libTemperature}
        ${libnetwork}
        ${libapplications}
        ${libinternet}
        ${libmobility}
        ${libpoint-to-point}
        ${libwifi}
)

