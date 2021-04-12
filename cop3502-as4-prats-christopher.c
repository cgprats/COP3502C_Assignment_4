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
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length); //Get the Next Nonblank Line from a File
void initialize_tree(FILE *ifp, tree_name_node *tree, int *num_commands); //Initialize the Tree from an Input File
void add_tree_node_child(tree_name_node *parent, tree_name_node *child); //Add a Child to a Tree Node Tree
void add_item_node_child(item_node *parent, item_node *child); //Add a Child to a Item Node Tree without the Tree Node Tree
void add_item_node_child_to_tree(tree_name_node *tree, item_node *child, char parent_tree[]); //Add a Child to a Item Node Tree with the Tree Node Tree
tree_name_node *search_for_name_node(tree_name_node *tree, char treeName[]); //Search for a Tree Name Node
item_node *search_in_name_node(tree_name_node *tree, char treeName[], char itemNodeName[]); //Search for an Item Node
void traverse_in_order(FILE *ofp, tree_name_node *tree); //Print the Tree in Order
void print_tree_name_nodes(FILE *ofp, tree_name_node *tree); //Print Tree Name Nodes
void print_item_nodes(FILE *ofp, item_node *item); //Print Item Nodes
void print_all_item_nodes(FILE *ofp, tree_name_node *tree); //Print All Item Nodes in a Tree
void execute_commands(FILE *ifp, FILE *ofp, tree_name_node *tree, int num_commands); //Execute Commands from a File
int items_before_item_node(tree_name_node *tree, char treeName[], char itemNodeName[]); //Find Number of Items Before a Node
int item_side_height(item_node *item); //Find the Height of a Specified Side
int item_count(item_node *item); //Find the Item Count in a Tree
item_node *delete_item(item_node *item, char name[]); //Delete an Item from a Tree
item_node *smallest_item(item_node *item); //Find the Smallest Item in a Tree
tree_name_node *delete_tree(tree_name_node *tree, char name[]); //Delete a Tree Name Node
tree_name_node *smallest_tree(tree_name_node *tree); //Find the Smallest Tree

// Constructor Prototypes
item_node *create_item_node(char name[], int count); //Constructor for Item Node
tree_name_node *create_tree_name_node(char treeName[]); //Constructor for Tree Name Node

// Destructor Prototypes
void dispose_item_node(item_node *del_item_node); //Destructor for Item Node
void dispose_item_node_tree(item_node *delete_item_node); //Destructor for Item Node Tree
void dispose_tree_name_node(tree_name_node *del_tree_name_node); //Destructor for Tree Name Node
void dispose_tree_name_node_tree(tree_name_node *del_tree_name_node); //Destructor for Tree Name Node Tree

