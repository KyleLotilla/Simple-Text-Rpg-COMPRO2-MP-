#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>

/* Move Data Type */
typedef struct movedata {
	char name[51];
	char description[201];
	int classification; // In this program, move classification is represented by numbers 1-3. 1 - Light, 2- Medium, 3 - Heavy
	int elemental; // In this program, elemental affinity of moves and characters is represented by numbers 1-4, 1 - Water, 2 - Earth, 3- Air, 4 -Fire
} movedata;
/* Move node */
typedef struct movenode{
	movedata move;
	struct movenode *Next;
	struct movenode *Prev;
} movenode;
/* Character Data Type */
typedef struct chardata {
	char name[51];
	char stage[51];
	int elemental;
	int hp;
	int pp;
	/* The moves assigned to a character is stored in an array, n - 1 index represent the classification. For example, index 0 represent light attack (1)*/
	movenode *moveset[3];
} chardata;
/* Character Node */
typedef struct charnode {
	chardata character;
	struct charnode *Next;
	struct charnode *Prev;
} charnode;
/* StoryBattle - This datatype is used to store the order of the opponents faced and which character is an opponent in a storyline */
typedef struct storybattle{
	struct storybattle *Next;
	struct storybattle *Prev;
	charnode *opponent;
	int bDel;
} storybattle;
/* Storyline Data Type */
typedef struct storydata {
    storybattle *battles; // This is the head for the ordered linked list of storybattles
	charnode *protagonist;
	char title[51];
	char description[1001];	
}storydata;
/* Storyline Node */
typedef struct storynode {
	struct storynode *Next;
	struct storynode *Prev;
	storydata story;
} storynode;
/* Combat Data Type - This datatype is used hold most the data needed for calculations in combat */
typedef struct combattype {
	charnode *player;
	charnode *opponent;
	int playerHP; // This is used to store Current HP of the player
	int opponentHP; // This is used to store Current HP of the opponent
	int playerPP; // This is used to store Current PP of the player
	int opponentPP; // This is used to store Current PP of the opponent
	int playerMovePP[3]; // This is used to store Required PP to perform moves for the player
	int opponentMovePP[3]; // This is used to store Required PP to perform moves for the opponent
	int stageNum; // This is used to display how far the player went in a storyline
	int turn; // This determines whether it's the player's or opponent's turn. turn is equal to 0 when the players are in free mode
} combattype;

/* Gameplay Module */
/* Combat Functions */
/* General Combat Functions */
void printCombat (combattype *pCombatData);
void printMoveOutcome (charnode *pAttacker, charnode *pDefender, combattype *pCombatData, int nInput, int nDamage);
/* Damage Calculations */
int getRandNum (int nLowerBound, int nUpperBound);
int calculatePPCost (int nPP, int nInput);
int willMoveHit (int nInput);
int calculateBaseDamage (int nDefenderHP, int nInput);
float getElemMultipler (int nAttackerElem, int nDefenderElem);
int calculateDamage (charnode *pAttacker, charnode *pDefender, combattype *pCombatData, int nInput);
/* Player Turn Functions */
int isMoveValidInCombat (combattype *pCombatData, int nInput);
void playerTurn (combattype *pCombatData);
/* Opponent Turn Function */
void opponentTurn (combattype *pCombatData);
/* Main Combat Function */
int combatMode (charnode *pPlayer, charnode *pOpponent, int nStageNum);

/* Modes Module */
void storylineMode(storynode *pHead);
void freeMode (charnode *pHead);

/* Main Gameplay Module */
void mainGame (storynode *pStoryHead, charnode *pCharHead);


/* Main Config Module */
void mainConfig (storynode **pStoryHead, movenode **pMoveHead, charnode **pCharHead);
/* Char Module */
/* General Char Functions */
void printElem (int elemental);
void printChar (charnode *pChar, char cFormat);
void printMoveset (charnode *pChar, char cFormat);
charnode *selectChar (charnode *pHead);
/* Add Character */
charnode *sortByAlpha(charnode *pHead, charnode *pChar);
charnode *addChar(charnode *pHead);
/* View Characters */
charnode *getFirstElemNode(charnode *pHead, int elemental);
charnode *traverseNextElemNode(charnode *pCurrent);
charnode *traversePrevElemNode(charnode *pCurrent);
void viewChars (charnode *pHead);
/* Modify Character */
charnode *modifyChar(movenode *pMoveHead, charnode *pCharHead);
/* Delete Character */
void markProtagDel (storynode *pStoryHead, charnode *pChar);
void markBattleDel (storynode *pStoryHead, charnode *pChar);
charnode *deleteChar (storynode *pStoryHead, charnode *pCharHead);
/* Fix Characters in Storylines */
charnode *getProFromBattle (storynode *pStory);
void fixDelPro (storynode *pStoryHead, charnode *pCharHead);
void fixDelBattle (storynode *pStoryHead, charnode *pCharHead);
/* Main Story Function */
int isCharListValid (charnode *pHead);
charnode *mainCharModule (movenode *pMoveHead, storynode *pStoryHead, charnode *pCharHead);

/* Storyline Module */

/* Story Battle Functions */
/* Select Battle */
charnode *getFirstCharForBattle (storynode *pStory, charnode *pCharHead);
charnode *traverseNextCharForBattle (storynode *pStory, charnode *pCurrent);
charnode *traversePrevCharForBattle (storynode *pStory, charnode *pCurrent);
charnode *selectCharForBattle (storynode *pStory, charnode *pCharHead);
/* Add Battle */
storybattle *addBattle(storynode *pStory, charnode *pCharHead);
/* Modify Battle */
void switchOpponentsInBattle (storybattle *pBattle1, storybattle *pBattle2);
storybattle *traverseBattleListNumTimes (storybattle *pHead, int nNum);
storybattle *modifyBattle (storynode *pStory, charnode *pCharHead);
/* Delete Battle */
storybattle *deleteBattle (storybattle *pBattleHead, storybattle *pBattle);

/* Storyline functions */
/* General Story Functions */
int isCharInStory (storynode *pStory, charnode *pChar);
void printStory(storynode *pStory, char format);
int countBattle (storybattle *pHead);
storynode *selectStory (storynode *pHead);
/* Add Story */
storynode *addStory(storynode *pStoryHead, charnode *pCharHead);
/* View Story */
storynode *getFirstProtagNode (storynode *pHead, char cProtag[]);
storynode *traverseNextProtagNode (storynode *pCurrent, char cProtag[]);
storynode *traversePrevProtagNode (storynode *pCurrent, char cProtag[]);
void viewStory (storynode *pHead);
/* Modify Story */
storybattle *getBattleOfChar (storybattle *pBattleHead, charnode *pChar);
void modifyStory (storynode *pStoryHead, charnode *pCharHead);
/* Delete Story */
void purgeBattle (storybattle *pHead);
storynode *deleteStory (storynode *pHead);
/* Main Story Function */
storynode *mainStoryModule (storynode *pStoryHead, charnode *pCharHead);

/* Move Module */
/* General Move Module */
int isMoveDataSame (movenode *pMove, int nClass, int nElem);
void printClassification (int classification);
void printMove (movenode *pMove, char cFormat);
movenode *selectMove (movenode *pHead);
void replaceMoveInChar (movenode *pMove, movenode *pMoveHead, charnode *pCharHead, int nClass);
/* Add Move */
movenode *addmove (movenode *pHead);
/* View Move */
movenode *getFirstMoveWithSameData (movenode *pHead, int nClass, int nElem);
movenode *traverseNextMoveWithSameData (movenode *pCurrent, int nClass, int nElem);
movenode *traversePrevMoveWithSameData (movenode *pCurrent, int nClass, int nElem);
void *viewMove (movenode *pHead);
/* Modify Move */
void modifyMove (movenode *pMoveHead, charnode *pCharHead);
/* Assign Move */
movenode *selectMoveWithSameData (movenode *pHead, int nClass, int nElem);
void assignMove (movenode *pMoveHead, charnode *pCharHead);
/* Main Move Function */
movenode *mainMoveModule (movenode* pMoveHead, charnode *pCharHead);

int main()
{
	charnode *pCharHead = NULL;
	storynode *pStoryHead = NULL;
	movenode *pMoveHead = NULL;
	char cInput;
	
	srand(time(NULL));
	
	do
	{
		system("cls");
		printf("[G]ameplay Mode\n");
		printf("[C]onfigure Mode\n");
		printf("[E]xit");
		
		cInput = getch();
		cInput = toupper(cInput);
		
		switch (cInput)
		{
			case 'G':
			   if (pCharHead == NULL)
			   {
			   	  system("cls");
			   	  printf("Add characters in the Character module in Config First\nPress any key to continue......");
			   	  getch();
			   } 
			   else
			      mainGame(pStoryHead, pCharHead); 
			   break;
			case 'C': mainConfig(&pStoryHead, &pMoveHead, &pCharHead); break;
		}
	} while (cInput != 'E');
	
	return 0;
}


/* Gameplay Module */
/* Combat Functions */
/* General Combat Functions */
void printCombat (combattype *pCombatData)
{
    /* Prints the battlefiled or general data about the player and the opponent, such as Names, HP, PP, etc. */
   system("cls");
   if (pCombatData->stageNum)
      printf("Stage: %d\n", pCombatData->stageNum);
   printf("\"%s\"\n\n", pCombatData->opponent->character.stage);
   printf("\t  %s\t\t\t  %s\n", pCombatData->player->character.name, pCombatData->opponent->character.name);
   printf("\t   \"");
   printElem(pCombatData->player->character.elemental);
   printf("\"\t\tvs\t   \"");
   printElem(pCombatData->opponent->character.elemental);
   printf("\"\n");
   printf("\tHP:  %d/%d\t\t\t", pCombatData->playerHP, pCombatData->player->character.hp);
   printf("HP:  %d/%d\n", pCombatData->opponentHP, pCombatData->opponent->character.hp);
   printf("\tPP:  %d/%d\t\t\t", pCombatData->playerPP, pCombatData->player->character.pp);
   printf("PP:  %d/%d\n\n", pCombatData->opponentPP, pCombatData->opponent->character.pp);
	
   if (pCombatData->turn)
      printf("Player's Turn!\n");
   else
	  printf("Opponent's Turn!\n");
}

