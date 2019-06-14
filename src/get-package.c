//Libs
#include "global/libs.h"
#include "global/declare.h"


//Definitions
#define max_search_com_pre 12

//Globals
char query[max_query];

//Functions
///Reload packages
///Reload packages
int load_pkg(void) {
	//Startup
	int8_t no_error = 1;
	printf("Retrieving packages... \n");
	
	//Get packages
	char search_com[max_search_com_pre + 3 +  strlen(query) + strlen(raw_search_path)];
	if (strcmp(query, "") == 0) strcpy(search_com, "eopkg la -N ");
	else { 
		strcpy(search_com, "eopkg sr -N ");
		strcat(search_com, query);
	}
	
	//Pipe command to the raw data file
	strcat(search_com, " > ");
	strcat(search_com, raw_search_path);
	system(search_com);	
	printf("Packages retrieved!\n");
	return no_error;
}