// The Main Function
int main() {
	//Set the Input File to ifp and the Output File to ofp
	FILE *ifp = fopen("cop3502-as4-input.txt", "r");
	FILE *ofp = fopen("cop3502-as4-output-prats-christopher.txt", "w");

	//Create and Initialize the Tree
	tree_name_node *tree = malloc(sizeof(tree_name_node));
	int num_commands;
	initialize_tree(ifp, tree, &num_commands);

	//Print the Tree
	traverse_in_order(ofp, tree);

	//Execute Commands
	execute_commands(ifp, ofp, tree, num_commands);

	//Free the Tree
	dispose_tree_name_node_tree(tree);

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
void initialize_tree(FILE *ifp, tree_name_node *tree, int *num_commands) {
	//Get the Item Counts from the File
	int num_tree_nodes, num_item_nodes;
	char buf[64];
	get_next_nonblank_line(ifp, buf, 63);
	remove_crlf(buf);
	sscanf(buf, "%d %d %d", &num_tree_nodes, &num_item_nodes, num_commands);

	//Create the Tree Nodes
	tree_name_node *tempTreeNode;
	for (int i = 0; i < num_tree_nodes; i++) {
		//Read the Node Name
		get_next_nonblank_line(ifp, buf, 63);
		remove_crlf(buf);

		//If the Item is the Head of the Tree
		if (i == 0) {
			tree->left = NULL;
			tree->right = NULL;
			tree->theTree = NULL;
			strcpy(tree->treeName, buf);
		}

		else {
			tempTreeNode = create_tree_name_node(buf);
			add_tree_node_child(tree, tempTreeNode);
		}
	}

	//Create the Item Nodes
	item_node *tempItemNode;
	char parent_tree[64];
	char name[32];
	int count;
	for (int i = 0; i < num_item_nodes; i++) {
		//Read the Node Data
		get_next_nonblank_line(ifp, buf, 63);
		remove_crlf(buf);
		sscanf(buf, "%s %s %d", parent_tree, name, &count);

		//Set the Node to be Added
		tempItemNode = create_item_node(name, count);

		//Add the Node
		add_item_node_child_to_tree(tree, tempItemNode, parent_tree);
	}
}

// This Function Adds a New Child Node to a Tree Node Tree
void add_tree_node_child(tree_name_node *parent, tree_name_node *child) {
	//If Child is Greater than the Parent, Add it to the Right
	if (strcmp(child->treeName, parent->treeName) > 0) {
		//Add it to the Right if it is Available
		if (parent->right == NULL) {
			parent->right = child;
		}

		//Otherwise Attempt the Process with the Parent's Right Node
		else {
			add_tree_node_child(parent->right, child);
		}
	}

	//Otherwise, Add it to the Left
	else {
		//Add it to the Left if it is Available
		if (parent->left == NULL) {
			parent->left = child;
		}

		//Otherwise Attempt the Process with the Parent's Left Node
		else {
			add_tree_node_child(parent->left, child);
		}
	}
}

// This Function Adds a New Child Node to an Item Node Tree
void add_item_node_child(item_node *parent, item_node *child) {
	//If Child is Greater than the Parent, Add it to the Right
	if (strcmp(child->name, parent->name) > 0) {
		//Add it to the Right if it is Available
		if (parent->right == NULL) {
			parent->right = child;
		}

		//Otherwise Attempt the Process with the Parent's Right Node
		else {
			add_item_node_child(parent->right, child);
		}
	}

	//Otherwise, Add it to the Left
	else {
		//Add it to the Left if it is Available
		if (parent->left == NULL) {
			parent->left = child;
		}

		//Otherwise Attempt the Process with the Parent's Left Node
		else {
			add_item_node_child(parent->left, child);
		}
	}
}

// This Function Adds a New Child Node to an Item Node Tree from a Tree Node Tree
void add_item_node_child_to_tree(tree_name_node *tree, item_node *child, char parent_tree[]) {
	//Find the Tree Node to Add the Child to
	tree = search_for_name_node(tree, parent_tree);

	//Exit the Function if the Parent Tree Does Not Exist
	if (tree == NULL) {
		return;
	}

	//Add the Child as a Head if the Tree Lacks an Item Node Tree
	else if (tree->theTree == NULL) {
		tree->theTree = child;
	}

	//Add the Child as a Child of the Item Node Tree
	else {
		add_item_node_child(tree->theTree, child);
	}
}

// This Function Finds and Returns a Tree Name Node
tree_name_node *search_for_name_node(tree_name_node *tree, char treeName[]) {
	//Set the Default Value of foundNode to Null if a Node is Not Found
	tree_name_node *foundNode = NULL;

	//Set the Starting Node to the Top of the Tree
	tree_name_node *currentNode = tree;

	//Search for the Node
	int does_not_match;
	do {
		//Compare the Strings
		does_not_match = strcmp(currentNode->treeName, treeName);

		//If the Node Matches, set the Found Node
		if (!does_not_match) {
			foundNode = currentNode;
		}

		//If the Current Node is Larger than the Node We're Looking For, Check the Node to the Left
		else if (does_not_match > 0) {
			currentNode = currentNode->left;
		}

		//If the Current Node is Smaller than the Node We're Looking For, Check the Node to the Right
		else if (does_not_match < 0) {
			currentNode = currentNode->right;
		}
	} while (does_not_match && currentNode != NULL);

	//Return the Found Node
	return foundNode;
}

// This Function Finds and Returns an Item Node
item_node *search_in_name_node(tree_name_node *tree, char treeName[], char itemNodeName[]) {
	//Set the Default Value of foundNode to Null if a Node is Not Found
	item_node *foundNode = NULL;

	//Find the Parent Tree Name Node
	tree = search_for_name_node(tree, treeName);

	//Start the Search if the Parent Tree Name Node Exists
	if (tree != NULL) {
		//Set the Starting Node to the Head of the Item Node Tree
		item_node *currentNode = tree->theTree;

		//Search for the Node
		int does_not_match;
		do {
			//Compare the Strings
			does_not_match = strcmp(currentNode->name, itemNodeName);

			//If the Node Matches, set the Found Node
			if (!does_not_match) {
				foundNode = currentNode;
			}

			//If the Current Node is Larger than the Node We're Looking For, Check the Node to the Left
			else if (does_not_match > 0) {
				currentNode = currentNode->left;
			}

			//If the Current Node is Smaller than the Node We're Lookdng For, Check the Node to the Right
			else if (does_not_match < 0) {
				currentNode = currentNode->right;
			}
		} while (does_not_match && currentNode != NULL);
	}

	//Return the Found Node
	return foundNode;
}

// This Function Prints the Tree in Order
void traverse_in_order(FILE *ofp, tree_name_node *tree) {
	//Print the Tree Name Nodes
	print_tree_name_nodes(ofp, tree);
	fprintf(ofp, "\n");

	//Print All Item Nodes
	print_all_item_nodes(ofp, tree);
}

//This Function Prints the Tree Name Nodes
void print_tree_name_nodes(FILE *ofp, tree_name_node *tree) {
	//Only Attempt to Print if the Tree Node Exists
	if (tree != NULL) {
		//Recursively Call the Function on the Left Node
		print_tree_name_nodes(ofp, tree->left);

		//Print the Current Tree Node
		fprintf(ofp, "%s ", tree->treeName);

		//Recursively Call the Function on the Right Node
		print_tree_name_nodes(ofp, tree->right);
	}
}

// This Function Prints the Item Name Nodes
void print_item_nodes(FILE *ofp, item_node *item) {
	//Only Attempt to Print if the Item Node Exists
	if (item != NULL) {
		//Recursively Call the Function on the Left Node
		print_item_nodes(ofp, item->left);

		//Print the Current Item Node
		fprintf(ofp, "%s ", item->name);

		//Recursively Call the Function on the Right Node
		print_item_nodes(ofp, item->right);
	}
}

// This Function Prints All Item Nodes in a Tree
void print_all_item_nodes(FILE *ofp, tree_name_node *tree) {
	//Only Attempt to Print if the Tree Exists
	if (tree != NULL) {
		//Recursively Call the Function on the Left Node
		print_all_item_nodes(ofp, tree->left);

		//Print the Current Tree Name
		fprintf(ofp, "===%s===\n", tree->treeName);

		//Print All Member Item Nodes
		print_item_nodes(ofp,tree->theTree);
		fprintf(ofp, "\n");

		//Recursively Call the Function on the Right Node
		print_all_item_nodes(ofp, tree->right);
	}
}

// This Function Executes Commands from a File
void execute_commands(FILE *ifp, FILE *ofp, tree_name_node *tree, int num_commands) {
	//Create Initial Variables
	char buf[64], command[64], tree_name[32], item_name[32];

	//Run the Specified Number of Commands
	fprintf(ofp, "=====Processing Commands=====\n");
	for (int i = 0; i < num_commands; i++) {
		//Read the Command
		get_next_nonblank_line(ifp, buf, 63);
		remove_crlf(buf);

		//Read Into Variables
		sscanf(buf, "%s %s %s", command, tree_name, item_name);

		//Perform Search
		if (!strcmp(command, "search")) {
			tree_name_node *foundTree = search_for_name_node(tree, tree_name);

			//If the Parent Tree was Found, Search for the Item
			if (foundTree != NULL) {
				item_node *foundItem = search_in_name_node(tree, tree_name, item_name);
				//If the Item Exists, Print its Information
				if (foundItem != NULL) {
					item_node *foundItem = search_in_name_node(tree, tree_name, item_name);
					fprintf(ofp, "%d %s found in %s\n", foundItem->count, foundItem->name, tree_name);
				}

				//If the Item Does Not Exist, Print a Message
				else {
					fprintf(ofp, "%s not found in %s\n", item_name, tree_name);
				}
			}

			//If the Parent Tree Does Not Exist, Print a Message
			else {
				fprintf(ofp, "%s does not exist\n", tree_name);
			}
		}

		//Find Items Before
		else if (!strcmp(command, "item_before")) {
			fprintf(ofp, "item before %s: %d\n", item_name, items_before_item_node(tree, tree_name, item_name));
		}

		//Balance Tree Height
		else if (!strcmp(command, "height_balance")) {
			tree_name_node *balance_tree = search_for_name_node(tree, tree_name);

			//Only Perform Operation if the Tree Exists
			if (balance_tree != NULL) {
				int left_height = item_side_height(balance_tree->theTree->left);
				int right_height = item_side_height(balance_tree->theTree->right);
				int difference = right_height - left_height;

				//If There is No Difference, the Tree is Balanced
				if (!difference) {
					fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced\n",
							tree_name, left_height, right_height, difference);
				}

				//Otherwise, it is Unbalanced
				else {
					fprintf(ofp, "%s: left height %d, right height %d, difference %d, not balanced\n",
							tree_name, left_height, right_height, difference);
				}
			}
		}

		//Count Items in a Tree Node
		else if (!strcmp(command, "count")) {
			//Find Tree to Count In
			tree_name_node *count_tree = search_for_name_node(tree, tree_name);

			//Perform Count if the Tree Exists
			if (count_tree != NULL) {
				fprintf(ofp, "%s count %d\n", tree_name, item_count(count_tree->theTree));
			}

			//Otherwise, Print a Message
			else {
				fprintf(ofp, "%s does not exist\n", tree_name);
			}
		}

		//Delete Item
		else if (!strcmp(command, "delete")) {
			tree_name_node *delete_from_tree = search_for_name_node(tree, tree_name);

			//Perform Deletion if the Tree Exists
			if (delete_from_tree != NULL) {
				delete_item(delete_from_tree->theTree, item_name);
				fprintf(ofp, "%s deleted from %s\n", item_name, tree_name);
			}

			//Otherwise, Print a Message
			else {
				fprintf(ofp, "%s does not exist\n", tree_name);
			}
		}

		//Delete Tree
		else if (!strcmp(command, "delete_tree")) {
			delete_tree(tree, tree_name);
			fprintf(ofp, "%s deleted\n", tree_name);
		}
	}
}

