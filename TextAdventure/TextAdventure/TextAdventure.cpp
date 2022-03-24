#include <iostream>	
#include <string>
#include <vector>
#include <fstream>

//Handles issues of case sensitivity 
#include <cctype>

using namespace std;

//Used to equate numbers to directions, rooms and verbs
enum Directions { NORTH, EAST, SOUTH, WEST };

//Inventory and the rooms following are pseudo-rooms, and therefore don't have any actual values as they are just containers used to store nouns
enum Rooms { LIVINGROOM, KITCHEN, PANTRY, CELLAR, STOREROOM, INVENTORY, CELLAR_FREEZER_CONTENT, KITCHEN_CUPBOARD_CONTENT, KITCHEN_FRIDGE_CONTENT };
enum Verbs { LOOK, TAKE, DROP, OPEN, CLOSE, PET };
enum Nouns { CELLAR_DOOR, STOREROOM_DOOR, PANTRY_DOOR, EGGS, BACON, JUICE, SAUCEPAN, DOG, CELLAR_FREEZER, KITCHEN_CUPBOARD, KITCHEN_FRIDGE };

//Used to identify if a room has an exit
const int NONE = -1;
const int DIRECTIONS = 4;
const int ROOMS = 9;
const int VERBS = 6;
const int NOUNS = 11;

//These booleans hold the state of all of the doors/containers 
static bool cellarDoorState = false;
static bool storeRoomDoorState = false;
static bool pantryDoorState = false;
static bool cellarFreezerState = false;
static bool kitchenCupboardState = false;
static bool kitchenFridgeState = false;

string introDescription = "\nAs you slowly wake up from your customary mid-afternoon nap on the living room couch, \nyou hear what sounds like the local forecast coming from the direction of the TV. \nYou look down at the floor to find your dog Marco asleep on his back, snoring. \nIt seems as though he too has adopted your tradition. \nLaughing softly, you suddenly feel your stomach rumble. \nYou know all too well what follows an afternoon nap, a hearty afternoon lunch! \nYou sit upright, and push off the couch with your arms, coming to a stand.";
string livingRoomDescription = "\nYou are in your Living Room, which is somewhat of a mess. \nAt it's center is a large exhausted looking couch littered with various food stains. \nOn the floor near the couch is Marco, still sound asleep. \nHis brown matted hair undulates slightly as he breathes in and out. \nOpposite the couch is the TV, propped up on some old delivery boxes, the news forecast is still ongoing.\n";
string kitchenDescription = "\nYou find yourself in your Kitchen, which is in a much worse state than that of your Living Room. \nIn the center of the Kitchen is an island, whose sink is largely dominated by a mass of plates and food scraps. \nOff to the side is a small white single door fridge. \nBehind the Kitchen island is a single raised cupboard that spans the length of the Kitchen.\n";
string pantryDescription = "\nYou are in your Pantry, which isn't much of a sight to behold, \nits shelves are scarce with a few canned foods here and there. \nIn the corner of the pantry is an open carton of eggs, of which 4 of the 6 are missing.\n";
string cellarDescription = "\nYou are in your Cellar. \nIts walls are whitewashed brick, though you can pick out the scattered specks of red, \nthat hide underneath the thick semi-translucent white coating. \nIt's rather cold, you feel the coming of goose pimples on your arms, and shiver every so slightly. \nIn the center of the Cellar is a large white freezer, that's plugged into a wall outlet.\n";
string storeRoomDescription = "\nYou are in your Storeroom. \nIt's full to the brim with random miscellaneous items, they scatter nearby shelving units, \nand have begun making their way to the group. \nYou're not entirely sure if you could find anything in it, even if you wanted to.\n";
string outroDescription = "\nYou turn on the stove, grab a knob of butter from the fridge, \ntoss it into your saucepan which you then place on the open flame of the stove. \nYou grab all five strips of bacon and toss them in alongside the butter. \nThey begin to sizzle, and you wait until they're cooked, turning them over every so often. \nYou grab a plate from the cupboard and leave the bacon on it to rest. \nYou then take hold of the eggs, gently break each of them, and add them into the saucepan. \nYou wait until they're cooked to your liking, and then scoop them unto your plate. \nGrabbing your plate you head into your Living room with an orange juice carton tucked under your arm. \nYou find Marco awake as you enter, and he stares as you approach, noticing the food you're carrying. \nSighing, you toss Marco one of the strips of bacon, he catches it in the air, and begin munching on it happily. \nYou jump onto the couch, and dig into your afternoon lunch.\n";

