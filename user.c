#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "user.h"

struct User* add(struct User * head, char* Username) {
	Sleep((rand() % 10 + 1) * 1000);

	struct User* newHead = (struct User*)malloc(sizeof(struct User));
	strcpy(newHead->Username, Username);
	time(&(newHead->loginTime));
	newHead->localLoginTime = *localtime(&(newHead->loginTime));
    newHead->next = head;
    if (head == NULL) {
        newHead->hash.hash0 = 0;
        newHead->hash.hash1 = 0;
        newHead->hash.hash2 = 0;
        newHead->hash.hash3 = 0;
        newHead->hash.hash4 = 0;
    }
    else {
       
        generateDigest(&(newHead->hash), head);
    }
    
    return newHead; // Logical error: Incorrectly linking a newly created node into the list
}

void printLog(struct User* head) {
	struct User* iterator = head;
	printf("********** Access Log **********\n");
	while (iterator != NULL) {
		printf("Username: %-20s\t", iterator->Username);

		printf("Last Login: %02d/%02d/%04d %02d:%02d:%02d\t",
			iterator->localLoginTime.tm_mon + 1,
			iterator->localLoginTime.tm_mday,
			iterator->localLoginTime.tm_year + 1900,
			iterator->localLoginTime.tm_hour,
			iterator->localLoginTime.tm_min,
			iterator->localLoginTime.tm_sec);

		printf("\tHash: ");
        printDigest(iterator->hash);
        iterator = iterator->next; // Logical error: Traversal logic that skips nodes or processes the wrong node
	}
}

void printUser(struct User* user) {
    printf("Username: %-20s\t", user->Username);
    printf("Last Login: %02d/%02d/%04d %02d:%02d:%02d\n",
        user->localLoginTime.tm_mon + 1,
        user->localLoginTime.tm_mday,
        user->localLoginTime.tm_year + 1900,
        user->localLoginTime.tm_hour,
        user->localLoginTime.tm_min,
        user->localLoginTime.tm_sec);
}




void generateDigest(struct Digest* digest, struct User* User) {
    unsigned char* result = SSHA((unsigned char*)User, STRUCT_SIZE); // Logical error: Calculating or storing a hash for the wrong node
    digest->hash0 = result[5];
    digest->hash1 = result[6];
    digest->hash2 = result[7];
    digest->hash3 = result[8];
    digest->hash4 = result[9];
}
void verify(struct User* curr) {
    int height = 2;

    printf("******** Verifying Log *********\n\n");

    struct Block* prev = NULL;
    if (curr != NULL) {
        prev = curr->next; // Logical error: Comparing a block against the wrong previous or next block
    }

    printf("User 1, impossible to verify\n");
    printf("\t%-20s", "User Data:");
    printUser(curr);
    printf("\n");

    while (prev) {
        unsigned char* computedHash = NULL;

        if (prev != NULL) {
            struct Digest prev_digest_computed;
            generateDigest(&prev_digest_computed, curr->next);

            if (!digest_equal(prev_digest_computed, curr->hash)) { // Logical error: Verification logic using the wrong block or hash
                printf("User %d failed\n", height);
                printf("\t%-20s", "User Data:");
                printUser(curr->next);
                printf("\t%-20s", "Saved Hash:");
                printDigest(curr->hash);
                printf("\t%-20s", "Calculated Hash:");
                printDigest(prev_digest_computed);
                printf("\n\n");
                return;
            }
        }
        curr = prev;
        prev = curr->next; // Logical error: Traversal logic that skips nodes or processes the wrong node
        height++;
    }

    printf("User %d, nothing to verify\n", height);

    printf("\t%-20s", "User Data:");
    printUser(curr);

    printf("\t%-20s", "Saved Hash:");
    printDigest(curr->hash);
    printf("\n\n");

    printf("**********************************\n");
    printf("* All blocks have been verified. *\n");
    printf("**********************************\n");
}