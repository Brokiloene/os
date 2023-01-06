#include "list.h"
#include "lab6_utils.h"

#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork
#include <string.h> //strcmp
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    check_null(requester, "requester error");
    
    List *childs = list_create();
    check_null(childs, "list_create error");

    char cmd[10] = {'0'};
    int c;
    printf("> ");
    fflush(stdout);
    while((c = getchar()) != EOF) {
        printf("> ");
        fflush(stdout);
        
        ungetc(c, stdin);

        //printf("[mn]: list: ");
        //print_list(childs);
        

        scanf("%s", cmd);
        if (strcmp(cmd, "create") == 0) {
        	//print_list(childs);

            int id, parent_id;
            scanf("%d %d", &id, &parent_id);
            id += PORT;


            if (parent_id == -1) {
                if (list_find(childs, id) == 1 || echo(childs, id, requester) != -1) {
                    printf("bad child id\n"); 
                    continue;   
                }

                int pid = fork();
                check_neg_one(pid, "fork error");

                if (pid == 0) {
                	zmq_close(requester);
    				zmq_ctx_destroy(context);
    				list_destroy(childs);

                    char id_str[8];
                    memset(id_str, 0, 8);
                    sprintf(id_str, "%d", id);
                    char *argvc[] = {"computing_node", id_str, NULL};
                    check_neg_one(execv("computing_node", argvc), "execv computing_node error");
                }

                //id -= PORT;
                //printf("[mn59]: %s %d %d\n", cmd, id, parent_id);
                sleep(1); // если создать/удалить/снова создать процесс, ping не сработает (???)
                //printf("ping= %d\n", ping(id));
                if (ping(id) == 1) {
	                push_back(childs, id);
	                printf("computing_node № %d with pid [%d] has been created\n", id, pid);
	            }
            } 

            else {
            	parent_id += PORT;
                // if (list_find(childs, parent_id) == 0 || list_find(childs, id) == 1) {
                //  printf("bad parent/child id\n"); 
                //  continue;
                // }

            	//printf("I am here\n");
               
                if (list_find(childs, parent_id) == 1 && ping(parent_id) == 1) {
                	void *requester2 = zmq_socket(context, ZMQ_REQ);
                    Message m = {"create", parent_id, id, ""};

                    //printf("childid=%d, parent_id=%d\n", id, parent_id);

                    char address[32];
                    client_address_gen(parent_id, address);
                    //printf("address=%s", address);
                    check_neg_one(zmq_connect(requester2, address), "zmq_connect error");
                    send_msg(&m, requester2);
                    recv_msg(&m, requester2);

                    zmq_close(requester2);

                    //push_back(childs, m.id);

                    if (strcmp(m.cmd, "bad") == 0) {
                        printf("%s: error with creating [%d] by [%d]\n", m.cmd, m.id, m.num);
                    } else {
                        printf("%s: [%d] was created by [%d]\n", m.cmd, m.id, parent_id);
                    }
                }
                else {
                 	//printf("I am here\n");
            		// break;
                    int cur_id = echo(childs, parent_id, requester);

                    //printf("[mn]: cur_id = %d\n", cur_id);

                    if (cur_id == -1 || ping(cur_id) != 1) {
                        printf("bad id\n");
                        continue;
                    }

                    Message m = {"create", parent_id, id, ""};

                    char address[32];
                    client_address_gen(cur_id, address);
                    check_neg_one(zmq_connect(requester, address), "zmq_connect error");
                    send_msg(&m, requester);
                    recv_msg(&m, requester);

                    //printf("[mn110]: %s %d %d\n", m.cmd, m.id, m.num);

                    if (strcmp(m.cmd, "bad") == 0) {
                        printf("%s: error with creating [%d] by [%d]\n", m.cmd, m.id, m.num);
                    }
                    else if (strcmp(m.cmd, "ok") == 0) {
                        printf("%s: [%d] was created by [%d] via [%d]\n", m.cmd, m.id, parent_id, cur_id);
                    }

                }

            }
        } else if (strcmp(cmd, "exec") == 0) {
            int id;
            scanf("%d", &id);
            id += PORT;
            getchar();  // scanf сам не считывает '\n'

            char *pattern = (char *) malloc(sizeof(char) * 1024), *text = NULL;  
            size_t len = 0;
            int pattern_size = getline(&pattern, &len, stdin);
            int text_size    = getline(&text,    &len, stdin);

            pattern_size -= 1;    // getline выдает количество считанных символов
            pattern[pattern_size] = '#';  // " для char *, ' для char
            char *all    = strcat(pattern, text);

            Message m = {"exec", pattern_size, id, ""};
            memset(m.str, 0, 1024);
            memcpy(m.str, all, strlen(all));

            //printf("[mn]: exec id=%d, lf=%d\n", id, list_find(childs, id));
            

            if (list_find(childs, id) == 1 && ping(id) == 1) {
            	void *requester2 = zmq_socket(context, ZMQ_REQ);
            	//printf("145\n");
            	char address[32];
	            client_address_gen(id, address);
	            //printf("[mn]: address=%s\n", address);
	            check_neg_one(zmq_connect(requester2, address), "zmq_connect error");
	            //printf("152\n");
	            send_msg(&m, requester2);
	            //printf("154\n");
	            recv_msg(&m, requester2);
	            //printf("156\n");

                if (strcmp(m.cmd, "ok") == 0) {
                    printf("%s:\n%s \nproduced by [%d]\n", m.cmd, m.str, m.id);
                } else if (strcmp(m.cmd, "0 matches") == 0) {
                    printf("%s\n", m.cmd);
                }


           	 	//printf("%s:\n%s \nproduced by [%d]\n", m.cmd, m.str, m.id);
           	 	zmq_close(requester2);
            } else {
            	//printf("160\n");
            	void *echo_requester = zmq_socket(context, ZMQ_REQ);
            	int cur_id = echo(childs, id, echo_requester);
            	zmq_close(echo_requester);

                //printf("[mn]: cur_id = %d\n", cur_id);

                if (cur_id == -1 || ping(cur_id) == -1) {
                    printf("bad id\n");
                    free(pattern);
        			free(text);
                    continue;
                }

                char address[32];
                client_address_gen(cur_id, address);
                //printf("[mn]: address=%s\n", address);
                check_neg_one(zmq_connect(requester, address), "zmq_connect error");
                send_msg(&m, requester);
                recv_msg(&m, requester);

                //printf("[mn110]: %s %d %d\n", m.cmd, m.id, m.num);

                if (strcmp(m.cmd, "bad") == 0) {
                    printf("bad id\n");
                    //printf("180\n");
                }
                else if (strcmp(m.cmd, "ok") == 0) {
                    printf("%s:\n%s \nproduced by [%d]\n", m.cmd, m.str, m.id);
                } else if (strcmp(m.cmd, "0 matches") == 0) {
                    printf("%s\n", m.cmd);
                }
            }
        	free(pattern);
        	free(text);

        }

        else if (strcmp(cmd, "ping") == 0) {
            int id;
            scanf("%d", &id);
            id += PORT;
            printf("[%d]: %d\n", id, ping(id));
        }
        else if (strcmp(cmd, "remove") == 0) {

            int id;
            scanf("%d", &id);
            id += PORT;

            Message m = {"remove", 0, id, ""};

            //printf("[mn]: %s %d %d\n", m.cmd, m.id, m.num);

            if (list_find(childs, id) == 1 && ping(id) == 1) {

            	void *requester2 = zmq_socket(context, ZMQ_REQ);

           		
	            char address[32];
	            client_address_gen(id, address);
	            check_neg_one(zmq_connect(requester2, address), "zmq_connect error");
	            send_msg(&m, requester2);
	            recv_msg(&m, requester2);

	            list_delete(childs, id);

	            zmq_close(requester2);

	            printf("%s: [%d] with pid %d has been removed\n", m.cmd, m.id, m.num);
	            wait(NULL);  // а то будет зомби
	            
            } else {
            	int cur_id = echo(childs, id, requester);

                //printf("[mn]: cur_id = %d\n", cur_id);

                if (cur_id == -1 || ping(cur_id) == -1) {
                    printf("bad id\n");
                    continue;
                }

                char address[32];
                client_address_gen(cur_id, address);
                check_neg_one(zmq_connect(requester, address), "zmq_connect error");
                send_msg(&m, requester);
                recv_msg(&m, requester);

                if (strcmp(m.cmd, "bad") == 0) {
                        printf("%s: cannot remove %d", m.cmd, m.id);
                }
                else if (strcmp(m.cmd, "ok") == 0) {
                    printf("%s: [%d] with pid %d has been removed via [%d]\n", m.cmd, m.id, m.num, cur_id);
                }

            }
        }
        else if (strcmp(cmd, "heartbit") == 0) {
        	int delay;
            scanf("%d", &delay);


            for (int i = 0, sz = size(childs); i < sz; ++i) {
            	int cur_id = list_get(childs, i);
            	check_neg_one(cur_id, "get error");

            	//printf("[mn]: delay=%d cur_id=%d\n", delay, cur_id);

            	int answers = 0;
            	for (int j = 0; j < 4; ++j) {
            		if (ping(cur_id) == 1) {
            			answers += 1;
            		}
            		usleep(delay * 1000);
            	}
				
				if (answers == 4) {
					printf("[%d]: ready\n", cur_id);

					Message m = {"heartbit", delay, 0, ""};

					char address[32];
					client_address_gen(cur_id, address);

					check_neg_one(zmq_connect(requester, address), "zmq_connect error");

					send_msg(&m, requester);
					recv_msg(&m, requester);

					zmq_disconnect(requester, address);
				} else {
					printf("[%d]: bad\n", cur_id);
				}

				

            }
        }
    }

    for (int i = 0, sz = size(childs); i < sz; ++i) {
    	void *requester2 = zmq_socket(context, ZMQ_REQ);
		int cur_id = list_get(childs, i);
		check_neg_one(cur_id, "get error");
		if (ping(cur_id) == 1) {

			Message m2 = {"remove", 0, cur_id, ""};

			char address[32];
			client_address_gen(cur_id, address);

			check_neg_one(zmq_connect(requester2, address), "zmq_connect error");

			send_msg(&m2, requester2);
			recv_msg(&m2, requester2);

			zmq_close(requester2);
		}
	}
    
    zmq_close(requester);
    zmq_ctx_destroy(context);
    list_destroy(childs);

    return 0;
}