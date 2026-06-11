# Plus++ Interpreter in C

## English

This repository contains an interpreter written in C for a small custom programming language called Plus++.

The interpreter reads `.ppp` source files, loads the program into memory, parses each line, manages declared variables, executes assignment and arithmetic operations, handles `write` statements, and supports `repeat ... times` blocks including nested repeat structures.

The project was developed to understand how programming language execution works after lexical analysis. Instead of only identifying tokens, this interpreter reads source code, interprets statements, updates variable values, controls program flow, and produces output directly from the input program.

---

## Project Overview

The interpreter processes Plus++ source files and executes the instructions inside them.

Supported operations include:

* Variable declaration with `number`
* Assignment with `:=`
* Addition assignment with `+=`
* Subtraction assignment with `-=`
* Output with `write`
* Newline handling with `newline`
* Output chaining with `and`
* Loop execution with `repeat ... times`
* Nested repeat blocks
* Syntax and runtime error reporting

---

## Plus++ Language Features

### Variable Declaration

Variables are declared using the `number` keyword.

```text
number size;
number sum;
```

Declared variables are stored in an internal variable table. Each variable has a name, declaration status, initialization status, and current value.

---

### Assignment Operations

The interpreter supports direct assignment:

```text
size := 5;
```

It also supports assigning one variable's value to another variable:

```text
y := x;
```

---

### Arithmetic Assignment

The language supports addition and subtraction assignment operations:

```text
sum += size;
step -= 1;
```

These operations update the current value of a variable during execution.

---

### Write Statements

The `write` statement prints strings, variables, and newline commands.

Example:

```text
write "Total is:" and total and newline;
```

The interpreter can combine string literals, variable values, and newline outputs in a single write statement.

---

### Repeat Blocks

The interpreter supports loop execution using `repeat ... times`.

Example:

```text
repeat size times
{
  write size and newline;
  sum += size;
}
```

The repeat count can be either a number or a declared variable.

---

### Nested Repeat Blocks

Nested loops are also supported.

Example:

```text
repeat i times
{
  repeat j times
  {
    write "i:" and i and " j:" and j and newline;
  }
}
```

This part shows that the interpreter can detect block boundaries, manage nested braces, and execute inner blocks recursively.

---

## Example Plus++ Program

```text
number size;
number sum;

size := 5;

repeat size times
{
  write size and newline;
  sum += size;
}

write newline and "Sum:" and sum;
```

---

## Example Output

```text
5
4
3
2
1

Sum:15
```

---

## Error Handling

The interpreter reports syntax and runtime errors with line numbers.

Handled errors include:

* Missing semicolon
* Invalid variable declaration
* Invalid assignment format
* Invalid variable name
* Undeclared variable usage
* Uninitialized variable usage
* Invalid number format
* Missing opening `{` for repeat block
* Missing closing `}` for repeat block
* Unrecognized statements
* Too many declared variables
* Program line limit overflow

This error handling makes the interpreter more reliable because it does not only execute valid programs; it also detects incorrect Plus++ code and explains where the problem occurs.

---

## Technologies Used

* C
* GCC / C Compiler
* File I/O
* Manual parsing
* Variable table management
* Recursive block execution
* Command-line program execution
* Syntax and runtime error handling

---

## How to Run

Compile the interpreter:

```bash
gcc myscript.c -o ppp
```

Run the interpreter with a `.ppp` file name without writing the extension:

```bash
./ppp myscript
```

The program automatically reads:

```text
myscript.ppp
```

You can also run other test files:

```bash
./ppp test_assign
./ppp test_math
./ppp test_nested
./ppp test_repeat
./ppp test_write
```

---

## Test Files

The project includes several sample Plus++ programs:

### `myscript.ppp`

Demonstrates variable declaration, repeat block execution, arithmetic assignment, and output.

### `test_assign.ppp`

Tests assigning one variable's value to another variable.

### `test_math.ppp`

Tests large integer values and arithmetic assignment operations.

### `test_nested.ppp`

Tests nested repeat block execution.

### `test_repeat.ppp`

Tests repeat loops with changing variable values.

### `test_write.ppp`

Tests string output, variable output, and newline handling.

---

## What I Implemented and Learned

In this project, I implemented the core logic of a basic programming language interpreter using C.

I worked on reading a source file line by line, storing the entire program in memory, parsing statements manually, and executing them according to the rules of the Plus++ language. This helped me understand how an interpreter manages program state during execution.

