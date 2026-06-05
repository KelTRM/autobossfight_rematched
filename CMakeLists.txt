cmake_minimum_required(VERSION 4.0.0)

project(autobossfight_rematched)

add_executable(autobossfight_rematched)

target_sources(autobossfight_rematched
	PRIVATE
		main.c
		rng.c
		player.c
		attack_manager.c
		entity.c
		register_attacks.c

		attacks/attack_normal.c
)
