set(udisksqt_SRC
	dblockdevice.cpp  
	dblockpartition.cpp  
	ddiskdevice.cpp	
	ddiskmanager.cpp  
	dudisksjob.cpp  
	udisks2_dbus_common.cpp
)

set(udisksqt_HEADERS
	dblockdevice.h	
	dblockpartition.h  
	ddiskdevice.h  
	ddiskmanager.h  
	dudisksjob.h	
	udisks2_dbus_common.h
)

set(udisksqt_HEADERS_PRIVATE
	private/dblockdevice_p.h
)

set_property(SOURCE org.freedesktop.UDisks2.xml PROPERTY INCLUDE udisks2_dbus_common.h)
set_property(SOURCE org.freedesktop.UDisks2.xml PROPERTY NO_NAMESPACE true)
qt5_add_dbus_interface(udisksqt_SRC org.freedesktop.UDisks2.xml udisks2_interface)

set_property(SOURCE org.freedesktop.DBus.ObjectManager.xml PROPERTY INCLUDE udisks2_dbus_common.h)
set_property(SOURCE org.freedesktop.DBus.ObjectManager.xml PROPERTY NO_NAMESPACE true)
qt5_add_dbus_interface(udisksqt_SRC org.freedesktop.UDisks2.ObjectManager.xml objectmanager_interface)

add_library(udisks2-qt5 SHARED ${udisksqt_SRC} ${udisksqt_HEADERS} ${udisksqt_HEADERS_PRIVATE})
set_target_properties(udisks2-qt5 PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${UDISKS_API_LEVEL})

list(APPEND udisksqt_HEADERS
 	${CMAKE_CURRENT_BINARY_DIR}/udisks2_interface.h
	${CMAKE_CURRENT_BINARY_DIR}/objectmanager_interface.h	
)
target_link_libraries(udisks2-qt5
    Qt5::Core
    Qt5::DBus
)

set_property(TARGET udisks2-qt5 PROPERTY COMPILE_DEFINITIONS UDISKS_STATIC)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/udisks2-qt5.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/udisks2-qt5.pc
  @ONLY
)

install(TARGETS udisks2-qt5 DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/udisks-qt5.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
install(FILES ${udisksqt_HEADERS}
        DESTINATION include/udisks2-qt5
)
install(FILES ${udisksqt_HEADERS_PRIVATE}
        DESTINATION include/udisks2-qt5/private
)
add_subdirectory(modules)
