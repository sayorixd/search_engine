# search_engine

### Локальный поисковой движок, написанный на C++ с использованием библиотеки "JSON for Modern C++"

В файле _config.json_ необходимо указать все файлы, по которым будет производиться поиск, а также максимальное количество ответов на один запрос.
В файле _requests.json_ вы можете указать запросы.
После исполнения _main.cpp_ в файле _answers.json_ будут находиться ответы на все ваши запросы, отсортированные по релевантности.

Для того чтобы запустить программу, в папку с _main.cpp_ необходимо распаковать архив с библиотекой [JSON for Modern C++](https://github.com/nlohmann/json) и запуситить _main_.
