
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ft_substr(char const *s, unsigned int start, size_t len) {
  size_t i;
  size_t s_len;
  char *ptr;

  s_len = strlen(s);
  if (start > s_len)
    return (strdup(""));
  if (len > s_len - start)
    len = s_len - start;
  ptr = (char *)malloc(len + 1);
  if (!ptr)
    return (NULL);
  i = 0;
  while (s[start + i] && i < len) {
    ptr[i] = s[start + i];
    i++;
  }
  ptr[i] = '\0';
  return (ptr);
}

typedef enum token_type {
  T_ZABA,
  T_WORD,
  T_PIPE,
  T_REDIRECT_IN,
  T_REDIRECT_OUT,
  T_REDIRECT_OUT_APPEND,
  T_REDIRECT_IN_HEREDOC,
  T_CMD,
  T_ARG,
  T_FILE,
  T_EOF
} token_type;

typedef struct token {
  int index;
  token_type type;
  char *value;
  struct token *next;
  struct token *prev;
} token;

typedef struct lexer {
  char *input;
  int position;
  int len;
  char current_char;
} lexer;

void add_back(token **head, token *new_token) {
  if (*head == NULL) {
    *head = new_token;
    return;
  }
  token *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = new_token;
  new_token->prev = temp;
}

void advance(lexer *lexer) {
  if (lexer->position < lexer->len) {
    lexer->position++;
    lexer->current_char = lexer->input[lexer->position];
  }
}

void skip(lexer *lexer) {
  while (isspace(lexer->current_char))
    advance(lexer);
}

token *create_tok(token_type type) {
  token *new_token = malloc(sizeof(token));
  if (!new_token)
    return NULL;
  new_token->type = type;
  new_token->value = NULL;
  new_token->next = NULL;
  new_token->prev = NULL;
  return new_token;
}

int is_op(char c) { return (c == '|' || c == '>' || c == '<'); }

int nword(char c) {
  return (c == '|' || c == '>' || c == '<' || isspace((int)c));
}

token_type op_type(lexer *lexer) {
  char first;
  char second;

  first = lexer->current_char;
  if (first == '|')
    return (advance(lexer), T_PIPE);
  second = lexer->input[lexer->position + 1];
  if (first == '>' && second == '>')
    return (advance(lexer), advance(lexer), T_REDIRECT_OUT_APPEND);
  if (first == '<' && second == '<')
    return (advance(lexer), advance(lexer), T_REDIRECT_IN_HEREDOC);
  if (first == '>')
    return (advance(lexer), T_REDIRECT_OUT);
  if (first == '<')
    return (advance(lexer), T_REDIRECT_IN);
  return T_ZABA;
}

token *handle_op(lexer *lexer) {
  token *node;

  node = create_tok(op_type(lexer));
  return node;
}

token *handle_word(lexer *lex, token_type type) {
  token *node;
  int start;
  int sq;
  int dq;

  start = lex->position;
  sq = 0;
  dq = 0;
  while (lex->position < lex->len) {
    if (lex->current_char == '\'' && !dq)
      sq = !sq;
    else if (lex->current_char == '"' && !sq)
      dq = !dq;
    else if (!sq && !dq && nword(lex->current_char))
      break;
    advance(lex);
  }
  if (start == lex->position)
    return (advance(lex), NULL);
  node = create_tok(type);
  if (!node)
    return NULL;
  node->value = ft_substr(lex->input, start, lex->position - start);
  return (node);
}

token *get_token(lexer *lexer) {
  skip(lexer);
  if (!lexer->current_char)
    return create_tok(T_EOF);
  if (is_op(lexer->current_char))
    return handle_op(lexer);
  return (handle_word(lexer, T_WORD));
}

token *create_list(lexer *lexer) {
  token *tmp;
  token *head;
  int i = 0;

  head = NULL;
  while (1) {
    tmp = get_token(lexer);
    if (!tmp)
      continue;
    tmp->index = i++;
    add_back(&head, tmp);
    if (tmp->type == T_EOF)
      break;
  }
  return head;
}

#define COLOR_RESET "\033[0m"
#define COLOR_CMD "\033[1;34m"      // Bright Blue
#define COLOR_ARG "\033[0;32m"      // Green
#define COLOR_FILE "\033[0;36m"     // Cyan
#define COLOR_REDIRECT "\033[0;33m" // Yellow
#define COLOR_PIPE "\033[0;35m"     // Magenta
#define COLOR_EOF "\033[1;31m"      // Bright Red
#define COLOR_UNKNOWN "\033[1;37m"  // Bright White
#define COLOR_WORD "\033[0;37m"     // Default Grey
#define COLOR_ZABA "\033[1;33m"     // Bright Yellow

const char *get_token_name(token_type type) {
  switch (type) {
  case T_ZABA:
    return "ZABA";
  case T_WORD:
    return "WORD";
  case T_PIPE:
    return "PIPE";
  case T_REDIRECT_IN:
    return "<";
  case T_REDIRECT_OUT:
    return ">";
  case T_REDIRECT_OUT_APPEND:
    return ">>";
  case T_REDIRECT_IN_HEREDOC:
    return "<<";
  case T_CMD:
    return "CMD";
  case T_ARG:
    return "ARG";
  case T_FILE:
    return "FILE";
  case T_EOF:
    return "EOF";
  default:
    return "UNKNOWN";
  }
}

const char *get_token_color(token_type type) {
  switch (type) {
  case T_CMD:
    return COLOR_CMD;
  case T_ARG:
    return COLOR_ARG;
  case T_FILE:
    return COLOR_FILE;
  case T_PIPE:
    return COLOR_PIPE;
  case T_REDIRECT_IN:
  case T_REDIRECT_OUT:
  case T_REDIRECT_OUT_APPEND:
  case T_REDIRECT_IN_HEREDOC:
    return COLOR_REDIRECT;
  case T_EOF:
    return COLOR_EOF;
  case T_ZABA:
    return COLOR_ZABA;
  case T_WORD:
    return COLOR_WORD;
  default:
    return COLOR_UNKNOWN;
  }
}

void print_token_list(token *head) {
  token *current = head;
  while (current != NULL) {
    const char *color = get_token_color(current->type);
    printf("Index: %d, Type: %s%s%s, Value: %s\n", current->index, color,
           get_token_name(current->type), COLOR_RESET,
           current->value ? current->value : "");
    current = current->next;
  }
}

int is_redir(token_type type) {
  return type == T_REDIRECT_IN || type == T_REDIRECT_OUT ||
         type == T_REDIRECT_IN_HEREDOC || type == T_REDIRECT_OUT_APPEND;
}

void classify(token *head) {
  int cmd_found = 0;

  while (head) {
    if (is_redir(head->type) && head->next && head->next->type == T_WORD)
      head->next->type = T_FILE;
    if ((!cmd_found && head->type == T_WORD) ||
        (head->type == T_WORD && head->prev && head->prev->type == T_FILE)) {
      head->type = T_CMD;
      cmd_found = 1;
    }
    if (head->type == T_WORD)
      head->type = T_ARG;
    if (head->type == T_PIPE)
      cmd_found = 0;
    head = head->next;
  }
}

int main() {
  lexer lex;
  lex.input = strdup("              cat file' 'zaba >> >>      haha | ls -al ./ >> zaba | wc > xta");
  lex.position = 0;
  lex.len = strlen(lex.input);
  lex.current_char = lex.input[lex.position];

  token *list = create_list(&lex);
  classify(list);
  print_token_list(list);
}