ROOT_STANDARD_LIBRARY_PACKAGE(TreeMap
  HEADERS
    RTreeMapBase.hxx
    RTreeMapPainter.hxx
  SOURCES
    RTreeMapBase.cxx
    RTreeMapImporter.cxx
    RTreeMapPainter.cxx
  DEPENDENCIES
    ROOTNTuple
    ROOTNTupleUtil
    Gpad
    RIO
)

#ROOT_ADD_TEST_SUBDIRECTORY(test)

if(clad)
  target_compile_definitions(TreeMap PUBLIC ROOT_SUPPORT_CLAD)
endif()