// This Command Finds the Number of Items Before Reaching an Item Node
int items_before_item_node(tree_name_node *tree, char treeName[], char itemNodeName[]) {
	//Create Initial Variables
	int items_before = 0;
	tree_name_node *currentTreeNode = tree;
	item_node *currentItemNode = NULL;
	int does_not_match;

	//Search for Parent Tree
	do {
		//Compare the Strings
		does_not_match = strcmp(currentTreeNode->treeName, treeName);

		//If the Current Node is Larger than the Node We're Looking For, Check the Node to the Left
		if (does_not_match > 0) {
			currentTreeNode = currentTreeNode->left;
		}

		//If the Current Node is Smaller than the Node We're Looking For, Check the Node to the Right
		else if (does_not_match < 0) {
			currentTreeNode = currentTreeNode->right;
		}

		//Increase the Number of Items Before on Each Run
		items_before++;
	} while (does_not_match && currentTreeNode != NULL);

	//Search for the Item Node
	if (currentTreeNode != NULL) {
		currentItemNode = currentTreeNode->theTree;
		do {
			//Compare the Strings
			does_not_match = strcmp(currentItemNode->name, itemNodeName);

			//If the Current Node is Larger than the Node We're Looking For, Check the Node to the Left
			if (does_not_match > 0) {
				currentItemNode = currentItemNode->left;
				items_before++;
			}

			//If the Current Node is Smaller than the Node We're Lookdng For, Check the Node to the Right
			else if (does_not_match < 0) {
				currentItemNode = currentItemNode->right;
				items_before++;
			}
		} while (does_not_match && currentItemNode != NULL);
	}


	//Return the Number of Items
	return items_before;
}

