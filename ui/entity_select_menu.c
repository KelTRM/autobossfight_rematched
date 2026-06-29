#include<stdlib.h>
#include"console_manager/console.h"
#include"../entity.h"
#include"ui.h"

Entity_t *SelectEntity(Entity_t **Entities, size_t EntityCount) {
	BUFHANDLE MainBuffer = ActiveBuffer;
	BUFHANDLE SelectionMenu = CreateBuffer();

	for (size_t i = 0; i < EntityCount; i++) {
		bprintf(SelectionMenu, "#%zu - ", i+1);
		Entities[i]->DisplayEntity(
				SelectionMenu,
				Entities[i],
				0, 0, 0, 0
		);
	}

	SwitchBuffer(SelectionMenu);

	Entity_t *ChosenEntity = NULL;
	char *Response;

	while (ChosenEntity == NULL) {
		Prompt("\nPlease select a target", &Response, 1);

		if (Response == NULL) {
			break;
		} else {
			size_t EntityNum = atoll(Response);

			if (EntityNum == 0 || EntityNum > EntityCount) {
				printf("Invalid target %s. Please select a valid target.\n", Response);
				continue;
			}

			ChosenEntity = Entities[EntityNum-1];
		}
	}

	SwitchBuffer(MainBuffer);
	DeleteBuffer(SelectionMenu);

	if (Response != NULL)
		free(Response);

	return ChosenEntity;
}
