cmake_minimum_required(VERSION 4.0.0)

project(autobossfight)

add_library(autobossfight_ui_manager)

target_include_directories(autobossfight_ui_manager
	INTERFACE .
	)

if (UNIX)
	target_link_libraries(autobossfight_ui_manager PUBLIC m)
endif(UNIX)

target_sources(autobossfight_ui_manager
	PRIVATE
		ask_attack.c
		scoreboard.c
		prompt.c
		strings.c
		progress_bar.c
		unicode_strlen.c
		entity_select_menu.c

		console_manager/create_buffer.c
		console_manager/bprintf.c
		console_manager/validate_buffer.c
		console_manager/switch_buffer.c
		console_manager/clear_screen.c
		console_manager/copy_buffer.c
		console_manager/delete_buffer.c

		color/background.c
		color/foreground.c
		color/color.c
		color/get_color_str.c
	)
