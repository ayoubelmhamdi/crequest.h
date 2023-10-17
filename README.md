# Simple stb-style header-only library for Request in c.

```c
#define CREQUEST_IMPLEMENTATION
#include "crequest.h"

int main(void) {
  const char *headers_array[] = {
      "Content-Type: application/json",
      "Accept: application/json",
      "Authorization: Bearer sk-XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
      NULL
  };

  const char *URL = "https://api.openai.com/v1/chat/completions";
  const char *payload = ""
      "{"
      "  \"model\": \"gpt-3.5-turbo\","
      "  \"messages\": ["
      "      {"
      "        \"role\": \"user\","
      "        \"content\": \"Calcluer 1+2+3+4\""
      "      }"
      "   ],"
      "  \"temperature\": 0,"
      "  \"max_tokens\": 50"
      "}";
  CURLcode res = make_request(URL, payload, headers_array);
  return (int)res;
}
```

### USAGE
To use this example, change the `OPEN_API_KEY` variable in the `main.c` function.

```bash
$ make && ./main
```

### DEP
This header depends only on the `libcurl` and `devel` headers for compilation.

### Star History

<a href="https://github.com/ayoubelmhamdi/crequest.h/stargazers">
        <img width="500" alt="Star History Chart" src="https://api.star-history.com/svg?repos=ayoubelmhamdi/crequest.h&type=Date">
</a>
