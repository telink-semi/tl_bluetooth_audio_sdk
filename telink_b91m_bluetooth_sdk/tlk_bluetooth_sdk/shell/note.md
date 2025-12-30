The shell script must end with LF (Line Feed) rather than CRLF (Carriage Return and Line Feed).
However, Windows may automatically convert it to CRLF.

To ensure the script is executable on Linux, please grant it the necessary permissions by running the command chmod +x script_name.sh.
You can use the following command to grant execute permissions to all .sh files in the current directory and its subdirectories:
find . -type f -name "*.sh" -exec chmod +x {} \;
(maybe need sudo)

Shell脚本必须以LF（换行符）结尾，而不是CRLF（回车换行符）。然而，Windows可能会自动将其转换为CRLF。
为了确保脚本在Linux上可执行，请通过运行chmod +x script_name.sh命令为其授予必要的权限。

您还可以使用以下命令，为当前目录及其子目录中的所有.sh文件授予执行权限：
find . -type f -name "*.sh" -exec chmod +x {} \;
(可能需要sudo权限)