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

include(ui/CMakeLists.txt)
include(text/CMakeLists.txt)
include(image/CMakeLists.txt)
include(audio/CMakeLists.txt)

set(mark_SRC
    main.cpp
    ${UI_SRC}
    ${TEXT_SRC}
    ${IMAGE_SRC}
    ${AUDIO_SRC}
)

add_executable(mark ${mark_SRC})

target_link_libraries(mark Qt5::Widgets)

install(TARGETS mark DESTINATION bin)
