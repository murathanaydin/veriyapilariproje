#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 1000

// Kullanýcý Yapýsý
typedef struct User {
    int id;
    int friendCount;
    struct User** friends;
} User;

// Red-Black Tree Node Yapýsý
typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    int id;
    User* user;
    Color color;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
} RBNode;

// Global Deðiþkenler
User* users[MAX_USERS];
int userCount = 0;
RBNode* root = NULL;

// Fonksiyon Ýmza Bildirimleri
User* createUser(int id);
void addFriendship(int id1, int id2);
User* getUser(int id);
void readDataset(const char* filename);
void dfs(User* user, int currentDepth, int targetDepth, int* visited, int* found, int* foundCount);
void findFriendsAtDistance(int id, int distance);
void findCommonFriends(int id1, int id2);
void exploreCommunity(User* user, int* visited, int* community, int* communitySize);
void detectCommunities();
void dfsInfluence(User* user, int* visited, int* count);
void calculateInfluence(int id);
void rbInsert(User* user);
void rbInsertFixup(RBNode* node);
void leftRotate(RBNode* x);
void rightRotate(RBNode* y);
RBNode* createRBNode(User* user);
User* rbSearch(int id);

// Kullanýcý oluþtur
User* createUser(int id) {
    User* user = (User*)malloc(sizeof(User));
    user->id = id;
    user->friendCount = 0;
    user->friends = (User*)malloc(sizeof(User) * MAX_USERS);
    return user;
}

// Kullanýcý ekle
User* getUser(int id) {
    int i = 0;
    for (i = 0; i < userCount; i++) {
        if (users[i]->id == id) {
            return users[i];
        }
    }
    return NULL;
}

// Arkadaþlýk ekle
void addFriendship(int id1, int id2) {
    User* user1 = getUser(id1);
    User* user2 = getUser(id2);
    if (user1 && user2) {
        user1->friends[user1->friendCount++] = user2;
        user2->friends[user2->friendCount++] = user1;
    }
}

// Veriseti dosyasýný oku
void readDataset(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Dosya açilamadi.\n");
        return;
    }
    
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char type[10];
        int id1, id2;
        sscanf(line, "%s", type);
        if (strcmp(type, "USER") == 0) {
            sscanf(line, "%*s %d", &id1);
            User* user = createUser(id1);
            users[userCount++] = user;
            rbInsert(user); // Red-Black Tree'ye ekle
        } else if (strcmp(type, "FRIEND") == 0) {
            sscanf(line, "%*s %d %d", &id1, &id2);
            addFriendship(id1, id2);
        }
    }
    fclose(file);
}

// DFS
void dfs(User* user, int currentDepth, int targetDepth, int* visited, int* found, int* foundCount) {
    int i = 0;
    if (!user || visited[user->id]) return;
    visited[user->id] = 1;
    
    if (currentDepth == targetDepth) {
        found[(*foundCount)++] = user->id;
        return;
    }
    
    for (i = 0; i < user->friendCount; i++) {
        dfs(user->friends[i], currentDepth + 1, targetDepth, visited, found, foundCount);
    }
}

// Belirli mesafedeki arkadaþlarý bul
void findFriendsAtDistance(int id, int distance) {
    int visited[MAX_USERS] = {0};
    int found[MAX_USERS];
    int foundCount = 0;
    int i = 0;
    
    User* user = getUser(id);
    if (!user) {
        printf("Kullanici bulunamadi.\n");
        return;
    }
    
    dfs(user, 0, distance, visited, found, &foundCount);
    
    printf("%d mesafedeki arkadaslar: ", distance);
    for (i = 0; i < foundCount; i++) {
        printf("%d ", found[i]);
    }
    printf("\n");
}

