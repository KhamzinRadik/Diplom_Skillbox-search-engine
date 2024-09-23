![GitHub repo size](https://img.shields.io/github/repo-size/OldScripter/Search-in-files)
![GitHub author](https://img.shields.io/badge/made_on-C++-red)
##  Поиск слов в файлах.

## Описание

Приложение для поиска запрашиваемых слов (JSON файл requests.json) в текстовых файлах ресурсов (путь к ресурсам указан в
config.json). Результат поиска помещается в answers.json.

## Technologies

```shell
- C++ 20
- Cmake 3.28
- Gtest 1.1
- Nlohmann JSON 3.11.3
- Spdlog 1.12

```

## Files specification

* config.json<br>
  File where specified name and version of application.<br>
  Here you can also change the maximal quantity of relevant pages that will be put into answers.json (max_respones).<br>
  Default content:<br>

```json
{
  "config": {
    "name": "search-engine",
    "version": "0.1",
    "max_responses": 5
  },
  "files": [
    "../bin/resources/file001.txt",
    "../bin/resources/file002.txt",
    "../bin/resources/file003.txt",
    "../examples/resources/lev1/file001.txt",
    "../examples/resources/lev1/file002.txt",
    "../examples/resources/lev1/file003.txt",
    "../examples/resources/lev2/file001.txt",
    "../examples/resources/lev2/file002.txt",
    "../examples/resources/lev2/file003.txt"
  ]
}
```

* requests.json<br>
  File where specified requests for search.<br>
  Each separate request sholud be entered after ',' on new line. Example below.<br>
  Example content:<br>

```json
{
  "requests": [
    "tiger fox",
    "wolf bird",
    "monkey"
  ]
}
```

* answers.json<br>
  File where search result will be written in JSON format.<br>
  Example content:<br>

```shell
  ./bin/answers.json
```
