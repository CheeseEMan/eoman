//Preprocessor
//Libs
///System
#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/stat.h>

///Local
#include "src/get-package.h"

////Global libs and declarations
#include "src/global/libs.h" //Libraries for the entire program
#include "src/global/declare.h" //Global declarations

//Definitions
///Maximum lengths
#define max_pkg_name 30
#define max_pkg_version_dec 3

///String frams
#define category_RB_frame "%s_pkg_RB"

//Structs
//Package boxes
typedef struct PkgItem {
	//Widgets
	GtkWidget *inst_CB, main_BOX, name_LB;
	
	//Package info
	char name[max_pkg_name];
	uint16_t version[max_pkg_version_dec]; //A package version example would be gnome_web.version = {3.32.1}
} pkgitem;

//Build
pkgitem mk_PKGI(char name[max_pkg_name], uint16_t version[max_pkg_version_dec]) {
	pkgitem PKGI;
	strcpy(PKGI.name, name);
	for (int i = 0; i < max_pkg_version_dec; i++) PKGI.version[i] = version[i];
	return PKGI;
}

//Globals

//Indexs
int8_t category_index = 0;

//Memory management arrays
//
//Warning handelling
void quit(void);

//UI
///Main
GtkBuilder *main_B;
GtkWidget *main_UI, *main_GRID, *pkg_LB;
GList *vis_packbox;


//Functions
///Custom gtk functions
///Remove all container children
void gtk_container_remove_all(GtkWidget *container, int destroy) {
	//Stole this code from the user "unwind" at Stack Overflow
	//Link: https://stackoverflow.com/questions/9192223/remove-gtk-container-children-repopulate-it-then-refresh
	GList *children, *iter;

	children = gtk_container_get_children(GTK_CONTAINER(container));
	for(iter = children; iter != NULL; iter = g_list_next(iter))
  		if (destroy) gtk_widget_destroy(GTK_WIDGET(iter->data));
		else gtk_container_remove(GTK_CONTAINER(container), GTK_WIDGET(iter->data));
	g_list_free(children);
}

//Interface
int8_t reload_pkg(void) {
	//Setup function
	int8_t no_error = 0;
	printf("===========Reloading packages!===========\n\n");
	load_pkg();
	
	//Setup listbox
	gtk_container_remove_all(pkg_LB, 1);
	
	//Finish
	if (no_error) printf("Reloaded packages successfully!\n");
	return no_error;
}



//Misc
///Temporary directory functions
int mk_tmp_dir(char name[]) {
	//Check if temporary directory is setup
	struct stat st = {0}; 
	if (stat(tmp_dir, &st) == -1) {
		printf("Temporary directory \"%s\" does not exist!\n", tmp_dir);
		return -1;
	}

	//Create directory
	char dir_path[strlen(tmp_dir) + strlen(name)];
	strcpy(dir_path, tmp_dir);
	strcat(dir_path, name);
	if (!mkdir(dir_path, 0777)) {
		printf("Could not create directory \"%s.\"\n", dir_path); 
		//Just in case some "special boi" made /tmp owned by root or the like
		return -1;
	}
	else return 1;
	
}

FILE *mk_tmp_file(char name[], char mode[]) {
	struct stat st = {0}; 
	if (stat(tmp_dir, &st) == -1) {
		printf("Temporary directory \"%s\" does not exist!\n", tmp_dir);
		return NULL;
	}

	char file_path[strlen(tmp_dir) + strlen(name)];
	strcpy(file_path, tmp_dir);
	strcat(file_path, name);
	return fopen(file_path, mode);
}

int tch_tmp_file(char name[]) { //"Touches" a temporary file without needing to manually close it
	FILE *file = mk_tmp_file(name, "w");
	if (file) {
		fclose(file);
		return 1;
	}
	
	else return -1;	
}

///Quit
void quit(void) {
	gtk_main_quit();
}

//Main function
int main(int argc, char *argv[]) {
	//Startup
	printf ("Welcome to Eoman!\n Eoman is a lower level GUI frontend for the Solus package manager known as Eopkg.\n\n");

	///Setup temporary directory
	mkdir(tmp_dir, 0777);
	mk_tmp_dir("search");
	tch_tmp_file("search/raw");
	
	
	//Setup main window
	///UI
	gtk_init(&argc, &argv);
	main_B = gtk_builder_new_from_file("ui/main.ui");
	main_UI = GTK_WIDGET(gtk_builder_get_object(main_B, "main_UI"));
	g_signal_connect(GTK_WINDOW(main_UI), "destroy", G_CALLBACK(quit), NULL);
	
	///Grid
	main_GRID = GTK_WIDGET(gtk_builder_get_object(main_B, "main_GRID"));
	

	///Package Listbox
	pkg_LB = GTK_WIDGET(gtk_builder_get_object(main_B, "pkg_LB"));
	reload_pkg();

	//Startup
	gtk_widget_show_all(main_UI);
	gtk_main();
	g_object_unref(main_B);
	return 0;
}
