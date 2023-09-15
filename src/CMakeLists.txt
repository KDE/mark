# Copyright (C) 2020 by Caio Jordão Carvalho <caiojcarvalho@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 3 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(KF5_MIN_VERSION 5.4)

include(KDEInstallDirs)
include(KDECMakeSettings)
#include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core    # QStringLiteral
    Widgets # QApplication, QAction
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      # KAboutData
    XmlGui          # KXmlGuiWindow, KActionCollection
    ConfigWidgets   # KStandardActions
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(audio/CMakeLists.txt)
include(core/CMakeLists.txt)
include(image/CMakeLists.txt)
include(text/CMakeLists.txt)
include(ui/CMakeLists.txt)
include(util/CMakeLists.txt)

set(mark_SRC
    main.cpp
    ${AUDIO_SRC}
    ${CORE_SRC}
    ${IMAGE_SRC}
    ${TEXT_SRC}
    ${UI_SRC}
    ${UTIL_SRC}
)

add_executable(mark ${mark_SRC})

target_link_libraries(mark
    Qt5::Core
    Qt5::Widgets
    KF5::CoreAddons
    KF5::XmlGui
)

install(TARGETS mark ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