The variable management part helped me practice symbol table-like logic. Each declared variable is stored with its name, value, declaration status, and initialization status. This structure made it possible to detect undeclared or uninitialized variable usage during runtime.

The repeat block system helped me understand recursive execution. The interpreter detects opening and closing braces, finds the correct block range, and executes the block multiple times. Supporting nested repeat blocks made the control flow logic more challenging and more realistic.

The error handling part helped me practice writing safer parsing logic. Instead of assuming that every input file is correct, the interpreter checks for missing semicolons, invalid assignments, missing braces, and undeclared variables.

Main topics I practiced:

* Building an interpreter in C
* Reading and storing source files
* Manual parsing of programming language statements
* Variable table management
* Runtime state tracking
* Assignment and arithmetic operation execution
* String and variable output handling
* Recursive execution of repeat blocks
* Nested block parsing
* Syntax and runtime error reporting
* Command-line based program execution

---

# C ile Plus++ Interpreter

## Türkçe

Bu repository, Plus++ adlı küçük özel bir programlama dili için C ile yazılmış bir interpreter içermektedir.

Interpreter, `.ppp` uzantılı kaynak dosyaları okur, programı belleğe yükler, satır satır ayrıştırır, tanımlanan değişkenleri yönetir, atama ve aritmetik işlemleri çalıştırır, `write` komutlarını işler ve iç içe geçmiş yapılar dahil olmak üzere `repeat ... times` bloklarını destekler.

Bu proje, lexical analysis sonrasında programlama dili çalıştırma mantığının nasıl ilerlediğini anlamak için geliştirildi. Proje yalnızca token tanıma yapmaz; kaynak kodu okur, statementları yorumlar, değişken değerlerini günceller, program akışını yönetir ve input programına göre çıktı üretir.

---

## Proje Özeti

Interpreter, Plus++ kaynak dosyalarını işler ve içindeki komutları çalıştırır.

Desteklenen işlemler:

* `number` ile değişken tanımlama
* `:=` ile değer atama
* `+=` ile toplamalı atama
* `-=` ile çıkarmalı atama
* `write` ile çıktı verme
* `newline` ile yeni satır yazdırma
* `and` ile çıktı parçalarını birleştirme
* `repeat ... times` ile döngü çalıştırma
* İç içe repeat blokları
* Syntax ve runtime hata raporlama

---

## Plus++ Dil Özellikleri

### Değişken Tanımlama

Değişkenler `number` keywordü ile tanımlanır.

```text
number size;
number sum;
```

Tanımlanan değişkenler interpreter içindeki değişken tablosunda saklanır. Her değişkenin adı, tanımlanma durumu, initialization durumu ve mevcut değeri tutulur.

---

### Atama İşlemleri

Interpreter doğrudan değer atamayı destekler:

```text
size := 5;
```

Ayrıca bir değişkenin değerini başka bir değişkene atamayı da destekler:

```text
y := x;
```

---

### Aritmetik Atama

Dil, toplamalı ve çıkarmalı atama işlemlerini destekler:

```text
sum += size;
step -= 1;
```

Bu işlemler execution sırasında değişkenin mevcut değerini günceller.

---

### Write Komutları

`write` komutu stringleri, değişkenleri ve newline komutlarını ekrana yazdırır.

Örnek:

```text
write "Total is:" and total and newline;
```

Interpreter tek bir write statement içinde string literal, değişken değeri ve newline çıktısını birlikte işleyebilir.

---

### Repeat Blokları

Interpreter `repeat ... times` yapısıyla döngü çalıştırmayı destekler.

Örnek:

```text
repeat size times
{
  write size and newline;
  sum += size;
}
```

Repeat sayısı doğrudan sayı olabilir veya tanımlanmış bir değişken üzerinden alınabilir.

---

### İç İçe Repeat Blokları

İç içe döngüler de desteklenir.

Örnek:

```text
repeat i times
{
  repeat j times
  {
    write "i:" and i and " j:" and j and newline;
  }
}
```

Bu bölüm, interpreter’ın blok sınırlarını bulabildiğini, iç içe süslü parantezleri yönetebildiğini ve iç blokları recursive şekilde çalıştırabildiğini gösterir.

---

## Örnek Plus++ Programı

```text
number size;
number sum;

size := 5;

repeat size times
{
  write size and newline;
  sum += size;
}

write newline and "Sum:" and sum;
```

---

## Örnek Çıktı