struct word {
	string word;
	int code;
};

struct room {
	string description;
	string longDescription;
	//Rooms that another room is connect to, i.e. rooms the player can travel to from a current one
	int connectionsToRooms[DIRECTIONS];
};

struct noun {
	string word;
	string description;
	int code;
	int location;
	bool canCarry;
};

void setRooms(room* rms) {

	rms[LIVINGROOM].description.assign("Living room");
	rms[LIVINGROOM].longDescription.assign(livingRoomDescription);
	rms[LIVINGROOM].connectionsToRooms[NORTH] = NONE;
	rms[LIVINGROOM].connectionsToRooms[EAST] = KITCHEN;
	rms[LIVINGROOM].connectionsToRooms[SOUTH] = NONE;
	rms[LIVINGROOM].connectionsToRooms[WEST] = NONE;

	rms[KITCHEN].description.assign("Kitchen");
	rms[KITCHEN].longDescription.assign(kitchenDescription);
	rms[KITCHEN].connectionsToRooms[NORTH] = NONE;
	rms[KITCHEN].connectionsToRooms[EAST] = NONE;
	rms[KITCHEN].connectionsToRooms[SOUTH] = NONE;
	rms[KITCHEN].connectionsToRooms[WEST] = LIVINGROOM;

	rms[PANTRY].description.assign("Pantry");
	rms[PANTRY].longDescription.assign(pantryDescription);
	rms[PANTRY].connectionsToRooms[NORTH] = NONE;
	rms[PANTRY].connectionsToRooms[EAST] = NONE;
	rms[PANTRY].connectionsToRooms[SOUTH] = NONE;
	rms[PANTRY].connectionsToRooms[WEST] = NONE;

	rms[CELLAR].description.assign("Cellar");
	rms[CELLAR].longDescription.assign(cellarDescription);
	rms[CELLAR].connectionsToRooms[NORTH] = NONE;
	rms[CELLAR].connectionsToRooms[EAST] = NONE;
	rms[CELLAR].connectionsToRooms[SOUTH] = NONE;
	rms[CELLAR].connectionsToRooms[WEST] = NONE;

	rms[STOREROOM].description.assign("Storeroom");
	rms[STOREROOM].longDescription.assign(storeRoomDescription);
	rms[STOREROOM].connectionsToRooms[NORTH] = NONE;
	rms[STOREROOM].connectionsToRooms[EAST] = NONE;
	rms[STOREROOM].connectionsToRooms[SOUTH] = NONE;
	rms[STOREROOM].connectionsToRooms[WEST] = NONE;

}

void setDirections(word* dir) {
	dir[NORTH].code = NORTH;
	dir[NORTH].word = "NORTH";

	dir[EAST].code = EAST;
	dir[EAST].word = "EAST";

	dir[SOUTH].code = SOUTH;
	dir[SOUTH].word = "SOUTH";

	dir[WEST].code = WEST;
	dir[WEST].word = "WEST";
}

void setVerbs(word* vbs) {

	vbs[LOOK].code = LOOK;
	vbs[LOOK].word = "LOOK";

	vbs[TAKE].code = TAKE;
	vbs[TAKE].word = "TAKE";

	vbs[DROP].code = DROP;
	vbs[DROP].word = "DROP";

	vbs[OPEN].code = OPEN;
	vbs[OPEN].word = "OPEN";

	vbs[CLOSE].code = CLOSE;
	vbs[CLOSE].word = "CLOSE";

	vbs[PET].code = PET;
	vbs[PET].word = "PET";
}

