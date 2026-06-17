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
		init.c
		main.c
		rng.c
		player.c
		boss.c
		attack.c
		attack_manager.c
		entity.c
		register_attacks.c

		entity_manager/entities.c

		attacks/attack_normal.c
		attacks/attack_full_power.c
		attacks/attack_combo.c
		attacks/attack_nothing.c
		attacks/attack_heal.c
		attacks/default_attack.c

		ui/ask_attack.c
		ui/scoreboard.c
		ui/prompt.c
		
		ui/console_manager/create_buffer.c
		ui/console_manager/bprintf.c
		ui/console_manager/validate_buffer.c
		ui/console_manager/switch_buffer.c
		ui/console_manager/clear_screen.c
		ui/console_manager/copy_buffer.c
		ui/console_manager/delete_buffer.c
		
		ui/color/background.c
		ui/color/foreground.c
		ui/color/color.c
)

if (UNIX)
	target_link_libraries(autobossfight_rematched PUBLIC m)
endif(UNIX)