void printMoveOutcome (charnode *pAttacker, charnode *pDefender, combattype *pCombatData, int nInput, int nDamage)
{  	
   /* Prints out the move used by the player or opponent and what the move did */
   printCombat(pCombatData);
		
   if (!nDamage)
      printf("%s has missed %s\n\n", pAttacker->character.name, pDefender->character.name);
   else 
   {
      if (pAttacker->character.elemental == 1 && pDefender->character.elemental == 4)
	     printf("It's super effective! ");
	  else if (pAttacker->character.elemental == pDefender->character.elemental + 1)
	     printf("It's super effective! ");
	  else if (pAttacker->character.elemental == pDefender->character.elemental)
		 printf("It's not very effective. ");
		      
	  printf("%s has used ", pAttacker->character.name);
	  if (pAttacker->character.moveset[nInput - 1]->move.name != NULL)
	     printf("%s" , pAttacker->character.moveset[nInput - 1]->move.name);
	  else
	   	 printClassification(nInput);
	  printf(", dealing %d damage to %s\n\n", nDamage, pDefender->character.name);
	}
	
   printf("Press any key to continue.......");
   getch();
}

/* Damage Calculations */
int getRandNum (int nLowerBound, int nUpperBound)
{
   /* This generates a random number within a lower and upper bound */
   int randValue;
   randValue = nLowerBound + rand() % (nUpperBound - nLowerBound + 1);
   return randValue;
}

int calculatePPCost (int nPP, int nInput)
{
   /* Generates the random PP cost for each move for either the player or the opponent */
   int nPPCost;
   
   switch (nInput)
   {
      case 1: nPPCost = getRandNum((int)(0.10 * nPP), (int)(0.20 * nPP)); break;
	  case 2: nPPCost = getRandNum((int)(0.30 * nPP), (int)(0.50 * nPP)); break;
	  case 3: nPPCost = getRandNum((int)(0.60 * nPP), (int)(0.80 * nPP)); break;
   }
   
   return nPPCost;
}

int willMoveHit (int nInput)
{
   /* Determines whether the attacker does or does not hit */
   int nHit;
	
   if (nInput == 1 || nInput == 2)
   {
      nHit = getRandNum(1, 100);
	  
	  if (nInput == 1)
	  {
	     if (nHit <= 25) /* Light Attacks have 25% chance of missing */
	        return 0;
      }
	  else if (nInput == 2)
	  {
	     if (nHit <= 10) /* Medium Attacks have 10% chance of missing */
	        return 0;
      }	   
   }
   
   return 1;
}

int calculateBaseDamage (int nDefenderHP, int nInput)
{
   /* Calculate the Base Damage. The Base damage is equal to the classificationMultipler * DefenderHP */
   int nBaseDamage;
   
   switch (nInput)
   {
   	  case 1: nBaseDamage = getRandNum((int)(0.05 * nDefenderHP), (int)(0.15 * nDefenderHP)); break;
   	  case 2: nBaseDamage = getRandNum((int)(0.20 * nDefenderHP), (int)(0.30 * nDefenderHP)); break;
   	  case 3: nBaseDamage = getRandNum((int)(0.40 * nDefenderHP), (int)(0.50 * nDefenderHP)); break;
   }
   
   return nBaseDamage;
}

float getElemMultipler (int nAttackerElem, int nDefenderElem)
{
   /* Determines the Elemental Multipler */
   /* In this program, the number of the weakness of an elemental is equal to the number of the elemental plus one, except for Fire */
   if (nAttackerElem == 1 && nDefenderElem == 4) // This accounts for the weakness of fire
      return 1.50; // The defender's elemental is weak to the attacker, the defender will take 1.5x damage
   else if (nAttackerElem == nDefenderElem + 1)
      return 1.50;
   else if (nAttackerElem == nDefenderElem) // If their elemental is the same, they only 75% damage
      return 0.75;
    else
      return 1.0;
}

int calculateDamage (charnode *pAttacker, charnode *pDefender, combattype *pCombatData, int nInput)
{
   /* Calculates the final damage for a turn */
   int nHit, nBaseDamage, nFinalDamage;
   float fElemMultiplier;
	
   if (nInput == 1 || nInput == 2)
   {
      nHit = willMoveHit(nInput);
      if (!nHit)
         return 0; // If the attacker misses, then the final damage will be 0
   }
   
   nBaseDamage = calculateBaseDamage(pDefender->character.hp, nInput);
   fElemMultiplier = getElemMultipler (pAttacker->character.elemental, pDefender->character.elemental);
   nFinalDamage = (int)(nBaseDamage * fElemMultiplier);
   
   return nFinalDamage;
}

/* Player Turn Functions */
int isMoveValidInCombat (combattype *pCombatData, int nInput)
{
   /* Checks whether the player input in combat is valid */
   if (nInput < 1 || nInput > 4)
      return 0;
   else if (nInput == 1 && pCombatData->playerPP == pCombatData->player->character.pp)
   {
      system("cls");
      printCombat(pCombatData);
      printf("You can't charge any more PP\n\nPress any key to continue....");
      getch();
      return 0;
   }
   else if (nInput >= 2 && nInput <= 4)
   {
      if (pCombatData->playerPP < pCombatData->playerMovePP[nInput - 2])
      {
          system("cls");
          printCombat(pCombatData);
          printf("You need more PP to do this move\n\nPress any key to continue....");
          getch();
          return 0;
	  }
   }
   
   return 1;
}

void playerTurn (combattype *pCombatData)
{
   /* This handles the input and move of the player for a turn */
   int nInput, i, nDamage;
	
   /* The player's input is from 1-4, 1 is Charge, 2-4 is the classification of the move used, minus 1 */
   do
   {
      printCombat(pCombatData);
	  printf("Select Move:\n");
      printf("[1] Charge - Gain 3 points of PP\n");
	   
	  for (i = 0; i < 3; i++)
	  {
	     printf("[%d] ", i + 2);
	     if (pCombatData->player->character.moveset[i] != NULL)
	        printf("%s - %s", pCombatData->player->character.moveset[i]->move.name, pCombatData->player->character.moveset[i]->move.description);
	     else
	     {
	        printClassification(i + 1);
	        printf(" - ");
	        printElem(pCombatData->player->character.elemental);
	        printf(" ");
	        printClassification(i + 1);
	     }
	      
	     printf(" (Required PP: %d)\n", pCombatData->playerMovePP[i]);
	   }
	   
	   nInput = getch() - '0';	   
    } while (!isMoveValidInCombat(pCombatData, nInput));
    
    if (nInput == 1)
    {
    	pCombatData->playerPP = pCombatData->playerPP + 3;
    	if (pCombatData->playerPP > pCombatData->player->character.pp) // Ensures PP does not go beyond the character's maximum PP
    	   pCombatData->playerPP = pCombatData->player->character.pp;
    	printCombat(pCombatData);
    	printf("%s charged up 3 PP\n\nPress any key to continue", pCombatData->player->character.name);
    	getch();
	}
	else
	{
		nDamage = calculateDamage(pCombatData->player, pCombatData->opponent, pCombatData, nInput - 1);
		pCombatData->playerPP = pCombatData->playerPP - pCombatData->playerMovePP[nInput - 2]; // Deducts the corresponding PP 
		
		pCombatData->opponentHP = pCombatData->opponentHP - nDamage; // Damages the opponent
		if (pCombatData->opponentHP < 0) // Prevents the HP from going down to negative
		   pCombatData->opponentHP = 0;
		   
		printMoveOutcome(pCombatData->player, pCombatData->opponent, pCombatData, nInput - 1, nDamage);
		
		pCombatData->playerPP = pCombatData->playerPP + 2; // Gains 2 PP when not using Charge
		if (pCombatData->playerPP > pCombatData->player->character.pp)
    	   pCombatData->playerPP = pCombatData->player->character.pp;
    }	
}

/* Opponent Turn Function */
void opponentTurn (combattype *pCombatData)
{
   /* Handles the AI of the opponent through randomized numbers */
   int nDamage, nCharge, nMove, nInput; // In this function, nInput works similarly to the nInput in playerTurn but the rang is from 0-3, instead 1-4
   
   nCharge = getRandNum(1, 100);
   
   /* If the opponent does not have enough PP for the light move, the opponent will always charge. Otherwise, the opponent has 40% chance to charge */
   if (nCharge <= 40 && pCombatData->opponentPP != pCombatData->opponent->character.pp || pCombatData->opponentPP < pCombatData->opponentMovePP[0])
      nInput = 0;
   else
   {
   /* Assuming the opponent has enough PP for all moves, the opponent has a 35% chance to use heavy move, another 35% chance to use a medium, and a 30%
      chance to use a light move. The opponent will never use a move it does not have enough PP for. */
      nMove = getRandNum(1, 100);
	  
	  if (nMove >= 65 && pCombatData->opponentPP >= pCombatData->opponentMovePP[2])
	     nInput = 3;
	  else if (nMove >= 30 && pCombatData->opponentPP >= pCombatData->opponentMovePP[1])
	     nInput = 2;
	  else
	     nInput = 1;
   }
   
   if (nInput == 0)
   {
      pCombatData->opponentPP = pCombatData->opponentPP + 3;
      if (pCombatData->opponentPP > pCombatData->opponent->character.pp)
         pCombatData->opponentPP = pCombatData->opponent->character.pp;
      printCombat(pCombatData);
      printf("%s charged up 3 PP\n\nPress any key to continue", pCombatData->opponent->character.name);
      getch();
   }
   else
   {
      nDamage = calculateDamage(pCombatData->opponent, pCombatData->player, pCombatData, nInput);
      pCombatData->opponentPP = pCombatData->opponentPP - pCombatData->opponentMovePP[nInput - 1];
      
      pCombatData->playerHP = pCombatData->playerHP - nDamage;
      if (pCombatData->playerHP < 0)
         pCombatData->playerHP = 0;
         
      printMoveOutcome(pCombatData->opponent, pCombatData->player, pCombatData, nInput, nDamage);
      
	  pCombatData->opponentPP = pCombatData->opponentPP + 2;
	  if (pCombatData->opponentPP > pCombatData->opponent->character.pp)
         pCombatData->opponentPP = pCombatData->opponent->character.pp;
   }
}