void setNouns(noun* nns) {

	nns[CELLAR_DOOR].word = "CELLAR";
	nns[CELLAR_DOOR].code = CELLAR_DOOR;
	nns[CELLAR_DOOR].description = "a cellar door";
	nns[CELLAR_DOOR].canCarry = false;
	nns[CELLAR_DOOR].location = LIVINGROOM;

	nns[STOREROOM_DOOR].word = "STOREROOM";
	nns[STOREROOM_DOOR].code = STOREROOM_DOOR;
	nns[STOREROOM_DOOR].description = "a storeroom door";
	nns[STOREROOM_DOOR].canCarry = false;
	nns[STOREROOM_DOOR].location = LIVINGROOM;

	nns[PANTRY_DOOR].word = "PANTRY";
	nns[PANTRY_DOOR].code = PANTRY_DOOR;
	nns[PANTRY_DOOR].description = "a pantry door";
	nns[PANTRY_DOOR].canCarry = false;
	nns[PANTRY_DOOR].location = KITCHEN;

	nns[EGGS].word = "EGGS";
	nns[EGGS].code = EGGS;
	nns[EGGS].description = "two eggs";
	nns[EGGS].canCarry = true;
	nns[EGGS].location = PANTRY;

	nns[BACON].word = "BACON";
	nns[BACON].code = BACON;
	nns[BACON].description = "five strips of bacon";
	nns[BACON].canCarry = true;
	nns[BACON].location = CELLAR_FREEZER_CONTENT;

	nns[JUICE].word = "JUICE";
	nns[JUICE].code = JUICE;
	nns[JUICE].description = "a half-empty carton of orange juice";
	nns[JUICE].canCarry = true;
	nns[JUICE].location = KITCHEN_FRIDGE_CONTENT;

	nns[SAUCEPAN].word = "SAUCEPAN";
	nns[SAUCEPAN].code = SAUCEPAN;
	nns[SAUCEPAN].description = "a stained non-stick saucepan";
	nns[SAUCEPAN].canCarry = true;
	nns[SAUCEPAN].location = KITCHEN_CUPBOARD_CONTENT;

	nns[DOG].word = "DOG";
	nns[DOG].code = DOG;
	nns[DOG].description = "Marco, your dog, who's fast asleep on the floor";
	nns[DOG].canCarry = true;
	nns[DOG].location = LIVINGROOM;

	nns[CELLAR_FREEZER].word = "FREEZER";
	nns[CELLAR_FREEZER].code = CELLAR_FREEZER;
	nns[CELLAR_FREEZER].description = "a large white freezer";
	nns[CELLAR_FREEZER].canCarry = false;
	nns[CELLAR_FREEZER].location = CELLAR;

	nns[KITCHEN_CUPBOARD].word = "CUPBOARD";
	nns[KITCHEN_CUPBOARD].code = KITCHEN_CUPBOARD;
	nns[KITCHEN_CUPBOARD].description = "a long cupboard that spans the length of the kitchen";
	nns[KITCHEN_CUPBOARD].canCarry = false;
	nns[KITCHEN_CUPBOARD].location = KITCHEN;

	nns[KITCHEN_FRIDGE].word = "FRIDGE";
	nns[KITCHEN_FRIDGE].code = KITCHEN_FRIDGE;
	nns[KITCHEN_FRIDGE].description = "a small white single door fridge";
	nns[KITCHEN_FRIDGE].canCarry = false;
	nns[KITCHEN_FRIDGE].location = KITCHEN;

}

