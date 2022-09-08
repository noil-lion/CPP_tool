# 关键字记录
1. static_cast  
static_cast是一个c++运算符，功能是把一个表达式转换为某种类型，但没有运行时类型检查来保证转换的安全性。
```
    int n = -1;
    char ch = 'a';
    double dle = 0.1;

    dle = n;  //c语言中的隐式转换
    dle = static_cast<double>(n);  //明确类型int转换为double型数据
    n = static_cast<int>(ch);    //明确将类型char转为int型数据


```