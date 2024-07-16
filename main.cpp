#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 100

typedef enum {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_IF, TOKEN_EQUAL, TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_SEMICOLON, TOKEN_UNKNOWN, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char text[MAX_TOKEN_LEN];
} Token;

void getNextToken(FILE *file, Token *token) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isspace(c)) continue;
        if (isalpha(c)) {
            int len = 0;
            token->text[len++] = c;
            while (isalnum(c = fgetc(file))) {
                if (len < MAX_TOKEN_LEN - 1) token->text[len++] = c;
            }
            ungetc(c, file);
            token->text[len] = '\0';
            if (strcmp(token->text, "int") == 0) token->type = TOKEN_INT;
            else if (strcmp(token->text, "if") == 0) token->type = TOKEN_IF;
            else token->type = TOKEN_IDENTIFIER;
            return;
        }
        if (isdigit(c)) {
            int len = 0;
            token->text[len++] = c;
            while (isdigit(c = fgetc(file))) {
                if (len < MAX_TOKEN_LEN - 1) token->text[len++] = c;
            }
            ungetc(c, file);
            token->text[len] = '\0';
            token->type = TOKEN_NUMBER;
            return;
        }
        switch (c) {
            case '=': token->type = TOKEN_ASSIGN; token->text[0] = '='; token->text[1] = '\0'; return;
            case '+': token->type = TOKEN_PLUS; token->text[0] = '+'; token->text[1] = '\0'; return;
            case '-': token->type = TOKEN_MINUS; token->text[0] = '-'; token->text[1] = '\0'; return;
            case '{': token->type = TOKEN_LBRACE; token->text[0] = '{'; token->text[1] = '\0'; return;
            case '}': token->type = TOKEN_RBRACE; token->text[0] = '}'; token->text[1] = '\0'; return;
            case ';': token->type = TOKEN_SEMICOLON; token->text[0] = ';'; token->text[1] = '\0'; return;
            default: token->type = TOKEN_UNKNOWN; token->text[0] = c; token->text[1] = '\0'; return;
        }
    }
    token->type = TOKEN_EOF;
    token->text[0] = '\0';
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }
    Token token;
    do {
        getNextToken(file, &token);
        printf("Token: %d, Text: %s\n", token.type, token.text);
    } while (token.type != TOKEN_EOF);
    fclose(file);
    return 0;
}
typedef struct Node {
    TokenType type;
    char text[MAX_TOKEN_LEN];
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(TokenType type, const char *text) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = type;
    strcpy(node->text, text);
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node *parseVariableDeclaration(Token *tokens, int *index) {
    Node *node = createNode(TOKEN_INT, tokens[*index].text);
    (*index)++;
    if (tokens[*index].type == TOKEN_IDENTIFIER) {
        node->left = createNode(TOKEN_IDENTIFIER, tokens[*index].text);
        (*index)++;
        if (tokens[*index].type == TOKEN_SEMICOLON) {
            (*index)++;
            return node;
        }
    }
    free(node);
    return NULL;
}

// Add more parsing functions for assignments, arithmetic operations, and conditionals.
void generateAssembly(Node *node) {
    if (node == NULL) return;
    switch (node->type) {
        case TOKEN_INT:
            printf("DECL %s\n", node->left->text);
            break;
        case TOKEN_ASSIGN:
            printf("MOV %s, %s\n", node->left->text, node->right->text);
            break;
        // Add more cases for arithmetic operations and conditionals.
    }
    generateAssembly(node->left);
    generateAssembly(node->right);
}