void commandSplitter(string cmd, string& fWord, string& sWord) {

	string subString;
	vector<string> words;
	char search = ' ';
	size_t i, j;

	//Splits the command into a vector
	for (i = 0; i < cmd.size(); i++) {

		/*Reads through the length of the command - if there isn't a blank space character
		at the current point, it amends the substring, inserting the current character*/
		if (cmd.at(i) != search) {
			subString.insert(subString.end(), cmd.at(i));
		}

		/*When the end of the command has been reached, it adds the substring
		as an element to the end of the vector, and erases the contents of the substring*/
		if (i == cmd.size() - 1) {
			words.push_back(subString);
			subString.clear();
		}

		/*When there's a blank space character, it adds the substring
		as an element to the end of the vector, and erases the contents of the substring*/
		if (cmd.at(i) == search) {
			words.push_back(subString);
			subString.clear();
		}

	}

	//Clears out any blanks - i.e. in the case that the user accidentally enters two blank space characters between words 
	for (i = words.size() - 1; i > 0; i--) {
		if (words.at(i) == "") {
			words.erase(words.begin() + i);
		}
	}

	for (i = 0; i < words.size(); i++) {
		for (j = 0; j < words.at(i).size(); j++) {
			//Checks if the characters of the word at position 'i' are lowercase
			if (islower(words.at(i).at(j))) {
				//Makes all the characters in the word uppercase
				words.at(i).at(j) = toupper(words.at(i).at(j));
			}
		}
	}

	if (words.size() == 0) {
		cout << "No command given." << endl;
	}
	if (words.size() == 1) {
		fWord = words.at(0);
	}
	if (words.size() == 2) {
		fWord = words.at(0);
		sWord = words.at(1);
	}
	if (words.size() > 2) {
		cout << "Command too long. Only type one or two words (direction or verb and noun)" << endl;
	}
}

void lookAround(int loc, room* rms, word* dir, noun* nns) {
	int i;
	cout << rms[loc].longDescription << endl;

	//Allows the player to see what other rooms are connected to the current one
	for (i = 0; i < DIRECTIONS; i++) {
		if (rms[loc].connectionsToRooms[i] != NONE) {
			cout << "There is corridor " << dir[i].word << " that leads to a " << rms[rms[loc].connectionsToRooms[i]].description << "." << endl;
		}
	}

	//Scans through all the nouns, if they happen to at the same location as the player, then it allows the player to see said noun(s)
	for (i = 0; i < NOUNS; i++) {
		if (nns[i].location == loc) {
			cout << "You see " << nns[i].description << "." << endl;
		}
	}

	//Reports whether the cellar door is open or closed
	if (loc == CELLAR || loc == LIVINGROOM) {
		if (cellarDoorState == false) {
			cout << "The cellar door is closed" << endl;
		}
		else {
			cout << "The cellar door is open" << endl;
		}
	}

	//Reports whether the storeroom door is open or closed
	if (loc == STOREROOM || loc == LIVINGROOM) {
		if (storeRoomDoorState == false) {
			cout << "The storeroom door is closed" << endl;
		}
		else {
			cout << "The storeroom door is open" << endl;
		}
	}

	//Reports whether the pantry door is open or closed
	if (loc == PANTRY || loc == KITCHEN) {
		if (pantryDoorState == false) {
			cout << "The pantry door is closed" << endl;
		}
		else {
			cout << "The pantry door is open" << endl;
		}
	}

	//Reports whether the freezer is open or closed
	if (loc == CELLAR) {
		if (cellarFreezerState == false) {
			cout << "The freezer is closed" << endl;
		}
		else {
			cout << "The freezer is open" << endl;
		}
	}

	//Reports whether the cupboard is open or closed
	if (loc == KITCHEN) {
		if (kitchenCupboardState == false) {
			cout << "The kitchen cupboard is closed" << endl;
		}
		else {
			cout << "The kitchen cupboard is open" << endl;
		}
	}

	//Reports whether the fridge is open or closed
	if (loc == KITCHEN) {
		if (kitchenFridgeState == false) {
			cout << "The fridge is closed" << endl;
		}
		else {
			cout << "The fridge is open" << endl;
		}
	}

}

