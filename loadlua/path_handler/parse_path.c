#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char **SplitPath(const char *Path, size_t *len);

char *ParsePath(char *Pathname) {
	size_t len = 0;
	char **PathComponents = SplitPath(Pathname, &len);

	size_t ComponentCount = 0;
	char **Components = malloc(ComponentCount);

	for (size_t i = 0; i < len; i++) {
		if (strcmp(".", PathComponents[i]) == 0)
			continue;

		if (strcmp("..", PathComponents[i]) == 0) {
			ComponentCount--;
			continue;
		}

		char **cmp = realloc(Components, ++ComponentCount);
		if (cmp == NULL)
			goto error;

		cmp[ComponentCount-1] = PathComponents[i];
		Components = cmp;
	}

	for (size_t i = 0; i < ComponentCount; i++) {
		printf("%s ", Components[i]);
	}

	return NULL;

error:
	// free stuff
	for (size_t i = 0; i < len; i++)
		free(PathComponents[i]);
	free(PathComponents);
	free(Components);

	// and a null for an error
	return NULL;
}
