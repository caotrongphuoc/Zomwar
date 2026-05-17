CFLAGS		+= -I./sources/app/game/game_zomwar
CPPFLAGS	+= -I./sources/app/game/game_zomwar

VPATH += sources/app/game/game_zomwar

# CPP source files 
# Zomwar 
SOURCES_CPP += sources/app/game/game_zomwar/zw_game_gunner.cpp
SOURCES_CPP += sources/app/game/game_zomwar/zw_game_bullet.cpp
SOURCES_CPP += sources/app/game/game_zomwar/zw_game_car.cpp