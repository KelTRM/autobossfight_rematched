#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum States {
	STATE_READ,
	STATE_LOCATION,
	STATE_CURR
};

char **SplitPath(const char *Path, size_t *len) {
	enum States State = STATE_CURR;

	char **Locations = malloc(0);
	size_t i = 0;

	if (*Path == '/')
		State = STATE_LOCATION;

	if (Locations == NULL)
		return NULL;

	for (const char *p = Path; *p != 0; p++) {
		switch (State) {
			case STATE_CURR:
//				printf("STATE_CURR %zu\n", i+1);
				{ char **l = realloc(Locations, (++i) + 1);
				if (l == NULL)
					return NULL;
				Locations = l;

				Locations[i-1] = ".";
				State = STATE_LOCATION; }
			case STATE_LOCATION:
//				printf("STATE_LOCATION %zu\n", i+1);
				{ char **l = realloc(Locations, ++i);
				if (l == NULL)
					return NULL;
				Locations = l;

				Locations[i-1] = malloc(0);
				State = STATE_READ;
				if (*p == '/') break; }
			case STATE_READ:
//				printf("STATE_READ %c\n", *p);
				{ if (*(p+1) == '/') {
					State = STATE_LOCATION;
					//break;
				}
				size_t len = strlen(Locations[i-1]);
				char *txt = realloc(Locations[i-1], len+2);
				if (txt == NULL)
					return NULL;
				txt[len] = *p;
				txt[len+1] = 0; }
		}
	}
	for (size_t j = 0; j < i; j++) {
		printf("\"%s\" ", Locations[j]);
	}
	fflush(stdout);

	if (len != NULL) *len = i;
	return Locations;
}
