#!/bin/bash

# 检查是否提供了指定目录作为脚本参数
if [ $# -eq 0 ]; then
  echo "Usage: $0 directory"
  exit 1
fi

# 检查指定目录是否存在
if [ ! -d "$1" ]; then
  echo "Error: $1 is not a directory"
  exit 1
fi

# 获取当前日期和时间
now=$(date +"%Y-%m-%d-%H-%M")

# 迭代指定目录中的所有文件
for file in "$1"/*; do
  # 检查文件是否是普通文件
  if [ -f "$file" ]; then
    # 提取文件名和扩展名
    filename=$(basename "$file")
    extension="${filename##*.}"
    filename="${filename%.*}"

    # 检查文件名是否已有后缀
    if [[ "$filename" != *-* ]]; then
      # 在文件名末尾添加日期和时间后缀
      newname="${filename}-${now}.${extension}"
      # 重命名文件
      mv "$file" "$1/$newname"
    else
      # 更新日期和时间后缀
      newname=$(echo "$filename" | sed "s/-[0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\}-[0-9]\{2\}-[0-9]\{2\}$//")-${now}.${extension}
      # 重命名文件
      mv "$file" "$1/$newname"
    fi
  fi
done

echo "All files in $1 have been renamed with timestamp suffix."

