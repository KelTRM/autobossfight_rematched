cmake_minimum_required(VERSION 4.0.0)

project(autobossfight_rematched)

add_executable(autobossfight_rematched)

if (MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

target_sources(autobossfight_rematched
	PRIVATE
		main.c
		rng.c
		player.c
		attack_manager.c
		entity.c
		register_attacks.c

		attacks/attack_normal.c

		ui/ask_attack.c
)
