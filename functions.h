#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
struct user {
    char *username;
    char *password;
    int last_post_id;
    struct user *next;
};
struct posts {
    char *username;
    char *content;
    int post_id;
    int likes;
    struct posts *next;
};
typedef struct likers {
    char *post_owner_username;
    char *post_liker_username;
    int post_id;
    struct likers *next;
};
typedef struct user USER;
typedef struct posts POSTS;
typedef struct likers LIKERS;
int nthwordfirstindex(char* string, int word_number){ // a function that returns the first index of the word number n.
    int space_counter = 0, i = 0;
    while(space_counter != (word_number - 1)){
        if(*(string + i) == ' '){
            space_counter++;
        }
        else if(*(string + i) == '\0'){
            return -1;
        }
        i++;
    }
    return i;
}
int number_of_words(char* string){// a function that returns number of word in a given string.
    int i = 0, count = 1;
    for (i = 0;string[i] != '\0';i++){
        if (string[i] == ' ' && string[i+1] != ' '){
            count++; 
        }    
    }
    return count;
}
int CommandStatus(char *str, int *login_status, int number_of_words) // a function that transforms different commands to integers in order to pass them to the next functions.
{
    int length_word1 = 0, signup_check = 1, login_check = 1, post_check = 1, like_check = 1, logout_check = 1, delete_check = 1, info_check = 1, find_user_check = 1;
    int order = 0;
    while (*(str + length_word1) != ' ')// splitting the string until the fist space.
    {
        if (length_word1 == strlen(str))
        {
            break;
        }
        length_word1++;
    }
    char *command = (char *)malloc((length_word1 + 1) * sizeof(char));// allocating memory for the command string.
    for (int i = 0; i < length_word1; i++)
    {
        command[i] = str[i];
    }
    command[length_word1] = '\0';
    // determining command status.
    if (!strcmp(command, "signup"))
    {
        order = 1;
    }
    else if (!strcmp(command, "login"))
        order = 2;
    else if (!strcmp(command, "post"))
        order = 3;
    else if (!strcmp(command, "like"))
        order = 4;
    else if (!strcmp(command, "logout"))
        order = 5;
    else if (!strcmp(command, "delete"))
        order = 6;
    else if (!strcmp(command, "info"))
        order = 7;
    else if (!strcmp(command, "find_user"))
        order = 8;
    // handling commands with determining login stats.
    if ((*login_status == 1) && (order == 1))
    {
        order = 0;
        printf("\nYou have to logout of your current account in order to register another account");
    }
    else if ((*login_status == 1) && (order == 2))
    {
        order = 0;
        printf("\nYou have to logout of your current account in order to login to another account");
    }
    else if ((*login_status == 0) && (order == 3))
    {
        order = 0;
        printf("\nYou have to login to an account in order to be able to post");
    }
    else if ((*login_status == 0) && (order == 4))
    {
        order = 0;
        printf("\nYou have to login to an account in order to be able to like a post");
    }
    else if ((*login_status == 0) && (order == 5))
    {
        order = 0;
        printf("\nYou can't logout when you are not logged into an account");
    }
    else if ((*login_status == 0) && (order == 6))
    {
        order = 0;
        printf("\nYou can't delete a post when you are not logged into an account");
    }
    else if ((*login_status == 0) && (order == 7))
    {
        order = 0;
        printf("\nYou can't see the info of an account when you are not logged into the account");
    }
    else if ((*login_status == 0) && (order == 8))
    {
        order = 0;
        printf("\nYou can't find a user when you are not logged into the account");
    }
    if((order == 1) && (number_of_words != 3)){
        order = 0;
        printf("\nInvalid command");
    }
    else if((order == 2) && (number_of_words != 3)){
        order = 0;
        printf("\nInvalid command");
    }
    else if((order == 3) && (number_of_words < 2)){
        order = 0;
        printf("\nThe post content is empty");
    }
    else if((order == 4) && (number_of_words != 3)){
        order = 0;
        printf("\nInvalid command");
    }
    else if((order == 5) && (number_of_words != 1)){
        order = 0;
        printf("\nInvalid command");
    }
    else if((order == 6) && (number_of_words != 2)){
        order = 0;
        printf("\nInvalid command");
    }
    else if((order == 7) && (number_of_words != 1)){
        order = 0;
        printf("\nInvalid command");
    }
    else if((order == 8) && (number_of_words != 2)){
        order = 0;
        printf("\nInvalid command");
    }
    free(command);
    return order;
}
USER* username_search(USER *uhead, char* string){ // a function that searches the USER linked list for a specific username. and returns the user's structure
// if found, otherwise returns NULL.
    int username_check;
    int string_length = strlen(string);
    int secondword_first_index = nthwordfirstindex(string, 2);
    int thirdword_first_index = nthwordfirstindex(string, 3);
    char* username = (char*)malloc((thirdword_first_index - secondword_first_index) * sizeof(char));
    for(int i = secondword_first_index; i < (thirdword_first_index - 1); i++){
        *(username + (i - secondword_first_index)) = *(string + i);
    }
    *(username + ((thirdword_first_index - secondword_first_index) - 1)) = '\0';
    USER* cur_user=uhead->next;
    while (cur_user != NULL){
        username_check = strcmp(cur_user -> username, username);
        if (username_check == 0){
            return cur_user; 
        }
        cur_user = cur_user->next;
    }
    free(username);
    return NULL;
}
USER* user_search(USER *uhead, char* string){// function that searches the USER linked list for a specific user by splitting the given string into
// username and password and checking each node of the USER linked list. if found it returns th user's node, otherwise it returns NULL.
    int username_check, password_check;
    int string_length = strlen(string);
    int secondword_first_index = nthwordfirstindex(string, 2);
    int thirdword_first_index = nthwordfirstindex(string, 3);
    char* username = (char*)malloc((thirdword_first_index - secondword_first_index) * sizeof(char));
    char* password = (char*)malloc(((string_length - thirdword_first_index) + 1) * sizeof(char));
    for(int i = secondword_first_index; i < (thirdword_first_index - 1); i++){
        *(username + (i - secondword_first_index)) = *(string + i);
    }
    *(username + ((thirdword_first_index - secondword_first_index) - 1)) = '\0';
    for(int i = thirdword_first_index; i < string_length; i++){
        *(password + (i - thirdword_first_index)) = *(string + i);
    }
    *(password + (string_length - thirdword_first_index)) = '\0';
    USER* cur_user = uhead->next;
    while (cur_user != NULL){
        username_check = strcmp(cur_user -> username, username);
        password_check = strcmp(cur_user -> password, password);
        if ((username_check == 0) && (password_check == 0)){
            return cur_user; 
        }
        cur_user = cur_user->next;
    }
    free(cur_user);
    free(password);
    free(username);
    return NULL;
}
POSTS* PostSearchForLike(POSTS *phead, char* string){// a function that searches the POSTS linked list in order to pass it to another function to like it.
// it returns the post's node if it finds it, otherwise returns NULL. and also checks whether the post_id is valid or not.
    int username_check, digit_check, is_digit = 1;
    int string_length = strlen(string);
    int secondword_first_index = nthwordfirstindex(string, 2);
    int thirdword_first_index = nthwordfirstindex(string, 3);
    char* username = (char*)malloc((thirdword_first_index - secondword_first_index) * sizeof(char));
    char* str_post_id = (char*)malloc(((string_length - thirdword_first_index) + 1) * sizeof(char));
    for(int i = secondword_first_index; i < (thirdword_first_index - 1); i++){
        *(username + (i - secondword_first_index)) = *(string + i);
    }
    *(username + ((thirdword_first_index - secondword_first_index) - 1)) = '\0';
    for(int i = thirdword_first_index; i < string_length; i++){
        *(str_post_id + (i - thirdword_first_index)) = *(string + i);
    }
    *(str_post_id + (string_length - thirdword_first_index)) = '\0';
    int str_post_id_length = strlen(str_post_id);
    for(int i = 0; i < str_post_id_length; i++){
        digit_check = isdigit(str_post_id[i]);// checking whether all of the id part of the string is digit.
        if(digit_check == 0){
            is_digit = 0;
        }
    }
    if(is_digit == 0){
        free(str_post_id);
        free(username);
        return NULL;
    }
    else{
        int int_post_id = atoi(str_post_id);// transforming a string of digits into an integer.
        POSTS* cur_post = phead->next;
        while(cur_post != NULL){
            username_check = strcmp(cur_post->username, username);
            if((username_check == 0) && (int_post_id == (cur_post->post_id))){
                return cur_post;
            }
        }
        free(str_post_id);
        free(username);
        return NULL;
    }
    
}
int PostIDToDelete(POSTS *phead, char* string){// a function that transforms the id part of the "delete" command from string to integer.
// if all of the characters of the id part are digits, it returns the integer form of them; otherwise it returns 0.
    int digit_check, is_digit = 1;
    int string_length = strlen(string);
    int secondword_first_index = nthwordfirstindex(string, 2);
    char* id = (char*)malloc(((string_length - secondword_first_index) + 1) * sizeof(char));
    for(int i= secondword_first_index; i < string_length; i++){
        *(id + (i - secondword_first_index)) = *(string + i);
    }
    *(id + (string_length - secondword_first_index)) = '\0';
    int id_length = strlen(id);
    for(int i = 0; i < id_length; i++){
        digit_check = isdigit(id[i]);
        if(digit_check == 0){
            is_digit = 0;
        }
    }
    if(is_digit == 0){
        free(id);
        return 0;
    }
    else{
        int int_id = atoi(id);
        free(id);
        return int_id;
    }
}
int DeletePost(POSTS **phead, USER *cur_user, int id){// a function that serches the POSTS linked list for a post with an specific id, if found it deletes
//it and returns 1; otherwise it returns 0.
    int username_check;
    POSTS *temp;
    POSTS *current = (*phead)->next;
    while(current != NULL){
        username_check = strcmp(current->username, cur_user->username);
        if((current->post_id = id) && (username_check == 0)){
            temp = current;
            current = current->next;
            free(temp);
            return 1;
        }
        else{
            current = current->next;
        }
    }
    free(temp);
    return 0;
}
void info(POSTS *phead, USER *cur_user){// a function that prints out the information of the logged in user and the user's posts with theis ids and number of likes.
    POSTS *current_post = phead->next;
    int username_check;
    printf("username: %s\n", cur_user->username);
    printf("password: %s\n", cur_user->password);
    while(current_post != NULL){
        username_check = strcmp(current_post->username, cur_user->username);
        if(username_check == 0){
            printf("post: %s\n", current_post->content);
            printf("post id: %d\n", current_post->post_id);
            printf("likes: %d\n", current_post->likes);
            printf("****************************************\n");
        }
        current_post = current_post->next;
    }
}
int InsertUser(USER **uhead, char *string){// a function to inserts the structure of a new user into the USER linked list after signing up.
// checks whther the desired username already exists or not, if yes it returns 0 and doesn't add the user to the linked list.
//checks for the invalid username "login", if the cindition is true it return -1 and doesn't add the user to the linked list.
//if the user is successfully inserted to the linked list it returns 1;
    int string_length = strlen(string);
    int secondword_first_index = nthwordfirstindex(string, 2);
    int thirdword_first_index = nthwordfirstindex(string, 3);
    char* username = (char*)malloc((thirdword_first_index - secondword_first_index) * sizeof(char));
    char* password = (char*)malloc(((string_length - thirdword_first_index) + 1) * sizeof(char));
    for(int i = secondword_first_index; i < (thirdword_first_index - 1); i++){
        *(username + (i - secondword_first_index)) = *(string + i);
    }
    *(username + ((thirdword_first_index - secondword_first_index) - 1)) = '\0';
    int login_username_check = strcmp(username, "login");
    if(username_search(*uhead,string)!=NULL){
        free(username);
        free(password);
        return 0;
    }
    else if(login_username_check == 0){
        free(username);
        free(password);
        return -1;
    }
    for(int i = thirdword_first_index; i < string_length; i++){
        *(password + (i - thirdword_first_index)) = *(string + i);
    }
    *(password + (string_length - thirdword_first_index)) = '\0';
    USER *newNode = (USER*)malloc(sizeof(USER));
    int username_length = strlen(username);
    int password_length = strlen(password);
    newNode->username = (char*)malloc((username_length + 1) * sizeof(char));
    newNode->password = (char*)malloc((password_length + 1) * sizeof(char));
    strcpy(newNode->username, username);
    strcpy(newNode->password, password);
    newNode->last_post_id = 0;
    newNode->next = NULL;
    USER *lastNode = *uhead;
    //last node's next address will be NULL.
    while(lastNode->next != NULL)
    {
        lastNode = lastNode->next;
    }
    //add the newNode at the end of the linked list
    lastNode->next = newNode;
    free(username);
    free(password);
    return 1;
}
int InsertPost(POSTS **phead, USER **cur_user, char *string){// a function to insert a new POST structure into the linked list when a user posts a post.
    int string_length = strlen(string);
    if(string_length == 5){
        printf("post content is empty");
        return 0;
    }
    else{
        int secondword_first_index = nthwordfirstindex(string, 2);
        char *post_content = (char *)malloc(((string_length - secondword_first_index) + 1) * sizeof(char));
        for(int i = secondword_first_index; i < string_length; i++){
            *(post_content + (i - secondword_first_index)) = *(string + i);
        }
        *(post_content + (string_length - secondword_first_index)) = '\0';
        POSTS *newNode = (POSTS*)malloc(sizeof(POSTS));
        int post_content_length = strlen(post_content);
        int post_owner_username_length = strlen((*cur_user)->username);
        newNode->content = (char*)malloc((post_content_length + 1) * sizeof(char));
        newNode->username = (char*)malloc((post_owner_username_length + 1) * sizeof(char));
        newNode->likes = 0;
        newNode->next = NULL;
        (*cur_user)->last_post_id++;
        newNode->post_id = (*cur_user)->last_post_id;
        strcpy(newNode->content, post_content);
        strcpy(newNode->username, (*cur_user)->username);
        POSTS *last_post = *phead;
        while(last_post->next != NULL){
            last_post = last_post->next;
        }
        last_post->next = newNode;
        free(post_content);
        return 1;
    }
}
int InsertLike(LIKERS **lhead, POSTS **desired_post, USER *cur_user){// a function that inserts a liker structure into the LIKERS linked list when a user
// likes a post.
    LIKERS *newNode = (LIKERS *) malloc(sizeof(LIKERS));
    int post_owner_username_length = strlen((*desired_post)->username);
    int post_liker_username_length = strlen(cur_user->username);
    newNode->post_owner_username = (char*)malloc((post_owner_username_length + 1)* sizeof(char));
    newNode->post_liker_username = (char*)malloc((post_liker_username_length + 1)* sizeof(char));
    newNode->post_id = (*desired_post)->post_id;
    newNode->next = NULL;
    (*desired_post)->likes++;
    strcpy(newNode->post_owner_username, (*desired_post)->username);
    strcpy(newNode->post_liker_username, cur_user->username);
    LIKERS *last_liker = *lhead;
    while(last_liker->next != NULL){
        last_liker = last_liker->next;
    }
    last_liker->next = newNode;
    return 1;
}
int RepetitiveLike(LIKERS *lhead, USER *cur_user, POSTS *desired_post){// a function that checks whther the liker user has liked the desired post before.
// if yes, it returns 1, otherwise it returns 0.
    int desired_post_id = desired_post->post_id;
    int liker_username_check, owner_username_check;
    LIKERS *temp = lhead->next;
    while(temp != NULL){
        liker_username_check = strcmp(temp->post_owner_username, desired_post->username);
        owner_username_check = strcmp(temp->post_liker_username, cur_user->username);
        if((owner_username_check == 0) && (liker_username_check == 0) && (temp->post_id == desired_post->post_id)){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}
void print_posts(POSTS *phead, USER *cur_user){// a function that prints the posts of a user with their ids and number of likes. it is used in the find_user function.
    POSTS *current_post = phead->next;
    int username_check;
    printf("username: %s\n", cur_user->username);
    while(current_post != NULL){
        username_check = strcmp(current_post->username, cur_user->username);
        if(username_check == 0){
            printf("post: %s\n", current_post->content);
            printf("post id: %d\n", current_post->post_id);
            printf("likes: %d\n", current_post->likes);
            printf("****************************************\n");
        }
        current_post = current_post->next;
    }
}
void find_user(USER *uhead, POSTS *phead, char *string){// a function that searches the USER linked list for a specific user with the username,
// if found it prints its posts with their ids and number of likes, otherwise it prints "user not found".
    int string_length = strlen(string);
    int secondword_first_index = nthwordfirstindex(string, 2);
    char* username = (char*)malloc(((string_length - secondword_first_index) + 1) * sizeof(char));
    for(int i= secondword_first_index; i < string_length; i++){
        *(username + (i - secondword_first_index)) = *(string + i);
    }
    *(username + (string_length - secondword_first_index)) = '\0';
    USER *desired_user = (USER*)malloc(sizeof(USER));
    desired_user = username_search(uhead, string);
    if(desired_user == NULL){
        printf("\nuser not found");
    }
    else{
        print_posts(phead, desired_user);
    }
}