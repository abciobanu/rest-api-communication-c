#include <iostream>
#include <arpa/inet.h>

#include "json.hpp"

#include "helpers.hpp"
#include "commands.hpp"
#include "requests.hpp"

#define SERVER_ADDR "34.241.4.235"
#define SERVER_PORT 8080

#define REGISTER_URL "/api/v1/tema/auth/register"
#define LOGIN_URL "/api/v1/tema/auth/login"
#define ACCESS_URL "/api/v1/tema/library/access"
#define BOOKS_URL "/api/v1/tema/library/books"
#define LOGOUT_URL "/api/v1/tema/auth/logout"

#define CONTENT_TYPE "application/json"

int main()
{
    int sockfd;

    char *cookie = NULL;
    char *token = NULL;

    bool is_logged_in = false;

    std::string input;
    while (true) {
        sockfd = open_connection(SERVER_ADDR, SERVER_PORT, AF_INET, SOCK_STREAM,
                                 0);

        std::cin >> input;
        if (input.compare("register") == 0) {
            if (is_logged_in) {
                std::cout << "You are already logged in! Log out first.\n";
            } else {
                compute_register_command(sockfd, SERVER_ADDR, REGISTER_URL,
                                         CONTENT_TYPE);
            }
        } else if (input.compare("login") == 0) {
            if (is_logged_in) {
                std::cout << "You are already logged in! Log out first.\n";
            } else {
                is_logged_in = compute_login_command(sockfd, SERVER_ADDR,
                                                     LOGIN_URL, CONTENT_TYPE,
                                                     &cookie);
            }
        } else if (input.compare("enter_library") == 0) {
            if (is_logged_in) {
                compute_enter_library_command(sockfd, SERVER_ADDR, ACCESS_URL,
                                              cookie, &token);
            } else {
                std::cout << "You are not logged in! Log in first.\n";
            }
        } else if (input.compare("get_books") == 0) {
            if (token == NULL) {
                std::cout << "You do not have access to the library! Get access"
                             " to it first.\n";
            } else {
                compute_get_books_command(sockfd, SERVER_ADDR, BOOKS_URL,
                                          token);
            }
        } else if (input.compare("get_book") == 0) {
            if (token == NULL) {
                std::cout << "You do not have access to the library! Get access"
                             " to it first.\n";
            } else {
                compute_get_book_command(sockfd, SERVER_ADDR, BOOKS_URL, token);
            }
        } else if (input.compare("add_book") == 0) {
            if (token == NULL) {
                std::cout << "You do not have access to the library! Get access"
                             " to it first.\n";
            } else {
                compute_add_book_command(sockfd, SERVER_ADDR, BOOKS_URL,
                                         CONTENT_TYPE, token);
            }
        } else if (input.compare("delete_book") == 0) {
            if (token == NULL) {
                std::cout << "You do not have access to the library! Get access"
                             " to it first.\n";
            } else {
                compute_delete_book_command(sockfd, SERVER_ADDR, BOOKS_URL,
                                            token);
            }
        } else if (input.compare("logout") == 0) {
            if (is_logged_in) {
                is_logged_in = compute_logout_command(sockfd, SERVER_ADDR,
                                                      LOGOUT_URL, cookie);

                if (!is_logged_in) {
                    token = NULL;
                    cookie = NULL;
                }
            } else {
                std::cout << "You are not logged in! Log in first.\n";
            }
        } else if (input.compare("exit") == 0) {
            close_connection(sockfd);
            break;
        } else {
            std::cout << "Unknown command!\n";
        }

        close_connection(sockfd);
    }

    return 0;
}
