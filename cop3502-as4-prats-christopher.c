#include <stdio.h>
#include <stdlib.h>

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

// Constructor Prototypes

// Destructor Prototypes

// The Main Function
int main() {
	//Set the Input File to ifp and the Output File to ofp
	FILE *ifp = fopen("cop3502-as4-input.txt", "r");
	FILE *ofp = fopen("cop3502-as4-output-prats-christopher.txt", "w");

	//Close the Input and Output Files Prior to Exit
	fclose(ifp);
	fclose(ofp);


	return 0;
}
