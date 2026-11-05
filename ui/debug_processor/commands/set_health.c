

static int SetHealth(int argc, const char **argv) {
	if (ParamRange(argc, argv, 3, INT_MAX))
		return 1;

	// find entity from parameter 1
	const char *Entity = argv[1];
	Entity_t *Target = GetEntityFromName(Entity);

	if (Target == NULL) {
		printf("Unable to find entity %s.\n", Entity);
		return 2;
	}

	Health_t NewHealth = atoll(argv[2]);
	Target->HealthPoints = NewHealth;

	printf("%s's health set to %d.\n", Entity, NewHealth);

	return 0;
}

