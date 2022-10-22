**Ciobanu Andrei-Bogdan - 322CB**

# Protocoale de Comunicație - Tema 3 - Client Web - Comunicație cu REST API

Punctul de plecare pentru această temă este laboratorul 10, împreună cu
scheletul său de cod.

Pentru a parsa răspunsurile primite de la server, am ales biblioteca lui
*nlohmann*, pentru *C++*. Motivul principal pentru care am ales-o pe aceasta și
nu *parson* (pentru *C*) este documentația mult mai bine scrisă. În plus, C++
STL, în sine, m-a determinat să aleg limbajul C++, măcar pentru câteva beneficii
din cele pe care le oferă.
<br><br>

### *json.hpp:*

- single include file pentru biblioteca de parsare JSON a lui nlohmann.

### *buffer.hpp/cpp:*

- din laboratorul 10.

### *helpers.hpp/cpp:*

- din laboratorul 10*;
- (*) - excepție face o funcție asemănătoare *basic_extract_json_response()*,
care extrage o listă de obiecte JSON din răspunsul dat de server:

        // extracts and returns a JSON list from a server response
        const char *basic_extract_json_list_response(const char *str);

### *utils.hpp/cpp:*

- conține o funcție ce are rolul de a extrage cookie-ul din răspunsul dat de
server:

        // extracts and returns a cookie from a server response
        char *get_cookie_from_response(char *response);

    - cookie-ul este delimitat de „connect.sid” la stânga și caracterul ';' la
    dreapta.

### *requests.hpp/cpp:*

- implementările funcțiilor *compute_get_request()* și *compute_post_request()*
diferă de cele de la laboratorul 10;
- este prezentă și funcția *compute_delete_request()* care are rolul de a
trimite un DELETE request;
- *compute_get_request():*
    - dacă este necesar, se adaugă un "Authorization Header" ce conține un token
    JWT, care demonstrează accesul la bibliotecă:

            Authorization: Bearer <token>

    - dacă este necesar, se adaugă un singur cookie - cookie de sesiune.

- *compute_post_request():*
    - dacă este necesar, se adaugă un "Authorization Header" ce conține un token
    JWT, care demonstrează accesul la bibliotecă:

            Authorization: Bearer <token>

    - în cadrul acestei teme, *Content-Type* este întotdeauna
    *application/json*;

    - în cadrul acestei teme, cookie-urile nu sunt utilizate la POST request.

- *compute_delete_request():*
    - dacă este necesar, se adaugă un "Authorization Header" ce conține un token
    JWT, care demonstrează accesul la bibliotecă:

            Authorization: Bearer <token>

    - dacă este necesar, se adaugă un singur cookie - cookie de sesiune.

### *commands.hpp/cpp:*

- implementările funcțiilor ce prelucrează comenzile de interacțiune cu
serverul:
    - `register` -> *compute_register_command():*
        - se citesc numele de utilizator și parola de la tastatură;
        - se construiește obiectul JSON, se serializează, apoi se trimite un
        POST REQUEST cu acesta la server;
        - se analizează răspunsul.
    - `login` -> *compute_login_command():*
        - se citesc numele de utilizator și parola de la tastatură;
        - se construiește obiectul JSON, se serializează, apoi se trimite un
        POST REQUEST la server;
        - se analizează răspunsul și se salvează cookie-ul de sesiune, dacă este
        cazul.
    - `enter_library` -> *compute_enter_library_command():*
        - se trimite un GET REQUEST corespunzător la server;
        - se analizează răspunsul - în caz de succes, răspunsul conține tokenul
        JWT.
    - `get_books` -> *compute_get_books_command():*
        - se trimite un GET REQUEST corespunzător la server;
        - se analizează răspunsul:
            - răspunsul poate conține lista de obiecte JSON - cărțile;
            - dacă nu există un obiect JSON în răspuns, înseamnă că nu există
            cărți în bibliotecă;
            - pot apărea erori, ce sunt afișate.
    - `get_book` -> *compute_get_book_command():*
        - se citește de la tastatură id-ul cărții;
        - se trimite un GET REQUEST corespunzător la server, conform enunțului;
        - se analizează răspunsul:
            - răspunsul poate conține obiectul JSON - cartea căutată;
            - dacă nu există un obiect JSON în răspuns, înseamnă că nu există în
            bibliotecă cartea cu id-ul dat;
            - pot apărea erori, ce sunt afișate.
    - `add_book` -> *compute_add_book_command():*
        - se citesc datele despre carte de la tastatură;
        - se construiește obiectul JSON;
        - se realizează POST REQUEST-ul aferent;
        - se analizează răspunsul primit de la server.
    - `delete_book` -> *compute_delete_book_command():*
        - se citește de la tastatură id-ul cărții;
        - se trimite un DELETE REQUEST corespunzător;
        - se analizează răspunsul primit de la server:
            - dacă răspunsul conține un obiect JSON, atunci ștergerea nu a avut
            loc cu succes, drept urmare se afișează mesajul primit.
    - `logout` -> *compute_logout_command():*
        - se trimite un GET REQUEST, conform enunțului;
        - se verifică răspunsul primit de la server;
        - se returnează starea utilizatorului: încă logat / delogat.

### *client.cpp:*

- *main():*
    - clientul interpretează comenzi de la tastatură pentru a putea interacționa
    cu serverul;
    - procesul se repetă până la introducerea comenzii `exit`.