/* Main Combat Function */
int combatMode (charnode *pPlayer, charnode *pOpponent, int nStageNum)
{
   /* This facilitates the main logic of combat in this game */
   combattype combatData;
   int i;
	
   combatData.player = pPlayer; // Player is the character controlled by the player
   combatData.opponent = pOpponent; // Opponent is the character controlled by the computer
   combatData.playerHP = pPlayer->character.hp; // Sets the current HP to Max HP at start
   combatData.opponentHP = pOpponent->character.hp;
   combatData.playerPP = 0; // Sets the current PP at start
   combatData.opponentPP = 0;
   combatData.stageNum = nStageNum;
   combatData.turn = rand() % 2; // Randomly Selects whose turn goes first
	
   for (i = 0; i < 3; i++)
   {
      combatData.playerMovePP[i] = calculatePPCost(combatData.player->character.pp, i + 1);
	  combatData.opponentMovePP[i] = calculatePPCost(combatData.opponent->character.pp, i + 1);
   }
	
   while (combatData.playerHP > 0 && combatData.opponentHP > 0)
   {
      if (combatData.turn)
	     playerTurn(&combatData);
	  else
	     opponentTurn(&combatData);
	  combatData.turn = !combatData.turn;
   }
	
   if (combatData.playerHP > 0) // Player Won
   {
   	  system("cls");
      printf("%s has defeated %s\nPress any key to continue......", combatData.player->character.name, combatData.opponent->character.name);
	  getch();
	  return 1; // If player won, returns 1
   }
   else if (combatData.opponentHP > 0) // Opponent Won
   {
   	  system("cls");
      printf("%s has defeated %s\nPress any key to continue......", combatData.opponent->character.name, combatData.player->character.name);
	  getch();
	  return 0; // If player lost, returns 0
   }
}


/* Modes Module */
void storylineMode(storynode *pHead)
{
   /* Allows the player to play a storyline and battle through a sequence of opponents */
   storynode *pStory;
   storybattle *pBattleRun;
   int nResult, i;
   char cAns = 'Y';
	
   pStory = selectStory(pHead);
   pBattleRun = pStory->story.battles;
	
   for (i = 1; pBattleRun != NULL && cAns != 'N'; i++) // i is the count of the number of opponent the player faced
   {
      system("cls");
	  printf("%s vs %s\nPress any key to continue......", pStory->story.protagonist->character.name, pBattleRun->opponent->character.name);
	  getch();
	  nResult = combatMode(pStory->story.protagonist, pBattleRun->opponent, i);
	  if (!nResult) // If the player lost a battle, the player has the option to play back at the start again or quit
	  {
	     system("cls");
		 printf("You lost. Do You want to play again? [Y/N] ");
		 do
		 {
		    cAns = getch();
			cAns = toupper(cAns);
		 } while (cAns != 'Y' && cAns != 'N');
		 if (cAns == 'Y')
		 {
		    i = 0;
			pBattleRun = pStory->story.battles;
		 }
	  }
	  else
	  {
	     if (pBattleRun->Next != NULL)
		 {
		    system("cls");
		    printf("You Won! Press any key to continue to the next opponent.......");
		    getch();
		 }
		 pBattleRun = pBattleRun->Next;
	  }
   }
	
   if (nResult) // The player won the entire storyline
   {
      system("cls");
	  printf("Congrations! You won the entire storyline '%s'\nPress any key to continue.......", pStory->story.title);
	  getch();
   }
}

void freeMode (charnode *pHead)
{
   /* Allows players to choose which characters to fight in combat */
   charnode *pPlayer;
   charnode *pOpponent;
   char cAns;
	
   do
	{
	   pPlayer = selectChar(pHead);
	
	   system("cls");
	   printf("%s vs.......\nPress any key to continue", pPlayer->character.name);
	   getch();
	
	   pOpponent = selectChar(pHead);
	   
	   system("cls");
	   printf("%s vs %s\nPress any key to continue", pPlayer->character.name, pOpponent->character.name);
	   getch();
	   
	   combatMode(pPlayer, pOpponent, 0);
	   
	   system("cls");
	   printf("Do you want to continue playing? [Y/N] ");
	   do
	   {
	   	  cAns = getch();
	   	  cAns = toupper(cAns);
	   } while (cAns != 'Y' && cAns != 'N');
    } while (cAns != 'N');
}

/* Main Gameplay Module */
void mainGame (storynode *pStoryHead, charnode *pCharHead)
{
	/* Allows player to choose between Free mode and Storyline mode */
   char cInput;
   do
   {
      system("cls");
	  printf("[F]ree Mode\n");
	  printf("[S]toryline Mode\n");
	  printf("[E]xit");
		
	  cInput = getch();
	  cInput = toupper(cInput);
		
	  switch (cInput)
      {
	     case 'S':
		    if (pStoryHead == NULL) // When there's no storyline, the player will not be able to enter storyline mode
			{
			   system("cls");
			   printf("Add a storyline in the Storyline module in Config First\nPress any key to continue......");
			   getch();
			} 
			else
			   storylineMode(pStoryHead); 
			break;
	     case 'F': freeMode(pCharHead); break;
	  }
	} while (cInput != 'E');
}


/* Main Config Module */
void mainConfig (storynode **pStoryHead, movenode **pMoveHead, charnode **pCharHead)
{
   /* Allows Players to access Character, Storyline, and Move Modules */
   char cInput;
		
   do
   {
      system("cls");
	  printf("Cofigurate Module\n");
	  printf("[C]haracter Module\n");
	  printf("[S]toryline Module\n");
	  printf("[M]ove Module\n");
      printf("[E]xit\n");
	   
	  cInput = getch();
	  cInput = toupper(cInput);
	   
	  switch (cInput)
	  {
	     case 'C': *pCharHead = mainCharModule(*pMoveHead , *pStoryHead, *pCharHead); break;
	   	 case 'S': 
	   	    if (*pCharHead == NULL) // When there is no characters created, player will not be able to enter the Storyline Module
	   	    {
	   	       system("cls");
	   	       printf("Add charaters first in the Character Module\nPress any key to continue.........");
	   	       getch();
			}
			else
			   *pStoryHead = mainStoryModule(*pStoryHead, *pCharHead); 
			break;
	     case 'M': *pMoveHead = mainMoveModule(*pMoveHead, *pCharHead); break;
	  }
   } while (cInput != 'E');
}


/* Char Module */
/* General Character Functions */
void printElem (int elemental)
{
   /* Print out the Elemental Affinity corresponding to its number (1-4) */
   switch (elemental)
   {
      case 1: printf("Water"); break;
	  case 2: printf("Earth"); break;
	  case 3: printf("Air"); break;
	  case 4: printf("Fire"); break;
   }
}

void printChar (charnode *pChar, char cFormat)
{
   /* Prints out general info of a character. */
   printf("****************************************************\n");
   if (cFormat == 'V') // This print out the info normally
   {
      printf("Name: %s\n", pChar->character.name);
      printf("Location: %s\n", pChar->character.stage);
      printf("Elemental Affinity: ");
      printElem(pChar->character.elemental);
      printf("\n");
      printf("HP: %d\n", pChar->character.hp);
      printf("PP: %d\n\n", pChar->character.pp);
      printf("Moveset\n");
      printMoveset(pChar, 'V');
   }
   else if (cFormat == 'M') // This print out the info and the number needed to input to modify that aspect of a character 
   {
   	  printf("[1] Name: %s\n", pChar->character.name);
      printf("[2] Location: %s\n", pChar->character.stage);
	  printf("[3] Elemental Affinity: ");
	  printElem(pChar->character.elemental);
	  printf("\n");
	  printf("[4] HP: %d\n", pChar->character.hp);
	  printf("[5] PP: %d\n", pChar->character.pp);
	  printf("[6] Modify Another Character\n");
	  printf("[7] Done Modifying\n\n");
   }
   printf("****************************************************\n");
}

void printMoveset (charnode *pChar, char cFormat)
{
   /* Prints out the moveset of a character */
   int i;
   for (i = 0; i < 3; i++)
   {
      if (cFormat == 'M') // Display numbers needed to be inputted to assign a move to the corresponding classification
         printf("[%d] ", i + 1);
      printClassification(i + 1);
      printf(": ");
      if (pChar->character.moveset[i] != NULL)
         printf("%s\n", pChar->character.moveset[i]->move.name);
      else // When there's no move of the corresponding classification assign to that character 
         printf("None Assigned\n");
   }
}

charnode *selectChar (charnode *pHead)
{
   /* Allows the player to select a character for the functions of modules or gameplay */
   char cInput;
   charnode *pCurrent;
   pCurrent = pHead;
	
   do
   {
      system("cls");
	  printf("Select Character\n");
	  printChar(pCurrent, 'V');
	  printf("[N]ext [P]revious [S]elect Character");
	  cInput = getch();
	  cInput = toupper(cInput);
		
	  if (cInput == 'N')
	  {
	     if (pCurrent->Next != NULL)
		    pCurrent = pCurrent->Next; // Goes through the next character in the linked list if not NULL
	  }
      else if (cInput == 'P')
         if(pCurrent->Prev != NULL)
            pCurrent = pCurrent->Prev;
   } while (cInput != 'S'); // Goes through the previous character in the linked list if not NULL
	
   return pCurrent;
}

/* Add Character */
charnode *sortByAlpha (charnode *pHead, charnode *pChar)
{
   /* Sorts the characters linked list alphabetically when a character is first added */
   charnode *pTrail, *pRun;
   int bFound = 0;
	
   if (pHead == NULL)
      pHead = pChar;
   else if (strcmp(pHead->character.name, pChar->character.name) > 0)
   {
      pChar->Next = pHead;
	  pHead->Prev = pChar;
	  pHead = pChar;
   }
   else
   {
      pTrail = pHead;
	  pRun = pHead->Next;
	  while (pRun != NULL && !bFound)
      {
         if (strcmp(pRun->character.name, pChar->character.name) > 0)
            bFound = !bFound;
         else
         {
            pTrail = pRun;
    	    pRun = pRun->Next;
	     }
      }
		
	  if (bFound)
	  {
	     pTrail->Next = pChar;
	     pRun->Prev = pChar;
	     pChar->Prev = pTrail;
	     pChar->Next = pRun;
	  }
	  else
	  {
	     pTrail->Next = pChar;
	     pChar->Prev = pTrail;
	     pChar->Next = NULL;
	  }
   }
	
   return pHead;
}