// This Function Finds and Returns the Height of a Specified Side of an Item Node Tree from a Tree Node Tree
int item_side_height(item_node *item) {
	//Create Initial Variables
	int height = 0;

	//If Item is NULL, Height is Decreased by -1
	if (item == NULL) {
		height--;
	}

	//If Item is Not NULL, Increase the Height by 1 and Continue Traveling Down the Tree
	else {
		//Increase the Height by 1
		height++;

		//If the Item to the Right Exists, Recursively Call the Function on It
		if (item->right != NULL) {
			height += item_side_height(item->right);
		}

		//Otherwise, Recursively Call the Function on the Left
		else {
			height += item_side_height(item->left);
		}
	}

	//Return the Calculated Height
	return height;
}

// This Function Finds and Returns the Number of Items in a Tree
int item_count(item_node *item) {
	//Set the Number of Items to 0, if Item is NULL
	int num_items = 0;

	if (item != NULL) {
		//Recursively Call the Function on the Left Node
		num_items += item_count(item->left);

		//Increment the Number of Items by the Count in the Current Item
		num_items += item->count;

		//Recursively Call the Function on the Right Node
		num_items += item_count(item->right);
	}

	//Return Calculated Number of Items
	return num_items;
}

// This Function Deletes an Item from an Item Tree
item_node *delete_item(item_node *item, char name[]) {
	//Only Perform Action if the Item is Not Null
	if (item != NULL) {
		//Compare Name with the Item's Name
		int comparison = strcmp(name, item->name);

		//If the Name is Greater than the Item's Name, Set item->right by Recursively Calling the Function
		if (comparison > 0) {
			item->right = delete_item(item->right, name);
		}

		//If the Name is Smaller than the Item's Name, Set item->left by Recursively Calling the Function
		else if (comparison < 0) {
			item->left = delete_item(item->left, name);
		}

		//If the Name Matches, Perform Deletion
		else {
			//Create Temporary Variables to Store Deleted Data
			item_node *temp_item;

			//Set the Return Value if there is No Left Child
			if (item->left == NULL) {
				temp_item = item->right;
				dispose_item_node(item);
				item = temp_item;
			}

			//Set the Return Value if there is No Right Child
			else if (item->right == NULL) {
				temp_item = item->left;
				dispose_item_node(item);
				item = temp_item;
			}

			//Both Children are Present
			else {
				temp_item = smallest_item(item->right);
				item->count = temp_item->count;
				strcpy(item->name, temp_item->name);
				item->right = delete_item(item->right, temp_item->name);
			}
		}
	}

	//Return the Current Item
	return item;
}