// Ortak arkadaþlar
void findCommonFriends(int id1, int id2) {
    User* user1 = getUser(id1);
    User* user2 = getUser(id2);
    int i = 0, j = 0;
    
    if (!user1 || !user2) {
        printf("Kullanici(lar) bulunamadi.\n");
        return;
    }
    
    printf("%d ve %d ortak arkadaslari: ", id1, id2);
    for (i = 0; i < user1->friendCount; i++) {
        for (j = 0; j < user2->friendCount; j++) {
            if (user1->friends[i]->id == user2->friends[j]->id) {
                printf("%d ", user1->friends[i]->id);
            }
        }
    }
    printf("\n");
}

// Topluluklarý keþfet
void exploreCommunity(User* user, int* visited, int* community, int* communitySize) {
    int i = 0;
    if (visited[user->id]) return;
    visited[user->id] = 1;
    community[(*communitySize)++] = user->id;
    
    for (i = 0; i < user->friendCount; i++) {
        exploreCommunity(user->friends[i], visited, community, communitySize);
    }
}

// Topluluklarý tespit et
void detectCommunities() {
    int visited[MAX_USERS] = {0};
    int i = 0;
    int community[MAX_USERS];
    int communitySize = 0;
    
    printf("Topluluklar:\n");
    
    for (i = 0; i < userCount; i++) {
        if (!visited[users[i]->id]) {
            communitySize = 0;
            exploreCommunity(users[i], visited, community, &communitySize);
            
            int j = 0;
            printf("[ ");
            for (j = 0; j < communitySize; j++) {
                printf("%d ", community[j]);
            }
            printf("]\n");
        }
    }
}

// Etki alaný için DFS
void dfsInfluence(User* user, int* visited, int* count) {
    int i = 0;
    if (visited[user->id]) return;
    visited[user->id] = 1;
    (*count)++;
    
    for (i = 0; i < user->friendCount; i++) {
        dfsInfluence(user->friends[i], visited, count);
    }
}

// Etki alaný hesapla
void calculateInfluence(int id) {
    int visited[MAX_USERS] = {0};
    int count = 0;
    
    User* user = getUser(id);
    if (!user) {
        printf("Kullanici bulunamadi.\n");
        return;
    }
    
    dfsInfluence(user, visited, &count);
    printf("%d numarali kullanicinin etki alani: %d\n", id, count - 1);
}

// Red-Black Tree iþlemleri
RBNode* createRBNode(User* user) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->id = user->id;
    node->user = user;
    node->color = RED;
    node->left = node->right = node->parent = NULL;
    return node;
}

void leftRotate(RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rightRotate(RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void rbInsertFixup(RBNode* node) {
    RBNode* uncle;
    while (node->parent && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            uncle = node->parent->parent->right;
            if (uncle && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rightRotate(node->parent->parent);
            }
        } else {
            uncle = node->parent->parent->left;
            if (uncle && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rightRotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                leftRotate(node->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void rbInsert(User* user) {
    RBNode* node = createRBNode(user);
    RBNode* y = NULL;
    RBNode* x = root;
    while (x != NULL) {
        y = x;
        if (node->id < x->id)
            x = x->left;
        else
            x = x->right;
    }
    node->parent = y;
    if (y == NULL)
        root = node;
    else if (node->id < y->id)
        y->left = node;
    else
        y->right = node;
    rbInsertFixup(node);
}

User* rbSearch(int id) {
    RBNode* current = root;
    while (current != NULL) {
        if (id == current->id)
            return current->user;
        else if (id < current->id)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

// Ana Fonksiyon
int main() {
    readDataset("veriseti.txt");

    printf("\n- Kullanici 101 için 1 mesafe uzakliktaki arkadaslar:\n");
    findFriendsAtDistance(101, 1);

    printf("\n- 101 ve 102 ortak arkadaslari:\n");
    findCommonFriends(102, 103);

    printf("\n- Topluluklar:\n");
    detectCommunities();

    printf("\n- 104 numarali kullanicinin etki alani:\n");
    calculateInfluence(104);

    printf("\n- Red-Black Tree'den 103 aramasi:\n");
    User* foundUser = rbSearch(103);
    if (foundUser) printf("Kullanici bulundu: %d\n", foundUser->id);
    else printf("Kullanici bulunamadi.\n");

    return 0;
}
