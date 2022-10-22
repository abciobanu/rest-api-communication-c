#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (cookie can be set to NULL if not
// needed)
char *compute_get_request(const char *host, const char *url, const char *cookie,
                          const char *token);

// computes and returns a POST request string (cookie can be NULL if not needed)
char *compute_post_request(const char *host, const char *url,
                           const char* content_type, const char *content,
                           const char *token);

// computes and returns a DELETE request string (cookie can be set to NULL if
// not needed)
char *compute_delete_request(const char *host, const char *url,
                             const char *cookie, const char *token);

#endif