// This Function Finds and Returns the Smallest Item in a Tree
item_node *smallest_item(item_node *item) {
	//Start from the Passed Item
	item_node *smallest_item_node = item;

	//Traverse While There is an Item to the Left
	while (smallest_item_node != NULL && smallest_item_node->left != NULL) {
		smallest_item_node = smallest_item_node->left;
	}

	//Return the Smallest Item Node
	return smallest_item_node;
}

// This Function Deletes a Tree Name Node
tree_name_node *delete_tree(tree_name_node *tree, char name[]) {
	//Only Perform Action if the Tree is Not Null
	if (tree != NULL) {
		//Compare Name with the Tree's Name
		int comparison = strcmp(name, tree->treeName);

		//If the Name is Greater than the Tree's Name, Set tree->right by Recursively Calling the Function
		if (comparison > 0) {
			tree->right = delete_tree(tree->right, name);
		}

		//If the Name is Smaller than the Tree's Name, Set tree->left by Recursively Calling the Function
		else if (comparison < 0) {
			tree->left = delete_tree(tree->left, name);
		}

		//If the Name Matches, Perform Deletion
		else {
			//Create Temporary Variables to Store Deleted Data
			tree_name_node *temp_tree;

			//Set the Return Value if there is No Left Child
			if (tree->left == NULL) {
				temp_tree = tree->right;
				dispose_tree_name_node(tree);
				tree = temp_tree;
			}

			//Set the Return Value if there is No Right Child
			else if (tree->right == NULL) {
				temp_tree = tree->left;
				dispose_tree_name_node(tree);
				tree = temp_tree;
			}

			//Both Children are Present
			else {
				temp_tree = smallest_tree(tree->right);
				strcpy(tree->treeName, temp_tree->treeName);
				tree->right = delete_tree(tree->right, temp_tree->treeName);
			}
		}
	}

	//Return the Current Tree
	return tree;
}

