#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#include "functions.h"
int main(){
    int login_status = 0;
    USER *uhead = (USER*)malloc(sizeof(USER));
    POSTS *phead = (POSTS*)malloc(sizeof(POSTS));
    LIKERS *lhead = (LIKERS*)malloc(sizeof(LIKERS));
    USER *cur_user =(USER*)malloc(sizeof(USER));
    cur_user = NULL;
    uhead->username = "a";
    uhead->password = "0";
    uhead->last_post_id = 0;
    uhead->next = NULL;
    phead->username = "a";
    phead->content = "a";
    phead->post_id = 0;
    phead->next = NULL;
    lhead->post_owner_username = "a";
    lhead->post_liker_username = "b";
    lhead->post_id = 0;
    lhead->next = NULL;
    while(1){
        system("cls");
        if(login_status == 1){
            printf("login_status: logged in as %s\n", cur_user->username);
        }
        else{
            printf("login_status: not logged in\n");
        }
        printf("please enter your command: ");
        char *str = (char *)malloc(sizeof(char));
        int string_length = 0, num_of_words, command;
        char ch;
        ch = getchar();
        while (ch != '\n'){
        *(str + string_length) = ch;
        string_length++;
        str = realloc(str, (string_length + 1) * sizeof(char));
        ch = getchar();
        }
        str[string_length] = '\0';
        num_of_words = number_of_words(str);
        command = CommandStatus(str, &login_status, num_of_words);
        if(command == 1){
            int str_len = strlen(str);
            int thirdwordfirstindex = nthwordfirstindex(str, 3);
            if(thirdwordfirstindex == str_len){
                printf("there is no password in order to signup");
            }
            else{
                if(username_search(uhead, str) != NULL){
                    printf("username already occupied");
                }
                else{
                    int signup_check = InsertUser(&uhead, str);
                    if(signup_check == 1){
                        printf("signup successful.");
                    }
                    else if(signup_check == -1){
                        printf("invalid username. you can not signup with <<login>> as username.");
                    }
                    else if(signup_check == 0){
                        printf("username already occupied.");
                    }
                }
            }
        }
        else if(command == 2){
            int str_len = strlen(str);
            int thirdwordfirstindex = nthwordfirstindex(str, 3);
            if(thirdwordfirstindex == str_len){
                printf("there is no password in order to login");
            }
            else{
                cur_user = user_search(uhead, str);
                if(cur_user == NULL){
                    printf("account not found. either username or password or both are incorrect.");
                }
                else{
                    printf("you successfully logged in as %s.", cur_user->username);
                    login_status = 1;
                }
            }
        }
        else if(command == 3){
            int str_len = strlen(str);
            int secondwordfirstindex = nthwordfirstindex(str, 2);
            if(secondwordfirstindex == str_len){
                printf("the content of the post is empty.");
            }
            else{
                InsertPost(&phead, &cur_user, str);
                printf("post succesfully posted");
            }
        }
        else if(command == 4){
            int str_len = strlen(str);
            int thirdwordfirstindex = nthwordfirstindex(str, 3);
            if(thirdwordfirstindex == str_len){
                printf("there is no post id in order to like.");
            }
            else{
                POSTS *desired_post = PostSearchForLike(phead, str);
                if(desired_post == NULL){
                    printf("post not found.");
                }
                else{
                    int repetitive_check = RepetitiveLike(lhead, cur_user, desired_post);
                    if(repetitive_check == 1){
                        printf("you have liked this post before.");
                    }
                    else{
                        InsertLike(&lhead, &desired_post, cur_user);
                        printf("desired post liked successfully");
                    }
                }
            }
        }
        else if(command == 5){
            login_status = 0;
            cur_user = NULL;
            printf("ypu logged out of your account successfully");
        }
        else if(command == 6){
            int post_id_check = PostIDToDelete(phead, str);
            if(post_id_check == 0){
                printf("post id invalid!");
            }
            else{
                int delete_post_check = DeletePost(&phead, cur_user, post_id_check);
                if(delete_post_check == 1){
                    printf("post deleted succefully.");
                }
                else if(delete_post_check == 0){
                    printf("post not found.");
                }
            }
        }
        else if(command == 7){
            info(phead, cur_user);
        }
        else if(command == 8){
            find_user(uhead, phead, str);
        }
        else{
            printf("invalid command!");
        }
        free(str);
        printf("\npress any key to continue.");
        getch();
    }
    return 0;
}