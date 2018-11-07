#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

static void run_tournament(int seed, int rounds, int num_progs, char **progs);

int main(int argc, char **argv)
{
  int rounds, seed;

  if (argc < 4) {
    fprintf(stderr, "%s: expects <random-seed> <round-count> <player-program> <player-program> ...\n", argv[0]);
    return 1;
  }
      
  seed = atoi(argv[1]);
  if (seed < 1) {
    fprintf(stderr, "%s: bad random seed; not a positive number: %s\n", argv[0], argv[1]);
    return 1;
  }

  rounds = atoi(argv[2]);
  if (rounds < 1) {
    fprintf(stderr, "%s: bad round count; not a positive number: %s\n", argv[0], argv[2]);
    return 1;
  }
  
  run_tournament(seed, rounds, argc - 3, argv + 3);

  return 0;
}



static size_t seven = 7; 

struct PlayerData
{
  int wins;
  int loses;
  int faliures;
  int meToPlayer[2];
  int playerToMe[2];
  int ID;
  char start[seven];
};

//seed is seed, rounds is for the tounry, n is player count progs is player_program_names
static void run_tournament(int seed, int rounds, int n, char **progs)
{
  int playerCount =n;
  int round = 0;
  // set up the game with the input
  char* seedString =  malloc(1*sizeof(char)); 
  char* playerCountString =  malloc(1*sizeof(char)); 
  sprintf(playerCountString, "%d", n); 
  //sprintf(seedString, "%d", seed); 
  
  // set up player W-L-F
  struct PlayerData player[n];
  int i;
  for(i = 0; i < n; i++)
  {
    player[i].wins = 0;
    player[i].loses = 0;
    player[i].faliures = 0;
    player[i].ID = 0;
    Pipe(player[i].meToPlayer);
    Pipe(player[i].playerToMe);


  }
  char buf[1];
  ///////begin war game
  while(round < rounds){
    sprintf(seedString, "%d", seed+round); 
    char* args[] = {"./game_maker", seedString, playerCountString, "5", "5", NULL};

    int meToMaker[2];
    int makerToMe[2];
    // int meToPlayer[2];
    // int playerToMe[2];
    char buffer[seven];
    ssize_t got;
    int status;

    //Me to maker
    Pipe(meToMaker);
    Pipe(makerToMe);
    //Me to Player
    //Pipe(meToPlayer);
   // Pipe(playerToMe);
    //Game process
    int gameID = Fork();
    if(gameID == 0){
      Dup2(meToMaker[0],0); //takes right arg (std::out) and repoitns it to a pipes out
      Close(meToMaker[1]);
      Dup2(makerToMe[1],1); 
      Close(makerToMe[0]);
      Execve(args[0], args, environ);
    }
    Close(meToMaker[0]);
    Close(makerToMe[1]);

    printf("GameID: %d\n",gameID);// What game we are in. Lobby name.
    got = Rio_readn(makerToMe[0], buffer, seven); // throw away win postion
    buffer[got] = 0;
    printf("Win Position: %s",buffer);
    
    int i;
    for(i = 0; i < playerCount; i++)
    {
      got = Rio_readn(makerToMe[0], buffer, seven);
      buffer[got] = 0;
      printf("Player %d: %s",i,buffer); // player one postion.
     // player[i].start = buffer;
      strcpy(player[i].start,buffer);
      //player[i].name = args[i+3];

      char* playerArgs[] = {progs[i+3], NULL}; // start a player with his starting pos
      player[i].ID = Fork();
      if(playerID == 0){
        Dup2(player[i].meToPlayer[0],0);
        Close(player[i].meToPlayer[1]);
        Dup2(player[i].playerToMe[1],1); 
        Close(player[i].meToPlayer[0]);
        Execve(playerArgs[0], playerArgs, environ);
      }
      Close(player[i].meToPlayer[0]);
      Close(player[i].meToPlayer[1]);
    }

    
    
    printf("PlayerID: %d\n",playerID);



    int count = 0;

    while(strcmp(buffer, "winner\n") != 0){
     // printf("SOB: test %d\n",count); 
      Rio_writen(meToPlayer[1], buffer, seven);
      got = Rio_readn(playerToMe[0], buffer, seven);
      buffer[got] = 0;
      Rio_writen(meToMaker[1], buffer, seven);
      got = Rio_readn(makerToMe[0], buffer, seven);
      buffer[got] = 0;

      // Rio_writen(meToPlayer[1], buffer, seven);
      count++;
    }

    //send the last thing. aka the win condin
    Rio_writen(meToPlayer[1], buffer, seven);

    printf("WHILE OVER\n"); 
    Close(meToPlayer[1]); 
    Close(playerToMe[0]);
    Close(meToMaker[1]);
    Close(makerToMe[0]);
    Waitpid(playerID, &status, 0); // waiting for game to end
    //buf[0] = WEXITSTATUS(status1) + '0';
    //printf("\nPlayer Status Code: %d\n",buf[0]);
    printf("\nPlayer Status Code: %d\n",status);

    
    Waitpid(gameID, &status, 0); // waiting for game to end
    //buf[0] = WEXITSTATUS(status2) + '0';
    //printf("Game Status Code: %d\n",buf[0]);
    printf("Game Status Code: %d\n", status);

    //maybe will be used to tally score
    switch (status)
    {
      case 0:
        player[0].wins += 1;
        break;

      default:
        break;
    }


    round++;
  }// end of round while loop


////////////////////The great divide///////////////////////////////////////////////


////////////////////Start of game report//////////////////////////////////////
  printf("End of Game Report:\n");
  printf("%d\n", rounds);
  int p;
  for(p = 0; p < n; p++)
  {
    printf("%d %d %d\n", player[p].wins, player[p].loses, player[p].faliures);
  }
///////////////////end of game report///////////////////////////////



 // fprintf(stderr, "run tournament is not implemented\n");
}