// This Function Finds and Returns the Smallest Tree
tree_name_node *smallest_tree(tree_name_node *tree) {
	//Start from the Passed Item
	tree_name_node *smallest_tree_node = tree;

	//Traverse While There is a Tree to the Left
	while (smallest_tree_node != NULL && smallest_tree_node->left != NULL) {
		smallest_tree_node = smallest_tree_node->left;
	}

	//Return the Smallest Tree Node
	return smallest_tree_node;
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
tree_name_node *create_tree_name_node(char treeName[]) {
	tree_name_node *new_tree_name_node = malloc(sizeof(tree_name_node));
	strcpy(new_tree_name_node->treeName, treeName);
	new_tree_name_node->left = NULL;
	new_tree_name_node->right = NULL;
	new_tree_name_node->theTree = NULL;

	//Return the Created Tree Name Node
	return new_tree_name_node;
}

/*
 * This section includes the functions for destroying the various
 * objects used in the program
 */

// This Function will Destroy a Item Node that was Constructed
void dispose_item_node(item_node *del_item_node) {
	free(del_item_node);
}

// This Function will Destroy a Item Node Tree that was Constructed
void dispose_item_node_tree(item_node *delete_item_node) {
	//Dispose the Left Children
	if (delete_item_node->left != NULL) {
		dispose_item_node_tree(delete_item_node->left);
	}

	//Dispose the Right Children
	if (delete_item_node->right != NULL) {
		dispose_item_node_tree(delete_item_node->right);
	}

	//Dispose the Current Item Node
	dispose_item_node(delete_item_node);
}

// This Function will Destroy a Tree Name Node that was Constructed
void dispose_tree_name_node(tree_name_node *del_tree_name_node) {
	dispose_item_node_tree(del_tree_name_node->theTree);
	free(del_tree_name_node);
}

// This Function will Destroy a Tree Name Node Tree that was Constructed
void dispose_tree_name_node_tree(tree_name_node *del_tree_name_node) {
	//Dispose Left Children
	if (del_tree_name_node->left != NULL) {
		dispose_tree_name_node_tree(del_tree_name_node->left);
	}

	//Dispose Right Children
	if (del_tree_name_node->right != NULL) {
		dispose_tree_name_node_tree(del_tree_name_node->right);
	}

	//Dispose the Current Item Node
	dispose_tree_name_node(del_tree_name_node);
}

