#ifndef _COMMANDS_H_
#define _COMMANDS_H_

// computes the "register" command according to the specifications described in
// the homework document
void compute_register_command(int sockfd, const char *host, const char *url,
                              const char *content_type);

// computes the "login" command according to the specifications described in the
// homework document
bool compute_login_command(int sockfd, const char *host, const char *url,
                           const char *content_type, char **cookie);

// computes the "enter_library" command according to the specifications
// described in the homework document
void compute_enter_library_command(int sockfd, const char *host,
                                   const char *url, const char *cookie,
                                   char **token);

// computes the "get_books" command according to the specifications described in
// the homework document
void compute_get_books_command(int sockfd, const char *host, const char *url,
                               const char *token);

// computes the "get_book" command according to the specifications described in
// the homework document
void compute_get_book_command(int sockfd, const char *host, const char *url,
                              const char *token);

// computes the "add_book" command according to the specifications described in
// the homework document
void compute_add_book_command(int sockfd, const char *host, const char *url,
                              const char *content_type, const char *token);

// computes the "delete_book" command according to the specifications described
// in the homework document
void compute_delete_book_command(int sockfd, const char *host, const char *url,
                                 const char *token);

// computes the "logout" command according to the specifications described in
// the homework document
bool compute_logout_command(int sockfd, const char *host, const char *url,
                            const char *cookie);

#endif // _COMMANDS_H_
