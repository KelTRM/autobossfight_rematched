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
		players.c
		attack_manager.c
		entity.c
		register_attacks.c

		attacks/attack_normal.c
		attacks/default_attack.c

		ui/ask_attack.c
		ui/scoreboard.c
		
		ui/console_manager/create_buffer.c
		ui/console_manager/bprintf.c
		ui/console_manager/validate_buffer.c
		ui/console_manager/switch_buffer.c
		ui/console_manager/clear_screen.c
		ui/console_manager/copy_buffer.c
		
		ui/color/background.c
		ui/color/foreground.c
		ui/color/color.c
)

if (UNIX)
	target_link_libraries(autobossfight_rematched PUBLIC m)
endif(UNIX)