charnode *addChar (charnode *pHead)
{
   /* Add a new character */
   charnode *pTemp = NULL, *pTrail = NULL, *pRun = NULL;
   chardata dataTemp;
   int bFound = 0, i;
	
   /* Player enters all neccessary data about the new character */
   system("cls");
   fflush(stdin);
   printf("Enter Character Name: ");
   fgets(dataTemp.name, sizeof(dataTemp.name), stdin);
   dataTemp.name[strlen(dataTemp.name) - 1] = '\0';
   printf("Enter Location: ");
   fgets(dataTemp.stage, sizeof(dataTemp.stage), stdin);
   dataTemp.stage[strlen(dataTemp.stage) - 1] = '\0';
   printf("Enter Elemental Affinity: [1] Water [2] Earth [3] Air [4] Fire \n");
   do
   {
      dataTemp.elemental = getch() - '0';
   } while (dataTemp.elemental <= 0 || dataTemp.elemental > 4);
   printf("Enter HP: ");
   scanf("%d", &dataTemp.hp);
   printf("Enter PP: ");
   scanf("%d", &dataTemp.pp);
	
   for (i = 0; i < 3; i++)
      dataTemp.moveset[i] = NULL;
      
   /* Exits when there's no more memory for the new character */
   if ((pTemp = malloc(sizeof(charnode))) == NULL)
   {
      printf("Not Enough Memory to Add Chracter\nPress Any Key to Continue.....");
	  getch();
	  return pHead;
   }
	
   pTemp->Next = NULL;
   pTemp->Prev = NULL;
   pTemp->character = dataTemp;
	
   pHead = sortByAlpha(pHead, pTemp);
	
   return pHead;
}

/* View Character */
charnode *getFirstElemNode (charnode *pHead, int elemental)
{
	/* Gets the first character in the linked list that has the same elemental in the parameter */
   int bFound = 0;
   charnode *pRun;
   pRun = pHead;
	
   while (pRun != NULL && !bFound) // When there's no character found for that elemental, pRun is equal NULL
   {
      if (elemental == pRun->character.elemental)
         bFound = !bFound;
      else
         pRun = pRun->Next;
   }
	
   return pRun; 
}

charnode *traverseNextElemNode (charnode *pCurrent)
{
   /* Gets the character after the current character in the linked list with the same elemental */
   int bFound = 0;
   charnode *pRun;
   pRun = pCurrent->Next; 	
	
   while (pRun != NULL && !bFound)
   {
      if (pRun->character.elemental == pCurrent->character.elemental)
	     bFound = !bFound;
      else
         pRun = pRun->Next;
   }
    
   if (!bFound) // When no character is found, it returns the current characters
      return pCurrent;
   else
      return pRun;
}

charnode *traversePrevElemNode (charnode *pCurrent)
{
   /* Gets the character before the current character in the linked list with the same elemental */
   int bFound = 0;
   charnode *pRun;
   pRun = pCurrent->Prev;
   
   while (pRun != NULL && !bFound)
   {
      if (pRun->character.elemental == pCurrent->character.elemental)
	     bFound = !bFound;
      else
         pRun = pRun->Prev;
   }

	if (!bFound)
	   return pCurrent;
	else
	   return pRun;
}

void viewChars (charnode *pHead)
{
   /* Views Characters' info. Can also view by elemental affinity */
   charnode *pCurrent;
   char cInput;
   int bElemSort = 0; // When bElemsort is equal to 0, it views all characters normally. Otherwise, it view characters with the inputted elemental
   pCurrent = pHead;
	
   do
   {
      system("cls");
	  if (!bElemSort)
	     printf("Viewing All Characters\n");
	  else
	  {
	     printf("Viewing All ");
	     printElem(pCurrent->character.elemental);
	     printf(" Element Characters\n");
	  }
	  printChar(pCurrent, 'V');
	  printf("[N]ext [P]revious [1] View Water [2] View Earth [3] View Air [4] View Fire [5] View All [E]xit");
	  cInput = getch();
	  cInput = toupper(cInput);
	    
	  if (cInput == 'N')
	  {
	     if (!bElemSort)
	     {
	        if (pCurrent->Next != NULL)
	    	   pCurrent = pCurrent->Next;
	     }
	     else
	        pCurrent = traverseNextElemNode(pCurrent);
	  }
	  else if (cInput == 'P')
	  {
	     if (!bElemSort)
	     {
	        if (pCurrent->Prev != NULL)
	    	   pCurrent = pCurrent->Prev;
	     }
	     else
	        pCurrent = traversePrevElemNode(pCurrent);
	  }
	  else if (cInput > '0' && cInput < '5')
	  {
	     bElemSort = 1;
		 pCurrent = getFirstElemNode(pHead, cInput - '0'); // Gets the first character in the linked list to start off the sort
		 if (pCurrent == NULL) // When none is found, the function goes back to viewing all character and starts at pHead
		 {
		    system("cls");
			printf("No ");
			printElem(cInput - '0');
			printf(" Element Found\nPress Any Key to Continue.....");
		    getch();	
		    bElemSort = 0;
			pCurrent = pHead;
		 }
	  }
	  else if (cInput == '5') // Views all characters and starts at pHead
	  {
	     bElemSort = 0;
	     pCurrent = pHead;
	  }
   } while (cInput != 'E');
}

/* Modify Character */
charnode *modifyChar(movenode *pMoveHead, charnode *pCharHead)
{
   /* Modifies any aspect of any character */
   int nInput, nFirstElem, i;
   charnode *pChar;
   
   do
   {
      pChar = selectChar(pCharHead);
      nFirstElem = pChar->character.elemental;
      do
      {
         system("cls");
         printChar(pChar, 'M');
	     nInput = getch() - '0';
	     fflush(stdin);
		
	     if (nInput == 1)
	     {
	        printf("Enter New Name: \n");
		    fgets(pChar->character.name, sizeof(pChar->character.name), stdin);
		    pChar->character.name[strlen(pChar->character.name) - 1] = '\0';
		    
		    /* Removes the character from the linked list in order to added back alphabetically */
		    if (pChar->Next != NULL)
		       pChar->Next->Prev = pChar->Prev;
		    if (pChar->Prev != NULL)
		       pChar->Prev->Next = pChar->Next;
		    else
		    {
		       if (pCharHead->Next != NULL)
	           {
	              pCharHead = pCharHead->Next;
	              pCharHead->Prev = NULL;
               }
		    }
		    
		    pChar->Next = NULL;
		    pChar->Prev = NULL;
		    
		    pCharHead = sortByAlpha(pCharHead, pChar);
	    }
	    else if (nInput == 2)
	    {
	       printf("Enter New Location: \n");
		   fgets(pChar->character.stage, sizeof(pChar->character.stage), stdin);
		   pChar->character.stage[strlen(pChar->character.stage) - 1] = '\0';
	    }
	    else if (nInput == 3)
	    {
	       printf("Enter New Elemental Affinity: [1] Water [2] Earth [3] Air [4] Fire");
	       do
		   {
		      pChar->character.elemental = getch() - '0';
		   } while (pChar->character.elemental < 0 || pChar->character.elemental > 4);
	    }
        else if (nInput == 4)
        {
	       printf("Enter New HP: ");
		   scanf("%d", &pChar->character.hp);
	    }
	    else if (nInput == 5)
	    {
	       printf("Enter New PP: ");
	       scanf("%d", &pChar->character.pp);
	    }
     } while (nInput != 6 && nInput != 7);
     /* Replace the moveset with random moves that corresponds to the character's new elemental affinity */
     if (pChar->character.elemental != nFirstElem)
        for (i = 0; i < 3; i++)
           pChar->character.moveset[i] = getFirstMoveWithSameData (pMoveHead, i + 1, pChar->character.elemental);
   } while (nInput != 7);
	
	return pCharHead;
}

/* Delete Character */
void markProtagDel (storynode *pStoryHead, charnode *pChar)
{
   /* When the deleted character is a protagonist in a storyline, it removes that protagonist in story */
   storynode *pStoryRun;
   pStoryRun = pStoryHead;
   while (pStoryRun != NULL)
   {
      if (pStoryRun->story.protagonist == pChar)
	     pStoryRun->story.protagonist = NULL;
	  pStoryRun = pStoryRun->Next;
   }
}

void markBattleDel (storynode *pStoryHead, charnode *pChar)
{
	/* When the deleted character is an opponent in a storyline, it flags the storybattle the character is in for Replacement or Deletion*/
	int bFound;
	storynode *pStoryRun;
	storybattle *pBattleRun;
	pStoryRun = pStoryHead;
	
	while (pStoryRun != NULL) // Runs through all the stories
	{
		bFound = 0;
		pBattleRun = pStoryRun->story.battles;
		while (pBattleRun != NULL && !bFound) // Runs through all the battles in the story until the character is foiund or there is more battles
		{
			if (pBattleRun->opponent == pChar)
			   bFound = !bFound;
			else
			   pBattleRun = pBattleRun->Next;
		}
		
		if (bFound)
		   pBattleRun->bDel = 1;
		
		pStoryRun = pStoryRun->Next;
	}
}

charnode *deleteChar (storynode *pStoryHead, charnode *pCharHead)
{
	/* Deletes a character */
	
   char cAns;
   charnode *pChar;
   pChar = selectChar(pCharHead);
	
   printf("\nAre you sure you want to delete '%s'? [Y/N]", pChar->character.name);
   do
   {
      cAns = getch();
	  cAns = toupper(cAns);
   } while (cAns != 'Y' && cAns != 'N');
	
   if (cAns == 'Y')
   {
      if (pChar->Next != NULL)
	     pChar->Next->Prev = pChar->Prev;
	  if (pChar->Prev != NULL)
		 pChar->Prev->Next = pChar->Next;
	  else
		 pCharHead = pChar->Next;
		
	  markProtagDel(pStoryHead, pChar);
	  markBattleDel(pStoryHead, pChar);
   }
	
   return pCharHead;
}

