## Package
```cpp
#include <stdio.h>
```
- 头文件 `<stdio.h>` 提供 C 标准输入输出功能，例如 `printf` 和 `scanf` 函数。主要用于基本的输入输出操作。

```cpp
#include <ctime>
```
- `<ctime>` 提供与时间相关的函数，例如 `time` 和 `clock`。在这里可能用于生成随机数的种子（`srand(time(NULL))`）。

```cpp
#include <cstring>
```
- `<cstring>` 是 C 字符串操作的头文件，提供常见的字符串处理函数，例如 `strcmp`、`strcpy` 等。

```cpp
#include <sstream>
```
- `<sstream>` 提供了基于流的字符串处理功能，允许将字符串与其他数据类型相互转换。例如，将字符串解析为整数，或将数值格式化为字符串。

```cpp
#include <iomanip>
```
- `<iomanip>` 提供输入输出流的格式化功能，例如控制小数点后的精度（`std::setprecision`）或设置字段宽度（`std::setw`）。

```cpp
#include <iostream>
```
- `<iostream>` 是 C++ 输入输出流的标准头文件，提供输入（`std::cin`）和输出（`std::cout`）功能。常用于控制台输入输出。

```cpp
#include <mpi.h>
```
- `<mpi.h>` 是 MPI（Message Passing Interface）的头文件，提供了实现并行计算的函数和工具，例如 `MPI_Init`、`MPI_Comm_rank` 和 `MPI_Reduce`。


## Pointer 

### 1. **指针的基本概念**
- **定义**：指针是一个变量，**其值是另一个变量的内存地址**。
- **语法**：`datatype *pointer_name;`
  - `datatype` 是指针指向的变量类型。
  - `*pointer_name` 是指针变量。

---

### 2. **常见操作**
- **取地址**：`&pointer_name` 运算符用于获取变量的地址。
- **解引用**：`*pointer_name` 运算符用于访问指针指向的变量的值。

---

### 3. **指针的简单例子**
```c
#include <stdio.h>

int main() {
    int a = 10;          // 定义一个整数变量
    int *p;              // 定义一个指向整数的指针变量

    p = &a;              // 将变量 a 的地址赋值给指针 p

    printf("变量 a 的值: %d\n", a);       // 输出变量 a 的值
    printf("变量 a 的地址: %p\n", &a);    // 输出变量 a 的地址
    printf("指针 p 的值: %p\n", p);       // 输出指针 p 的值，即 a 的地址
    printf("指针 p 指向的值: %d\n", *p);  // 输出指针 p 指向的值，即 a 的值

    *p = 20;             // 修改指针指向的值，相当于修改 a
    printf("修改后的变量 a 的值: %d\n", a);

    return 0;
}
```

**运行结果**：
```bash
变量 a 的值: 10
变量 a 的地址: 0x7ffc0b0d
指针 p 的值: 0x7ffc0b0d
指针 p 指向的值: 10
修改后的变量 a 的值: 20
```

---

### 4. **动态内存分配**
C语言使用指针来动态分配内存，常用的函数有 `malloc` 和 `free`。

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr;
    int n = 5;

    // 动态分配内存
    arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("内存分配失败\n");
        return 1;
    }

    // 使用指针操作动态数组
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    printf("动态数组的值: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    // 释放内存
    free(arr);

    return 0;
}
```

**运行结果**：
```bash
动态数组的值: 1 2 3 4 5
```

---

### 5. **指针数组**
指针也可以用来存储地址数组。

```c
#include <stdio.h>

int main() {
    int a = 10, b = 20, c = 30;
    int *arr[3];  // 指针数组

    arr[0] = &a;
    arr[1] = &b;
    arr[2] = &c;

    for (int i = 0; i < 3; i++) {
        printf("arr[%d] 指向的值: %d\n", i, *arr[i]);
    }

    return 0;
}
```

**运行结果**：
```
arr[0] 指向的值: 10
arr[1] 指向的值: 20
arr[2] 指向的值: 30
```

---

### 6. **指向指针的指针（多级指针）**
指针可以指向另一个指针，形成多级指针。

```c
#include <stdio.h>

int main() {
    int a = 10;
    int *p = &a;         // 一级指针
    int **pp = &p;       // 二级指针

    printf("变量 a 的值: %d\n", a);
    printf("一级指针 p 指向的值: %d\n", *p);
    printf("二级指针 pp 指向的值: %d\n", **pp);

    return 0;
}
```

**运行结果**：
```
变量 a 的值: 10
一级指针 p 指向的值: 10
二级指针 pp 指向的值: 10
```

---

### 7. **函数中的指针**
指针可以用来修改函数外的变量（传递引用）。

```c
#include <stdio.h>

