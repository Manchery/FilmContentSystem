# 输入输出及操作相关说明

更多信息详见实验报告。

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
OUTPUT_DIR = "output"
```

只有配置以上7个键值是有效的。其中路径请用 `""` 包围，bool类型的值请配置 `true` 或 `false`。请不要输入多余字符。

`USE_HMM` 和 `USE_STOP` 分别表示分词时是否使用HMM和停用词表。

请保证config文件的正确性，并**存储为 UTF-8 编码**，否则将不保证程序能正确运行。

## 输入方式

在 `INPUT_DIR` 下放置输入的 html 文件，**请保证它们以 UTF-8 存储**。

词典、HMM参数、停用词典可参考已有格式进行修改，但是**请保证以 UTF-8 存储**。

## 运行方式

在以上都确保无误的情况下，点击可执行文件运行即可。在屏幕上可能会打印一些日志信息，如读取、解析、分词等的运行时间，这是正常的。

## 输出方式

在 `OUTPUT_DIR` 下将输出每个 html 文件同名的 info 和 txt 文件，分别存储电影信息和分词结果，编码也为 UTF-8 。