/* Fix Character in Storyline */
charnode *getProFromBattle (storynode *pStory)
{
   /* Replaces the protagonist with a random character in the storyline when there is no character that is not the storyline */
   int bFound = 0;
   charnode *pTemp;
   storybattle *pRun;
   pRun = pStory->story.battles;
	
   while (!bFound)
      if (!(pRun->bDel)) // Will only get that character if the character is not flagged for replacement or deletion
	  {
	    pTemp = pRun->opponent;
	   	pStory->story.battles = deleteBattle(pStory->story.battles, pRun); // Removes the character from the battles linked list
	   	bFound = !bFound;
	  }
	  else
	     pRun = pRun->Next;   
	     
   return pTemp;
}

void fixDelPro (storynode *pStoryHead, charnode *pCharHead)
{
   /* Replaces the storylines' protagonist whose protagonist got deleted */
   storynode *pRun;
   pRun = pStoryHead;
	
   while (pRun != NULL)
   {
      if (pRun->story.protagonist == NULL)
	  {
	     pRun->story.protagonist = getFirstCharForBattle(pRun, pCharHead); // Gets the first character in the linked list who's not in the storyline
		 if (pRun->story.protagonist == NULL) //Ot
		    pRun->story.protagonist = getProFromBattle(pRun);
	  }
	  pRun = pRun->Next;
   }
}

void fixDelBattle (storynode *pStoryHead, charnode *pCharHead)
{
   /* Replaces flagged characters in the battles with characters not in the storyline */
   storynode *pStoryRun;
   storybattle *pBattleRun;
   pStoryRun = pStoryHead;
	
   while (pStoryRun != NULL)
   {
      pBattleRun = pStoryRun->story.battles;
	  while (pBattleRun != NULL)
	  {
	     if (pBattleRun->bDel)
		 {
		    pBattleRun->opponent = getFirstCharForBattle(pStoryRun, pCharHead); // Gets the first character that not in the storyline
		    if (pBattleRun->opponent == NULL)
		   	   pStoryRun->story.battles = deleteBattle(pStoryRun->story.battles, pBattleRun); // If it can't, then it would remove the battle from the storyline
		    pBattleRun->bDel = 0;
		 }
		 pBattleRun = pBattleRun->Next;
	  }
	  pStoryRun = pStoryRun->Next;
   }	
}

/* Main Character Function */
int isCharListValid (charnode *pHead)
{
   /* Checks if the player made at least 4 character, with 1 in each elemental */
   int i, bFound = 0;
   charnode *pRun;
   pRun = pHead;
	
   for (i = 1; pRun != NULL; i++) // Counts the number of characters
      pRun = pRun->Next;
	
   if (i < 4) // Returns 0 when the number of characters is less than 4
   {
      system("cls");
	  printf("You must create at least 4 characters to exit this module\nPress Any Key to Continue......");
	  getch();
	  return 0;
   }
	
   for (i = 1; i <= 4; i++) // Checks if there's all 4 elemental affinities
   {
      pRun = pHead;
	  bFound = 0;
	  while (pRun != NULL && !bFound)
	  {
	      if (pRun->character.elemental == i)
		     bFound = !bFound;
		  else
		     pRun = pRun->Next;
	  }
	    
	  if (!bFound) // Returns 0 when there's a missing elemental
	  {
	     system("cls");
	     printf("You must create one character for each elemental affinity to exit this module\nPress Any Key to Continue.....");
	     getch();
	     return 0;
	  }
	}
	
	return 1;
}

charnode *mainCharModule (movenode *pMoveHead, storynode *pStoryHead, charnode *pCharHead)
{
   /* Allows the player to add, modify, view, and delete characters */
   char cInput;
	
   do
   {
      do
	  {
	     system("cls");
		 printf("Character Module\n");
		 printf("****************************************************\n");
		 printf("[A]dd Character\n");
		 printf("[V]iew Characters\n");
		 printf("[M]odify Character\n");
		 printf("[D]elete Character\n");
		 printf("[E]xit\n");
		 printf("****************************************************\n");
		
		 cInput = getch();
		 cInput = toupper(cInput);
		  
		 if (cInput == 'A')
		    pCharHead = addChar(pCharHead);
		 else if (cInput == 'V' || cInput == 'M' || cInput == 'D')
		 {
		    if (pCharHead == NULL) // The player would need to add a character to access every other function
		    {
		       system("cls");
		       printf("Add a character first\nPress any key continue..........");
		       getch();
			} 
			else if (cInput == 'V')
		       viewChars(pCharHead);
			else if (cInput == 'M')
			   pCharHead = modifyChar(pMoveHead, pCharHead);
			else if (cInput == 'D')
			   pCharHead = deleteChar(pStoryHead, pCharHead);
	     }
	   } while (cInput != 'E');
   } while (!isCharListValid(pCharHead)); // Will not exit until there is at least 4 character created and 1 character for each elemental
   
   /* Replaces all deleted characters in storylines at the end to account to newly created characters */
   fixDelPro(pStoryHead, pCharHead);
   fixDelBattle(pStoryHead, pCharHead);
   return pCharHead;
}


/* Storyline Module */

/* Story Battle Functions */
/* Select Battle */
charnode *getFirstCharForBattle (storynode *pStory, charnode *pCharHead)
{
	/* Gets the first character in the linked list who is not in the storyline */
   charnode *pRun;
   int bFound = 0;
	
   pRun = pCharHead;
   while (pRun != NULL && !bFound)
      if (!isCharInStory(pStory, pRun))
	     bFound = !bFound;
	 else
	     pRun = pRun->Next;
	 
   return pRun;
}

charnode *traverseNextCharForBattle (storynode *pStory, charnode *pCurrent)
{
   /* Gets the character after the current character who is not in the storyline */
   charnode *pRun;
   int bFound = 0;
	
   pRun = pCurrent->Next;
   while (pRun != NULL && !bFound)
   {
      if (!isCharInStory(pStory, pRun))
	     bFound = !bFound;
	  else
	     pRun = pRun->Next;
   }
	 
   if (pRun == NULL)
      return pCurrent;
   else
	  return pRun;
}

charnode *traversePrevCharForBattle (storynode *pStory, charnode *pCurrent)
{
	/* Gets the character after the current character who is not in the storyline */
   charnode *pRun;
   int bFound = 0;
	
   pRun = pCurrent->Prev;
   while (pRun != NULL && !bFound)
   {
      if (!isCharInStory(pStory, pRun))
	     bFound = !bFound;
	  else
	     pRun= pRun->Prev;
   }
    
   if (pRun == NULL)
      return pCurrent;
   else
      return pRun;
}

charnode *selectCharForBattle (storynode *pStory, charnode *pCharHead)
{
   /* Allows player to select a character to be in a battle or be an opponent in a storyline. Will only allow the player to select characters not in the storyline */
   char cInput;
   charnode *pCurrent;
   pCurrent = getFirstCharForBattle(pStory, pCharHead);
	
   if (pCurrent == NULL) // When there is no more character that is not in the storyline, it returns NULL
   {
       system("cls");
	   printf("There are no more possible characters to select for this storyline.\n Press any key to continue.....");
	   getch();
	   return NULL;
   }
	
   do
   {
      system("cls");
	  printf("Select Character for Storyline\n");
	  printChar(pCurrent, 'V');
	  printf("[N]ext [P]revious [S]elect Character");
	  cInput = getch();
	  cInput = toupper(cInput);
		
	  if (cInput == 'N')
	     pCurrent = traverseNextCharForBattle(pStory, pCurrent);
      else if (cInput == 'P')
         pCurrent = traversePrevCharForBattle(pStory, pCurrent);
   } while (cInput != 'S');
	
   return pCurrent;
}

/* Add Battle */
storybattle *addBattle(storynode *pStory, charnode *pCharHead)
{
   /* Adds a battle or opponent in the storyline */
   storybattle *pTemp, *pRun, *pBattleHead;
   charnode *pChar;
	
   pBattleHead = pStory->story.battles;
   pChar = selectCharForBattle(pStory, pCharHead);
	
   if (pChar == NULL)
      return pBattleHead;
	
   if ((pTemp = malloc(sizeof(storybattle))) == NULL)
   {
      printf("Not Enough Memory\nPress any key to continue.....");
	  getch();
	  return NULL;
   }
	
   pTemp->Next = NULL;
   pTemp->Prev = NULL;
   pTemp->opponent = pChar;
   pTemp->bDel = 0;
	
   if (pBattleHead == NULL)
      pBattleHead = pTemp;
   else
   {
      pRun = pBattleHead;
	  while (pRun->Next != NULL)
	     pRun = pRun->Next;
	  pRun->Next = pTemp;
	  pTemp->Prev = pRun;
   }
	
   return pBattleHead;
}

/* Modify Battle */
void switchOpponentsInBattle (storybattle *pBattle1, storybattle *pBattle2)
{
   /* Switches the opponents of battles to change the order of battles */
   charnode *pTemp;
   pTemp = pBattle1->opponent;
   pBattle1->opponent = pBattle2->opponent;
   pBattle2->opponent = pTemp;
}

storybattle *traverseBattleListNumTimes (storybattle *pHead, int nNum)
{
   /* Traverse the linked list of battles NUM times to access a specific battle */
   storybattle *pRun;
   int i;
   pRun = pHead;
	
   for (i = 1; i <= nNum - 1; i++)
      pRun = pRun->Next;
	
   return pRun;
}

