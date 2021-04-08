#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structs
// This is the Item Node Struct
struct item_node_struct {
	char name[32];
	int count;
	struct item_node_struct *left, *right;
};
typedef struct item_node_struct item_node;

//This is the Tree Name Node Struct
struct tree_name_node_struct {
	char treeName[32];
	struct tree_name_node_struct *left, *right;
	item_node *theTree;
};
typedef struct tree_name_node_struct tree_name_node;

// Function Prototypes
void remove_crlf(char *s); //Remove Carriage Return
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length);
void initialize_tree(FILE *ifp, tree_name_node *tree);

// Constructor Prototypes
item_node *create_item_node(char name[], int count); //Constructor for Item Node
tree_name_node *create_tree_name_node(char treeName[], item_node *theTree); //Constructor for Tree Name Node

// Destructor Prototypes
void dispose_item_node(item_node *del_item_node); //Destructor for Item Node
void dispose_tree_name_node(tree_name_node *del_tree_name_node); //Destructor for Tree Name Node

// The Main Function
int main() {
	//Set the Input File to ifp and the Output File to ofp
	FILE *ifp = fopen("cop3502-as4-input.txt", "r");
	FILE *ofp = fopen("cop3502-as4-output-prats-christopher.txt", "w");

	//Create and Initialize the Tree
	tree_name_node *tree = malloc(sizeof(tree_name_node));
	initialize_tree(ifp, tree);

	//Initialize the Tree

	//Close the Input and Output Files Prior to Exit
	fclose(ifp);
	fclose(ofp);

	return 0;
}

/*
 * Remove Carriage Return. This function is from the lectures by the professor,
 * Matthew Gerber. We are permittied to use this code in our assignments.
 * Its Purpose is to Remove Carriage Returns and Line Feeds from the End of a String
 */
void remove_crlf(char *s) {
	/*
	 * Remember that in C, a string is an array of characters ending with a '\0' character.  We
	 * are given s, a pointer to this array.  We want to find the end of the string that s points
	 * to.  We know that strlen() will give us the *length* of s, so we add s's length to its
	 * location to find the end of s.
	 */

	char *t = s + strlen(s);

	//t is now at s's null terminator.

	t--;

	//t now points to the last character of s.  (Importantly, we haven't changed s.)

	/*
	 * \n is the UNIX-like newline character.  MS-DOS and Windows occasionally add \r, which is a
	 * separate "carriage return" character.  We need to find and clobber both.
	 */

	/*
	 * We have two cases where t can end up to the left of s: if we're passed a zero-length string,
	 * or if literally the entire string is made up of \n and/or \r.  Stop everything if t ends up
	 * to the left of s to prevent bad things from happening in that case.
	 */

	//Look at the contents of t as a single character.  As long as it's still a newline or a CR...

	while((t >= s) && (*t == '\n' || *t == '\r')) {
		*t = '\0';  // ...clobber it by overwriting it with null, and...
		t--;        // decrement t *itself (i.e., change the pointed location)

		/*
		 * t now still points to the last character of s, and we keep going until it's something
		 * other than a CR or newline.
		 */
	}
}

/*
 * This function will write the next line from ifp that isn't blank into buf, removing any
 * newline characters.  It writes a zero length string at eof, and only then.  It returns
 * true if it successfully read a line and false if it didn't. This function is from the
 * lectures by the professor, Matthew Gerber.
 */
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length) {
	buf[0] = '\0';

	while(!feof(ifp) && (buf[0] == '\0')) {
		fgets(buf, max_length, ifp);
		remove_crlf(buf);
	}

	if(buf[0] != '\0') {
		return 1;
	}
	else {
		return 0;
	}
}

// This Function will Initialize the Tree from a File
void initialize_tree(FILE *ifp, tree_name_node *tree) {
	//Get the Item Counts from the File
	int num_tree_nodes, num_item_nodes, num_commands;
	char buf[64];
	get_next_nonblank_line(ifp, buf, 63);
	sscanf(buf, "%d %d %d", &num_tree_nodes, &num_item_nodes, &num_commands);

	//Create the Tree Nodes
	for (int i = 0; i < num_tree_nodes; i++) {
		//Read the Node Name
		get_next_nonblank_line(ifp, buf, 63);

		//If the Item is the Head of the Tree
		if (i == 0) {
			strcpy(tree->treeName, buf);
		}

		else {
		}
	}

	//Create the Item Nodes
	for (int i = 0; i < num_item_nodes; i++) {
	}
}

/*
 * This section includes the functions for constructing the various
 * objects used in this program
 */

// This Function will Create and Return a New Item Node
item_node *create_item_node(char name[], int count) {
	item_node *new_item_node = malloc(sizeof(item_node));
	strcpy(new_item_node->name, name);
	new_item_node->count = count;
	new_item_node->left = NULL;
	new_item_node->right = NULL;

	//Return the Created Item Node
	return new_item_node;
}

// This Function will Create and Return a New Tree Name Node
tree_name_node *create_tree_name_node(char treeName[], item_node *theTree) {
	tree_name_node *new_tree_name_node = malloc(sizeof(tree_name_node));
	strcpy(new_tree_name_node->treeName, treeName);
	new_tree_name_node->left = NULL;
	new_tree_name_node->right = NULL;
	new_tree_name_node->theTree = theTree;

	//Return the Created Tree Name Node
	return new_tree_name_node;
}

/*
 * This section includes the functions for destroying the various
 * objects used in the program
 */

// This Function will Destroy a Item Node that was Constructed
void dispose_item_node(item_node *del_item_node) {
}

// This Function will Destroy a Tree Name Node that was Constructed
void dispose_tree_name_node(tree_name_node *del_tree_name_node) {
}
