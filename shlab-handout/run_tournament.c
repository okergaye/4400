#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

static void run_tournament(int seed, int rounds, int num_progs, char **progs);

int main(int argc, char **argv)
{
  int rounds, seed;

  if (argc < 4)
  {
    fprintf(stderr, "%s: expects <random-seed> <round-count> <player-program> <player-program> ...\n", argv[0]);
    return 1;
  }

  seed = atoi(argv[1]);
  if (seed < 1)
  {
    fprintf(stderr, "%s: bad random seed; not a positive number: %s\n", argv[0], argv[1]);
    return 1;
  }

  rounds = atoi(argv[2]);
  if (rounds < 1)
  {
    fprintf(stderr, "%s: bad round count; not a positive number: %s\n", argv[0], argv[2]);
    return 1;
  }

  run_tournament(seed, rounds, argc - 3, argv + 3);

  return 0;
}

static size_t seven = 7;

typedef struct 
{
  int wins;
  int loses;
  int faliures;
  int meToPlayer[2];
  int playerToMe[2];
  int ID;
  char start[(size_t) 7];
  char* name;
  int skip;
  int winner;
  int status;
}PlayerData;

void AssignWinnersAndLosers(int i,  PlayerData* player, int playerCount){
  int j;
  // int status;
         for(j = 0; j < playerCount; j++)
         {
                  
            if (i != j) {
              player[j].loses += 1;

              kill(player[j].ID, SIGKILL);

            }else{
              player[j].wins += 1;
              player[j].winner = 1;
            }         
             
         }

}

void AssignFAndL(int i,  PlayerData* player, int playerCount){
  int j;
  // int status;
         for(j = 0; j < playerCount; j++)
         {
                  
            if (i != j) {
             player[j].faliures += 1;
             player[j].loses += 1;
             player[j].skip = 1;
            //  kill(player[j].ID, SIGKILL);

            }else{
              

     
            }         
             
         }

}
//seed is seed, rounds is for the tounry, n is player count progs is player_program_names
static void run_tournament(int seed, int rounds, int n, char **progs)
{
  int playerCount = n;
  int round = 0;

  // set up the game with the input
  char *seedString = malloc(1 * sizeof(char));
  char *playerCountString = malloc(1 * sizeof(char));
  sprintf(playerCountString, "%d", n);
  int meToMaker[2];
  int makerToMe[2];
  char buffer[seven];
  int status;
  ssize_t got;
  // set up player W-L-F
   PlayerData player[n];
  int i;
  for (i = 0; i < n; i++)
  {
    player[i].wins = 0;
    player[i].loses = 0;
    player[i].faliures = 0;
    player[i].ID = 0;
    player[i].name = malloc(10*sizeof(char));
    player[i].skip = 0;
    player[i].winner = 0;
    // player[i].status = -1;
   
  }
  ///////begin war game
  while (round < rounds)
  {
    sprintf(seedString, "%d", seed + round);
    // // printf("seedString: %s R; %d\n", seedString, round);
    char *args[] = {"./game_maker", seedString, playerCountString, "40", "45", NULL};
    //Me to maker
    Pipe(meToMaker);
    Pipe(makerToMe);
    
    for( i = 0; i < playerCount; i++)
    {
      Pipe(player[i].meToPlayer);
      Pipe(player[i].playerToMe);
    }
    

    int gameID = Fork(); //Game Process
    if (gameID == 0)
    {
      Dup2(meToMaker[0], 0); //takes right arg (std::out) and repoitns it to a pipes out
      Close(meToMaker[1]);
      Dup2(makerToMe[1], 1);
      Close(makerToMe[0]);
      Execve(args[0], args, environ);
    }
     Close(meToMaker[0]);
     Close(makerToMe[1]);


    // // printf("GameID: %d\n", gameID);               // What game we are in. Lobby name.
    got = Rio_readn(makerToMe[0], buffer, seven); // throw away win postion
    buffer[got] = 0;
    // // printf("\n\nWin Position: %s", buffer);


    for (i = 0; i < playerCount; i++)
    {
      got = Rio_readn(makerToMe[0], buffer, seven);
      buffer[got] = 0;
      // printf("Player %d: %s", i, buffer); // player one postion.
      // strcpy(player[i].start, buffer);
      
      char *playerArgs[] = {progs[i], NULL}; // start a player with his starting pos
      strcpy(player[i].name, progs[i]);
      //printf("%s  %s",player[i].name, buffer);
      player[i].ID = Fork();

      if (player[i].ID == 0)
      {
        player[i].winner = 0;
        player[i].skip = 0;
        Dup2(player[i].meToPlayer[0], 0);
        Close(player[i].meToPlayer[1]);

        Dup2(player[i].playerToMe[1], 1);
        Close(player[i].playerToMe[0]);

        Execve(playerArgs[0], playerArgs, environ);
      }
       Close(player[i].meToPlayer[0]);
       Close(player[i].playerToMe[1]);
       Rio_writen(player[i].meToPlayer[1], buffer, seven);

    }
  
    //this next sec is about... play ball
    int count = 0;
    while (strcmp(buffer, "winner\n") != 0)
    {

      for (i = 0; i < playerCount; i++)
      {
            //  printf("SOB: test %d\n", count);
            
            if (player[i].skip) {
              continue;
            }
            

        got = Rio_readn(player[i].playerToMe[0], buffer, seven);
        // buffer[got] = 0;
        Rio_writen(meToMaker[1], buffer, seven);
        got = Rio_readn(makerToMe[0], buffer, seven);
        // buffer[got] = 0;
        Rio_writen(player[i].meToPlayer[1], buffer, seven);
        // printf("%s  given %s",player[i].name, buffer);

        if(strcmp(buffer, "winner\n") == 0){
         AssignWinnersAndLosers(i, player, playerCount);// stuipd struct cant work
        //  i = playerCount+5;
         break;

        }else if(strcmp(buffer, "wrong!\n") == 0){
          AssignFAndL(i, player, playerCount);// stuipd struct cant work
        //  i = playerCount+5;
         break;

        }

        
       // printf("after %s\n", buffer);

        

      }

      count++;
    }

 

    // // printf("WHILE OVER %d\n", count);
 
     
    
    Waitpid(gameID, &status, 0); // waiting for game to end
    printf("Game Status Code: %d\n", status);
     Close(meToMaker[1]);
     Close(makerToMe[0]);
     
     
     for( i = 0; i < playerCount; i++)
     {
       Close(player[i].meToPlayer[1]);
       Close(player[i].playerToMe[0]);
       
      Waitpid(player[i].ID,&player[i].status,0);
       if(player[i].status != 0 && player[i].winner){
          player[i].faliures += 1;

       }
       printf("Player Status Code: %d winner: %d\n", player[i].status, player[i].winner);
     }
 
    

    round++;
  } // end of round while loop

  ////////////////////The great divide///////////////////////////////////////////////

  ////////////////////Start of game report//////////////////////////////////////
  // // printf("End of Game Report:\n");
  printf("%d\n", rounds);
  int p;
  for (p = 0; p < playerCount; p++)
  {
    //printf("Name: %s %d %d %d\n",player[p].name, player[p].wins, player[p].loses, player[p].faliures);
    printf("%d %d %d\n", player[p].wins, player[p].loses, player[p].faliures);
  }
  ////////////////////end of game report///////////////////////////////

  // fprintf(stderr, "run tournament is not implemented\n");
}