storybattle *modifyBattle (storynode *pStory, charnode *pCharHead)
{
   /* Modifies Battles or Opponents in a storyline. Players add, replace, delete battles and switch the order of battles */
   
   /* nInput1 and pBattle1 represent the battle the player select to modify, nInput2 and pBattle2 represent the second battle the player selected for switching, 
      and  nCount is the total number of battles in the linked list */
   int nInput1, nInput2, i, nCount;
   char cInput;
   charnode *pCharTemp;
   storybattle *pBattle1, *pBattle2, *pBattleRun, *pBattleHead;
   pBattleHead = pStory->story.battles;
	
   do
   {
       system("cls");
	   printf("Modify Battles\n");
	   printf("**************************\n");
	   pBattleRun = pBattleHead;
	   for (i = 1; pBattleRun != NULL; i++)
	   {
	      printf("[%d] %s\n", i, pBattleRun->opponent->character.name);
		  pBattleRun = pBattleRun->Next;
	   }
	   nCount = countBattle(pBattleHead);
	   printf("[%d] Add Battle\n", nCount + 1);
	   printf("[%d] Finish Modifying Battles\n", nCount + 2);
	   printf("Select Battle to Modify or Select Operation: ");
	   scanf("%d", &nInput1);
		
	   if (nInput1 > 0 && nInput1 <= nCount)
	   {
	      pBattle1 = traverseBattleListNumTimes(pBattleHead, nInput1);
		  printf("What to Do With: [%d] %s\n", nInput1, pBattle1->opponent->character.name);
		  printf("[S]witch Order [R]eplace [D]elete\n");
		  cInput = getch();
		  cInput = toupper(cInput);
		  switch (cInput)
		  {
		     case 'S': // Switchs Order
		        printf("Switch with which Battle? "); // Asks the player to input the battle to switch with
		        scanf("%d", &nInput2);
		        pBattle2 = traverseBattleListNumTimes(pBattleHead, nInput2);
		        switchOpponentsInBattle(pBattle1, pBattle2);
		        break;
		     case 'R': // Replaces character with a character that is not in the storyline
		        pCharTemp = selectCharForBattle(pStory, pCharHead);
		       	if (pCharTemp != NULL)
		       	   pBattle1->opponent = pCharTemp;
		       	break;
		     case 'D': // Deletes a battle
		        if (nCount > 3)
			       pBattleHead = deleteBattle(pBattleHead, pBattle1);
			    else // Will not allow the player delete anymore battles when there is only 3
			    {
			       system("cls");
			       printf("You Cannot have below 3 Opponents in a Storyline\nPress any key to continue......");
			       getch();
				}
		   }
	   }
	   else if (nInput1 == nCount + 1) // Add a battle in the storyline
	      pBattleHead = addBattle(pStory, pCharHead);	   
	} while (nInput1 != nCount + 2);
	
	return pBattleHead;
}

/* Delete Battle */
storybattle *deleteBattle (storybattle *pBattleHead, storybattle *pBattle)
{
   /* Deletes a Battle */
   if (pBattle->Next != NULL)
      pBattle->Next->Prev = pBattle->Prev;
   if (pBattle->Prev != NULL)
      pBattle->Prev->Next = pBattle->Next;
   else
      pBattleHead = pBattle->Next;
       
   free(pBattle);
   return pBattleHead;
}


/* Storyline Functions */
/* General Story Functions */
int isCharInStory (storynode *pStory, charnode *pChar)
{
   /* Checks if a character is in a storyline */
   storybattle *pRun;
   pRun = pStory->story.battles;
	
   if (pChar == pStory->story.protagonist) // Checks if the character is the protagonist
       return 1;
	
   while (pRun != NULL) // Checks if the character is an opponent in the storyline
      if (pRun->opponent == pChar)
	      return 1;
	  else
	      pRun = pRun->Next;
	 
   return 0;
}

void printStory (storynode *pStory, char cFormat)
{
   /* Prints the info of a storyline, similar to printChar */
   storybattle *pRun;
   pRun = pStory->story.battles;
	
   if (cFormat == 'V')
   {
      printf("**************\n");
	  printf("Title: %s\n", pStory->story.title);
	  printf("Protangonist: %s\n", pStory->story.protagonist);
	  printf("Description: \n%s\n", pStory->story.description);
	  printf("Battles: \n");
  }
  else if (cFormat == 'M')
  {
     printf("**************\n");
	 printf("[1] Title: %s\n", pStory->story.title);
	 printf("[2] Protangonist: %s\n", pStory->story.protagonist);
	 printf("[3] Description: \n%s\n", pStory->story.description);
	 printf("[4] Battles: \n");
  }
	
   while (pRun != NULL) // Prints the battles or opponent in the storyline
   {
          printf("%s\n", pRun->opponent->character.name);
          pRun = pRun->Next;
   }
   
   if (cFormat == 'M')
      printf("[5] Modify Another Story\n[6] Finish Modifying\n");
      
   printf("**************\n");
}

int countBattle (storybattle *pHead)
{
   /* Counts the number of battles in a storyline */
   int i;
   storybattle *pRun;
   pRun = pHead;
   for (i = 0; pRun != NULL; i++)
      pRun = pRun->Next;
	
   return i;
}

storynode *selectStory (storynode *pHead)
{
   /* Selects a Storyline for other functions and the storyline module */
   char cInput;
   storynode *pCurrent;
   pCurrent = pHead;
	
   do
   {
      system("cls");
	  printf("Select a Storyline\n\n");
	  printStory(pCurrent, 'V');
	  printf("[N]ext [P]revious [S]elect Story\n");
	  cInput = getch();
	  cInput = toupper(cInput);
		
	  if (cInput == 'N')
	  {
	     if (pCurrent->Next != NULL)
		    pCurrent = pCurrent->Next;
	  }
      else if (cInput == 'P')
      {
          if (pCurrent->Prev != NULL)
             pCurrent = pCurrent->Prev;
      }
   } while (cInput != 'S');
	
   return pCurrent;
}

/* Add Story */
storynode *addStory(storynode *pStoryHead, charnode *pCharHead)
{
   /* Add a storyline */
   int nCount; // Number of battles in the linked list
   storynode *pTemp, *pRun;
   char cInput;
	
   if((pTemp = malloc(sizeof(storynode))) == NULL)
   {
      printf("Not Enough Memory\n Press any key to continue.....");
	  getch();
	  return pStoryHead;
   }
	
   pTemp->Next = NULL;
   pTemp->Prev = NULL;
   pTemp->story.protagonist = selectChar(pCharHead); // Selects Protagonist
   
   system("cls");
   printf("Protangonist: %s\n", pTemp->story.protagonist);
   printf("Enter Title: ");
   fflush(stdin);
   fgets(pTemp->story.title, sizeof(pTemp->story.title), stdin);
   pTemp->story.title[strlen(pTemp->story.title) - 1] = '\0';
   printf("Enter Description: ");
   fgets(pTemp->story.description, sizeof(pTemp->story.description), stdin);
   pTemp->story.description[strlen(pTemp->story.description) - 1] = '\0';
   pTemp->story.battles = NULL;
   pTemp->story.battles = addBattle(pTemp, pCharHead); // Adds the first battle or opponent
   nCount = 1; // Sets the battle count to 1
    
   do
   {
      system("cls");
      printStory(pTemp, 'V');
      printf("\n[A]dd Character to Story [F]inish");
      cInput = getch();
      cInput = toupper(cInput);
      if (cInput == 'A') // Add more battle
      {
         pTemp->story.battles = addBattle(pTemp, pCharHead);
         nCount = countBattle(pTemp->story.battles);
      }
      if (cInput == 'F' && nCount < 3)
      {
          system("cls");
       	  printf("You must have at 3 least Battles in a Storyline\nPress any key to Continue.......");
       	  getch();
	  }
   } while (cInput != 'F' ||  nCount < 3); // Will not allow the player to leave the function until there are at least 3 battles
   
    if (pStoryHead == NULL)
       pStoryHead = pTemp;
    else
    {
       pRun = pStoryHead;
       while (pRun->Next != NULL)
          pRun = pRun->Next;
       pRun->Next = pTemp;
       pTemp->Prev = pRun;
    }
   
    return pStoryHead;
}

/* View Story */
storynode *getFirstProtagNode (storynode *pHead, char cProtag[])
{
   /* Gets the first storyline in the linked list with the same protagonist */
   int bFound = 0;
   storynode *pRun;
   pRun = pHead;
	
   while (pRun != NULL && !bFound)
   {
      if (strcmp(cProtag, pRun->story.protagonist->character.name) == 0)
	     bFound = !bFound;
	  else
	     pRun = pRun->Next;
   }
	
   return pRun;
}

storynode *traverseNextProtagNode (storynode *pCurrent, char cProtag[])
{
   /* Gets the storyline after the current storyline which has the same protagonist */
   int bFound = 0;
   storynode *pRun;
   pRun = pCurrent->Next;
	
   while (pRun != NULL && !bFound)
   {
      if (strcmp(cProtag, pRun->story.protagonist->character.name) == 0)
	     bFound = !bFound;
	  else
	     pRun = pRun->Next;
   }
	
   if (!bFound)
      return pCurrent;
   else
	  return pRun;
}

storynode *traversePrevProtagNode (storynode *pCurrent, char cProtag[])
{
   /* Gets the storyline before the current storyline which has the same protagonist */
   int bFound = 0;
   storynode *pRun;
   pRun = pCurrent->Prev;
	
   while (pRun != NULL && !bFound)
   {
      if (strcmp(cProtag, pRun->story.protagonist->character.name) == 0)
	     bFound = !bFound;
	  else
		 pRun = pRun->Prev;
   }
	
   if (!bFound)
      return pCurrent;
   else
	  return pRun;
}

void viewStory (storynode *pHead)
{
   /* Views storylines. Can View by protagonist */
   char cInput, cProtagFilter[51], bProtag = 0; // cProtagFilter is the name of protagonist whose storylines is being viewed.
   storynode *pCurrent;
   pCurrent = pHead;
	
   do
   {
      system("cls");
	  if (bProtag)
	     printf("Viewing %s's Storylines\n\n", cProtagFilter);
	  else
		 printf("Viewing Storylines\n\n");
	  printStory(pCurrent, 'V');
	  printf("[N]ext [P]revious [1] View By Protagonist [2] View All [E]xit\n");
	  cInput = getch();
	  cInput = toupper(cInput);
		
      if (cInput == 'N')
	  {
	     if (bProtag) // When bProtag is equal to 1, the function only views storyline of the desinated protagonist
		    pCurrent = traverseNextProtagNode(pCurrent, cProtagFilter);
		 else if (pCurrent->Next != NULL)
			pCurrent = pCurrent->Next;
	  }
      else if (cInput == 'P')
      {
         if (bProtag)
		    pCurrent = traversePrevProtagNode(pCurrent, cProtagFilter);
         else if (pCurrent->Prev != NULL)
            pCurrent = pCurrent->Prev;
      }
      else if (cInput == '1')
      {
         bProtag = 1;
         printf("Enter Protagonist's name to View Storylines of: ");
         fflush(stdin);
         fgets(cProtagFilter, sizeof(cProtagFilter), stdin);
         cProtagFilter[strlen(cProtagFilter) - 1] = '\0';
         pCurrent = getFirstProtagNode(pHead, cProtagFilter);
        	
         if (pCurrent == NULL)
         {
            system("cls");
        	printf("No Storyline with protagonist '%s' found\nPress any key to continue........");
        	getch();
        	pCurrent = pHead;
        	bProtag = 0;
		 }
	  }
	  else if (cInput == '2')
	  {
	     bProtag = 0;
		 pCurrent = pHead;
	  }   
   } while (cInput != 'E');
}

