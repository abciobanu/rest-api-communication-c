#include "commands.hpp"

#include <iostream>
#include <string>

#include "requests.hpp"
#include "helpers.hpp"
#include "utils.hpp"
#include "json.hpp"

void compute_register_command(int sockfd, const char *host, const char *url,
                              const char *content_type)
{
    std::string username;
    std::string password;

    std::cout << "username=";
    std::cin >> username;

    std::cout << "password=";
    std::cin >> password;

    // build the JSON object
    nlohmann::json json_content;
    json_content["username"] = username;
    json_content["password"] = password;

    // serialize the JSON object
    std::string content = json_content.dump();

    char *message = compute_post_request(host, url, content_type,
                                         content.c_str(), NULL);
    send_to_server(sockfd, message);
    free(message);

    char *response = receive_from_server(sockfd);

    // check for errors
    const char *error_msg = basic_extract_json_response(response);
    if (error_msg == NULL) {
        std::cout << "You have successfully registered!\n";
    } else {
        std::cout << error_msg << "\n";
    }

    free(response);
}

bool compute_login_command(int sockfd, const char *host, const char *url,
                           const char *content_type, char **cookie)
{
    bool logged_in = false;
    std::string username;
    std::string password;

    std::cout << "username=";
    std::cin >> username;

    std::cout << "password=";
    std::cin >> password;

    // build the JSON object
    nlohmann::json json_content;
    json_content["username"] = username;
    json_content["password"] = password;

    // serialize the JSON object
    std::string content = json_content.dump();

    char *message = compute_post_request(host, url, content_type,
                                         content.c_str(), NULL);
    send_to_server(sockfd, message);
    free(message);

    char *response = receive_from_server(sockfd);

    // check for errors
    const char *error_msg = basic_extract_json_response(response);
    if (error_msg == NULL) {
        // the user is now logged in
        logged_in = true;

        // authentication depends on session cookies
        *cookie = get_cookie_from_response(response);

        std::cout << "You have successfully logged in!\n";
    } else {
        std::cout << error_msg << "\n";
    }

    free(response);

    return logged_in;
}

void compute_enter_library_command(int sockfd, const char *host,
                                   const char *url, const char *cookie,
                                   char **token)
{
    char *message = compute_get_request(host, url, cookie, NULL);
    send_to_server(sockfd, message);
    free(message);

    char *response = receive_from_server(sockfd);

    const char *extracted_json = basic_extract_json_response(response);
    if (extracted_json == NULL) {
        // unexpectedly, there is no JSON object in the message received from
        // the server
        std::cout << response << "\n";

        free(response);
        return;
    }

    nlohmann::json json_response = nlohmann::json::parse(extracted_json);

    // check for errors
    if (json_response.contains("error")) {
        std::cout << json_response["error"] << "\n";
    } else if (json_response.contains("token")) {
        // library access depends on JSON Web Tokens
        std::string token_string = json_response["token"].dump();
        token_string.erase(token_string.begin()); // delete leading '"'
        token_string.erase(token_string.end() - 1); // delete trailing '"'

        *token = strdup(token_string.c_str());

        std::cout << "You have successfully gained access to the library!\n";
    }

    free(response);
}

void compute_get_books_command(int sockfd, const char *host, const char *url,
                               const char *token)
{
    char *message = compute_get_request(host, url, NULL, token);
    send_to_server(sockfd, message);
    free(message);

    char *response = receive_from_server(sockfd);

    const char *extracted_json = basic_extract_json_list_response(response);
    if (extracted_json == NULL) {
        std::cout << "There are no books!\n";

        free(response);
        return;
    }

    nlohmann::json json_response = nlohmann::json::parse(extracted_json);

    // check for errors
    if (json_response.contains("error")) {
        std::cout << json_response["error"] << "\n";
    } else {
        // there are books
        std::cout << json_response << "\n";
    }

    free(response);
}

void compute_get_book_command(int sockfd, const char *host, const char *url,
                              const char *token)
{
    int book_id;

    std::cout << "id=";
    std::cin >> book_id;

    std::string final_url = url + std::string("/") + std::to_string(book_id);
    char *message = compute_get_request(host, final_url.c_str(), NULL,
                                        token);
    send_to_server(sockfd, message);
    free(message);

    char *response = receive_from_server(sockfd);

    const char *extracted_json = basic_extract_json_list_response(response);
    if (extracted_json == NULL) {
        // it is not a JSON object list (with a book)
        extracted_json = basic_extract_json_response(response);
        if (extracted_json == NULL) {
            std::cout << "There is no book with id " << book_id << "!\n";

            free(response);
            return;
        }
    }

    nlohmann::json json_response = nlohmann::json::parse(extracted_json);

    // check for errors
    if (json_response.contains("error")) {
        std::cout << json_response["error"] << "\n";
    } else {
        // if a JSON list is received, extract the only object it contains
        if (json_response.is_array()) {
            std::cout << json_response[0] << "\n";
        } else {
            std::cout << json_response << "\n";
        }
    }

    free(response);
}

void compute_add_book_command(int sockfd, const char *host, const char *url,
                              const char *content_type, const char *token)
{
    std::string title;
    std::string author;
    std::string genre;
    std::string publisher;
    int page_count;

    // ignore the newline character at the end of the command
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "title=";
    std::getline(std::cin, title);

    std::cout << "author=";
    std::getline(std::cin, author);

    std::cout << "genre=";
    std::getline(std::cin, genre);

    std::cout << "publisher=";
    std::getline(std::cin, publisher);

    std::cout << "page_count=";
    std::cin >> page_count;

    // build the JSON object
    nlohmann::json book_json;
    book_json["title"] = title;
    book_json["author"] = author;
    book_json["genre"] = genre;
    book_json["publisher"] = publisher;
    book_json["page_count"] = page_count;

    // serialize the JSON object
    std::string content = book_json.dump();
    std::cout << content << "\n";

    char *message = compute_post_request(host, url, content_type,
                                         content.c_str(), token);
    send_to_server(sockfd, message);
    free(message);

    char *response = receive_from_server(sockfd);

    // check for errors
    const char *error_msg = basic_extract_json_response(response);
    if (error_msg == NULL) {
        std::cout << "The book with id has been successfully added!\n";
    } else {
        std::cout << error_msg << "\n";
    }

    free(response);
}

void compute_delete_book_command(int sockfd, const char *host, const char *url,
                                 const char *token)
{
    int book_id;

    std::cout << "id=";
    std::cin >> book_id;

    std::string final_url = url + std::string("/") + std::to_string(book_id);
    char *message = compute_delete_request(host, final_url.c_str(),
                                           NULL, token);
    send_to_server(sockfd, message);
    free(message);

    char *response = receive_from_server(sockfd);

    // check for errors
    const char *error_msg = basic_extract_json_response(response);
    if (error_msg == NULL) {
        std::cout << "The book with id " << book_id << " has been successfully "
                                                       "deleted!\n";
    } else {
        std::cout << error_msg << "\n";
    }

    free(response);
}

bool compute_logout_command(int sockfd, const char *host, const char *url,
                            const char *cookie)
{
    bool is_still_logged_in = true;

    char *message = compute_get_request(host, url, cookie, NULL);
    send_to_server(sockfd, message);
    free(message);

    char *response = receive_from_server(sockfd);

    // check for errors
    const char *error_msg = basic_extract_json_response(response);
    if (error_msg == NULL) {
        is_still_logged_in = false;
        std::cout << "You have successfully logged out!\n";
    } else {
        std::cout << error_msg << "\n";
    }

    free(response);

    return is_still_logged_in;
}