void changeValue(int *p) {
    *p = 100;  // 修改指针指向的值
}

int main() {
    int a = 10;

    printf("修改前 a 的值: %d\n", a);
    changeValue(&a);
    printf("修改后 a 的值: %d\n", a);

    return 0;
}
```

**运行结果**：
```
修改前 a 的值: 10
修改后 a 的值: 100
```

## Functions
在 C++ 中，函数是组织代码的基本单位。函数用于封装一段代码逻辑，可以被多次调用，接受输入并返回输出。下面是 C++ 中函数的一般格式和使用方法：

---

### **函数的通用格式**

```cpp
return_type function_name(parameter_list) {
    // 函数主体
    // 执行代码逻辑
    return value;  // 如果 return_type 是 void，则可以省略
}
```

- **`return_type`**：函数的返回类型，可以是任意基本类型（如 `int`、`float`），也可以是自定义类型（如类或结构体）。如果函数没有返回值，则使用 `void`。
- **`function_name`**：函数的名称，用于标识和调用函数。
- **`parameter_list`**：参数列表，表示函数接受的输入，可以为空。每个参数需要指定类型和名称。
- **`return`**：返回值，用于将结果返回给调用者。如果返回类型是 `void`，则可以省略。

---

### **简单示例：无返回值、无参数**
```cpp
#include <iostream>

// 定义一个简单的函数
void sayHello() {
    std::cout << "Hello, World!" << std::endl;
}

int main() {
    sayHello();  // 调用函数
    return 0;
}
```

**输出**：
```
Hello, World!
```

---

### **有返回值、无参数**
```cpp
#include <iostream>

// 定义一个返回整数的函数
int getNumber() {
    return 42;  // 返回固定值
}

int main() {
    int num = getNumber();  // 接收函数返回值
    std::cout << "The number is: " << num << std::endl;
    return 0;
}
```

**输出**：
```
The number is: 42
```

---

### **有返回值、有参数**
```cpp
#include <iostream>

// 定义一个带参数的函数
int add(int a, int b) {
    return a + b;  // 返回两个参数的和
}

int main() {
    int result = add(10, 20);  // 传递参数并接收返回值
    std::cout << "The sum is: " << result << std::endl;
    return 0;
}
```

**输出**：
```
The sum is: 30
```

---

### **无返回值、有参数**
```cpp
#include <iostream>

// 打印传入的数字
void printNumber(int number) {
    std::cout << "The number is: " << number << std::endl;
}

int main() {
    printNumber(100);  // 传递参数
    return 0;
}
```

**输出**：
```
The number is: 100
```

---

### **函数的默认参数**
你可以为函数的参数设置默认值，这样在调用时可以选择省略某些参数。

```cpp
#include <iostream>

// 带有默认参数的函数
int multiply(int a, int b = 2) {
    return a * b;
}

int main() {
    std::cout << multiply(5) << std::endl;    // 使用默认值，输出 10
    std::cout << multiply(5, 3) << std::endl; // 覆盖默认值，输出 15
    return 0;
}
```

**输出**：
```
10
15
```

---

### **函数的声明与定义**
函数可以先声明后定义，特别是在多个文件中使用时。

#### 声明：
```cpp
// 函数声明
int add(int a, int b);
```

#### 定义：
```cpp
// 函数定义
int add(int a, int b) {
    return a + b;
}
```

#### 示例：
```cpp
#include <iostream>

// 函数声明
int add(int a, int b);

int main() {
    std::cout << "The sum is: " << add(3, 7) << std::endl;
    return 0;
}

// 函数定义
int add(int a, int b) {
    return a + b;
}
```

---

### **函数的重载**
C++ 支持函数重载，即多个同名函数可以根据参数类型或数量的不同来区分。

```cpp
#include <iostream>

// 重载函数
int multiply(int a, int b) {
    return a * b;
}

double multiply(double a, double b) {
    return a * b;
}

int main() {
    std::cout << multiply(3, 4) << std::endl;        // 调用第一个函数
    std::cout << multiply(3.5, 2.0) << std::endl;    // 调用第二个函数
    return 0;
}
```

**输出**：
```
12
7
```

---

### **递归函数**
函数可以调用自己，这被称为递归。

```cpp
#include <iostream>

// 递归函数：计算阶乘
int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);  // 递归调用
}

int main() {
    std::cout << "Factorial of 5 is: " << factorial(5) << std::endl;
    return 0;
}
```

**输出**：
```
Factorial of 5 is: 120
```

---

### **总结**
1. 函数格式：`return_type function_name(parameter_list) { ... }`。
2. 根据需求选择是否有返回值和参数。
3. 支持默认参数、函数重载、递归等特性。
4. 可以将函数声明和定义分离，便于组织代码。