//The location is passed referenced so that the parser function can permanently modify it
bool parser(int& loc, string fWord, string sWord, word* dir, word* vbs, room* rms, noun* nns) {

	int i;
	for (i = 0; i < DIRECTIONS; i++) {
		if (fWord == dir[i].word) {
			/*Checks if the player's current location is connected to a room in the direction they've entered.
			If it is, then the player's current location is set to that of the room in the direction entered, thereby 'moving' the player to that room.
			The new room's description is printed to the console.*/
			if (rms[loc].connectionsToRooms[dir[i].code] != NONE) {
				loc = rms[loc].connectionsToRooms[dir[i].code];
				cout << "You are in a " << rms[loc].description << "." << endl;
				return true;
			}
			//When there is no room connected to current room in the direction entered
			else {
				cout << "There is no room that way." << endl;
				return true;
			}
		}
	}

	int nounMatch = NONE;
	int verbAction = NONE;

	/*Loops through the list of verbs and compares them to player input
	if a comparison is found 'verbAction' is initialized and the loop is broken out of*/
	for (i = 0; i < VERBS; i++) {
		if (fWord == vbs[i].word) {
			verbAction = vbs[i].code;
			break;
		}
	}

	/*If the player enters a second word the loop scans through the list of nouns,
	if it finds a match 'nounMatch' is initialized and the loop is broken out of*/
	if (sWord != "") {
		for (i = 0; i < NOUNS; i++) {
			if (sWord == nns[i].word) {
				nounMatch = nns[i].code;
				break;
			}
		}
	}

	//Calls the 'lookAround' function if the verb entered is 'look'
	if (verbAction == LOOK) {
		lookAround(loc, rms, dir, nns);
		return true;
	}

	/*If the first word is 'OPEN' and the second is 'CELLAR',
	and the player is in either the 'LIVING ROOM' or the 'CELLAR',
	the Cellar door's state is made true - it's open*/
	if (verbAction == OPEN) {
		if (nounMatch == CELLAR_DOOR) {
			if (loc == CELLAR || loc == LIVINGROOM) {
				if (cellarDoorState == false) {
					cellarDoorState = true;

					//Adds a path in between the living room and the cellar
					rms[LIVINGROOM].connectionsToRooms[SOUTH] = CELLAR;
					rms[CELLAR].connectionsToRooms[NORTH] = LIVINGROOM;

					cout << "I've opened the cellar door." << endl;
					return true;
				}
				else if (cellarDoorState == true) {
					cout << "The cellar door is already open." << endl;
					return true;
				}
			}
			else {
				cout << "There is no door to open here." << endl;
				return true;
			}
		}

		/*If the first word is 'OPEN' and the second is 'STOREROOM',
		and the player is in either the 'LIVING ROOM' or the 'STOREROOM',
		the Storeroom door's state is made true - it's open*/
		else if (nounMatch == STOREROOM_DOOR) {
			if (loc == STOREROOM || loc == LIVINGROOM) {
				if (storeRoomDoorState == false) {
					storeRoomDoorState = true;

					//Adds a path in between the living room and the store room
					rms[LIVINGROOM].connectionsToRooms[WEST] = STOREROOM;
					rms[STOREROOM].connectionsToRooms[EAST] = LIVINGROOM;

					cout << "You've opened the storeroom door." << endl;
					return true;
				}
				else if (storeRoomDoorState == true) {
					cout << "The store room door is already open." << endl;
					return true;
				}
			}
			else {
				cout << "There is no door to open here." << endl;
				return true;
			}
		}

		/*If the first word is 'OPEN' and the second is 'PANTRY',
		and the player is in either the 'KITCHEN' or the 'PANTRY',
		the pantry door's state is made true - it's open*/
		else if (nounMatch == PANTRY_DOOR) {
			if (loc == PANTRY || loc == KITCHEN) {
				if (pantryDoorState == false) {
					pantryDoorState = true;

					//Adds a path in between the kitchen and the pantry
					rms[KITCHEN].connectionsToRooms[SOUTH] = PANTRY;
					rms[PANTRY].connectionsToRooms[NORTH] = KITCHEN;

					cout << "You've opened the pantry door." << endl;
					return true;
				}
				else if (pantryDoorState == true) {
					cout << "The pantry door is already open." << endl;
					return true;
				}
			}
			else {
				cout << "There is no door to open here." << endl;
				return true;
			}
		}

		/*If the first word is 'OPEN' and the second is 'FREEZER',
		and the player is in the 'CELLER' the freezer's state is made true - it's open*/
		else if (nounMatch == CELLAR_FREEZER) {
			if (loc == CELLAR) {
				if (cellarFreezerState == false) {
					cellarFreezerState = true;
					cout << "You've opened the freezer. Inside is some bacon." << endl;
					return true;
				}
				else if (cellarFreezerState == true) {
					cout << "The freezer is already open." << endl;
					return true;
				}
			}
			else {
				cout << "There is no freezer to open here." << endl;
				return true;
			}
		}

		/*If the first word is 'OPEN' and the second is 'CUPBOARD',
		and the player is in the 'KITCHEN' the cupboard's state is made true - it's open*/
		else if (nounMatch == KITCHEN_CUPBOARD) {
			if (loc == KITCHEN) {
				if (kitchenCupboardState == false) {
					kitchenCupboardState = true;
					cout << "You've opened the cupboard. Inside is a saucepan." << endl;
					return true;
				}
				else if (kitchenCupboardState == true) {
					cout << "The cupboard is already open." << endl;
					return true;
				}
			}
			else {
				cout << "There is no cupboard to open here." << endl;
				return true;
			}
		}

		/*If the first word is 'OPEN' and the second is 'FRIDGE',
		and the player is in the 'KITCHEN' the fridge's state is made true - it's open*/
		else if (nounMatch == KITCHEN_FRIDGE) {
			if (loc == KITCHEN) {
				if (kitchenFridgeState == false) {
					kitchenFridgeState = true;
					cout << "You've opened the fridge. Inside is some orange juice." << endl;
					return true;
				}
				else if (kitchenFridgeState == true) {
					cout << "The fridge is already open." << endl;
					return true;
				}
			}
			else {
				cout << "There is fridge to open here." << endl;
				return true;
			}
		}

		else {
			cout << "You can't open that." << endl;
			return true;
		}
	}

	/*If the first word is 'CLOSE' and the second is 'CELLAR',
	and the player is in either the 'LIVING ROOM' or the 'CELLAR',
	the Cellar door's state is made false - it's closed*/
	if (verbAction == CLOSE) {
		if (nounMatch == CELLAR_DOOR) {
			if (loc == CELLAR || loc == LIVINGROOM) {
				if (cellarDoorState == true) {
					cellarDoorState = false;
					//Removes the path in between the living room and the cellar
					rms[LIVINGROOM].connectionsToRooms[SOUTH] = NONE;
					rms[CELLAR].connectionsToRooms[NORTH] = NONE;
					cout << "You've closed the cellar door." << endl;
					return true;
				}
				else if (cellarDoorState == false) {
					cout << "The door is already closed." << endl;
					return true;
				}
			}
			else {
				cout << "There is no door to close here." << endl;
				return true;
			}
		}

		/*If the first word is 'CLOSE' and the second is 'STOREROOM',
		and the player is in either the 'LIVING ROOM' or the 'STOREROOM',
		the storeroom door's state is made false - it's closed*/
		else if (nounMatch == STOREROOM_DOOR) {
			if (loc == STOREROOM || loc == LIVINGROOM) {
				if (storeRoomDoorState == true) {
					storeRoomDoorState = false;
					//Removes the path in between the living room and the storeroom
					rms[LIVINGROOM].connectionsToRooms[WEST] = NONE;
					rms[STOREROOM].connectionsToRooms[EAST] = NONE;
					cout << "You've closed the storeroom door." << endl;
					return true;
				}
				else if (storeRoomDoorState == false) {
					cout << "The door is already closed." << endl;
					return true;
				}
			}
			else {
				cout << "There is no door to close here." << endl;
				return true;
			}
		}
		/*If the first word is 'CLOSE' and the second is 'PANTRY',
		and the player is in either the 'KITCHEN' or the 'PANTRY',
		the pantry door's state is made false - it's closed*/
		else if (nounMatch == PANTRY_DOOR) {
			if (loc == PANTRY || loc == KITCHEN) {
				if (pantryDoorState == true) {
					pantryDoorState = false;
					//Removes the path in between the kitchen and the pantry
					rms[KITCHEN].connectionsToRooms[SOUTH] = NONE;
					rms[PANTRY].connectionsToRooms[NORTH] = NONE;
					cout << "You've closed the pantry door." << endl;
					return true;
				}
				else if (pantryDoorState == false) {
					cout << "The door is already closed." << endl;
					return true;
				}
			}
			else {
				cout << "There is no door to close here." << endl;
				return true;
			}
		}

		/*If the first word is 'CLOSE' and the second is 'FREEZER',
		and the player is in the 'CELLER' the freezer's state is made false - it's closed*/
		else if (nounMatch == CELLAR_FREEZER) {
			if (loc == CELLAR) {
				if (cellarFreezerState == true) {
					cellarFreezerState = false;
					cout << "You've closed the freezer." << endl;
					return true;
				}
				else if (cellarFreezerState == false) {
					cout << "The freezer is already closed." << endl;
					return true;
				}
			}
			else {
				cout << "There is no freezer to close here." << endl;
				return true;
			}
		}

		/*If the first word is 'CLOSE' and the second is 'CUPBOARD',
		and the player is in the 'KITCHEN' the cupboard's state is made false - it's closed*/
		else if (nounMatch == KITCHEN_CUPBOARD) {
			if (loc == KITCHEN) {
				if (kitchenCupboardState == true) {
					kitchenCupboardState = false;
					cout << "You've closed the cupboard." << endl;
					return true;
				}
				else if (kitchenCupboardState == false) {
					cout << "The cupboard is already closed." << endl;
					return true;
				}
			}
			else {
				cout << "There is no cupboard to close here." << endl;
				return true;
			}
		}

		/*If the first word is 'CLOSE' and the second is 'FRIDGE',
		and the player is in the 'KITCHEN' the fridge's state is made false - it's closed*/
		else if (nounMatch == KITCHEN_FRIDGE) {
			if (loc == KITCHEN) {
				if (kitchenFridgeState == true) {
					kitchenFridgeState = false;
					cout << "You've closed the fridge." << endl;
					return true;
				}
				else if (kitchenFridgeState == false) {
					cout << "The fridge is already closed." << endl;
					return true;
				}
			}
			else {
				cout << "There is no fridge to close here." << endl;
				return true;
			}
		}


		else {
			cout << "Closing that is not possible." << endl;
			return true;
		}
	}

	//If the player is in a room and takes an object in that room, that object is assigned to the location of the inventory
	if (verbAction == TAKE) {
		if (nounMatch == EGGS) {
			if (loc == PANTRY) {
				nns[EGGS].location = INVENTORY;
				cout << "You take the eggs." << endl;
			}
		}
		else if (nounMatch == BACON) {
			//You can only take the item if you're in the room it's in, and the container its in is open
			if (loc == CELLAR && cellarFreezerState == true) {
				nns[BACON].location = INVENTORY;
				cout << "You take the bacon." << endl;
			}
			else {
				cout << "You can't take that." << endl;;
			}
		}
		else if (nounMatch == JUICE) {
			//You can only take the item if you're in the room it's in, and the container its in is open
			if (loc == KITCHEN && kitchenFridgeState == true) {
				nns[JUICE].location = INVENTORY;
				cout << "You take the orange juice." << endl;
			}
			else {
				cout << "You can't take that." << endl;
			}
		}
		else if (nounMatch == SAUCEPAN) {
			//You can only take the item if you're in the room it's in, and the container its in is open
			if (loc == KITCHEN && kitchenCupboardState == true) {
				nns[SAUCEPAN].location = INVENTORY;
				cout << "You take the saucepan." << endl;
			}
			else {
				cout << "You can't take that." << endl;
			}
		}
		else {
			cout << "You can't take that." << endl;
		}
	}

	//If the player drops an object then it's removed from the inventory, and is assigned the current location - i.e. stays were it was dropped
	if (verbAction == DROP) {
		if (nounMatch == EGGS) {
			nns[EGGS].location = loc;
			cout << "You place the eggs down." << endl;
		}
		else if (nounMatch == BACON) {
			nns[BACON].location = loc;
			cout << "You place the bacon down." << endl;
		}
		else if (nounMatch == JUICE) {
			nns[JUICE].location = loc;
			cout << "You place the orange juice down." << endl;
		}
		else if (nounMatch == SAUCEPAN) {
			nns[SAUCEPAN].location = loc;
			cout << "You place the saucepan down." << endl;
		}
		else {
			cout << "You can't place that." << endl;
		}
	}

	//Allows the player to pet the dog, if they're in the same room as it 
	if (verbAction == PET) {
		if (nounMatch == DOG) {
			if (loc == LIVINGROOM) {
				cout << "\nYou reach over to Marco, and lightly pet the base of his neck. \nHis thick fur hugs your hand, and you feel his back rise and fall, as he breathes in and out while asleep." << endl;
			}
		}
		else {
			cout << "You can't pet that." << endl;
		}
	}

	if (loc == KITCHEN) {
		if (fWord == "MAKE" && sWord == "FOOD") {

			if (nns[EGGS].location == INVENTORY && nns[BACON].location == INVENTORY && nns[SAUCEPAN].location == INVENTORY && nns[JUICE].location == INVENTORY) {
				cout << outroDescription << endl;
				//Terminates the program
				exit(EXIT_SUCCESS);
			}
			else {
				cout << "You don't have all the ingredients to make afternoon lunch";
			}
		}
	}

	//Prints the following when the player enters an unrecognized verb, direction, or a word that isn't 'QUIT'
	if (verbAction == NONE && fWord != "QUIT") {
		cout << "No valid command entered." << endl;
		return true;
	}

	return false;
}


int main() {

	string command;

	//The max number of words that are interpreted in the input handler
	string firstWord;
	string secondWord;

	room rooms[ROOMS];
	setRooms(rooms);

	word directions[DIRECTIONS];
	setDirections(directions);

	word verbs[VERBS];
	setVerbs(verbs);

	noun nouns[NOUNS];
	setNouns(nouns);

	//Holds the player's current position in the map, the initial value is the start location
	int playerLocation = LIVINGROOM;

	cout << introDescription << endl;

	//Executes while the input condition isn't met
	while (firstWord != "QUIT") {

		//Erases the contents of the string, ensuring the most recent input is being handled 
		command.clear();
		cout << "\nWhat shall you do? ";
		getline(cin, command);

		firstWord.clear();
		secondWord.clear();

		//Calls the input handler function
		commandSplitter(command, firstWord, secondWord);

		//Calls the parser function, which takes inputted commands and interprets them
		parser(playerLocation, firstWord, secondWord, directions, verbs, rooms, nouns);

	}

	return 0;

}
