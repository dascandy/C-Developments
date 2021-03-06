cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

add_executable(game 
  src/Managers/TileSheetManager.cpp
  src/Managers/EntityManager.cpp
  src/Managers/StateManager.cpp
  src/Managers/GameManager.cpp
  src/Managers/CollisionManager.cpp
  src/Managers/EventManager.cpp
  src/Timer.cpp
  src/Window/Window.cpp
  src/Map/TileLayer.cpp
  src/Map/InteractiveTileLayer.cpp
  src/Map/WorldMap.cpp
  src/Map/CollidableTileLayer.cpp
  src/Locators/TileSheetManagerLocator.cpp
  src/Locators/TextureManagerLocator.cpp
  src/Locators/EventManagerLocator.cpp
  src/States/StateWinGame.cpp
  src/States/StateGame.cpp
  src/States/StateMainMenu.cpp
  src/Utilities.cpp
  src/main.cpp
  src/Game/Game.cpp
  src/Tiles/InteractiveTile.cpp
  src/Tiles/MovingCoinTile.cpp
  src/Tiles/CoinTile.cpp
  src/Tiles/CoinDispenserTile.cpp
  src/Tiles/LockTile.cpp
  src/Tiles/SpikeTile.cpp
  src/Tiles/DoorTile.cpp
  src/Tiles/BouncyTile.cpp
  src/Tiles/KeyTile.cpp
  src/Parser/StateParser.cpp
  src/Parser/LevelParser.cpp
  src/Animations/AnimationPlayer.cpp
  src/Animations/Animation.cpp
  src/Sprite/TileSheet.cpp
  src/Entities/Entity.cpp
  src/Entities/Character.cpp
  src/Entities/Player.cpp
  src/XML/tinyxmlerror.cpp
  src/XML/tinyxmlparser.cpp
  src/XML/tinystr.cpp
  src/XML/tinyxml.cpp
  src/Base64.cpp
)

# Change this to your local SFML install path (would be nice to have SFML as submodule)
set(SFML_INSTALL_PATH "${CMAKE_SOURCE_DIR}/../sfml/install")

set(CMAKE_MODULE_PATH "${SFML_INSTALL_PATH}/cmake/Modules" ${CMAKE_MODULE_PATH})
set(SFML_ROOT ${SFML_INSTALL_PATH})
find_package(SFML 2 REQUIRED system window graphics)

target_link_libraries(game
  PRIVATE
    ${SFML_LIBRARIES}
    z
)

target_include_directories(game
  PRIVATE
    ${SFML_INSTALL_PATH}/../include
    ${CMAKE_SOURCE_DIR}/include
)
