cmake_minimum_required(VERSION 4.0.0)

project(autobossfight_rematched)

add_library(autobossfight_attack_mgr)

target_sources(autobossfight_attack_mgr
	PRIVATE
		# non-attacks
		attack_manager.c
		get_applicable_entities.c
		default_attack.c

		#attacks
		attacks/attack_combo.c
		attacks/attack_normal.c
		attacks/attack_full_power.c
		attacks/attack_heal.c
		attacks/attack_normal.c
		attacks/attack_nothing.c
		attacks/attack_revive.c
		attacks/attack_transform.c
		
		#list
#		linked_list/list.c
)

#add_subdirectory(../ui)

target_include_directories(autobossfight_attack_mgr
	INTERFACE .
)

#target_link_libraries(autobossfight_attack_mgr
#	PUBLIC
#		autobossfight_ui_manager
#)
