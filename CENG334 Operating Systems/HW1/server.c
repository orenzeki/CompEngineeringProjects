#include <stdio.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include "message.h"
#include "logging.h"
#include "logging.c"
#include <sys/socket.h>
#include<poll.h>
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)


int main(int argc, char** argv){
	
	int str_bid = 0, min_inc = 0, num_bid = 0, num_bid2 = 0;
	int fd[num_bid][2];
	char name[100];
	char *buffer_iterator;
    	int current_bid = 0;
    	int r = 0, result =0, bidder_counter = 0, win_bid = 0;
    	int ch_stat;
	cm cm1;
	sm sm1;
	smp smp1;
	cei cei1;
	bi bi1;
	wi wi1;
	oi oi1;
	ii ii1;
	scanf("%d %d %d", &str_bid, &min_inc, &num_bid);
	pid_t pid[num_bid];
	int client_id[num_bid];
    	int control[num_bid];
    	int bid_number = num_bid;
	for(int i=0; i<num_bid; i++){
        control[i] = 1;
		client_id[i] = i+1;
		PIPE(fd[i]);
		scanf("%s %d", name, &num_bid2);
		int bid_param[num_bid2]; 
		char* bid_args[num_bid2+2];
		bid_args[0] = name;
		bid_args[num_bid2+1]=NULL;
		
		for(int j=0; j<num_bid2; j++){
			buffer_iterator = malloc(sizeof(char*));
			scanf("%d", &bid_param[j]);
			sprintf(buffer_iterator,"%d", bid_param[j]);
			bid_args[j+1] = buffer_iterator;  
		}
		pid[i] = fork();
		if(pid[i] == 0){
			dup2(fd[i][0],0);
			dup2(fd[i][0],1);
			close(fd[i][0]);
			dup2(fd[i][1],0);
			dup2(fd[i][1],1);
			close(fd[i][1]);
			
			execv(bid_args[0], bid_args);
		
		}else if(pid[i] > 0){
		}	
		else{
			perror("fork");
			abort();
		}
	}

	
	for (int i = 0; i < num_bid; i++){
		read(fd[i][0],&cm1,sizeof(cm1));
		ii1.pid = pid[i];
		ii1.type = cm1.message_id;
		ii1.info.bid = cm1.params.bid;
		ii1.info.delay = cm1.params.delay;
		ii1.info.status = cm1.params.status;
		
		print_input(&ii1, client_id[i]);
		
		sm1.message_id = 1;
		sm1.params.start_info.client_id = client_id[i];
		
		sm1.params.start_info.current_bid = 0;
		sm1.params.start_info.starting_bid = str_bid;
		sm1.params.start_info.minimum_increment = min_inc;
		oi1.pid = pid[i];
		oi1.type = sm1.message_id;
		oi1.info.start_info.client_id = sm1.params.start_info.client_id;
		oi1.info.start_info.current_bid = sm1.params.start_info.current_bid;
		oi1.info.start_info.minimum_increment = sm1.params.start_info.minimum_increment;
		oi1.info.start_info.starting_bid = sm1.params.start_info.starting_bid;
		write(fd[i][0],&sm1,sizeof(sm1));

		print_output(&oi1, client_id[i]);
	}
    struct pollfd pfd[num_bid];
    for (int k = 0; k < num_bid; ++k) {
        pfd[k].fd = fd[k][0];
        pfd[k].events = POLLIN;
        pfd[k].revents = 0;
    }
	while(1){
        if(bidder_counter == num_bid){
            print_server_finished(win_bid, current_bid);
            for (int i = 0; i < num_bid; ++i) {
                sm1.message_id = 3;
                sm1.params.winner_info.winner_id = win_bid;
                sm1.params.winner_info.winning_bid = current_bid;
                write(fd[i][0], &sm1, sizeof(sm1));
                oi1.pid = pid[i];
                oi1.type = 3;
                oi1.info.winner_info.winner_id = win_bid;
                oi1.info.winner_info.winning_bid = current_bid;
                print_output(&oi1,client_id[i]);
            }
            break;
        }
        for (int i = 0; i < num_bid; i++) {
            if(control[i] == 0){
                continue;
            }

            poll(pfd, num_bid, 0);
            if (pfd[i].revents & POLLIN) {

                r = read(pfd[i].fd, &cm1, sizeof(cm1));
                if(r == 0){
                    continue;
                }
                else {
                    if(cm1.params.bid < str_bid){
                        result = 1;
                    }
                    else if(cm1.params.bid < current_bid){
                        result = 2;
                    }
                    else if((cm1.params.bid - current_bid) < min_inc){
                        result = 3;
                    }
                    else{
                        result = 0;
                        current_bid = cm1.params.bid;
                        win_bid = client_id[i];
                    }

                    ii1.pid = pid[i];
                    ii1.type = cm1.message_id;
                    ii1.info.bid = cm1.params.bid;
                    print_input(&ii1, client_id[i]);
                    if(cm1.message_id == 3 && cm1.params.status == 0){
                        control[i] = 0;
                        bidder_counter++;
                    }

                }
                if(cm1.message_id != 3){
                    sm1.params.result_info.current_bid = current_bid;
                    sm1.params.result_info.result = result;
                    sm1.message_id = 2;
                    oi1.pid = pid[i];
                    oi1.info.result_info.result = result;
                    oi1.info.result_info.current_bid = current_bid;
                    oi1.type = 2;
                    write(pfd[i].fd,&sm1,sizeof(sm1));
                    print_output(&oi1, client_id[i]);
                }

            }
        }
	}

	int status;
	int status_match;
	for(int i=0; i<num_bid; i++){
		pid_t wait_pid = wait(&ch_stat);
		if(WIFEXITED(ch_stat)){
		    status=0;
		    status_match=1;
		}else{
		    status=ch_stat;
		    status_match=0;
		}

		print_client_finished(client_id[i],status,status_match);
	}
	
	return 0;
}