```text
5
4
3
2
1

Sum:15
```

---

## Hata Yönetimi

Interpreter syntax ve runtime hatalarını satır numaralarıyla birlikte raporlar.

Yakalanan hata türleri:

* Eksik noktalı virgül
* Geçersiz değişken tanımlama
* Geçersiz assignment formatı
* Geçersiz değişken adı
* Tanımlanmamış değişken kullanımı
* Initialize edilmemiş değişken kullanımı
* Geçersiz sayı formatı
* Repeat bloğu için eksik açılış `{`
* Repeat bloğu için eksik kapanış `}`
* Tanınmayan statementlar
* Çok fazla değişken tanımlanması
* Program satır limitinin aşılması

Bu hata yönetimi sayesinde interpreter yalnızca doğru programları çalıştırmakla kalmaz, hatalı Plus++ kodlarını da yakalar ve problemin hangi satırda olduğunu gösterir.

---

## Kullanılan Teknolojiler

* C
* GCC / C Compiler
* Dosya okuma
* Manuel parsing
* Değişken tablosu yönetimi
* Recursive block execution
* Komut satırı üzerinden program çalıştırma
* Syntax ve runtime hata yönetimi

---

## Çalıştırma

Interpreter’ı derlemek için:

```bash
gcc myscript.c -o ppp
```

`.ppp` uzantısını yazmadan dosya adıyla çalıştırmak için:

```bash
./ppp myscript
```

Program otomatik olarak şu dosyayı okur:

```text
myscript.ppp
```

Diğer test dosyalarını da çalıştırabilirsiniz:

```bash
./ppp test_assign
./ppp test_math
./ppp test_nested
./ppp test_repeat
./ppp test_write
```

---

## Test Dosyaları

Projede birkaç örnek Plus++ programı bulunmaktadır:

### `myscript.ppp`

Değişken tanımlama, repeat bloğu çalıştırma, aritmetik atama ve çıktı verme işlemlerini gösterir.

### `test_assign.ppp`

Bir değişkenin değerini başka bir değişkene atama işlemini test eder.

### `test_math.ppp`

Büyük integer değerleri ve aritmetik assignment işlemlerini test eder.

### `test_nested.ppp`

İç içe repeat bloğu çalıştırmayı test eder.

### `test_repeat.ppp`

Değişen değişken değerleriyle repeat döngülerini test eder.

### `test_write.ppp`

String çıktısı, değişken çıktısı ve newline kullanımını test eder.

---

## Bu Projede Ne Uyguladım ve Ne Öğrendim?

Bu projede C diliyle temel bir programlama dili interpreter’ının çalışma mantığını implemente ettim.

Kaynak dosyayı satır satır okuma, tüm programı bellekte saklama, statementları manuel olarak ayrıştırma ve Plus++ dilinin kurallarına göre çalıştırma işlemlerini uyguladım. Bu süreç, interpreter’ın execution sırasında program state’ini nasıl yönettiğini anlamamı sağladı.

Değişken yönetimi kısmında symbol table benzeri bir yapı kullandım. Her tanımlanan değişken; adı, değeri, tanımlanma durumu ve initialization durumu ile saklanır. Bu yapı sayesinde runtime sırasında tanımlanmamış veya initialize edilmemiş değişken kullanımı yakalanabilir.

Repeat block sistemi recursive execution mantığını anlamamı sağladı. Interpreter açılış ve kapanış süslü parantezlerini tespit eder, doğru blok aralığını bulur ve bloğu belirtilen sayıda çalıştırır. İç içe repeat bloklarını desteklemek, control flow mantığını daha gerçekçi ve daha zor hale getirdi.

Hata yönetimi kısmında daha güvenli parsing mantığı yazmayı uyguladım. Her input dosyasının doğru olduğunu varsaymak yerine eksik noktalı virgül, geçersiz atama, eksik süslü parantez ve tanımlanmamış değişken gibi hataları kontrol ettim.

Bu projede pratik yaptığım ana konular:

* C ile interpreter geliştirme
* Kaynak dosya okuma ve bellekte saklama
* Programlama dili statementlarını manuel parse etme
* Değişken tablosu yönetimi
* Runtime state takibi
* Assignment ve aritmetik işlem yürütme
* String ve değişken çıktısı işleme
* Repeat bloklarını recursive çalıştırma
* İç içe blok parsing
* Syntax ve runtime hata raporlama
* Komut satırı tabanlı program çalıştırma
