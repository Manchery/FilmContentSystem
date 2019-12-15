# 输入输出及操作相关说明

以下信息与实验报告互为补充。

## 配置方式

在可执行文件同名文件夹下应有 `FilmContentSystem.config`  文件，程序将从中读取相应配置信息，否则若不存在配置文件，将采用默认配置。

配置文件（对应默认配置）示例如下：

```
DICT_PATH = "dict/dict.txt"
HMM_PATH = "dict/HMM.txt"
STOP_PATH = "dict/stopwords.txt"
USE_HMM = true
USE_STOP = true
INPUT_DIR = "input"
OUTPUT_DIR = "input/info"
RETRIEVAL_INPUT = "query1.txt"
RETRIEVAL_OUTPUT = "result1.txt"
RECOMMEND_INPUT = "query2.txt"
RECOMMEND_OUTPUT = "result2.txt"
```

- 只有配置以上7个键值是有效的。其中路径请用 `""` 包围，bool 类型的值请配置 `true` 或 `false`。请不要输入多余字符。

- `USE_HMM` 和 `USE_STOP` 分别表示分词时是否使用HMM和停用词表。

- 请保证config文件的正确性，并**存储为 UTF-8 编码**，否则将不保证程序能正确运行。

- 在 INPUT_DIR 下已经放置了数据库文件（电影文件、解析和分词结果等），**请不要修改**。

## 1. 批量检索和批量推荐 query.exe

### 输入输出格式

输入 query1.txt 为一行一个查询，关键词之间使用空格分开；

输出 result1.txt 为每行多个用空格隔开的 (urlID, occurTimes)，分别表示电影序号和关键字出现总次数。

输入 query2.txt 为一行一个查询，为电影名；

输出 result2.txt 为一行 5 个空格隔开的推荐 (docID, newsName) ，分别表示电影序号和标题。

query1.txt , query2.txt 请**以UTF-8编码存储**，否则将不保证能正常运行。

### 运行方式

在以上都确保无误的情况下，点击 query.exe 运行即可。在屏幕上可能会打印一些日志信息，这是正常的。

## 2. 图形用户界面 gui.exe

图形界面采用标签页的形式，共有三种标签页：

- 主标签页：由 logo 和输入框组成，点击搜索按钮时
  - 若输入**与数据库中电影标题完全匹配**，则直接跳转至对应的电影标签页
  - 否则以输入为检索关键字跳转到检索结果。（如果想以电影名为检索关键字，则可以在输入末尾加入空格等空字符）

- 检索结果标签页：以列表形式显示检索结果，点击电影标题即可跳转至电影信息页。

- 电影信息标签页：显示电影信息及推荐，点击推荐的电影可跳转至对应的电影信息页。