#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "user.h"

struct User* getNode(struct User* head, int n) {
	struct User* cur = head;
	for (int i = 0; i < n && cur != NULL; i++)  cur = cur->next;
	return cur;
}
void modifyUsername(struct User* node, const char* name) {
	strcpy(node->Username, name);
}
void modifyTimestamp(struct User* node, time_t newTime) {
	node->loginTime = newTime;
	node->localLoginTime = *localtime(&(node->loginTime));
}
void rehashNode(struct User* node) {
	if (node->next != NULL) generateDigest(&(node->hash), node->next);
}
struct User* removeNode(struct User* head, int n) {
	struct User* before = getNode(head, n - 1);
	struct User* removed = before->next;
	before->next = removed->next;
	return removed; 
}
void swapNodes(struct User* head, int n) {
	struct User* before = getNode(head, n - 1);
	struct User* a = before->next;
	struct User* b = a->next;
	a->next = b->next;
	b->next = a;
	before->next = b;
}
void freeLog(struct User* head) {
	while (head != NULL) {
		struct User* next = head->next;
		free(head);
		head = next;
	}
}

int main(void) {
	struct User * head=NULL;
	head = add(head, "rob");
	head = add(head, "hanif");
	head = add(head, "gahyun");
	head = add(head, "matt");
	head = add(head, "sumita");
	head = add(head, "james");

	printf("\nT1: block contents\n");
	printLog(head);

	printf("\nT2: chain placement\n");
	printf("expected james first, rob last\n");
	for (struct User* c = head; c != NULL; c = c->next) printf("%s ", c->Username);

	printf("\nT3: stored hash vs SSHA of older block\n");
	int n = 0;
	for (struct User* c = head; c->next != NULL; c = c->next, n++) {
		unsigned char* r = SSHA((unsigned char*)c->next, STRUCT_SIZE);
		int ok = (r[0] == c->hash.hash0 && r[1] == c->hash.hash1 &&
				  r[2] == c->hash.hash2 && r[3] == c->hash.hash3 &&
				  r[4] == c->hash.hash4);
		printf("block %d (%s): %s\n", n, c->Username, ok ? "PASS" : "FAIL");
		free(r);
	}
	printf("\nT4: verify clean chain\n");
	verify(head);

	printf("\nT5a: Altered username, hashest not updated\n");
	struct User* t = getNode(head, 3);
	char oldName[20];
	strcpy(oldName, t->Username);
	modifyUsername(t, "hacker");
	verify(head);
	modifyUsername(t, oldName);

	printf("\nT5b: Altered timestamp, hashest not updated\n");
	time_t oldTime = t->loginTime;
	modifyTimestamp(t, oldTime + 3600);
	verify(head);
	modifyTimestamp(t, oldTime);

	printf("\n T6: altered block 3, block 2 rehash to match\n");
	struct User* newer = getNode(head, 2);
	struct Digest oldHash = newer->hash;
	modifyUsername(t, "hacker");
	rehashNode(newer);
	verify(head);
	modifyUsername(t, oldName);
	newer->hash = oldHash;

	freeLog(head);
	return 0;
}