/* Modify Story */
storybattle *getBattleOfChar (storybattle *pBattleHead, charnode *pChar)
{
   /* Gets the storybattle with the charcter in the parameter */
   int bFound = 0;
   storybattle *pRun;
   pRun = pBattleHead;
	
   while (pRun != NULL && !bFound)
      if (pRun->opponent == pChar)
	     bFound = !bFound;
	  else
	     pRun = pRun->Next;
	
   return pRun;
}

void modifyStory (storynode *pStoryHead, charnode *pCharHead)
{
   /* Modifies aspects of a storyline */
   int nInput;
   charnode *pCharTemp;
   storynode *pStory;
   storybattle *pBattleTemp;
	
   do
   {
      pStory = selectStory(pStoryHead);
	  do 
	  {
	     system("cls");
		 printf("Modify What?\n");
		 printStory(pStory, 'M');
		 nInput = getch() - '0';
		 fflush(stdin);
		
		 if (nInput == 1)
		 {
		    printf("Enter New Title: \n");
		    fgets(pStory->story.title, sizeof(pStory->story.title), stdin);
		    pStory->story.title[strlen(pStory->story.title) - 1] = '\0';
		 }
	     else if (nInput == 2)
   		 {
		    pCharTemp = selectChar(pCharHead);
   			if (isCharInStory(pStory, pCharTemp))
		   	{
	   		   if (pCharTemp != pStory->story.protagonist)
	   		   {
   			      pBattleTemp = getBattleOfChar(pStory->story.battles, pCharTemp);
			   	  pBattleTemp->opponent = pStory->story.protagonist;
   			   }
   			}
            pStory->story.protagonist = pCharTemp;
   	     }
   	     else if (nInput == 3)
   	     {
	        printf("Enter New Description: \n");
	   	    fgets(pStory->story.description, sizeof(pStory->story.description), stdin);
		    pStory->story.description[strlen(pStory->story.description) - 1] = '\0';
  	     }
  	     else if (nInput == 4) // Modify Battles
  	        pStory->story.battles = modifyBattle(pStory, pCharHead);
      } while (nInput != 5 && nInput != 6);
   } while (nInput != 6);
}

/* Delete Story */
void purgeBattle (storybattle *pHead)
{
   /* Deletes all Battles in a storyline */
   storybattle *pTemp;

   while (pHead != NULL)
   {
      pTemp = pHead;
	  pHead = pHead->Next;
	  free(pTemp);
   }
}

storynode *deleteStory (storynode *pHead)
{
   /* Deletes a storyline */
   char cAns;
   storynode *pStory;
	
   pStory = selectStory(pHead);
   printf("Are you sure you want to delete the '%s' Storyline? [Y/N]", pStory->story.title);
	
   do
   {
      cAns = getch();
	  cAns = toupper(cAns);
   } while (cAns != 'Y' && cAns != 'N');
	
   if (cAns == 'Y')
   {
      if (pStory->Next != NULL)
	     pStory->Next->Prev = pStory->Prev;
	  if (pStory->Prev != NULL)
	     pStory->Prev->Next = pStory->Next;
	  else
	     pHead = pStory->Next;
	  purgeBattle(pStory->story.battles); // Deletes all battles in the storyline
	  free(pStory);
   }
	
   return pHead;
}

/* Main Story Module */
storynode *mainStoryModule (storynode *pStoryHead, charnode *pCharHead)
{
   /* Allows the player to add, view, modify, delete storylines */
   char cInput;
	
   do
   {
      system("cls");
	  printf("Storyline Module\n");
	  printf("[A]dd Story\n");
	  printf("[V]iew Story\n");
	  printf("[M]odify Story\n");
	  printf("[D]elete Story\n");
	  printf("[E]xit\n");
	   
	  cInput = getch();
	  cInput = toupper(cInput);
	   
	  if (cInput == 'A')
	     pStoryHead = addStory(pStoryHead, pCharHead);
	  else if (cInput == 'V' || cInput == 'M' || cInput == 'D')
	  {
	     if (pStoryHead == NULL)
	     {
	        system("cls");
	        printf("Add a storyline first\nPress any key to continue......");
	        getch();
		 }
		 else if (cInput == 'V')
		    viewStory(pStoryHead);
		  else if (cInput == 'M')
		    modifyStory (pStoryHead, pCharHead);
		  else if (cInput == 'D')
		    pStoryHead = deleteStory(pStoryHead);
	  }
   } while (toupper(cInput) != 'E');
	
   return pStoryHead;
}


/* Move Module */
/* General Move Functions */
int isMoveDataSame (movenode *pMove, int nClass, int nElem)
{
   /* Checks if the move has the classification and elemental as the parameter. If nClass is equal to 0, it will not check for classification. Likeweise for nElem and elemental */
   if (nClass)
   {
      if (pMove->move.classification != nClass)
	     return 0;
   }
   if (nElem)
   {
      if (pMove->move.elemental != nElem)
	     return 0;
   }
	
   return 1;
}

void printClassification (int classification)
{
   /* Print the classification of the corresponding number (1-3) */
   switch (classification)
   {
      case 1: printf("Light Attack"); break;
	  case 2: printf("Medium Attack"); break;
	  case 3: printf("Heavy Attack"); break;
   }
}

void printMove (movenode *pMove, char cFormat)
{
   /* Print the general info of a move. Similar to printChar and printStory */
   printf("***************\n");
   if (cFormat == 'V')
   {
      printf("Name: %s\n", pMove->move.name);
	  printf("Description:\n%s\n", pMove->move.description);
	  printf("Classification: ");
	  printClassification(pMove->move.classification);
	  printf("\nElemental Affinity: ");
	  printElem(pMove->move.elemental);
   }
   else if (cFormat == 'M')
   {
      printf("[1] Name: %s\n", pMove->move.name);
	  printf("[2] Description: %s\n", pMove->move.description);
	  printf("[3] Classification: ");
	  printClassification(pMove->move.classification);
      printf("\n[4] Elemental Affinity: ");
	  printElem(pMove->move.elemental);
	  printf("\n[5] Modify Another Move\n[6] Done Modifying");
   }
   printf("\n**************\n");
}

movenode *selectMove (movenode *pHead)
{
   /* Selects a Move for other functions */
   char cInput;
   movenode *pCurrent;
   pCurrent = pHead;
	
   do
   {
      system("cls");
	  printf("Select Move\n");
	  printMove(pCurrent, 'V');
	  printf("[N]ext [P]revious [S]elect Move");
      cInput = getch();
	  cInput = toupper(cInput);
		
	  if (cInput == 'N')
	  {
	     if (pCurrent->Next != NULL)
		    pCurrent = pCurrent->Next;
	  }
	  else if (cInput == 'P')
	  {
	     if (pCurrent->Prev != NULL)
		    pCurrent = pCurrent->Prev;
	  }
   } while (cInput != 'S');
	
   return pCurrent;
}

void replaceMoveInChar (movenode *pMove, movenode *pMoveHead, charnode *pCharHead, int nClass)
{
   /* Replaces a move in the character's moveset if a move is modified or deleted */
   /* nClass is the original classification of the move before modification or deletion */
   charnode *pCharRun;
   pCharRun = pCharHead;
	
   while (pCharRun != NULL) // Runs through every character
   {
      if (pMove == pCharRun->character.moveset[nClass - 1]) // Checks if the move is in the character's moveset
	     pCharRun->character.moveset[nClass - 1] = getFirstMoveWithSameData(pMoveHead, nClass, pCharRun->character.elemental);
      pCharRun = pCharRun->Next;
   }
}

/* Add Move */
movenode *addMove (movenode *pHead)
{
   /* Adds Move */
   int nInput;
   movedata dataTemp;
   movenode *pTemp, *pRun;
	
   system("cls");
   printf("Enter Move Name: ");
   fflush(stdin);
   fgets(dataTemp.name, sizeof(dataTemp.name), stdin);
   dataTemp.name[strlen(dataTemp.name) - 1] = '\0';
   printf("Enter Description: ");
   fgets(dataTemp.description, sizeof(dataTemp.description), stdin);
   dataTemp.description[strlen(dataTemp.description) - 1] = '\0';
   printf("Enter Move Classification: [1] Light Attack [2] Medium Attack [3] Heavy Attack\n");
   do
   {
      dataTemp.classification = getch() - '0';
   } while (dataTemp.classification < 0 || dataTemp.classification > 3); 
   printf("Enter Elemental Affinity: [1] Water [2] Earth [3] Air [4] Fire\n");
   do
   {
      dataTemp.elemental = getch() - '0';
   } while (dataTemp.elemental < 0 || dataTemp.elemental > 4);

   if ((pTemp = malloc(sizeof(movenode))) == NULL)
   {
      system("cls");
	  printf("Not Enough Memory\nPress any key to continue.....");
	  getch();
	  return pHead;
   }
	
   pTemp->Next = NULL;
   pTemp->Prev = NULL;
   pTemp->move = dataTemp;
	
   if (pHead == NULL)
      pHead = pTemp;
   else
   {
      pRun = pHead;
	  while (pRun->Next != NULL)
	     pRun = pRun->Next;
	  pRun->Next = pTemp;
	  pTemp->Prev = pRun;
   }
	
   return pHead;
}

/* View Move */
movenode *getFirstMoveWithSameData (movenode *pHead, int nClass, int nElem)
{
   /* Gets the first move in the linked list with the same classification and elemental affinity in the parameter */
   int bFound = 0;
   movenode *pRun;
   pRun = pHead;
	
   while (pRun != NULL && !bFound)
   {
      if (isMoveDataSame(pRun, nClass, nElem))
	     bFound = !bFound;
	  else
	     pRun = pRun->Next;
   }
	
   return pRun;
}

