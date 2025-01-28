## Notation 
### **1. 单行注释 (`//`)**
- 用于注释一行内容。
- 注释从 `//` 开始，到行尾结束。

#### 示例：
```cpp
#include <iostream>
int main() {
    // 打印 "Hello, World!" 到控制台
    std::cout << "Hello, World!" << std::endl; // 这是单行注释
    return 0;
}
```

- **优点**：
  - 简单快捷，适合注释少量文字或说明一行代码。
  - 通常用于短注释或临时记录。

---

### **2. 多行注释 (`/* */`)**
- 用于注释多行内容。
- 注释从 `/*` 开始，到 `*/` 结束，之间的内容全部被注释。

#### 示例：
```cpp
#include <iostream>
int main() {
    /*
        这是多行注释
        可以用来解释一段代码的逻辑
        或者暂时屏蔽多行代码
    */
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
```

- **优点**：
  - 适合长注释或多行说明。
  - 可以用来临时屏蔽多行代码。

#### **注意**：
多行注释不能嵌套，例如以下代码是非法的：
```cpp
/* This is a comment
/* Nested comment */ // 错误
*/
```

---

### **3. 常见的注释实践**

#### **1) 描述代码意图**
为每个函数、类或复杂代码段添加注释，说明其功能或逻辑。

```cpp
// 计算两个整数的和
int add(int a, int b) {
    return a + b;
}
```

#### **2) 记录 TODO 或 FIX**
标记需要改进或未来实现的代码。

```cpp
// TODO: 优化下面的循环逻辑
for (int i = 0; i < 100; i++) {
    std::cout << i << std::endl;
}
```

#### **3) 屏蔽调试代码**
使用注释屏蔽暂时不需要的调试代码：

```cpp
// std::cout << "This is for debugging" << std::endl;
```

---

### **4. 注释风格指南**

#### **1) 行尾注释**
- 注释与代码保持在同一行，说明某行代码的作用。
```cpp
int x = 42; // 初始化 x 为 42
```

#### **2) 代码段注释**
- 注释一段代码的逻辑或目的，通常放在代码上方。
```cpp
// 打印 1 到 10 的所有数字
for (int i = 1; i <= 10; i++) {
    std::cout << i << std::endl;
}
```

#### **3) 函数注释**
- 为函数添加注释，说明参数、返回值和功能。

```cpp
/**
 * 计算两个整数的和
 * @param a 第一个整数
 * @param b 第二个整数
 * @return 两个整数的和
 */
int add(int a, int b) {
    return a + b;
}
```

---

### **5. 使用 Doxygen 生成文档**
C++ 支持使用注释工具（如 Doxygen）生成自动化文档。你可以在注释中使用特殊的格式，如：

```cpp
/**
 * @brief 计算两个整数的和
 * @param a 第一个整数
 * @param b 第二个整数
 * @return 两个整数的和
 */
int add(int a, int b) {
    return a + b;
}
```

- **常见标签**：
  - `@brief`：简要描述。
  - `@param`：参数说明。
  - `@return`：返回值说明。

生成的文档会包含详细的函数说明，便于代码阅读和维护。

---

### **总结**
| 注释类型       | 用法                     | 场景                     |
|----------------|--------------------------|--------------------------|
| 单行注释 `//`  | 注释一行内容             | 简单注释或标注重要代码   |
| 多行注释 `/* */` | 注释多行内容             | 长段注释或屏蔽多行代码   |
| Doxygen 注释    | `/** */` 格式化注释      | 自动生成文档或详细说明   |

如果你有具体的代码需要注释，可以告诉我，我可以帮你完善！
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



## Memory Distribution 
`new` 是 C++ 中的动态内存分配运算符，用于在堆（heap）上分配内存。在你的代码中：

```cpp
float* vectors = new float[total];
```

### **作用解释**
1. **动态分配内存**：
   - 这里 `new` 为一个大小为 `total` 的浮点型数组分配内存。
   - `vectors` 是一个指针，指向分配的内存区域的起始地址。

2. **返回指针**：
   - `new` 返回的是分配内存的起始地址，将其存储在 `vectors` 指针中。
   - 之后可以通过 `vectors[i]` 访问分配的数组元素。

3. **存储位置**：
   - 动态分配的内存是在堆中，而非栈中。
   - 堆上的内存需要手动释放（用 `delete[]`），否则会导致内存泄漏。

---

### **为什么使用 `new`？**
在某些情况下，数组大小（如 `total`）只有在运行时才确定，这种情况下无法使用静态分配（如 `float vectors[total];`）。

例如：
- 如果 `total` 是一个运行时计算的值（不是编译时的常量），必须使用动态分配。
- 动态分配允许分配更大的内存空间，突破栈的限制。

---

### **对应的释放内存**
使用 `new` 动态分配的内存必须显式释放，否则会导致内存泄漏：

```cpp
delete[] vectors;
```
- `delete[]` 会释放由 `new` 分配的数组内存。
- 每个 `new` 必须对应一个 `delete[]`。

---

### **示例：动态分配和释放**
```cpp
#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter size of array: ";
    cin >> n;

    // 动态分配内存
    float* arr = new float[n];

    // 初始化数组
    for (int i = 0; i < n; i++) {
        arr[i] = i * 1.1f;  // 浮点值赋值
    }

    // 输出数组
    for (int i = 0; i < n; i++) {
        cout << "arr[" << i << "] = " << arr[i] << endl;
    }

    // 释放内存
    delete[] arr;

    return 0;
}
```

**运行结果**（假设输入数组大小为 5）：
```
Enter size of array: 5
arr[0] = 0
arr[1] = 1.1
arr[2] = 2.2
arr[3] = 3.3
arr[4] = 4.4
```

---

### **`new` 和 `malloc` 的区别**
1. **`new` 是 C++ 专有的**，而 `malloc` 是 C 风格的。
2. **构造函数的调用**：
   - `new` 分配内存时会自动调用对象的构造函数。
   - `malloc` 只分配内存，不会初始化。
3. **语法**：
   - `new` 不需要显式指定分配大小，只需提供元素类型。
   - `malloc` 需要手动指定大小（以字节为单位）。

**示例对比**：
```cpp
// 使用 new
int* arr1 = new int[10];  // 分配 10 个整型元素的内存
delete[] arr1;            // 释放内存

// 使用 malloc
int* arr2 = (int*)malloc(10 * sizeof(int));  // 分配 10 个整型元素的内存
free(arr2);                                  // 释放内存
```

---

### **总结**
- `new` 是 C++ 中用于动态分配内存的关键字。
- 它在运行时分配所需大小的内存，并返回一个指针。
- 使用 `new` 动态分配的内存需要手动释放，使用 `delete` 或 `delete[]` 防止内存泄漏。
- 动态分配在处理运行时确定大小的数据结构（如数组）时非常有用。