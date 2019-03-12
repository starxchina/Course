**本资料来http://wowubuntu.com/markdown/basic.html**

**使用软件 Typora	https://www.typora.io**





# 段落

使用一到六级#进行分段



# 区块引用

使用>进行表示

##### eg.

> mark
>
> down
>
> > mark
> >
> > down
> >
> > > markdown
> > >
> > > markdown



# 强调和修饰

1、使用\*...*修饰，此为倾斜

##### eg.

markdown*markdown*

2、使用\**...**修饰，此为加粗

##### eg.

markdown**markdown**



# 列表

### 无序列表

使用*作为项目标记(也可以使用加号或减号)

##### eg.

* 1

* 2

* 3

### 有序列表

使用一个数字加一个点表示,注意之后要空一个

如果想再次添加的话，直接打tab键

##### eg.

1. markdown
2. happy



# 链接

> 行内形式：This is a link [github](http://www.github.com)
>
> > 也可以为之加上title属性:	This is a link [github](http://www.github.com "With a title")

参考形式：参考形式的链接让你可以为链接定一个名称，之后你可以在文件的其他地方定义该链接的内容 	形式：  \[名字][编号]

I get 10 times more traffic from [Google][1] than from
[Yahoo][2] or [MSN][3].

[1]: http://google.com/ "Google"
[2]: http://search.yahoo.com/ "Yahoo Search"
[3]: http://search.msn.com/ "MSN Search"



# 图片

行内形式（title 是选择性的）：	!\[alt text](路径 "Title")

 参考形式   	!\[alt text][id]

​			\[id]:路径   “Title”

 

# 代码

在一般的段落文字中，你可以使用反引号 ``` 来标记代码区段，区段内的 `&、<和 > 都会被自动的转换成 HTML 实体，这项特性让你可以很容易的在代码区段内插入 HTML 码：

I strongly recommend against using any `<blink>` tags. I wish SmartyPants used named entities like `&mdash;` instead of decimal-encoded entites like `&#8212;`.