movenode *traverseNextMoveWithSameData (movenode *pCurrent, int nClass, int nElem)
{
   /* Gets the move after the current move which has the same classification and elemental */
   int bFound = 0;
   movenode *pRun;
   pRun = pCurrent->Next;
	
   while (pRun != NULL && !bFound)
   {
      if (isMoveDataSame(pRun, nClass, nElem))
	     bFound = !bFound;
	  else
	     pRun = pRun->Next;
   }
   if (!bFound)
      return pCurrent;
   else
	  return pRun;
}

movenode *traversePrevMoveWithSameData (movenode *pCurrent, int nClass, int nElem)
{
	/* Gets the move before the current move which has the same classification and elemental */
   int bFound = 0;
   movenode *pRun;
   pRun = pCurrent->Prev;
	
   while (pRun != NULL && !bFound)
   {
      if (isMoveDataSame(pRun, nClass, nElem))
         bFound = !bFound;
	  else
	     pRun = pRun->Prev;
   }
   if (!bFound)
      return pCurrent;
   else
	  return pRun;
}

void *viewMove (movenode *pHead)
{
   /* Views moves. Can view moves by elemental, classification, or both */
   char cInput;
   int nClass = 0, nElem = 0; // 0 means viewing all elementals or classifications
   movenode *pCurrent;
   pCurrent = pHead;
	
   do
   {
      system("cls");
	  printf("Viewing");
	  if (nElem)
	  {
	  	 printf(" ");
	     printElem(nElem);
	  }
	  if (nClass)
	  {
	     printf(" ");
	     printClassification(nClass);
	  }
	  printf(" Moves\n");
	  printMove(pCurrent, 'V');
	  printf("[N]ext [P]revious [1] View By Classification [2] View By Elemental Affinity [3] View All [E]xit");
	  cInput = getch();
	  cInput = toupper(cInput);
		
	  if (cInput == 'N')
	  {
	     if (nClass || nElem)
	        pCurrent = traverseNextMoveWithSameData(pCurrent, nClass, nElem);
	     else if (pCurrent->Next != NULL)
	        pCurrent = pCurrent->Next;
	  }
	  else if (cInput == 'P')
	  {
	     if (nClass || nElem)
            pCurrent = traversePrevMoveWithSameData(pCurrent, nClass, nElem);
         else if (pCurrent->Prev != NULL)
            pCurrent = pCurrent->Prev;
	  }
	  else if (cInput == '1')
	  {
	     printf("\nView what Classification: [0] All [1] Light Attack [2] Medium Attack [3] Heavy Attack");
			
		 do
		 {
		    nClass = getch() - '0';
		 } while (nClass < 0 && nClass > 3);
			
		 pCurrent = getFirstMoveWithSameData(pHead, nClass, nElem);
			
		 if (pCurrent == NULL)
		 {
		    system("cls");
			printf("There is no move that has this classification and elemental affinity\nPress any key to continue..........");
			getch();
			nClass = 0;
			nElem = 0;
			pCurrent = pHead;
		  }
	  }
	  else if (cInput == '2')
	  {
	     printf("\nView what Elemental Afiinity: [0] All [1] Water [2] Earth [3] Air [4] Fire");
		 do
		 {
		    nElem = getch() - '0';
		 } while (nElem< 0 && nElem > 4);
			
		 pCurrent = getFirstMoveWithSameData(pHead, nClass, nElem);
			
		 if (pCurrent == NULL)
		 {
		    system("cls");
			printf("There is no move that has this classification and elemental affinity\nPress any key to continue..........");
			getch();
			nClass = 0;
			nElem = 0;
			pCurrent = pHead;
		 }
	  }
	  else if (cInput == '3')
	  {
	     nClass = 0;
		 nElem = 0;
		 pCurrent = pHead;
	  }
   } while (cInput != 'E');
}

/* Modify Move */
void modifyMove (movenode *pMoveHead, charnode *pCharHead)
{
   /* Modifies aspects of a move */
   int nFirstClass, nFirstElem, nInput; // nFirstClass and nFirstElem stores the original classification and elemental before modification
   movenode *pMove;
	
   do
   {
      pMove = selectMove(pMoveHead);
	  nFirstClass = pMove->move.classification;
	  nFirstElem = pMove->move.elemental;
	  do
	  {
	     system("cls");
		 printf("Modify Move\n");
		 printMove(pMove, 'M');
	     nInput = getch() - '0';
			
		 if (nInput == 1)
		 {
	        printf("Enter New Name: \n");
			fflush(stdin);
			fgets(pMove->move.name, sizeof(pMove->move.name), stdin);
			pMove->move.name[strlen(pMove->move.name) - 1] = '\0';
		 }
		 else if (nInput == 2)
		 { 
		    printf("Enter New Description: \n");
			fflush(stdin);
			fgets(pMove->move.description, sizeof(pMove->move.description), stdin);
			pMove->move.description[strlen(pMove->move.description) - 1] = '\0';
		 }
		 else if (nInput == 3)
		 {
		    printf("Enter New Classification: [1] Light Attack [2] Medium Attack [3] Heavy Attack");
			do
			{
			   pMove->move.classification = getch() - '0';
			} while (pMove->move.classification < 0 || pMove->move.classification > 3);
		 }
		 else if (nInput == 4)
		 {
		    printf("Enter New Elemental Affinity: [1] Water [2] Earth [3] Air [4] Fire");
			do
			{
			   pMove->move.elemental = getch() - '0';
			} while (pMove->move.elemental < 0 || pMove->move.elemental > 4);
		 }
	   } while (nInput != 5 && nInput != 6);
	   
	   /* If the classification or elemental changes, then the modified move will be replaced in characters' moveset */
	   if (pMove->move.classification != nFirstClass || pMove->move.elemental != nFirstElem) 
	      replaceMoveInChar(pMove, pMoveHead, pCharHead, nFirstClass);
	} while (nInput != 6);
}

/* Delete Move */
movenode *deleteMove (movenode *pMoveHead, charnode *pCharHead)
{
   /* Deletes a move */ 
   int nClass;
   char cAns;
   movenode *pMove, *pTemp; // both will point the deleted move, but pMove will be freed while, pTemp will be used to track the deleted move in movesets
   pMove = selectMove(pMoveHead);
   pTemp = pMove;
   nClass = pMove->move.classification;
	
   printf("\nAre you sure you want to delete this move '%s'? [Y/N] ", pMove->move.name);
   do
   {
      cAns = getch();
	  cAns = toupper(cAns);
   } while (cAns != 'Y' && cAns != 'N');
	
   if (cAns == 'Y')
   {
      if (pMove->Next != NULL)
	     pMove->Next->Prev = pMove->Prev;
	  if (pMove->Prev != NULL)
		 pMove->Prev->Next = pMove->Next;
	  else
		 pMoveHead = pMove->Next;
		
	  free(pMove);		
	  replaceMoveInChar (pTemp, pMoveHead, pCharHead, nClass);
   }
	
   return pMoveHead;
}

/* Assign Move */
movenode *selectMoveWithSameData (movenode *pHead, int nClass, int nElem)
{
  /* Selects a move for a character's moveset */
   char cInput;
   movenode *pCurrent;
   pCurrent = getFirstMoveWithSameData(pHead, nClass, nElem);
	
   if (pCurrent == NULL)
   {
      system("cls");
	  printf("There is no more move that satisfies the move classification and the elemental affinity of the character.\nPress any key to continue........");
	  getch();
	  return NULL;
   }
	
   do
   {
      system("cls");
	  printf("Select Move\n");
	  printMove(pCurrent, 'V');
	  printf("[N]ext [P]revious [S]elect Move");
	  cInput = getch();
	  cInput = toupper(cInput);
		
	  if (cInput == 'N')
	     pCurrent = traverseNextMoveWithSameData(pCurrent, nClass, nElem);
	  else if (cInput == 'P')
         pCurrent = traversePrevMoveWithSameData(pCurrent, nClass, nElem);
   } while (cInput != 'S');
	
   return pCurrent;
}

void assignMove (movenode *pMoveHead, charnode *pCharHead)
{
   /* Assign moves to a character's moveset */
   int nInput;
   charnode *pChar;
	
   do
   {
      pChar = selectChar(pCharHead);
	  do
	  {
	     system("cls");
		 printf("Assign Which?\n");
		 printf("**************************\n");
		 printMoveset(pChar, 'M');
		 printf("[4] Assign moveset to Character\n");
		 printf("[5] Done Assigning");
			
		 nInput = getch() - '0';
		 /* The move must have the same classification to classification being assigned and the same elemental as the character */
		 if (nInput > 0 && nInput < 4)
		    pChar->character.moveset[nInput - 1] = selectMoveWithSameData(pMoveHead, nInput, pChar->character.elemental); 
			
		} while (nInput != 4 && nInput != 5);
	} while (nInput != 5);
}

/* Main Move Function */
movenode *mainMoveModule (movenode *pMoveHead, charnode *pCharHead)
{
   /* Allows the player to add, view, delete, modify moves */
   char cInput;
	
   do
   {
      system("cls");
	  printf("Move Module\n");
	  printf("[A]dd Move\n");
	  printf("[V]iew Moves\n");
	  printf("[M]odify Move\n");
	  printf("[D]elete Move\n");
	  printf("[S]et Move to a Character\n");
	  printf("[E]xit\n");
	   
	  cInput = getch();
	  cInput = toupper(cInput);
	   
	  if (cInput == 'A')
	     pMoveHead = addMove(pMoveHead);
	  else if (cInput == 'V' || cInput == 'M' || cInput == 'D' || cInput == 'S')
	  {
	     if (pMoveHead == NULL)
	   	 {
	   	    system("cls");
			printf("Add a Move First\nPress any key to continue.........");
			getch();	
		 }
		  else if (cInput == 'V')
		     viewMove(pMoveHead);
		  else if (cInput == 'M')
		     modifyMove(pMoveHead, pCharHead);
		  else if (cInput == 'D')
		     pMoveHead = deleteMove(pMoveHead, pCharHead);
		  else if (cInput == 'S')
		  {
		     if (pCharHead == NULL)
		  	 {
		        system("cls");
			    printf("Add a character first in the character module\nPress any key to continue.........");
			    getch();	
			 }
			 else
		        assignMove(pMoveHead, pCharHead);
	      }
	   }
   } while (toupper(cInput) != 'E');
	
	return pMoveHead;
}

